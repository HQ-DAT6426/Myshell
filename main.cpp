/*#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif*/
#include <stdio.h>
#include <windows.h>
#include <string.h>
int timer = 100;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT umsg, WPARAM wParam, LPARAM lParam){
    switch(umsg){
    case WM_TIMER:
        timer--;
        if(timer <0){
            DestroyWindow(hwnd);
        }else{
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
        }
        break;
    case WM_CLOSE:
        if (MessageBoxA(hwnd, "Really quit?", "My application", MB_OKCANCEL) == IDOK)
        {
            DestroyWindow(hwnd);
        }
        // Else: User canceled. Do nothing.
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

        char num[3];
        sprintf(num,"%d",timer);

        TextOutA( hdc, 100,100,num,3);
        // All painting occurs here, between BeginPaint and EndPaint.



        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, umsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow){
    const char CLASS_NAME[] = "Sample WindowClass";

    WNDCLASSA wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClassA(&wc); // đăng kí với hệ điều hành

    HWND hwnd = CreateWindowExA(
                    0,
                    CLASS_NAME,
                    "My 676776window",
                    WS_OVERLAPPEDWINDOW,
                    300,300,300,300,
                    NULL,
                    NULL,
                    hInstance,
                    NULL
    );
    ShowWindow(hwnd,nCmdShow);

    SetTimer(hwnd,1,1000,NULL);
    MSG msg = {};
    while( GetMessageA(&msg, NULL,0,0)!=0){
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return 0;
}
