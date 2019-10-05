#pragma once

#ifdef __MINGW32__
#else
#include <sys/socket.h>
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "../../include/string/string.h"
#include "../../include/crypt/sha1.h"
#include "../../include/string/base64.h"

#define WEBSOCKET_SWITCHING_PROTOCOL "HTTP/1.1 101 Switching Protocols\r\n" \
"Upgrade: websocket\r\n" \
"Connection: Upgrade\r\n" \
"Sec-WebSocket-Accept: %s \r\n" \
"\r\n"

void websocket_frame(char *input, uint32_t length, uint8_t **out, uint32_t *outLength);

void websocket_send(int socket, void *data, uint32_t len);

int websocket_handshake(char *headers, char *key);

void websocket_switch_protocol_header(char *handshake, char *out);

int websocket_switch_protocol(int socket, char *headers);

void websocket_parse_frame(char *input, char *out);
