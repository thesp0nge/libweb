// [The "BSD licence"]
//
// get.c - GET HTTP method implementation
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
#include <string.h>

#include <errno.h>
#include "libweb.h"
#include "log.h"

/*
 * Sends an HTTP GET request and wait for a response.
 *
 * @param url the url we want to retrieve
 * @return the HTTP response from the web server or NULL if an error occured.
 */
http_response *web_get(char *urlstr) {
	http_response *ret;
	url *__url;
	int sd, rb, wb;
	char *buffer, *get_string;
	
	if (urlstr == NULL) {
		fprintf(stderr, "%s: NULL pointer dereference detected. Giving up.\n", __FUNCTION__);
		return NULL;
	}
	
	if ( (__url = string2url(urlstr)) == NULL ) {
		fprintf(stderr, "%s: invalid url (\"%s\")\n", __FUNCTION__, urlstr);
		return NULL;
	}
	
	sd = web_client(__url->website, __url->port);
	if (sd < 0) {
		fprintf(stderr, "%s: can't communicate with %s\n", __FUNCTION__, __url->website);
		return NULL;
	}
	
	get_string = malloc(512);
	memset(get_string, 0, 512);
	sprintf(get_string, "GET %s HTTP//1.0\n\n", __url->resource);
	
	if ( ( wb = web_send(sd, get_string, strlen(get_string))) <0 ) {
		fprintf(stderr, "%s: can't get %s\n", __FUNCTION__, __url->resource);
		return NULL;
	}
	
	// get at least BUFFER_RESPONSE bytes
	buffer = malloc(BUFFER_RESPONSE);
	if (buffer == NULL) {
		fprintf(stderr, "%s: aiee, a malloc() error here (%s)\n", __FUNCTION__, strerror(errno));
		return NULL;
	}
	
	memset(buffer, 0, BUFFER_RESPONSE);
	rb = web_recv(sd, buffer, BUFFER_RESPONSE, GET_TIMEOUT);
	if (rb == 0) {
		fprintf(stderr, "%s: I just read 0 bytes from remote host\n", __FUNCTION__);
		return NULL;
	}
	
	// preparing return value
	ret = malloc(sizeof(http_response));
	
	// copying raw http response
	strcpy(ret->response, buffer);
	
	// splitting http return code
	ret->code = web_http_code(buffer);
	if (ret->code == -1) {
		fprintf(stderr, "%s: aiee, invalid response received from web server\n", __FUNCTION__);
		return NULL;
	}
	
	// extracting server banner if present
	web_http_server(buffer, ret);
	return ret;
}