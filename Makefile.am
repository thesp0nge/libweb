ACLOCAL_AMFLAGS=-I m4
lib_LTLIBRARIES=libweb.la
libweb_la_SOURCES=libweb.c get.c commons.c log.c libweb.h log.h
libweb_la_CCFLAGS=-shared

bin_PROGRAMS=libweb_test
libweb_test_SOURCES=libweb_test.c
libweb_test_LDFLAGS=-lweb