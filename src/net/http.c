#include <arpa/inet.h>
#include <netdb.h>
#include "../../include/net/http.h"
#include "../../include/debug/logger.h"

#define CHECK_SOCKET_ERROR(X, MSG) if ((X) == -1) { LOGGER_HALT(__FILE__, __LINE__, MSG); }

int http_create_server(char *host, int port) {
    // Create socket
    int listenSocketId = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_SOCKET_ERROR(listenSocketId, "Could not create TCP listening socket")

    // Resolve host name
    struct hostent *ipAddr = gethostbyname(host);
    if (ipAddr == 0) LOGGER_HALT(__FILE__, __LINE__, "Can't resolve hostname '%s'", host)
    struct in_addr **addrList = (struct in_addr **) ipAddr->h_addr_list;

    // Set socket type and ip address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, inet_ntoa(*addrList[0]), &(addr.sin_addr));

    // Bind socket
    int bindStatus = bind(listenSocketId, (struct sockaddr *) &addr, sizeof(addr));
    CHECK_SOCKET_ERROR(bindStatus, "Could not bind")

    // Listen socket
    int listenStatus = listen(listenSocketId, 100);
    CHECK_SOCKET_ERROR(listenStatus, "Could not listen")

    return listenSocketId;
}

void http_client_handler(int httpSocketId, void (listener)(int, struct Dict *)) {
    for (;;) {
        // Accept client
        int clientSocketId = accept(httpSocketId, NULL, NULL);
        if (clientSocketId == -1) continue;

        // Read headers
        char buffer[2048];
        int readBytes = recv(clientSocketId, buffer, 2048, 0);

        // Empty
        if (readBytes == 0) continue;

        // Have data
        if (readBytes > 0) {
            buffer[readBytes] = '\0';
            puts(buffer);

            // Headers dict
            NEW_DICT(headers, char *)

            // Split line
            EQU_STRARR(headersTuple) = chars_split(buffer, "\r\n", 0);
            for (size_t i = 0; i < headersTuple->length; ++i) {
                // Split header
                EQU_STRARR(lineTuple) = chars_split(headersTuple->list[i]->list, ": ", 1);
                if (lineTuple->length > 1)
                    dict_add(headers, lineTuple->list[0]->list, chars_clone(lineTuple->list[1]->list));
                DESTROY_STRING_ARRAY(lineTuple)
            }

            listener(clientSocketId, headers);
            for (size_t j = 0; j < headers->length; ++j)
                MEMORY_FREE(headers->list[j]);
            DESTROY_DICT(headers)
            DESTROY_STRING_ARRAY(headersTuple)

            MEMORY_PRINT_STATE
        }
    }
}

void http_response_string(int clientSocketId, struct Dict *headers, char *data) {
    // Header
    NEW_STRING(X)
    string_put(X, "HTTP/1.1 200 OK\r\n");

    // Set headers
    if (headers)
        for (size_t i = 0; i < headers->length; ++i)
            string_add(X, "%s: %s\r\n", headers->keys[i], (char *) headers->list[i]);

    // Set default headers
    string_put(X, "Connection: close\r\n");
    string_put(X, "Content-Length: %zu\r\n");
    string_put(X, "\r\n%s");

    // Content
    NEW_STRING(X1)
    string_put(X1, data);

    // Final
    NEW_STRING(FINAL)
    string_add(FINAL, X->list, X1->length, X1->list);

    // Send
    send(clientSocketId, FINAL->list, FINAL->length, 0);

    // Remove strings
    DESTROY_STRING(X)
    DESTROY_STRING(X1)
    DESTROY_STRING(FINAL)
}

struct HttpQuery *http_query_get(char *url) {
    EQU_STRARR(X) = chars_split(url, "/", 0);
    char *hostname = X->list[2]->list;
    int port = 80;

    // Resolve host name
    struct hostent *ipAddr = gethostbyname(hostname);
    if (ipAddr == 0) LOGGER_HALT(__FILE__, __LINE__, "Can't resolve hostname '%s'", hostname)
    struct in_addr **addrList = (struct in_addr **) ipAddr->h_addr_list;

    // Set socket type and ip address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, inet_ntoa(*addrList[0]), &(addr.sin_addr));

    // Create socket
    int requestSocketId = socket(AF_INET, SOCK_STREAM, 0);
    CHECK_SOCKET_ERROR(requestSocketId, "Could not create TCP listening socket")

    // Connect to server
    int connectStatus = connect(requestSocketId, (struct sockaddr *) &addr, sizeof(addr));
    if (connectStatus < 0) LOGGER_HALT(__FILE__, __LINE__, "Can't connect to hostname '%s'", hostname)

    // Required path
    string_array_remove_at(X, 0, 3);
    EQU_STR(path) = string_array_join(X, "/");

    // Request string
    NEW_STRING(X1)
    string_add(X1, "GET /%s HTTP/1.1\r\n", path->list);
    string_add(X1, "Connection: close\r\n", path->list);
    string_add(X1, "Host: %s\r\n\r\n", hostname);

    // Send request
    send(requestSocketId, X1->list, X1->length, 0);

    NEW_BLOB(responseBody)

    char recvBuff[2048];
    int n = read(requestSocketId, recvBuff, sizeof(recvBuff) - 1);
    recvBuff[n] = 0;
    size_t startOfHeader = 0;

    // Search headers
    for (ssize_t i = 0; i < n; ++i) {
        if (strncmp(recvBuff + i, "\r\n\r\n", 4) == 0) {
            startOfHeader = i + 4;
            blob_put_bytes(responseBody, (uint8_t *)recvBuff + startOfHeader, -1, n - startOfHeader);
            break;
        }
    }

    // Get rest
    while ((n = read(requestSocketId, recvBuff, sizeof(recvBuff) - 1)) > 0) {
        recvBuff[n] = 0;
        blob_put_bytes(responseBody, (uint8_t *)recvBuff, -1, n);
    }

    struct HttpQuery *httpQuery = MEMORY_ALLOCATE_STRUCT(HttpQuery);
    httpQuery->body = responseBody;

    return httpQuery;
}
