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
#include <string.h>
#include "libweb.h"

/*
 * Sends an HTTP GET request and wait for a response.
 *
 * @param url the url we want to retrieve
 * @return the HTTP response from the web server or NULL if an error occured.
 */
http_response *web_get(char *url) {
	http_response *ret;
	url *__url;
	int sd, rb;
	char *buffer;
	
	if (url == NULL) {
		fprintf(stderr, "%s: NULL pointer dereference detected. Giving up.\n", __FUNCTION__);
		return NULL;
	}
	
	if ( (__url = string2url(url)) == NULL ) {
		fprintf(stderr, "%s: invalid url (\"%s\")", __FUNCTION__, url);
		return NULL;
	}
	
	sd = web_client(__url->website, __url->proto);
	if (sd < 0) {
		fprintf(stderr, "%s: can't communicate with %s\n", __FUNCTION__, __url->website);
		return NULL;
	}
	if (web_send(sd, __url->resource, strlen(__url->resource)) <0 ) {
		fprintf(stderr, "%s: can't get %s\n", __FUNCTION__, __url->resource);
		return NULL;
	}
	
	// get at least BUFFER_RESPONSE bytes
	buffer = malloc(BUFFER_RESPONSE);
	memset(buffer, 0, BUFFER_RESPONSE);
	rb = web_recv(sd, buffer, BUFFER_RESPONSE, GET_TIMEOUT);
	return ret;
}