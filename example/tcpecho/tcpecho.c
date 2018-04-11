/* 
 * Copyright (C) 2016 Jerry Han (hanj4096@gmail.com)
 * All rights reserved.
 */

/*
 * A simple tcp echo server
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
#include <assert.h>

#include "posix_api.h"


#define MAX_CONN  (50000)
#define MAX_EVENTS (MAX_CONN * 3)
#define BUF_LEN 1024


static int accept_conn(int epoll_fd, int lis_fd)
{
	struct epoll_event ev;
	int fd;

	fd = ngflow_accept(lis_fd, NULL, NULL);

	if (fd >= 0) {
		int true = 1;
		ngflow_ioctl(fd, FIONBIO, &true);
		ev.events = EPOLLIN;
		ev.data.fd = fd;
		ngflow_epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
	} else {
		if (errno != EAGAIN) {
			printf("ngflow_accept() error %s\n", strerror(errno));
		}
	}

	return fd;
}


static void tcpecho_func()
{
	int lis_fd;
	int epoll_fd;
	struct epoll_event *events;
	int i, ret, do_accept;
	struct epoll_event ev;
	struct sockaddr_in saddr;
	char buf[BUF_LEN];


	/* create socket and set it as nonblocking */
	lis_fd = ngflow_socket(AF_INET, SOCK_STREAM, 0);
	if (lis_fd < 0) {
		printf("Failed to create listening socket!\n");
		exit(-1);
	}

	int true = 1;
	ret = ngflow_ioctl(lis_fd, FIONBIO, &true);
	if (ret < 0) {
		printf("Failed to set socket in nonblocking mode.\n");
		exit(-1);
	}

	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = INADDR_ANY;
	saddr.sin_port = htons(7);
	ret = ngflow_bind(lis_fd, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in));
	if (ret < 0) {
		printf("Failed to bind to the listening socket!\n");
		exit(-1);
	}

	ret = ngflow_listen(lis_fd, 4096);
	if (ret < 0) {
		printf("ngflow_listen() failed!\n");
		exit(-1);
	}

	epoll_fd = ngflow_epoll_create(MAX_EVENTS);
	if (epoll_fd < 0) {
		printf("Failed to create epoll descriptor!\n");
		exit(-1);
	}

	/* wait for incoming accept events */
	ev.events = EPOLLIN;
	ev.data.fd = lis_fd;
	ngflow_epoll_ctl(epoll_fd, EPOLL_CTL_ADD, lis_fd, &ev);

	events = (struct epoll_event *) calloc(MAX_EVENTS, sizeof(struct epoll_event));
	if (!events) {
		printf("Failed to create event struct!\n");
		exit(-1);
	}

	do {
		ret = ngflow_epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		if (ret < 0) {
			if (errno != EINTR)
				perror("ngflow_epoll_wait");
			break;
		}

		do_accept = 0;
		for (i = 0; i < ret; i++) {

			if (events[i].data.fd == lis_fd) {
				/* if the event is for the lis_fd, accept connection */
				do_accept = 1;
			}
			else if (events[i].events & EPOLLERR) {
				ngflow_epoll_ctl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, NULL);
				ngflow_close(events[i].data.fd);

			}
			else if (events[i].events & EPOLLIN) {
				memset(buf, 0, BUF_LEN);
				ret = ngflow_read(events[i].data.fd, buf, BUF_LEN);
				if (ret > 0) {
					ngflow_write(events[i].data.fd, buf, ret);
				}
				else if (ret == 0) {
					/* connection closed by remote host */
					ngflow_epoll_ctl(epoll_fd, EPOLL_CTL_DEL,
							 events[i].data.fd, NULL);
					ngflow_close(events[i].data.fd);
				}
				else if (ret < 0) {
					/* if not EAGAIN, it's an error */
					if (errno != EAGAIN) {
						ngflow_epoll_ctl(epoll_fd, EPOLL_CTL_DEL,
								 events[i].data.fd, NULL);
						ngflow_close(events[i].data.fd);
					}
				}

			} else if (events[i].events & EPOLLOUT) {

				/* Doesn't support large stream currently */

			} else {
				break;
			}
		}

		/* If do_accept flag is set, accept connections */
		if (do_accept) {
			while (1) {
				ret = accept_conn(epoll_fd, lis_fd);
				if (ret < 0) {
					break;
				}
			}
		}
	} while (1);
}


int main (void)
{
    /* Initialize ngflow */
    if (ngflow_init(NULL)) {
        printf("Invalid ngflow startup config file!\n");
        exit(-1);
    }

    tcpecho_func();

    ngflow_exit();

    return 0;
}


