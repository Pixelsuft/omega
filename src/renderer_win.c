#include <omega/renderer_win.h>

#if OMG_SUPPORT_WIN
#include <omega/omega.h>
#include <omega/texture_win.h>
#define base ((OMG_Renderer*)this)
#define tex_base ((OMG_Texture*)tex)
#define win_base ((OMG_Window*)base->win)
#define omg_base ((OMG_Omega*)base->omg)
#define _OMG_WIN_OMG_RGB(col) (RGB( \
    (int)((col)->r * (omg_color_t)255 / OMG_MAX_COLOR),\
    (int)((col)->g * (omg_color_t)255 / OMG_MAX_COLOR),\
    (int)((col)->b * (omg_color_t)255 / OMG_MAX_COLOR) \
))
#define OMG_NO_SCALE() ((base->scale.x == 1.0f) && (base->scale.y == 1.0f))

// TODO: fix transparency

void omg_renderer_win_update_scale(OMG_RendererWin* this) {
    if (omg_base->support_highdpi) {
        base->dpi_scale.x = win_base->scale.w;
        base->dpi_scale.y = win_base->scale.y;
    }
    base->size.w = win_base->size.w * base->dpi_scale.x;
    base->size.h = win_base->size.h * base->dpi_scale.y;
}

bool omg_renderer_win_destroy(OMG_RendererWin* this) {
    if (OMG_ISNOTNULL(this->hwdc)) {
        this->u32->ReleaseDC(this->hwnd, this->hwdc);
        this->hwdc = this->cur_hwdc = NULL;
    }
    this->hpdc = this->cur_hpdc = NULL;
    return false;
}

bool omg_renderer_win_begin(OMG_RendererWin* this) {
    this->cur_hpdc = this->hpdc = this->u32->BeginPaint(this->hwnd, &this->ps);
    return OMG_ISNULL(this->hpdc);
}

bool omg_renderer_win_clear(OMG_RendererWin* this, const OMG_Color* col) {
    HBRUSH brush = this->g32->CreateSolidBrush(_OMG_WIN_OMG_RGB(col));
    // RECT fill_rect = { .left = 0, .top = 0, .right = (LONG)base->size.w, .bottom = (LONG)base->size.h };
    // this->u32->FillRect(this->cur_hwdc, &fill_rect, brush);
    this->g32->SelectObject(this->cur_hpdc, brush);
    this->g32->Rectangle(this->cur_hpdc, -1, -1, (int)base->size.w + 2, (int)base->size.h + 2);
    this->g32->DeleteObject(brush);
    return false;
}

bool omg_renderer_win_draw_rect(OMG_RendererWin* this, const OMG_FRect* rect, const OMG_Color* col) {
    HBRUSH brush = this->g32->CreateSolidBrush(_OMG_WIN_OMG_RGB(col));
    if (OMG_ISNULL(brush))
        return true;
    RECT w_rect = {
        .left = (LONG)((rect->x + base->offset.x) * base->scale.x),
        .top = (LONG)((rect->y + base->offset.y) * base->scale.y),
        .right = (LONG)((rect->w + rect->x + base->offset.x) * base->scale.x),
        .bottom = (LONG)((rect->h + rect->y + base->offset.y) * base->scale.y)
    };
    bool res = !this->u32->FrameRect(this->cur_hpdc, &w_rect, brush);
    this->g32->DeleteObject(brush);
    return res;
}

bool omg_renderer_win_fill_rect(OMG_RendererWin* this, const OMG_FRect* rect, const OMG_Color* col) {
    HBRUSH brush = this->g32->CreateSolidBrush(_OMG_WIN_OMG_RGB(col));
    if (OMG_ISNULL(brush))
        return true;
    RECT w_rect = {
        .left = (LONG)((rect->x + base->offset.x) * base->scale.x),
        .top = (LONG)((rect->y + base->offset.y) * base->scale.y),
        .right = (LONG)((rect->w + rect->x + base->offset.x) * base->scale.x),
        .bottom = (LONG)((rect->h + rect->y + base->offset.y) * base->scale.y)
    };
    bool res = !this->u32->FillRect(this->cur_hpdc, &w_rect, brush);
    this->g32->DeleteObject(brush);
    return res;
}

