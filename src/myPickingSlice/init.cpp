// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.  Place custom code in custominit.h.
void mcExitClass_myPickingSlice();
void mcInitClass_myPickingSlice();


extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void amirapackage_myPickingSlice_init()
{
    static bool isInitialized = false;
    if (isInitialized)
      return;

    isInitialized = true;

    mcInitClass_myPickingSlice();

}


extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void amirapackage_myPickingSlice_finish()
{
    static bool isFinished = false;
    if (isFinished)
      return;

    isFinished = true;


    mcExitClass_myPickingSlice();
}

#if defined(_WIN32)
#  include <windows.h>
BOOL WINAPI DllMain(
    __in  HINSTANCE hinstDLL,
    __in  DWORD fdwReason,
    __in  LPVOID lpvReserved
    )
{
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
        amirapackage_myPickingSlice_init();
        break;
    case DLL_PROCESS_DETACH:
        amirapackage_myPickingSlice_finish();
        break;
    default:
        ;
    }
    return true;
}


#endif

#if defined(__GNUC__)
void __attribute__((constructor)) soconstructor_myPickingSlice() {
    amirapackage_myPickingSlice_init();
}

void __attribute__((destructor)) sodestructor_myPickingSlice() {
    amirapackage_myPickingSlice_finish();
}
#endif
