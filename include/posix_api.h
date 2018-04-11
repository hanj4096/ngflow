/*
 * Copyright (C) 2016 Jerry Han (hanj4096@gmail.com)
 * All rights reserved.
 */

#ifndef __POSIX_API_H__
#define __POSIX_API_H__

#include <stdint.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/epoll.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
 * Initialize ngflow
 */
int ngflow_init(char *config_file);

/*
 * Clean up ngflow resource
 */
void ngflow_exit();


/*
 * Create a new socket of type TYPE in domain DOMAIN, using
 * protocol PROTOCOL.  If PROTOCOL is zero, one is chosen automatically.
 * Returns a file descriptor for the new socket, or -1 for errors.
 */
extern int ngflow_socket(int __domain, int __type, int __protocol);

/*
 * Create a new socket of type TYPE in domain DOMAIN, using
 * protocol PROTOCOL.  If PROTOCOL is zero, one is chosen automatically.
 * Returns a file descriptor for the new socket, or -1 for errors.
 */
extern int ngflow_bind(int __fd, const struct sockaddr *__addr, socklen_t __len);

/*
 * Open a connection on socket FD to peer at ADDR (which LEN bytes long).
 * For connectionless socket types, just set the default address to send to
 * and the only address from which to accept transmissions.
 * Return 0 on success, -1 for errors.
 */
extern int ngflow_connect(int __fd, const struct sockaddr *__addr, socklen_t __len);


/*
 * Prepare to accept connections on socket FD.
 * N connection requests will be queued before further requests are refused.
 * Returns 0 on success, -1 for errors.
 */
extern int ngflow_listen(int __fd, int __n);

/*
 * Await a connection on socket FD.
 * When a connection arrives, open a new socket to communicate with it,
 * set *ADDR (which is *ADDR_LEN bytes long) to the address of the connecting
 * peer and *ADDR_LEN to the address's actual length, and return the
 * new socket's descriptor, or -1 for errors.
 */
extern int ngflow_accept(int __fd, struct sockaddr *__addr, socklen_t *__addr_len);

/*
 * Shut down all or part of the connection open on socket FD.
 * HOW determines what to shut down:
 *   SHUT_RD   = No more receptions;
 *   SHUT_WR   = No more transmissions;
 *   SHUT_RDWR = No more receptions or transmissions.
 * Returns 0 on success, -1 for errors.
 *
 * Only SHUT_RDWR is supported.
 */
extern int ngflow_shutdown(int __fd, int __how);






/*
 * Read N bytes into BUF from socket FD.
 * Returns the number read or -1 for errors.
 *
 * Don't support flags right now.
 */
extern ssize_t ngflow_recv(int __fd, void *__buf, size_t __n, int __flags);

/*
 * Send N bytes of BUF to socket FD.  Returns the number sent or -1.
 *
 * Don't support flags right now.
 */
extern ssize_t ngflow_send(int __fd, const void *__buf, size_t __n, int __flags);




/*
 * Read N bytes into BUF through socket FD.
 * If ADDR is not NULL, fill in *ADDR_LEN bytes of it with tha address of
 * the sender, and store the actual size of the address in *ADDR_LEN.
 * Returns the number of bytes read or -1 for errors.
 */
extern ssize_t ngflow_recvfrom(int __fd, void *__buf, size_t __n,
				int __flags, struct sockaddr *__addr,
				socklen_t *__addr_len);


/*
 * Send N bytes of BUF on socket FD to peer at address ADDR (which is
 * ADDR_LEN bytes long).  Returns the number sent, or -1 for errors.
 */
extern ssize_t ngflow_sendto (int __fd, const void *__buf, size_t __n,
				int __flags, const struct sockaddr *__addr,
				socklen_t __addr_len);



/*
 * Set socket FD's option OPTNAME at protocol level LEVEL
 * to *OPTVAL (which is OPTLEN bytes long).
 * Returns 0 on success, -1 for errors.
 */
extern int ngflow_setsockopt(int __fd, int __level, int __optname,
				const void *__optval, socklen_t __optlen);

/*
 * Put the current value for socket FD's option OPTNAME at protocol level LEVEL
 * into OPTVAL (which is *OPTLEN bytes long), and set *OPTLEN to the value's
 * actual length.  Returns 0 on success, -1 for errors.
 */
