// AUTOMATICALLY GENERATED FILE.  DO NOT MODIFY.  Place custom code in custominit.h.
void mcExitClass_SonifySpatial();
void mcInitClass_SonifySpatial();


extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void amirapackage_SonifySpatial_init()
{
    static bool isInitialized = false;
    if (isInitialized)
      return;

    isInitialized = true;

    mcInitClass_SonifySpatial();

}


extern "C"
#ifdef _WIN32
__declspec(dllexport)
#endif
void amirapackage_SonifySpatial_finish()
{
    static bool isFinished = false;
    if (isFinished)
      return;

    isFinished = true;


    mcExitClass_SonifySpatial();
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
        amirapackage_SonifySpatial_init();
        break;
    case DLL_PROCESS_DETACH:
        amirapackage_SonifySpatial_finish();
        break;
    default:
        ;
    }
    return true;
}


#endif

#if defined(__GNUC__)
void __attribute__((constructor)) soconstructor_SonifySpatial() {
    amirapackage_SonifySpatial_init();
}

void __attribute__((destructor)) sodestructor_SonifySpatial() {
    amirapackage_SonifySpatial_finish();
}
#endif
