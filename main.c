#include <windows.h>
#include <commctrl.h>
#include <stdbool.h>

#include "resource.h"

HINSTANCE hInst;

const DWORD mseconds = 4000;

DWORD WINAPI open_in_other_thread(void* unused);

BOOL CALLBACK DlgMain(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
    case WM_INITDIALOG:
    {
        HINSTANCE instance_hdl = GetModuleHandle(NULL);

        SendMessage(hwndDlg, WM_SETICON, ICON_SMALL,
                   (LPARAM)LoadIcon(instance_hdl, MAKEINTRESOURCE(ICO_HAPPY)));
        SendMessage(hwndDlg, WM_SETICON, ICON_SMALL2,
                   (LPARAM)LoadIcon(instance_hdl, MAKEINTRESOURCE(ICO_HAPPY)));
        SendMessage(hwndDlg, WM_SETICON, ICON_BIG,
                   (LPARAM)LoadIcon(instance_hdl, MAKEINTRESOURCE(ICO_HAPPY)));

        CreateThread(NULL, 0, open_in_other_thread, NULL, 0, NULL);

    }
    return TRUE;

    case WM_CLOSE:
    {
        return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL,
                     (DLGPROC)DlgMain);
    }

    case WM_COMMAND:
    {
        switch(LOWORD(wParam))
        {
        case IDCLOSE:
            PostMessage(hwndDlg, WM_CLOSE, 0, 0);
            break;
        }
    }
    return TRUE;
    }
    return FALSE;
}

DWORD WINAPI infloop (void* unused)
{
    while(true) Beep(5000, 1000);
    return 1;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    HANDLE thread_hdl = CreateThread(NULL, 0, infloop, NULL, 0, NULL);
    if(!thread_hdl)
        ExitProcess(3);

    hInst=hInstance;
    const INITCOMMONCONTROLSEX cm_ctrls = {
        sizeof(INITCOMMONCONTROLSEX),
        ICC_WIN95_CLASSES
    };

    InitCommonControlsEx(&cm_ctrls);
    return DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
}

DWORD WINAPI open_in_other_thread(void* unused)
{
    while(true){
        Sleep(mseconds);
        DialogBox(hInst, MAKEINTRESOURCE(DLG_MAIN), NULL, (DLGPROC)DlgMain);
    }

    return 1;
}
