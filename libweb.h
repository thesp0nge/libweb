#ifndef __LIBWEB_H

#define __LIBWEB_H
#define LIBWEB_ERROR	-1;
#define LIBWEB_TIMEOUT	-2;


static int known_port[]={80, 8080, 8008, 8009, 8010};
static int last_http_code;

int web_client(char *hostname, unsigned int port);
int web_recv(int sd, void *buffer, size_t len, int to);
int web_send(int sd, void *buffer, size_t len);
int web_helo(int sd, char *buffer, size_t len);
int web_http_code(char *buffer);
#endif //#ifndef __LIBWEB_H