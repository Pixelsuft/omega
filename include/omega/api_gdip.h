#pragma once
#include <omega/config.h>
#include <omega/ostd.h>
#if OMG_IS_WIN && OMG_SUPPORT_GDIP
#include <omega/api_win.h>
#if defined(_ARM_)
#define OMG_WINGDIPAPI
#else
#define OMG_WINGDIPAPI __stdcall
#endif
#define GDIPCONST const

#ifdef _MSC_VER
#define OMG_GDIP_CB
#else
#define OMG_GDIP_CB OMG_WINGDIPAPI
#endif
#define OMG_GPIP_PREFIX OMG_WIN_STD_PREFIX

#ifndef _COM_interface
#define _COM_interface struct
#endif

typedef _COM_interface IStream IStream;

typedef enum GpStatus {
	Ok = 0,
	GenericError = 1,
	InvalidParameter = 2,
	OutOfMemory = 3,
	ObjectBusy = 4,
	InsufficientBuffer = 5,
	NotImplemented = 6,
	Win32Error = 7,
	WrongState = 8,
	Aborted = 9,
	FileNotFound = 10,
	ValueOverflow = 11,
	AccessDenied = 12,
	UnknownImageFormat = 13,
	FontFamilyNotFound = 14,
	FontStyleNotFound = 15,
	NotTrueTypeFont = 16,
	UnsupportedGdiplusVersion = 17,
	GdiplusNotInitialized = 18,
	PropertyNotFound = 19,
	PropertyNotSupported = 20,
	ProfileNotFound = 21
} GpStatus;

typedef void* DebugEventProc;

typedef struct GdiplusStartupInput {
	UINT32 GdiplusVersion;
	DebugEventProc DebugEventCallback;
	BOOL SuppressBackgroundThread;
	BOOL SuppressExternalCodecs;
} GdiplusStartupInput;

typedef GpStatus OMG_GDIP_CB(*NotificationHookProc)(ULONG_PTR*);
typedef VOID OMG_GDIP_CB(*NotificationUnhookProc)(ULONG_PTR);

typedef struct GdiplusStartupOutput {
	NotificationHookProc NotificationHook;
	NotificationUnhookProc NotificationUnhook;
} GdiplusStartupOutput;

typedef struct ImageCodecInfo {
	CLSID Clsid;
	GUID FormatID;
	WCHAR* CodecName;
	WCHAR* DllName;
	WCHAR* FormatDescription;
	WCHAR* FilenameExtension;
	WCHAR* MimeType;
	DWORD Flags;
	DWORD Version;
	DWORD SigCount;
	DWORD SigSize;
	BYTE* SigPattern;
	BYTE* SigMask;
} ImageCodecInfo;

typedef struct BitmapData {
	UINT Width;
	UINT Height;
	INT Stride;
	INT PixelFormat;
	VOID *Scan0;
	UINT_PTR Reserved;
} BitmapData;

typedef struct Rect {
	INT X;
	INT Y;
	INT Width;
	INT Height;
} Rect;

typedef Rect GpRect;

typedef INT GDIP_PixelFormat;
typedef void GpBitmap;
typedef void GpImage;

#if !OMG_WINAPI_DYNAMIC
GpStatus OMG_WINGDIPAPI GdiplusStartup(ULONG_PTR*, GDIPCONST GdiplusStartupInput*, GdiplusStartupOutput*);
void OMG_WINGDIPAPI GdiplusShutdown(ULONG_PTR);
GpStatus OMG_WINGDIPAPI GdipGetImageDecodersSize(UINT*, UINT*);
GpStatus OMG_WINGDIPAPI GdipGetImageDecoders(UINT, UINT, ImageCodecInfo*);
GpStatus OMG_WINGDIPAPI GdipGetImageEncodersSize(UINT*, UINT*);
GpStatus OMG_WINGDIPAPI GdipGetImageEncoders(UINT, UINT, ImageCodecInfo*);
GpStatus OMG_WINGDIPAPI GdipBitmapLockBits(GpBitmap*, GDIPCONST GpRect*, UINT, GDIP_PixelFormat, BitmapData*);
GpStatus OMG_WINGDIPAPI GdipBitmapUnlockBits(GpBitmap*, BitmapData*);
GpStatus OMG_WINGDIPAPI GdipGetImageWidth(GpImage*, UINT*);
GpStatus OMG_WINGDIPAPI GdipGetImageHeight(GpImage*, UINT*);
GpStatus OMG_WINGDIPAPI GdipGetImagePixelFormat(GpImage*, GDIP_PixelFormat*);
GpStatus OMG_WINGDIPAPI GdipLoadImageFromStream(IStream*, GpImage**);
GpStatus OMG_WINGDIPAPI GdipLoadImageFromFile(GDIPCONST WCHAR*, GpImage**);
GpStatus OMG_WINGDIPAPI GdipCreateBitmapFromStreamICM(IStream*, GpBitmap**);
GpStatus OMG_WINGDIPAPI GdipCreateBitmapFromFileICM(GDIPCONST WCHAR*, GpBitmap**);
#endif

typedef struct {
    HANDLE handle;
    GpStatus OMG_GPIP_PREFIX (*GdiplusStartup)(ULONG_PTR*, GDIPCONST GdiplusStartupInput*, GdiplusStartupOutput*);
    void OMG_GPIP_PREFIX (*GdiplusShutdown)(ULONG_PTR);
    GpStatus OMG_GPIP_PREFIX (*GdipGetImageDecodersSize)(UINT*, UINT*);
    GpStatus OMG_GPIP_PREFIX (*GdipGetImageDecoders)(UINT, UINT, ImageCodecInfo*);
    GpStatus OMG_GPIP_PREFIX (*GdipGetImageEncodersSize)(UINT*, UINT*);
    GpStatus OMG_GPIP_PREFIX (*GdipGetImageEncoders)(UINT, UINT, ImageCodecInfo*);
    GpStatus OMG_GPIP_PREFIX (*GdipBitmapLockBits)(GpBitmap*, GDIPCONST GpRect*, UINT, GDIP_PixelFormat, BitmapData*);
    GpStatus OMG_GPIP_PREFIX (*GdipBitmapUnlockBits)(GpBitmap*, BitmapData*);
    GpStatus OMG_GPIP_PREFIX (*GdipGetImageWidth)(GpImage*, UINT*);
    GpStatus OMG_GPIP_PREFIX (*GdipGetImageHeight)(GpImage*, UINT*);
    GpStatus OMG_GPIP_PREFIX (*GdipGetImagePixelFormat)(GpImage*, GDIP_PixelFormat*);
    GpStatus OMG_GPIP_PREFIX (*GdipLoadImageFromStream)(IStream*, GpImage**);
    GpStatus OMG_GPIP_PREFIX (*GdipLoadImageFromFile)(GDIPCONST WCHAR*, GpImage**);
    GpStatus OMG_GPIP_PREFIX (*GdipCreateBitmapFromStreamICM)(IStream*, GpBitmap**);
    GpStatus OMG_GPIP_PREFIX (*GdipCreateBitmapFromFileICM)(GDIPCONST WCHAR*, GpBitmap**);
} OMG_Gdip;

OMG_API bool omg_winapi_gdip_load(OMG_Gdip* this);
OMG_API bool omg_winapi_gdip_free(OMG_Gdip* this);
#endif
