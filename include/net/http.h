#pragma once

#ifdef __MINGW32__
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/list/dict.h"

struct HttpQuery {
    char *hostname;
    uint16_t status;
    struct Dict *headers;
    struct Blob *body;
};

int http_create_server(char *host, int port);

void http_client_handler(int httpSocketId, void (listener)(int, struct Dict *));

void http_response_string(int clientSocketId, struct Dict *headers, char *data);

struct HttpQuery *http_query_get(char *url);
