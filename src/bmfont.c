#include <omega/bmfont.h>

#if OMG_SUPPORT_BMFONT
bool omg_bmfont_destroy(OMG_Bmfont* this) {
    this->page = NULL;
    this->chars_count = 0;
    return false;
}

bool omg_bmfont_init(OMG_Bmfont* this, OMG_Texture* page, OMG_Renderer* ren) {
    this->ren = ren;
    this->page = page;
    this->chars_count = 0;
    this->size = 0;
    this->line_height = 0;
    this->base = 0;
    this->scale_w = 0;
    this->scale_h = 0;
    this->bold = false;
    this->italic = false;
    this->unicode = false;
    this->smooth = false;
    this->aa = false;
    return false;
}
#endif
