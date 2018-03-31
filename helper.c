#include "helper.h"
#include <stdio.h>

/*  Read a line from a socket  */
ssize_t Readline(int sockd, void *vptr, size_t maxlen) {
	// printf("Enter readline.\n");
    ssize_t n, rc;
    char    c, *buffer;

    buffer = vptr;

    for ( n = 0; n < maxlen; n++ ) {
		// printf("%lu. Entered for loop of readline.\n", n);
		// printf("Max len = %lu\n", maxlen);
		if ( (rc = read(sockd, &c, 1)) == 1 ) {
			*buffer++ = c;
		}
		else {
			--n;
		}
    }

    *buffer = 0;
    return n;
}


/*  Write a line to a socket  */
ssize_t Writeline(int sockd, const void *vptr, size_t n) {
    size_t      nleft;
    ssize_t     nwritten;
    const char *buffer;

    buffer = vptr;
    nleft  = n;

    while ( nleft > 0 ) {
	if ( (nwritten = write(sockd, buffer, nleft)) <= 0 ) {
	    if ( errno == EINTR )
			nwritten = 0;
	    else
			return -1;
	}
	nleft  -= nwritten;
	buffer += nwritten;
    }

    return n;
}




