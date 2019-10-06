#include "../../include/net/websocket.h"


void websocket_frame(char *input, uint32_t length, uint8_t **out, uint32_t *outLength) {
    uint32_t finalLength = length;
    if (finalLength == 0) finalLength = strnlen(input, UINT32_MAX);

    uint8_t *package = malloc(2 + finalLength);
    package[0] = 0x81; // 0b10000001;
    package[1] = finalLength;
    memcpy(package + 2, input, finalLength);

    *out = package;
    *outLength = finalLength + 2;
}

void websocket_send(int socket, void *data, uint32_t len) {
    // Generate websocket package
    uint8_t *pack;
    uint32_t packLen;
    websocket_frame(data, len, &pack, &packLen);

    // Send package
    send(socket, pack, packLen, 0);

    // Free package
    free(pack);
}

int websocket_handshake(char *headers, char *key) {
    // Client key
    char clientKey[64];
    memset(clientKey, 0, 64);

    // Parse headers
    /*char **keys;
    char **values;
    size_t kvLength;*/
    struct StringArray *kv = chars_split(headers, "\r\n", 0);
    // chars_split_kv(headers, "\r\n", ": ", &keys, &values, &kvLength);

    // Search client key
    /*for (size_t i = 0; i < kvLength; ++i) {
        if (strcmp(keys[i], "Sec-WebSocket-Key") == 0) {
            memcpy(clientKey, values[i], strlen(values[i]));
        }
    }*/

    for (size_t i = 0; i < kv->length; ++i) {
        struct StringArray *tmp = chars_split(kv->list[i]->list, ": ", 0);

        if (tmp->length > 1) {
            //PRINT(tmp->list[1]);
            if (strcmp(tmp->list[0]->list, "Sec-WebSocket-Key") == 0) {
                memcpy(clientKey, tmp->list[1]->list, tmp->list[1]->length);
            }
        }
    }

    // Free keys & values
    //chars_array_free(keys, kvLength);
    //chars_array_free(values, kvLength);

    // Sec-WebSocket-Key not found
    if (!clientKey[0])
        return 0;

    printf("Client key %s\n", clientKey);

    // Calculate hash for handshake
    char outHash[255];
    memset(outHash, 0, 255);
    strcat(outHash, clientKey);
    strcat(outHash, "258EAFA5-E914-47DA-95CA-C5AB0DC85B11");

    // Generate sha1
    // uint8_t results[20];
    EQU_BLOB(results) = sha1_chars(outHash);

    // Generate base64
    char *final = base64_encode(results->list, 20);
    memcpy(key, final, strnlen(final, 2048));
    key[strnlen(final, 2048)] = '\0';

    printf("Key %s\n", key);

    free(final);

    return 1;
}

void websocket_switch_protocol_header(char *handshake, char *out) {
    sprintf(out, WEBSOCKET_SWITCHING_PROTOCOL, handshake);
}

int websocket_switch_protocol(int socket, char *headers) {
    char header[1024];
    char handshake[255];
    int hsFound = websocket_handshake(headers, handshake);
    if (!hsFound)
        return 0;

    websocket_switch_protocol_header(handshake, header);
    send(socket, header, strnlen(header, sizeof(header)), 0);

    return 1;
}

void websocket_parse_frame(char *input, char *out) {
    int opCode = input[0] & 0x0Fu; //0b0001111;
    int isMask = (input[1] & 0x80u) >> 7u; //(input[1] & 0b10000000) >> 7;
    int length = (input[1] & 0x7F); // (input[1] & 0b01111111);
    char mask[4];
    memcpy(&mask, input + 2, 4);

    for (int i = 0; i < length; ++i) {
        input[i + 2 + 4] ^= mask[i % 4];
    }

    input[6 + length] = '\0';

    // printf("opcode %d is mask %d len %d mask %d\n", opCode, isMask, length, mask);
    // printf("data %s\n", input + 2 + 4);

    memcpy(out, input + 6, length + 1);
}
