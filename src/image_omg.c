#include <omega/image_omg.h>

#if OMG_SUPPORT_OMG_IMAGE
#include <omega/omega.h>
#if OMG_SUPPORT_LODEPNG
#include <omega_libs/lodepng.h>
#endif
#define base ((OMG_ImageLoader*)this)
#define omg_base ((OMG_Omega*)base->omg)

bool omg_image_loader_omg_image_from_fp(OMG_ImageLoaderOmg* this, const OMG_String* path, void* buf, int format) {
    if ((format != OMG_IMG_FORMAT_AUTO) && (format != OMG_IMG_FORMAT_PNG)) // Currently only PNG
        return true;
    OMG_File* file = omg_base->file_from_fp(omg_base, NULL, path, OMG_FILE_MODE_RB);
    if (OMG_ISNULL(file))
        return true;
    int64_t size = file->get_size(file);
    if (size < 0) {
        file->destroy(file);
        return true;
    }
    void* file_buf = OMG_MALLOC(omg_base->mem, sizeof(size));
    if ((int64_t)file->read(file, file_buf, 1, (size_t)size) < size) {
        OMG_FREE(omg_base->mem, file_buf);
        file->destroy(file);
        return true;
    }
    unsigned char* image;
    unsigned int w, h;
    unsigned int error = lodepng_decode_memory(&image, &w, &h, file_buf, size, LCT_RGBA, 8);
    // unsigned int error = lodepng_decode32_file(&image, &w, &h, path->ptr);
    if (error != 0) {
        OMG_FREE(omg_base->mem, file_buf);
        _OMG_LOG_ERROR(omg_base, "Failed to load PNG (", lodepng_error_text(error), ")");
        return true;
    }
    struct {
        void* data;
        int w, h;
    } *img_buf = buf;
    img_buf->data = (void*)image;
    img_buf->w = (int)w;
    img_buf->h = (int)h;
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
