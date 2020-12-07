// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <Windows.h>
#include "stdio.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>

using namespace std;

HMODULE _hmod = NULL;
HHOOK _hook = NULL;

// be sure to export your hook functions from the DLL..

LRESULT CALLBACK CallWndProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    CWPSTRUCT* pMouseStruct = (CWPSTRUCT*)lParam;

    if (pMouseStruct != NULL)
    {
        wcout << pMouseStruct->message;
        //MessageBox(NULL, (LPCWSTR)pMouseStruct->message, (LPCWSTR)"Yes", MB_ICONERROR);
        printf("Message is from %d  And Message is %d\n", pMouseStruct->hwnd, pMouseStruct->message);
    }

    //std::cout << "That's nothing";

    return CallNextHookEx(_hook, nCode, wParam, lParam);
}

extern "C" {

    __declspec(dllexport) void SetHook()
    {
        if (!_hook)
        {
            _hook = SetWindowsHookEx(WH_CALLWNDPROC, CallWndProc, _hmod, 0);
            if (!_hook)
            {
                OutputDebugString(TEXT("Failed to Install hook"));
            }
        }

        OutputDebugString(TEXT("Exiting SETHOOK METHOD"));
        if (AllocConsole())
        {
            int hCrt = _open_osfhandle((long)GetStdHandle(STD_OUTPUT_HANDLE), 4);
            *stdout = *(::_fdopen(hCrt, "w"));
            ::setvbuf(stdout, NULL, _IONBF, 0);
            *stderr = *(::_fdopen(hCrt, "w"));
            ::setvbuf(stderr, NULL, _IONBF, 0);
            std::ios::sync_with_stdio();
        }
        wcout << "Hooked";
        MessageBox(NULL, (LPCWSTR)"Hooked!", (LPCWSTR)"Yes", MB_ICONERROR);
    }
}

extern "C" {

    __declspec(dllexport) void ReleaseHook()
    {
        if (_hook)
        {
            UnhookWindowsHookEx(_hook);
            _hook = NULL;
        }
    }
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    _hmod = hModule;

    OutputDebugString(TEXT("Entered DLL"));

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        break;
    }

    return TRUE;
}