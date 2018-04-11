/*
 * Copyright (C) 2016 Jerry Han (hanj4096@gmail.com)
 * All rights reserved.
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
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>

#include "posix_api.h"

#define BUF_LEN 1024

static void udpecho_func(void)
{
	int lis_fd;
	int ret;
	socklen_t addr_len;
	struct sockaddr_in saddr, remote_addr;
	char buf[BUF_LEN];

	/* create socket and set it as nonblocking */
	lis_fd = ngflow_socket(AF_INET, SOCK_DGRAM, 0);
	if (lis_fd < 0) {
		printf("Failed to create listening socket!\n");
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

	while (1) {
		ret = ngflow_recvfrom(lis_fd, buf, BUF_LEN, 0,
				      (struct sockaddr *)&remote_addr, &addr_len);
		if (ret > 0) {
			ngflow_sendto(lis_fd, buf, ret, 0,
				      (const struct sockaddr *)&remote_addr,
				      sizeof(struct sockaddr_in));
		} else if (ret < 0) {
			/* if not EAGAIN, it's an error */
			if (errno != EAGAIN) {
				ngflow_close(lis_fd);
				break;
			}
		}
	}

}


int main (void)
{

	/* initialize ngflow */
	if (ngflow_init(NULL)) {
		printf("Invalid ngflow startup config file!\n");
		exit(-1);
	}


    udpecho_func();

    ngflow_exit();

    return 0;
}


