#pragma once

#include <wchar.h>
#include "../geom/rect.h"
#include "../string/string.h"
#include "../string/wstring.h"
#include "../list/dict.h"
#include "../list/vector.h"
#include "../list/map.h"

#define TERMUI_RENDER_NO_OUTPUT 1

struct TermUiDocument {
    EQU_MAP(idList);
    EQU_VECTOR(styleNodes);
    struct TermUiNode *root;
    wchar_t *screenBuffer;
    uint32_t *paramBuffer;
    struct Rectangle screen;
    bool isTerminalOutput;
};

struct TermUiNode {
    // Base
    EQU_STRING(id);
    EQU_STRING(elementType);

    // Params
    EQU_DICT(styles);
    EQU_DICT(attributes);
    struct Rectangle area;
    uint16_t contentOffsetY;

    char testChar;

    // Text content
    EQU_WSTRING(content);

    // Children
    EQU_VECTOR(nodes);
};

struct TermUiDocument *termui_create_document();
struct TermUiNode *termui_create_node();

struct TermUiNode *termui_get_node(struct TermUiDocument *document, char *id);

void termui_render(struct TermUiDocument *document, uint8_t params);

void termui_destroy_node(struct TermUiNode *node);
void termui_destroy_document(struct TermUiDocument *document);
