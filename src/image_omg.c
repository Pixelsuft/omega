#include <omega/image_omg.h>

#if OMG_SUPPORT_OMG_IMAGE
#include <omega/omega.h>
#if OMG_SUPPORT_LODEPNG
#include <stdlib.h>
#include <omega_libs/lodepng.h>
#endif
#if OMG_SUPPORT_SPNG
#include <stdlib.h>
#include <omega_libs/spng.h>
#endif
#define base ((OMG_ImageLoader*)this)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_image_loader_omg_image_from_fp(OMG_ImageLoaderOmg* this, const OMG_String* path, void* buf, int format) {
    if ((format != OMG_IMG_FORMAT_AUTO) && (format != OMG_IMG_FORMAT_PNG)) // Currently only PNG
        return true;
#if OMG_SUPPORT_LODEPNG
    OMG_File* file = omg_base->file_from_fp(omg_base, NULL, path, OMG_FILE_MODE_RB);
    if (OMG_ISNULL(file))
        return true;
    int64_t size = file->get_size(file);
    if (size < 0) {
        file->destroy(file);
        return true;
    }
    void* file_buf = OMG_MALLOC(omg_base->mem, size);
    if ((int64_t)file->read(file, file_buf, 1, (size_t)size) < size) {
        OMG_FREE(omg_base->mem, file_buf);
        file->destroy(file);
        return true;
    }
    unsigned char* image;
    unsigned int w, h;
    unsigned int error = lodepng_decode_memory(&image, &w, &h, file_buf, size, LCT_RGBA, 8);
    if (error != 0) {
        OMG_FREE(omg_base->mem, file_buf);
        _OMG_LOG_ERROR(omg_base, "Failed to load PNG (", lodepng_error_text(error), ")");
        return true;
    }
    struct {
        void* data;
        int w, h;
    } *img_buf = buf;
    /*img_buf->data = OMG_MALLOC(omg_base->mem, (size_t)w * (size_t)h * 4);
    if (OMG_ISNULL(img_buf->data)) {
        free(image);
        return true;
    }
    omg_base->std->memcpy(img_buf->data, image, (size_t)w * (size_t)h * 4);
    free(image);*/
    img_buf->data = (void*)image;
    img_buf->w = (int)w;
    img_buf->h = (int)h;
    return false;
#endif
#if OMG_SUPPORT_SPNG
    OMG_File* file = omg_base->file_from_fp(omg_base, NULL, path, OMG_FILE_MODE_RB);
    if (OMG_ISNULL(file))
        return true;
    int64_t size = file->get_size(file);
    if (size < 0) {
        file->destroy(file);
        return true;
    }
    void* file_buf = OMG_MALLOC(omg_base->mem, size);
    if ((int64_t)file->read(file, file_buf, 1, (size_t)size) < size) {
        OMG_FREE(omg_base->mem, file_buf);
        file->destroy(file);
        return true;
    }
    file->destroy(file);
    spng_ctx* ctx = spng_ctx_new(0);
    spng_set_png_buffer(ctx, file_buf, size);
    size_t img_size = 0;
    spng_decoded_image_size(ctx, SPNG_FMT_RGBA8, &img_size);
    struct {
        void* data;
        int w, h;
    } *img_buf = buf;
    printf("%i\n", (int)img_size);
    img_buf->data = OMG_MALLOC(omg_base->mem, img_size);
    spng_decode_image(ctx, img_buf->data, img_size, SPNG_FMT_RGBA8, 0);
    spng_ctx_free(ctx);
    img_buf->w = 100;
    img_buf->h = 100;
    return false;
#endif
    return true;
}

bool omg_image_loader_omg_init(OMG_ImageLoaderOmg* this) {
    omg_image_loader_init(base);
    base->type = OMG_IMAGE_LOADER_TYPE_OMG;
    OMG_BEGIN_POINTER_CAST();
    base->image_from_fp_internal = omg_image_loader_omg_image_from_fp;
    OMG_END_POINTER_CAST();
    return false;
}
#endif
