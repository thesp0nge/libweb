// [The "BSD licence"]
//
// libweb - tiny, portable and fast HTTP communcation library
//
// Copyright (c) 2010 Paolo Perego, paolo@armoredcode.com
// http://www.armoredcode.com
// http://www.linkedin.com/in/thesp0nge
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. The name of the author may not be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <string.h>
#include <sys/socket.h>

#include "libweb.h"
#include "log.h"

int web_send(int sd, void *buffer, size_t len)
{
	int nb = 0, wb = 0;

	for (nb=0; nb < len; nb+=wb) {
		wb = write(sd, (const void *)buffer, len);
		if (wb == -1) 
			return fprintf(stderr, "%s(): write error (%s)", __FUNCTION__, strerror(errno));
	}
	return nb;
}

int web_recv(int sd, void *buffer, size_t len, int to)
{
	fd_set fds;
	int n;
	struct timeval tv;

	// set up the file descriptor set
	FD_ZERO(&fds);
	FD_SET(sd, &fds);

	// set up the struct timeval for the timeout
	tv.tv_sec = to;
	tv.tv_usec = 0;

	// wait until timeout or data received
	n = select(sd+1, &fds, NULL, NULL, &tv);
	if (n == 0) return LIBWEB_TIMEOUT; // timeout!
	if (n == -1) return LIBWEB_ERROR; // error

	// data must be here, so do a normal recv()
	return recv(sd, buffer, len, 0);
}

/*
 * Creates a client communication endpoint to the specified hostname and to the
 * specified port number.
 * This routine takes care of calling:
 * 	+ socket()
 * 	+ connect()
 *
 * @param hostname the server hostname
 * @param port the server port number to connect to
 * @return a valid socket descriptor or -1 if an error occurs
 */
int web_client(char *hostname, unsigned int port) 
{
	int sd = 0;
	struct sockaddr_in sin;
	struct hostent *he;

	if ((hostname == NULL) || (port <=0)) {
		fprintf(stderr, "attempt to feed %s() with invalid parameters (%s, %d)\n", 
				__FUNCTION__, 
				hostname,
				port);
		return -1;
	}
	// 1. cook the socket...
	if ((sd = socket (PF_INET, SOCK_STREAM, 0)) == -1) {
		fprintf(stderr, "%s(): can't cook socket (%s)\n", 
				__FUNCTION__, strerror(errno));
		return -1;
	}
	
	// 2. fill my peer information
	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
	// 2.1 resolve server hostname
	if (inet_aton(hostname, &(sin.sin_addr)) == 0) {
		he = gethostbyname(hostname);
		if (he == NULL) {
			fprintf(stderr,"%s(): can't resolve %s (%s)\n",
				__FUNCTION__,
				hostname,
				hstrerror(h_errno));
			return -1;
		}
		memcpy(&(sin.sin_addr), he->h_addr, sizeof(struct in_addr));

	}
	// 3. so please... connect
	if ( connect(sd, (const struct sockaddr *)&sin, sizeof(struct sockaddr)) == -1) {
		fprintf(stderr,"%s(): can't connect to %s:%d (%s)\n",
			__FUNCTION__,
			hostname,
			port,
			strerror(errno));
		return -1;
	}
	return sd;
}

/*
 * Sends a GET / HTTP/1.0 request to get web server return banner.
 * 
 * @param sd: a previously opened socked descriptor.
 * @param buf: a previously allocated memory buffer to store web server answer
 * @param len: the buffer lenght
 * @return the number of bytes read from the web server or -1 if some error occured.
 */
int web_helo(int sd, char *buffer, size_t len) {
	
	if ((sd<3)) {
		fprintf(stderr, "%s(): invalid or reserved file descriptor (%d)\n", 
			__FUNCTION__, 
			sd);
		return -1;
	}
	if (buffer == NULL) { 
		fprintf(stderr, "%s(): a NULL memory pointer has been passed as memory pointer. Don't.\n", 
			__FUNCTION__); 
		return -1;
	}
	
	if (web_send(sd, HELO, strlen(HELO))<0)
		return -1;
	
	// fprintf(stderr, "%s() a custom implementation of strlen(), must be provided", __FUNCTION__);
	
	/*
	 * This is a sample answer for the HTTP helo given from my apache installation on my 
	 * SnowLeopard MacBook.
	 */
	
	// HTTP/1.1 200 OK
	// Date: Mon, 22 Mar 2010 17:17:55 GMT
	// Server: Apache/2.2.13 (Unix) mod_ssl/2.2.13 OpenSSL/0.9.8l DAV/2
	// Content-Location: index.html.en
	// Vary: negotiate
	// TCN: choice
	// Last-Modified: Sat, 20 Nov 2004 20:16:24 GMT
	// ETag: "c39dd9-2c-3e9564c23b600"
	// Accept-Ranges: bytes
	// Content-Length: 44
	// Connection: close
	// Content-Type: text/html
	// Content-Language: en
	
	int ret = web_recv(sd, buffer, len, 5);
	
	return ret;
}
/*
 * Looks in the given buffer for an HTTP/1.1 header so to give the return code
 * 
 * @param buffer the char buffer containing, maybe, an HTTP answer
 * @return the HTTP return code or -1 if something wrong occured
 */
int web_http_code(const char *buffer) 
{
	char *p = NULL;
	char *s = NULL;
	char code_s[4];
	
	if (buffer == NULL) { 
		fprintf(stderr, "%s(): a NULL memory pointer has been passed as memory pointer. Don't.\n", 
			__FUNCTION__); 
		return -1;
	}
	p = strstr(buffer, "HTTP/1.");
	if (p == NULL) {
		fprintf(stderr, "%s(): doesn't seem to be a good HTTP response block\n", __FUNCTION__);
		return -1;
	}
	
	s = p+(9*sizeof(char));
	strncpy(code_s, s, 3);
	return atoi(code_s);
}

/*
 * Looks in the given buffer for a Server: field and copy it to the http_response parameter.
 * This information can be used for web server footprinting pourposes.
 *
 * @param buffer the char buffer containing, maybe, an HTTP answer
 * @param hr the http_response variable to be used to store results.
 * @return 0 if the Server: header has been coped to hr, or -1 otherwise
 */
int web_http_server(const char *buffer, http_response *hr) {
	char *s;
	char *nl;
	if (buffer == NULL || hr == NULL) { 
		fprintf(stderr, "%s(): a NULL memory pointer has been passed as memory pointer. Don't.\n", 
			__FUNCTION__); 
		return -1;
	}
	
	s = strstr(buffer, SERVER_HEADER);
	if (s == NULL) {
		strcpy(hr->server, NO_SERVER_HEADER);
		return -1;
	}
	// SERVE UNA STRCPY che COPY FINO AD UN CERTO CARATTERE
	nl = strstr(s+1*sizeof(char), "\n");
	strncpy(hr->server, s+(strlen(SERVER_HEADER) * sizeof(char)), strlen(s)-strlen(nl));
	
	debug(hr->server);
	printf("---\n");
	return 0;
}
