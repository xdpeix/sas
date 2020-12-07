#include "stdio.h"
#include "Windows.h"
#include <iostream>


//bool Inject(DWORD pID, const char* path)
//{
//
//    HANDLE proc_handle;
//    LPVOID RemoteString;
//    LPCVOID LoadLibAddy;
//
//    if (pID == 0)
//        return false;
//
//    proc_handle = OpenProcess(PROCESS_ALL_ACCESS, false, pID);
//
//    if (proc_handle == 0)
//        return false;
//
//    //LoadLibAddy = GetProcAddress(GetModuleHandle((LPCWSTR)"kernel32.dll"), "LoadLibraryA");
//    RemoteString = VirtualAllocEx(proc_handle, NULL, strlen(path), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
//
//    WriteProcessMemory(proc_handle, RemoteString, path, strlen(path), NULL);
//    CreateRemoteThread(proc_handle, NULL, NULL, (LPTHREAD_START_ROUTINE)LoadLibraryA, RemoteString, NULL, NULL);
//    CloseHandle(proc_handle);
//
//    return true;
//}




DWORD WINAPI MyMouseLogger(LPVOID lpParm)
{

    /*Inject(id, "C:\\Users\\niniou\\source\\repos\\hooktest\\Debug\\dllcpp.dll");

    HANDLE proc_handle;
    
    proc_handle = OpenProcess(PROCESS_ALL_ACCESS, false, id);

    if (proc_handle == 0)
        return false;

    HOOKPROC proc = (HOOKPROC)GetProcAddress(GetModuleHandle((LPCWSTR)"dllcpp.dll"), "CallWndProc");
    CloseHandle(proc_handle);*/

    /*HOOKPROC hkprcSysMsg;
    static HINSTANCE hinstDLL;
    static HHOOK hhookSysMsg;

    hinstDLL = LoadLibrary(TEXT("C:\\Users\\bob\\Desktop\\hooktest\\Debug\\dllcpp.dll"));
    hkprcSysMsg = (HOOKPROC)GetProcAddress(hinstDLL, "CallWndProc");*/

    typedef void (*LPFN_SH)();
    typedef void (*LPFN_RH)();

    HMODULE hMod = LoadLibrary(TEXT("C:\\Users\\bob\\Desktop\\hooktest\\Debug\\dllcpp.dll"));
    LPFN_SH SetHook = (LPFN_SH)GetProcAddress(hMod, "SetHook");
    LPFN_RH ReleaseHook = (LPFN_RH)GetProcAddress(hMod, "ReleaseHook");
    SetHook();

    


    MSG message;
    while (GetMessage(&message, NULL, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    ReleaseHook();
    FreeLibrary(hMod);

    return 0;
}


int main(int argc, char** argv)
{
    HANDLE hThread = NULL;

    hThread = CreateThread(NULL, 0, MyMouseLogger, NULL, 0, NULL);

    if (hThread != NULL)
        return WaitForSingleObject(hThread, INFINITE);
    else
        return 1;
}
