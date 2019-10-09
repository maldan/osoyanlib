#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "../../include/list/vector.h"
#include "../../include/list/bitset.h"
#include "../../include/list/blob.h"

struct HuffmanResult {
    struct HuffmanTreeNode *tree;
    struct BitSet *result;
};

struct HuffmanTreeNode {
    uint64_t priority;
    struct HuffmanTreeNode * left;
    struct HuffmanTreeNode * right;
    uint64_t value;
    bool isNull;
};

struct HuffmanResult huffman_compress(const char *buffer, uint64_t contentLength);
struct Blob * huffman_uncompress(struct HuffmanResult result);