extern int ngflow_getsockopt(int __fd, int __level, int __optname,
				void *__optval, socklen_t *__optlen);


/*
 * Put the address of the peer connected to socket FD into *ADDR
 * (which is *LEN bytes long), and its actual length into *LEN.
 */
extern int ngflow_getpeername(int __fd, struct sockaddr *__addr,
				socklen_t *__len);


/*
 * Put the local address of FD into *ADDR and its length in *LEN.
 */
extern int ngflow_getsockname(int __fd, struct sockaddr *__addr,
				socklen_t *__len);




/*
 * /usr/include/unistd.h
 */

/*
 * Close the file descriptor FD.
 */
extern int ngflow_close(int __fd);


/*
 * Read NBYTES into BUF from FD.  Return the
 * number read, -1 for errors or 0 for EOF.
 */
extern ssize_t ngflow_read(int __fd, void *__buf, size_t __nbytes);



/*
 * Write N bytes of BUF to FD.  Return the number written, or -1.
 */
extern ssize_t ngflow_write(int __fd, const void *__buf, size_t __n);





/*
 * /usr/include/x86_64-linux-gnu/sys/ioctl.h
 */


/*
 * Perform the I/O control operation specified by REQUEST on FD.
 * One argument may follow; its presence and type depend on REQUEST.
 * Return value depends on REQUEST.  Usually -1 indicates error.
 *
 * but only FIONREAD/FIONBIO is supported currently
 */
extern int ngflow_ioctl(int __fd, unsigned long int __request, void *argp);






/*
 * /usr/include/x86_64-linux-gnu/sys/uio.h
 */

/*
 * Read data from file descriptor FD, and put the result in the
 * buffers described by IOVEC, which is a vector of COUNT 'struct iovec's.
 * The buffers are filled in the order specified.
 * Operates just like 'read' (see <unistd.h>) except that data are
 * put in IOVEC instead of a contiguous buffer.
 */
extern ssize_t ngflow_readv(int __fd, const struct iovec *__iovec, int __count);

/*
 * Write data pointed by the buffers described by IOVEC, which
 * is a vector of COUNT 'struct iovec's, to file descriptor FD.
 * The data is written in the order specified.
 * Operates just like 'write' (see <unistd.h>) except that the data
 * are taken from IOVEC instead of a contiguous buffer.
 */
extern ssize_t ngflow_writev(int __fd, const struct iovec *__iovec, int __count);






/*
 * /usr/include/x86_64-linux-gnu/sys/epoll.h
 */



/*
 * Creates an epoll instance.  Returns an fd for the new instance.
 * The "size" parameter is a hint specifying the number of file
 * descriptors to be associated with the new instance.  The fd
 * returned by epoll_create() should be closed with close().
 */
extern int ngflow_epoll_create(int __size);

/*
 * Same as epoll_create but with an FLAGS parameter.  The unused SIZE
 * parameter has been dropped.
 *
 * To be implemented later.
 */
extern int ngflow_epoll_create1 (int __flags);


/*
 * Manipulate an epoll instance "epfd". Returns 0 in case of success,
 * -1 in case of error ( the "errno" variable will contain the
 * specific error code ) The "op" parameter is one of the EPOLL_CTL_*
 * constants defined above. The "fd" parameter is the target of the
 * operation. The "event" parameter describes which events the caller
 * is interested in and any associated user data.
 */
extern int ngflow_epoll_ctl(int __pefd, int __op, int __fd,
				struct epoll_event *__event);


/*
 * Wait for events on an epoll instance "epfd". Returns the number of
 * triggered events returned in "events" buffer. Or -1 in case of
 * error with the "errno" variable set to the specific error code. The
 * "events" parameter is a buffer that will contain triggered
 * events. The "maxevents" is the maximum number of events to be
 * returned ( usually size of "events" ). The "timeout" parameter
 * specifies the maximum wait time in milliseconds (-1 == infinite).
 */
int ngflow_epoll_wait(int __epfd, struct epoll_event *__events,
			int __maxevents, int __timeout);



#ifdef __cplusplus
};
#endif

#endif /* __POSIX_API_H__ */
