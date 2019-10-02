#include "../../include/ui/termui.h"
#include "../../include/io/print.h"

// Create
struct TermUiDocument *termui_create_document() {
    struct TermUiDocument *document = MEMORY_ALLOCATE_STRUCT(TermUiDocument);
    INIT_MAP(document->idList, struct TermUiNode *)
    INIT_VECTOR(document->styleNodes, struct TermUiNode *)

    // Create root node
    struct TermUiNode *rootNode = termui_create_node();
    string_put(rootNode->elementType, "root");
    document->root = rootNode;

    document->screenBuffer = NULL;
    document->paramBuffer = NULL;

    return document;
}

struct TermUiNode *termui_create_node() {
    struct TermUiNode *node = MEMORY_ALLOCATE_STRUCT(TermUiNode);

    INIT_STRING(node->id, "")
    INIT_STRING(node->elementType, "")
    INIT_DICT(node->styles)
    INIT_DICT(node->attributes)
    INIT_WSTRING(node->content, "")
    INIT_VECTOR(node->nodes, struct TermUiNode *)

    return node;
}

// Render
void ____termui_prepare_document(struct TermUiDocument *document) {
    // Get terminal size
    struct winsize winSize = console_get_window_size();

    // Allocate screen buffer if not exists
    if (!document->screenBuffer)
        document->screenBuffer = MEMORY_ALLOCATE(
                winSize.ws_col *
                winSize.ws_row *
                sizeof(document->screenBuffer[0]));

    // Allocate param buffer if not exists
    if (!document->paramBuffer)
        document->paramBuffer = MEMORY_ALLOCATE(
                winSize.ws_col *
                winSize.ws_row *
                sizeof(document->paramBuffer[0]));

    // If terminal changed the screen size
    if (document->screen.width != winSize.ws_col || document->screen.height != winSize.ws_row) {
        document->screen.width = winSize.ws_col;
        document->screen.height = winSize.ws_row;

        // Reallocate screen & param buffers
        document->screenBuffer = MEMORY_REALLOCATE(
                document->screenBuffer,
                document->screen.width * document->screen.height * sizeof(document->screenBuffer[0]));
        document->paramBuffer = MEMORY_REALLOCATE(
                document->paramBuffer,
                document->screen.width * document->screen.height * sizeof(document->paramBuffer[0]));
    }
}

void ____termui_calc_layout(struct TermUiDocument *document, struct TermUiNode *node, struct Rectangle area) {
    // Set node area
    node->area = area;

    int paddingLeft = dict_get_int(node->styles, "padding-left");
    int paddingRight = dict_get_int(node->styles, "padding-right");
    int paddingTop = dict_get_int(node->styles, "padding-top");
    int paddingBottom = dict_get_int(node->styles, "padding-bottom");

    // Block params
    int flexMaxWidthArea = area.width - paddingLeft - paddingRight;
    int flexMaxHeightArea = area.height - paddingTop - paddingBottom;
    int amountOfBlocks = node->nodes->length;
    int totalBlockOffsetX = 0;
    int totalBlockOffsetY = 0;
    bool isRowDirection = true;
    if (dict_value_is(node->styles, "flex-direction", "column")) {
        isRowDirection = false;
    }

    // Child block current width and offset
    int16_t blockWidth[node->nodes->length];
    int16_t blockHeight[node->nodes->length];
    int16_t blockOffsetX[node->nodes->length + 1];
    int16_t blockOffsetY[node->nodes->length + 1];
    bzero(blockWidth, node->nodes->length);
    bzero(blockHeight, node->nodes->length);
    bzero(blockOffsetX, node->nodes->length);
    bzero(blockOffsetY, node->nodes->length);

    // Calculate child block
    for (size_t k = 0; k < node->nodes->length; ++k) {
        struct TermUiNode *tempNode = node->nodes->list[k];

        // Child block width
        if (dict_has_key(tempNode->styles, "width")) {
            blockWidth[k] = dict_get_int(tempNode->styles, "width");

            if (isRowDirection) {
                amountOfBlocks -= 1;
                flexMaxWidthArea -= blockWidth[k];
            }
        }

        // Child block height
        if (dict_has_key(tempNode->styles, "height")) {
            blockHeight[k] = dict_get_int(tempNode->styles, "height");

            if (isRowDirection) {

            } else {
                amountOfBlocks -= 1;
                flexMaxHeightArea -= blockHeight[k];
            }
        }

        // Child block margin
        if (isRowDirection) {
            flexMaxWidthArea -= dict_get_int(tempNode->styles, "margin-left");
            flexMaxWidthArea -= dict_get_int(tempNode->styles, "margin-right");
        } else {
            flexMaxHeightArea -= dict_get_int(tempNode->styles, "margin-top");
            flexMaxHeightArea -= dict_get_int(tempNode->styles, "margin-bottom");
        }
    }

    // Calculate flex child block
    for (size_t k = 0; k < node->nodes->length; ++k) {
        struct TermUiNode *tempNode = node->nodes->list[k];

        if (isRowDirection) {
            // Child block width
            if (!dict_has_key(tempNode->styles, "width"))
                blockWidth[k] = flexMaxWidthArea / amountOfBlocks;

            // Child block height
            if (!dict_has_key(tempNode->styles, "height"))
                blockHeight[k] = flexMaxHeightArea;
        } else {
            // Child block width
            if (!dict_has_key(tempNode->styles, "width"))
                blockWidth[k] = flexMaxWidthArea;

            // Child block height
            if (!dict_has_key(tempNode->styles, "height"))
                blockHeight[k] = flexMaxHeightArea / amountOfBlocks;
        }
    }

    // Calculate final offset X
    totalBlockOffsetX = paddingLeft;
    for (size_t k = 0; k < node->nodes->length; ++k) {
        struct TermUiNode *tempNode = node->nodes->list[k];

        if (isRowDirection) {
            totalBlockOffsetX += dict_get_int(tempNode->styles, "margin-left");
            blockOffsetX[k] = totalBlockOffsetX;
            totalBlockOffsetX += blockWidth[k] + dict_get_int(tempNode->styles, "margin-right");
        } else {
            blockOffsetX[k] = dict_get_int(tempNode->styles, "margin-left") + paddingLeft;
        }
    }

    // Calculate final offset Y
    totalBlockOffsetY = paddingTop;
    for (size_t k = 0; k < node->nodes->length; ++k) {
        struct TermUiNode *tempNode = node->nodes->list[k];

        if (isRowDirection) {
            blockOffsetY[k] = dict_get_int(tempNode->styles, "margin-top") + paddingTop;
        } else {
            totalBlockOffsetY += dict_get_int(tempNode->styles, "margin-top");
            blockOffsetY[k] = totalBlockOffsetY;
            totalBlockOffsetY += blockHeight[k] + dict_get_int(tempNode->styles, "margin-bottom");
        }
    }

    // Calculate child block
    for (size_t k = 0; k < node->nodes->length; ++k) {
        struct Rectangle rect = {blockOffsetX[k] + area.x, blockOffsetY[k] + area.y, blockWidth[k], blockHeight[k]};
        ____termui_calc_layout(document, node->nodes->list[k], rect);
    }
}

