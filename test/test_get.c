// [The "BSD licence"]
//
// test_get.c - web_get() test program
//
// test_get is part of libweb library
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
#include <assert.h>

#include "libweb.h"

int main(int argc, char **argv) {
	http_response *res = NULL;
	
	
	fprintf(stderr, "call web_get() over existing localhost root...");
	res = web_get("http://localhost/");
	
	if (res == NULL) {
		fprintf(stderr, "received NULL form server\n");
		return -1;
	}
	fprintf(stdout, "return code is %d\n", res->code);
	if (!strcmp(res->server, NO_SERVER_HEADER))
		fprintf(stdout, "server banner is: %s\n", res->server);
	else 
		fprintf(stdout, "no server banner\n");
	fprintf(stdout, "got this from server\n%s\n", res->response);
	
	return 0;
	
}