bool omg_renderer_win_draw_line(OMG_RendererWin* this, const OMG_FRect* start_end, const OMG_Color* col) {
    HBRUSH brush = this->g32->CreateSolidBrush(_OMG_WIN_OMG_RGB(col));
    if (OMG_ISNULL(brush))
        return true;
    // TODO: https://stackoverflow.com/questions/53270959/cwinapi-simple-program-drawing-lines
    OMG_UNUSED(start_end);
    bool res = true;
    this->g32->DeleteObject(brush);
    return res;
}

bool omg_renderer_win_draw_point(OMG_RendererWin* this, const OMG_FPoint* pos, const OMG_Color* col) {
    if (OMG_NO_SCALE()) {
        return !this->g32->SetPixel(
            this->cur_hpdc,
            (int)(pos->x + base->offset.x),
            (int)(pos->y + base->offset.y),
            _OMG_WIN_OMG_RGB(col)
        );
    }
    OMG_FRect rect = { .x = pos->x, .y = pos->y, .w = 1.0f, .h = 1.0f };
    return omg_renderer_win_fill_rect(this, &rect, col);
}

bool omg_renderer_win_draw_circle(OMG_RendererWin* this, const OMG_FPoint* pos, float rad, const OMG_Color* col) {
    HPEN pen = this->g32->CreatePen(PS_SOLID, (int)base->a_scale, _OMG_WIN_OMG_RGB(col));
    if (OMG_ISNULL(pen))
        return true;
    this->g32->SelectObject(this->cur_hpdc, pen);
    bool res = !this->g32->Ellipse(
        this->cur_hpdc,
        (int)((pos->x + base->offset.x - rad) * base->scale.x),
        (int)((pos->y + base->offset.y - rad) * base->scale.y),
        (int)((pos->x + base->offset.x + rad) * base->scale.x),
        (int)((pos->y + base->offset.y + rad) * base->scale.y)
    );
    this->g32->DeleteObject(pen);
    return res;
}


bool omg_renderer_win_fill_circle(OMG_RendererWin* this, const OMG_FPoint* pos, float rad, const OMG_Color* col) {
    HBRUSH brush = this->g32->CreateSolidBrush(_OMG_WIN_OMG_RGB(col));
    if (OMG_ISNULL(brush))
        return true;
    this->g32->SelectObject(this->cur_hpdc, brush);
    bool res = !this->g32->Ellipse(
        this->cur_hpdc,
        (int)((pos->x + base->offset.x - rad) * base->scale.x),
        (int)((pos->y + base->offset.y - rad) * base->scale.y),
        (int)((pos->x + base->offset.x + rad) * base->scale.x),
        (int)((pos->y + base->offset.y + rad) * base->scale.y)
    );
    this->g32->DeleteObject(brush);
    omg_renderer_win_draw_circle(this, pos, rad, col); // Ugly border hack
    return res;
}

bool omg_renderer_win_flip(OMG_RendererWin* this) {
    bool res = false;
    this->u32->EndPaint(this->hwnd, &this->ps);
    this->cur_hpdc = NULL;
    if (win_base->vsync && OMG_ISNOTNULL(this->dwm->DwmFlush))
        this->dwm->DwmFlush();
    return res;
}

bool omg_renderer_win_set_target(OMG_RendererWin* this, OMG_TextureWin* tex) {
    this->cur_hwdc = OMG_ISNULL(tex) ? this->hwdc : tex->dc;
    if (1 || OMG_ISNOTNULL(this->cur_hpdc))
        this->cur_hpdc = OMG_ISNULL(tex) ? this->hpdc : tex->dc;
    base->target = tex_base;
    return false;
}

