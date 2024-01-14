#include <omega/image_omg.h>

#if OMG_SUPPORT_OMG_IMAGE
#include <omega/omega.h>
#include <omega/api_spng.h>
#define base ((OMG_ImageLoader*)this)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_image_loader_omg_destroy(OMG_ImageLoaderOmg* this) {
    if (!base->inited)
        return false;
#if OMG_SUPPORT_SPNG
    if (OMG_ISNOTNULL(this->spng.loaded))
        omg_spng_dll_free(&this->spng);
#endif
    omg_image_loader_destroy(base);
    return false;
}

bool omg_image_loader_omg_image_from_fp(OMG_ImageLoaderOmg* this, const OMG_String* path, void* buf, int format) {
    if ((format != OMG_IMG_FORMAT_AUTO) && (format != OMG_IMG_FORMAT_PNG)) // Currently only PNG
        return true;
#if OMG_SUPPORT_SPNG
    if (!this->spng.loaded)
        return true;
    OMG_File* file = omg_base->file_from_fp(omg_base, NULL, path, OMG_FILE_MODE_RB);
    if (OMG_ISNULL(file))
        return true;
    int64_t size = file->get_size(file);
    if (size < 0) {
        file->destroy(file);
        return true;
    }
    void* file_buf = OMG_MALLOC(omg_base->mem, size);
    if (OMG_ISNULL(file_buf)) {
        file->destroy(file);
        return true;
    }
    if ((int64_t)file->read(file, file_buf, 1, (size_t)size) < size) {
        OMG_FREE(omg_base->mem, file_buf);
        file->destroy(file);
        return true;
    }
    file->destroy(file);
    spng_ctx* ctx = this->spng.spng_ctx_new(0);
    this->spng.spng_set_png_buffer(ctx, file_buf, (size_t)size);
    size_t img_size = 0;
    this->spng.spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &img_size);
    struct {
        void* data;
        int w, h, depth;
    } *img_buf = buf;
    img_buf->data = OMG_MALLOC(omg_base->mem, img_size);
    this->spng.spng_decode_image(ctx, img_buf->data, img_size, SPNG_FMT_RGBA8, 0);
    struct spng_ihdr ihdr;
    this->spng.spng_get_ihdr(ctx, &ihdr);
    this->spng.spng_ctx_free(ctx);
    OMG_FREE(omg_base->mem, file_buf);
    img_buf->w = (int)ihdr.width;
    img_buf->h = (int)ihdr.width;
    img_buf->depth = (int)ihdr.bit_depth;
    return false;
#endif
    return true;
}

bool omg_image_loader_omg_init(OMG_ImageLoaderOmg* this) {
    omg_image_loader_init(base);
#if OMG_SUPPORT_SPNG
    if (omg_spng_dll_load(&this->spng, omg_base->spng_dll_path))
        _OMG_LOG_WARN(omg_base, "Failed to load spng DLL");
#endif
    base->type = OMG_IMAGE_LOADER_TYPE_OMG;
    OMG_BEGIN_POINTER_CAST();
    base->destroy = omg_image_loader_omg_destroy;
    base->image_from_fp_internal = omg_image_loader_omg_image_from_fp;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
