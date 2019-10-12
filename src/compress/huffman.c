#include "../../include/compress/huffman.h"

// Bubble sort for huffman tree nodes
void ____huffman_vector_sort(struct Vector *vector) {
    for (size_t i = 0; i < vector->length - 1; ++i) {
        struct HuffmanTreeNode *left = vector->list[i];
        struct HuffmanTreeNode *right = vector->list[i + 1];

        if (left->priority > right->priority) {
            vector->list[i + 1] = vector->list[i];
            vector->list[i] = right;
            i = -1;
            continue;
        }
    }
}

bool ____huffman_search(struct HuffmanTreeNode *node, uint64_t searchValue, uint64_t *depth, uint64_t *temp) {
    // Node with value was found
    if (!node->isNull) {
        if (node->value == searchValue) return true;
        else return false;
    }

    bool status;

    // Check left node
    if (node->left) {
        *depth += 1;
        status = ____huffman_search(node->left, searchValue, depth, temp);

        // Not found in left
        if (!status) *depth -= 1;

        // Found in left
        if (status) return true;
    }

    // Check right node
    if (node->right) {
        // Add 1 to code
        *temp += 1u << *depth;
        *depth += 1;
        status = ____huffman_search(node->right, searchValue, depth, temp);

        // Not found in right
        if (!status) {
            *depth -= 1;
            // Sub 1 from code
            *temp -= 1u << *depth;
        }

        // Found in right
        if (status) return true;
    }

    return false;
}

struct HuffmanResult huffman_compress(const char *buffer, uint64_t contentLength) {
    // Init count table
    uint64_t tableSize = 256;
    uint64_t table[tableSize];
    memset(table, 0, sizeof(table));

    // Count each symbol usage
    for (uint64_t i = 0; i < contentLength; ++i) table[(uint8_t)buffer[i]]++;

    // Fill table
    NEW_VECTOR(XX, struct HuffmanTreeNode *)
    for (uint64_t i = 0; i < tableSize; ++i) {
        if (table[i] != 0) {
            // Add node as value and amount of usage for each symbol
            struct HuffmanTreeNode *node = MEMORY_ALLOCATE_STRUCT(HuffmanTreeNode);
            node->value = (uint8_t)i;
            node->priority = table[i];
            vector_push(XX, node);
        }
    }

    // Sort table by priority
    ____huffman_vector_sort(XX);

    // Table of all used chars
    uint8_t usedChars[256];
    memset(usedChars, 0, sizeof(usedChars));
    size_t usedCharsLength = XX->length;

    // Fill table of all used chars
    for (size_t j = 0; j < XX->length; ++j)
        usedChars[j] = (uint8_t)((struct HuffmanTreeNode *)XX->list[j])->value;

    // Build tree
    for (size_t j = 0; j < XX->length - 1; ++j) {
        // Left right
        struct HuffmanTreeNode *left = XX->list[j];
        struct HuffmanTreeNode *right = XX->list[j + 1];

        // Create new node with both nodes
        struct HuffmanTreeNode *node = MEMORY_ALLOCATE_STRUCT(HuffmanTreeNode);
        node->left = left;
        node->right = right;
        node->priority = left->priority + right->priority;
        node->isNull = true; // This node doesn't contain values
        vector_remove_at(XX, 0, 2);
        vector_insert_at(XX, 0, node);

        if (XX->length <= 1) break;
        // Sort nodes
        ____huffman_vector_sort(XX);
        j = -1;
    }

    // Tables with values and len. For example
    // a = 001  = 3
    // b = 1100 = 4
    uint64_t codeTable[256];
    uint64_t codeLenTable[256];
    memset(codeTable,    0, sizeof(codeTable));
    memset(codeLenTable, 0, sizeof(codeLenTable));

    // Search code for each symbol and put it to the table
    for (size_t k = 0; k < usedCharsLength; ++k)
        ____huffman_search(XX->list[0], usedChars[k],
                &codeLenTable[(uint8_t)usedChars[k]], &codeTable[(uint8_t)usedChars[k]]);

    // Encode data to bit array
    NEW_BITSET(X)
    for (size_t i = 0; i < contentLength; ++i) {
        // Put code to bit array, this function get code and code length
        bitset_put(X, codeTable[(uint8_t)buffer[i]], codeLenTable[(uint8_t)buffer[i]]);
    }

    // Set result
    struct HuffmanResult result;
    result.result = X;
    result.tree = XX->list[0];
    DESTROY_VECTOR(XX)
    return result;
}

struct Blob * huffman_uncompress(struct HuffmanResult result) {
    // Get first node
    struct HuffmanTreeNode *tempNode = result.tree;
    NEW_BLOB(X)

    // Go through each bit
    for (size_t i = 0; i < result.result->length; ++i) {
        //  Check bit to define left or right
        if (BITSET_GET(result.result, i)) tempNode = tempNode->right;
        else tempNode = tempNode->left;

        // Node contain result
        if (!tempNode->isNull) {
            // Put result and reset tree
            blob_put8(X, (uint8_t)tempNode->value);
            tempNode = result.tree;
        }
    }
    return X;
}