void ____termui_render_layout(struct TermUiDocument *document, struct TermUiNode *node) {
    for (int j = 0; j < node->area.height; ++j) {
        for (int i = 0; i < node->area.width; ++i) {
            // Calculate position
            size_t position = (node->area.x + i) % document->screen.width + (node->area.y + j) % document->screen.height * document->screen.width;
            document->screenBuffer[position] = node->testChar;
        }
    }

    for (size_t k = 0; k < node->nodes->length; ++k) {
        ____termui_render_layout(document, node->nodes->list[k]);
    }
}

void termui_render(struct TermUiDocument *document) {
    // Create buffers, check if resized etc
    ____termui_prepare_document(document);

    // Calculate nodes
    ____termui_calc_layout(document, document->root, document->screen);

    ____termui_render_layout(document, document->root);

    NEW_WSTRING(finalString)
    wstring_put(finalString, "\x1b[H");
    wstring_put(finalString, ANSI_HIDE_CURSOR);

    for (int j = 0; j < document->screen.height; ++j) {
        for (int i = 0; i < document->screen.width; ++i) {
            // printf("%d\n", document->screenBuffer[i % document->screen.width + j % document->screen.height * document->screen.width]);
            wstring_put_wchar(finalString, document->screenBuffer[i % document->screen.width + j % document->screen.height * document->screen.width]);
        }
    }

    // Print on screen
    wstring_put(finalString, ANSI_SHOW_CURSOR);

    fputws(finalString->list, stdout);
    fflush(stdout);

    DESTROY_WSTRING(finalString)
}

// Destroy node recursively
void termui_destroy_node(struct TermUiNode *node) {
    DESTROY_STRING(node->id)
    DESTROY_STRING(node->elementType)
    DESTROY_DICT(node->styles)
    DESTROY_DICT(node->attributes)
    DESTROY_WSTRING(node->content)

    // Destroy children
    for (size_t i = 0; i < node->nodes->length; ++i) {
        termui_destroy_node(node->nodes->list[i]);
    }

    DESTROY_VECTOR(node->nodes)
    MEMORY_FREE(node)
}

// Destroy document with nodes
void termui_destroy_document(struct TermUiDocument *document) {
    DESTROY_MAP(document->idList)
    DESTROY_VECTOR(document->styleNodes)

    if (document->screenBuffer) MEMORY_FREE(document->screenBuffer)
    if (document->paramBuffer) MEMORY_FREE(document->paramBuffer)
    termui_destroy_node(document->root);

    MEMORY_FREE(document)
}
