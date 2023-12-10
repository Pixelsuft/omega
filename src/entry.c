#include <omega/entry.h>

#if OMG_IS_WIN && defined(OMG_DLL_BUILD)
#include <omega/api_win.h>

OMG_API BOOL WINAPI DllMain(HINSTANCE const instance, DWORD const reason, LPVOID const reserved) {
    OMG_UNUSED(instance, reason, reserved);
    switch (reason)
    {
    case DLL_PROCESS_ATTACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

OMG_API BOOL WINAPI DllEntryPoint(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    return DllMain(hinstDLL, fdwReason, lpReserved);
}

#if !OMG_HAS_STD
OMG_API BOOL WINAPI DllMainCRTStartup(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    return DllMain(hinstDLL, fdwReason, lpReserved);
}
#endif
#endif
