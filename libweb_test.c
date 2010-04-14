#include <stdio.h>
#include <stdlib.h>
#include <string.h> // memset()
#include "config.h"
#include "libweb.h"

int main(int argc, char *argv[]) {
	int sd, l;
	char buffer[8192];
	fprintf(stdout, "%s v%s\n", PACKAGE_NAME, PACKAGE_VERSION);
	if ((sd = web_client("localhost", 80)) == -1) {
		return -1;
	}
	fprintf(stdout, "%s: sending HELO... ", PACKAGE_NAME);
	fflush(stdout);
	memset(buffer, 0, 8192);
	if ((l = web_helo(sd, buffer, 8192)) < 0)
		return -1;
	fprintf(stdout, "done.\n");
	printf("%s (%d)\n", buffer, l);
	printf("Ret code: %d\n", web_http_code(buffer));
	strcpy(buffer, "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
	printf("Ret code should be -1 now: %d\n", web_http_code(buffer));
	
	return 0;
}