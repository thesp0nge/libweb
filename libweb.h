#ifndef __LIBWEB_H

#define __LIBWEB_H
#define LIBWEB_ERROR	-1;
#define LIBWEB_TIMEOUT	-2;

#define HELO				"GET / HTTP\\1.0\n\n"
#define BUFFER_RESPONSE		524288
#define BUFFER_LINE_SIZE	80


#define HTTP_OK				200
#define HTTP_MISSING		404

typedef struct hr {
	int code;
	int rtt; // round trip type - useful for HTTP pinging
	char date[BUFFER_LINE_SIZE];
	char server[BUFFER_LINE_SIZE];
	char response[BUFFER_RESPONSE];
} http_response;

/*
	TODO: check the RFC for max website length 
*/
typedef struct uu {
	char proto[10];
	char website[512];
	int port;
	char resource[8192];
} url;
	
static int known_port[]={80, 8080, 8008, 8009, 8010};
static int last_http_code;

// high level APIs
http_response *web_get(char *url);

url *string2url(char *urlstring);

// low level APIs
int web_client(char *hostname, unsigned int port);
int web_recv(int sd, void *buffer, size_t len, int to);
int web_send(int sd, void *buffer, size_t len);
int web_helo(int sd, char *buffer, size_t len);
int web_http_code(char *buffer);
#endif //#ifndef __LIBWEB_H