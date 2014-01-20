
AM_CFLAGS = -Ijansson/src/ $(CJ_CLIENT_CFLAGS)
SUBDIRS = jansson
bin_PROGRAMS = cj-client

JANSSON_LIB=jansson/src/libjansson.la

CJ_CLIENT_H=bitcoin.h	\
        buffer.h	\
        config.h	\
        global.h	\
        gtk-coin-selector.h	\
        gtk-ui.h	\
        gtk-util.h	\
        joiner.h	\
        jsonrpc.h	\
        output.h	\
        settings.h

cj_client_SOURCES=main.c	\
        bitcoin.c	\
        buffer.c	\
        joiner.c	\
        jsonrpc.c	\
        gtk-ui.c	\
        gtk-util.c	\
        gtk-coin-selector.c	\
        output.c	\
        settings.c	\
        $(CJ_CLIENT_H)	\
        $(JANSSON_LIB)

cj_client_LDADD = $(CJ_CLIENT_LIBS) $(JANSSON_LIB)

$(JANSSON_LIB): jansson
	make -C jansson