OMG_TextureWin* omg_renderer_win_tex_create(OMG_RendererWin* this, const OMG_FPoint* size, int access, bool has_alpha) {
    OMG_UNUSED(access);
    OMG_TextureWin* tex = OMG_MALLOC(omg_base->mem, sizeof(OMG_TextureWin));
    if (OMG_ISNULL(tex))
        return (OMG_TextureWin*)omg_renderer_tex_create(base, size, access, has_alpha);
    tex->dc = this->g32->CreateCompatibleDC(this->hwdc);
    if (OMG_ISNULL(tex->dc)) {
        OMG_FREE(omg_base->mem, tex);
        _OMG_LOG_ERROR(omg_base, "Failed to create Win32 HDC for texture");
        return (OMG_TextureWin*)omg_renderer_tex_create(base, size, access, has_alpha);
    }
    tex->bm = this->g32->CreateCompatibleBitmap(this->hwdc, (int)size->w, (int)size->h);
    if (OMG_ISNULL(tex->bm)) {
        this->g32->DeleteDC(tex->dc);
        OMG_FREE(omg_base->mem, tex);
        _OMG_LOG_ERROR(omg_base, "Failed to create Win32 bitmap for texture");
        return (OMG_TextureWin*)omg_renderer_tex_create(base, size, access, has_alpha);
    }
    this->g32->SelectObject(tex->dc, tex->bm);
    tex_base->has_alpha = has_alpha;
    tex_base->size.w = size->w;
    tex_base->size.h = size->h;
    return tex;
}

bool omg_renderer_win_tex_destroy(OMG_RendererWin* this, OMG_TextureWin* tex) {
    if (OMG_ISNULL(tex) || OMG_ISNULL(tex->dc) || OMG_ISNULL(tex->bm)) {
        _OMG_NULL_TEXTURE_WARN();
        return true;
    }
    bool res = false;
    if (!this->g32->DeleteObject(tex->bm)) {
        res = true;
        _OMG_LOG_WARN(omg_base, "Failed to delete win32 bitmap");
    }
    if (!this->g32->DeleteDC(tex->dc)) {
        res = true;
        _OMG_LOG_WARN(omg_base, "Failed to delete Win32 DC");
    }
    OMG_FREE(omg_base->mem, tex);
    return  res;
}

bool omg_renderer_win_copy(OMG_RendererWin* this, OMG_TextureWin* tex, const OMG_FPoint* pos) {
    this->g32->SetStretchBltMode(this->cur_hpdc, HALFTONE);
    this->g32->StretchBlt(
        this->cur_hpdc,
        (int)((pos->x + base->offset.x) * base->scale.x), (int)((pos->y + base->offset.y) * base->scale.y),
        (int)(tex_base->size.w * base->scale.x), (int)(tex_base->size.h * base->scale.y),
        tex->dc, 0, 0, (int)tex_base->size.w, (int)tex_base->size.h,
        SRCCOPY
    );
    return false;
}

bool omg_renderer_win_init(OMG_RendererWin* this) {
    OMG_BEGIN_POINTER_CAST();
    omg_renderer_init(this);
    this->cur_hwdc = this->hwdc = this->u32->GetDC(this->hwnd);
    if (OMG_ISNULL(this->hwdc))
        _OMG_LOG_WARN(omg_base, "Failed to get Win32 HDC for window");
    base->_on_update_window_size = omg_renderer_win_update_scale;
    base->destroy = omg_renderer_win_destroy;
    base->begin = omg_renderer_win_begin;
    base->clear = omg_renderer_win_clear;
    base->flip = omg_renderer_win_flip;
    base->draw_line = omg_renderer_win_draw_line;
    base->draw_rect = omg_renderer_win_draw_rect;
    base->fill_rect = omg_renderer_win_fill_rect;
    base->draw_point = omg_renderer_win_draw_point;
    base->draw_circle = omg_renderer_win_draw_circle;
    base->fill_circle = omg_renderer_win_fill_circle;
    base->set_target = omg_renderer_win_set_target;
    base->tex_create = omg_renderer_win_tex_create;
    base->tex_destroy = omg_renderer_win_tex_destroy;
    base->copy = omg_renderer_win_copy;
    OMG_END_POINTER_CAST();
    base->type = OMG_REN_TYPE_RAYLIB;
    base->soft_scale = true; // IDK how without
    base->aa = true;
    base->inited = true;
    omg_renderer_win_update_scale(this);
    _OMG_LOG_INFO(omg_base, "Win32 renderer created successfuly");
    return false;
}
#endif
