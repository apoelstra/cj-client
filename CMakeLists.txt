CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(CJClient)

FIND_PACKAGE(PkgConfig)

ADD_DEFINITIONS(-Wall -W -Wextra -Werror --std=c99 -pedantic -g)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
PKG_CHECK_MODULES(GTHREAD REQUIRED gthread-2.0)
PKG_CHECK_MODULES(GOBJECT REQUIRED gobject-2.0)
PKG_CHECK_MODULES(LIBCURL REQUIRED libcurl) 
PKG_CHECK_MODULES(JANSSON REQUIRED jansson) 
INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS} ${GTHREAD_INCLUDE_DIRS}
                    ${GOBJECT_INCLUDE_DIRS} ${LIBCURL_INCLUDE_DIRS}
                    ${JANSSON_INCLUDE_DIRS})

ADD_EXECUTABLE(cj-client main.c bitcoin.c buffer.c joiner.c
                         jsonrpc.c gtk-ui.c gtk-coin-selector.c
                         output.c settings.c)
TARGET_LINK_LIBRARIES(cj-client ${GTK3_LIBRARIES} ${GTHREAD_LIBRARIES}
                                ${GOBJECT_LIBRARIES} ${LIBCURL_LIBRARIES}
                                ${JANSSON_LIBRARIES} -lm)

