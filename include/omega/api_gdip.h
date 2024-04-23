#pragma once
#include <omega/config.h>
#include <omega/ostd.h>
#if OMG_IS_WIN && OMG_SUPPORT_GDIP || 1
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

typedef INT PixelFormat;
typedef void GpBitmap;

#if !OMG_WINAPI_DYNAMIC || 1 // TODO: Remove
GpStatus OMG_WINGDIPAPI GdiplusStartup(ULONG_PTR*, GDIPCONST GdiplusStartupInput*, GdiplusStartupOutput*);
void OMG_WINGDIPAPI GdiplusShutdown(ULONG_PTR);
GpStatus OMG_WINGDIPAPI GdipGetImageDecodersSize(UINT*, UINT*);
GpStatus OMG_WINGDIPAPI GdipGetImageDecoders(UINT, UINT, ImageCodecInfo*);
GpStatus OMG_WINGDIPAPI GdipGetImageEncodersSize(UINT*, UINT*);
GpStatus OMG_WINGDIPAPI GdipGetImageEncoders(UINT, UINT, ImageCodecInfo*);
GpStatus OMG_WINGDIPAPI GdipBitmapLockBits(GpBitmap*, GDIPCONST GpRect*, UINT, PixelFormat, BitmapData*);
GpStatus OMG_WINGDIPAPI GdipBitmapUnlockBits(GpBitmap*, BitmapData*);
#endif

#endif
