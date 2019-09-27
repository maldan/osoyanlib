#pragma once

#include "../memory/memory.h"
#include "../string/string.h"
#include "../string/wstring.h"
#include "../string/wchars.h"
#include "../list/map.h"
#include "../list/dict.h"
#include "../util/color.h"
#include "../io/print.h"

struct TermUiDocument {
    EQU_MAP(idList);
    EQU_VECTOR(styleNodes);
    struct TermUiNode *root;
    wchar_t *buffer;
    uint8_t *paramBuffer;
    uint16_t width;
    uint16_t height;
};

struct TermUiNode {
    EQU_STRING(id);
    EQU_STRING(blockType);
    EQU_DICT(styles);
    EQU_DICT(attributes);
    EQU_WSTRING(content);
    EQU_VECTOR(nodes);
    uint16_t x;
    uint16_t y;
    uint16_t width;
    uint16_t height;
    uint16_t skipLines;
};

struct TermUiStyleNode {
    EQU_STRING(color);
    EQU_STRING(background);
    EQU_STRING(bold);
};

struct TermUiStyleNode * termui_new_style_node();
struct TermUiNode * termui_new_node();
void termui_parse_styles(struct TermUiNode *node, char *styleList);
struct TermUiDocument * termui_parse(struct WString *content);
struct TermUiDocument * termui_parse_file(char *path);

struct String * termui_css_color_to_xterm_background(char *color);
struct String * termui_css_color_to_xterm_color(char *color);
void termui_render(struct TermUiDocument *node);

struct TermUiNode *termui_get_node(struct TermUiDocument *document, char *id);

void termui_set_css(struct TermUiNode *node, char *key, char *value);
void termui_set_content(struct TermUiNode *node, char *content);
