#include "../../include/geom/rect.h"

struct Rectangle rect_add(struct Rectangle r1, struct Rectangle r2) {
    r1.x += r2.x;
    r1.y += r2.y;
    r1.width += r2.width;
    r1.height += r2.height;
    return r1;
}
