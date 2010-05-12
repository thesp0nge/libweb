#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "libweb.h"

void __string2url(char *urlstring, url *url) {
	// p is the pointer to url 2 port delimiter, 
	// the ':' between the FQDN and the port number
	char *p;
	
	// r is the pointer to FQDN 2 webresource delimiter, 
	// the '/' character
	char *r;
	char tmp[4096];
	
	memset(tmp, 0, 4096);
	// No proto here. The url string is in the form:
	// fqdn[:port][/resource]
	
	// Since no protocol preamble is there, 
	// if a ':' or a '/' is present it will be the port 
	// and web resource delimiters
	p = strchr(urlstring, ':');
	r = strchr(urlstring, '/');
	if ( p == NULL) { 
		if ( r == NULL ) {
			// urlstring = www.website.com
			memcpy(url->website, urlstring, strlen(urlstring));
			strcpy(url->resource, "/");
		} else {
			// urlstring = www.website.com/resource
			memcpy(url->website, urlstring, strlen(urlstring) - strlen(r));
			if (strlen(r) == 1) 
				strcpy(url->resource, "/");
			else
				strcpy(url->resource, r + (1 * sizeof(char)));
		}
		url->port = 80;
	} else {
		if (r == NULL) {
			// urlstring = www.website.com:port
			memcpy(tmp, (p+1 * sizeof(char)), strlen(urlstring));
			url->port = atoi(tmp);
			strcpy(url->resource, "/");
		} else {
			// urlstring = www.website.com:port/resource
			memcpy(tmp, (p+1 * sizeof(char)), strlen(urlstring) - strlen(r));
			url->port = atoi(tmp);
			memcpy(url->resource, r + (1 * sizeof(char)), strlen(r));
		}
		memcpy(url->website, urlstring, strlen(urlstring) - strlen(p));
	}
	
	return ;
}
/*
 * Take a string and tokenize it trying to extract url information.
 * If the parameter string is not a good url, a NULL value will be returned.
 *
 * @param urlstring the string containing a url to call
 * @return a url information or NULL if an error occured.
 */
url *string2url(char *urlstring) {
	url *ret;

	char *p_string;
	
	
	char tmp[4096];
	
	if (urlstring == NULL) {
		fprintf(stderr, "%s: NULL pointer dereference detected. Giving up.\n", __FUNCTION__);
		return NULL;
	}
	if (strcmp(urlstring, "") == 0) {
		fprintf(stderr, "%s: an empty string is not a good URL\n", __FUNCTION__);
		return NULL;
	}
	
	// Ok, it seems that we've a real string to process.
	// I expect a string in the form:
	//		[proto://]fqdn[:port][/resource]
	//
	// Note that also resource is optional, and we will force
	// the website root ('/') in that case.
	// Also proto can be optional, in this case proto will be 
	// forced to http and port number to 80.
	
	ret = malloc(sizeof(url));
	if (ret == NULL) {
		fprintf(stderr, "%s: malloc error (%s)\n", __FUNCTION__,  strerror(errno));
		return NULL;
	}
	memset(ret, 0, sizeof(url));
	
	
	if ((p_string = strstr(urlstring, "://")) == NULL) {
		__string2url(urlstring, ret);
		strcpy(ret->proto, "http");
	} else {
		strcpy(tmp, p_string + 3 *sizeof(char));
		__string2url(tmp, ret);
		memcpy(ret->proto, urlstring, strlen(urlstring)-strlen(p_string));
	}
#ifdef REJECT_NONWEB_PROTOCOLS
	if (strcmp(ret->proto, "http") != 0 && strcmp(ret->proto, "https") != 0)
		return NULL;
#endif
	return ret;
}