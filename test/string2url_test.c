#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "libweb.h"

void dump(url *u) {
	fprintf(stdout, "w:%s\np:%d\nr:%s\npp:%s\n", u->website, u->port, u->resource, u->proto);
	return ;
}
int main(void) {
	int passed = 0;
	int failed = 0;
	url *u;
	fclose(stderr);
	fprintf(stdout, "%s v%s: test tool for string2url()\n", PACKAGE_NAME, PACKAGE_VERSION);
	fprintf(stdout, "testing string2url(NULL)...");
	if (string2url(NULL) == NULL) {
		fprintf(stdout, " passed.\n");
		passed++;
	} else {
		fprintf(stdout, " failed.\n");
		failed++;
		
	}
	
	fprintf(stdout, "testing string2url(\"\")...");
	if (string2url("") == NULL) {
		fprintf(stdout, " passed.\n");
		passed++;
	} else {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	
	fprintf(stdout, "testing string2url(\"www.google.com\")...");
	u = string2url("www.google.com");
	if (u == NULL) {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "/") == 0 &&
		u->port == 80) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		failed++;
		dump(u);
	}
	free(u);
	fprintf(stdout, "testing string2url(\"www.google.com/a_resource_here\")...");
	u = string2url("www.google.com/a_resource_here");
	if (u == NULL) {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "a_resource_here") == 0 &&
		u->port == 80) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		
		failed++;
		dump(u);
	}
	free(u);
	fprintf(stdout, "testing string2url(\"www.google.com:82\")...");
	u = string2url("www.google.com:82");
	if (u == NULL) {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "/") == 0 &&
		u->port == 82) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		
		failed++;
		dump(u);
	}
	free(u);
	fprintf(stdout, "testing string2url(\"www.google.com:82/a_resource_here\")...");
	u = string2url("www.google.com:82/a_resource_here");
	if (u == NULL) {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "a_resource_here") == 0 &&
		u->port == 82) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		
		failed++;
		dump(u);
	}
	
	// url with HTTP proto
	free(u);
	fprintf(stdout, "testing string2url(\"http://www.google.com\")...");
	u = string2url("http://www.google.com");
	if (u == NULL) {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "/") == 0 &&
		u->port == 80 &&
		strcmp(u->proto, "http") == 0) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		failed++;
		dump(u);
	}
	free(u);
	fprintf(stdout, "testing string2url(\"http://www.google.com/a_resource_here\")...");
	u = string2url("http://www.google.com/a_resource_here");
	if (u == NULL) {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "a_resource_here") == 0 &&
		u->port == 80 &&
		strcmp(u->proto, "http") == 0) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		
		failed++;
		dump(u);
	}
	free(u);
	fprintf(stdout, "testing string2url(\"http://www.google.com:82\")...");
	u = string2url("http://www.google.com:82");
	if (u == NULL) {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "/") == 0 &&
		u->port == 82 &&
		strcmp(u->proto, "http") == 0) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		
		failed++;
		dump(u);
	}
	free(u);
	fprintf(stdout, "testing string2url(\"http://www.google.com:82/a_resource_here\")...");
	u = string2url("http://www.google.com:82/a_resource_here");
	if (u == NULL) {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "a_resource_here") == 0 &&
		u->port == 82 &&
		strcmp(u->proto, "http") == 0) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		
		failed++;
		dump(u);
	}
	
	// url with HTTPS proto
	free(u);
	fprintf(stdout, "testing string2url(\"https://www.google.com\")...");
	u = string2url("https://www.google.com");
	if (u == NULL) {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "/") == 0 &&
		u->port == 80 &&
		strcmp(u->proto, "https") == 0) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		failed++;
		dump(u);
	}
	free(u);
	fprintf(stdout, "testing string2url(\"https://www.google.com/a_resource_here\")...");
	u = string2url("https://www.google.com/a_resource_here");
	if (u == NULL) {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "a_resource_here") == 0 &&
		u->port == 80 &&
		strcmp(u->proto, "https") == 0) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		
		failed++;
		dump(u);
	}
	free(u);
	fprintf(stdout, "testing string2url(\"https://www.google.com:82\")...");
	u = string2url("https://www.google.com:82");
	if (u == NULL) {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "/") == 0 &&
		u->port == 82 &&
		strcmp(u->proto, "https") == 0) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		
		failed++;
		dump(u);
	}
	free(u);
	fprintf(stdout, "testing string2url(\"https://www.google.com:82/a_resource_here\")...");
	u = string2url("https://www.google.com:82/a_resource_here");
	if (u == NULL) {
		fprintf(stdout, " failed.\n");
		failed++;
	}
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "a_resource_here") == 0 &&
		u->port == 82 &&
		strcmp(u->proto, "https") == 0) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		
		failed++;
		dump(u);
	}
	
	// testing a nor HTTP nor HTTPS protocol
	free(u);
	fprintf(stdout, "testing string2url(\"ftp://www.google.com:82/a_resource_here\")...");
	u = string2url("ftp://www.google.com:82/a_resource_here");
#ifdef REJECT_NONWEB_PROTOCOLS
	if (u == NULL) {
		fprintf(stdout, " passed.\n");
		passed++;
	} else {
		fprintf(stdout, " failed.\n");
		failed++;
		dump(u);
	}
#else
	if (strcmp(u->website, "www.google.com") == 0 && 
		strcmp(u->resource, "a_resource_here") == 0 &&
		u->port == 82 &&
		strcmp(u->proto, "ftp") == 0) {
			fprintf(stdout, " passed.\n");
			passed++;
	} else {
		fprintf(stdout, " failed.\n");
		failed++;
		dump(u);
	}
#endif
	fprintf(stdout, "%.2f%% test passed\n", (100.0*passed)/(passed+failed));
	return 0;
}