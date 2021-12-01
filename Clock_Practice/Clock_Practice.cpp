
#include "framework.h"
#include "Clock_Practice.h"
#include <Windowsx.h>
#include <Windows.h>
#include <CommCtrl.h>

#define MAX_LOADSTRING 100
#define TIMER 2000
#define CMD_BUTTON_START_TIMER 2001


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND static_seconds;
HWND static_minutes;
HWND static_hours;
HWND static_clicks;
HWND start_timer;
HWND progress;
WCHAR str[100];
UINT seconds = 0;
UINT minutes = 0;
UINT hours = 0;
UINT count_click = 0;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CLOCKPRACTICE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLOCKPRACTICE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLOCKPRACTICE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CLOCKPRACTICE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_CREATE:
    {
        /* static1 = CreateWindowW(L"Static", L"",
             WS_CHILD | WS_VISIBLE | SS_CENTER,
             10, 10, 50, 15, hWnd, 0, hInst, NULL);*/
        static_seconds = CreateWindowW(L"Static", L"",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            110, 40, 40, 15, hWnd, 0, hInst, NULL);
        static_minutes = CreateWindowW(L"Static", L"",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            60, 40, 40, 15, hWnd, 0, hInst, NULL);
        static_hours = CreateWindowW(L"Static", L"",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, 40, 40, 15, hWnd, 0, hInst, NULL);
        start_timer = CreateWindowW(L"Button", L"START",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, 70, 70, 23, hWnd, (HMENU)CMD_BUTTON_START_TIMER, hInst, NULL);
        static_clicks = CreateWindowW(L"Static", L"",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, 100, 40, 15, hWnd, 0, hInst, NULL);
        progress = CreateWindowW(PROGRESS_CLASSW, L"",
            WS_CHILD | WS_VISIBLE | PBS_SMOOTH | PBS_VERTICAL,
            160, 100, 15, 100, hWnd, 0, hInst, NULL);
        SendMessageW(progress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
        SendMessageW(progress, PBM_SETBARCOLOR, 0, RGB(100, 200, 250));
        SendMessageW(progress, PBM_DELTAPOS, 0, 0);
       

       
       

        break;
    }
    
    case WM_TIMER:
    {
        if (wParam == TIMER) {
            SendMessageW(static_seconds, PBM_DELTAPOS, 1, 0);
            SendMessageW(progress, PBM_DELTAPOS, 1, 0);
            seconds++;
            _itow_s(seconds, str, 10);
            SendMessageW(static_seconds, WM_SETTEXT, 60, (LPARAM)str);
            if (seconds == 60) {
                seconds = 0;
                minutes++;
                _itow_s(minutes, str, 10);
                SendMessageW(static_minutes, WM_SETTEXT, 60, (LPARAM)str);
                if (minutes == 60) {
                    minutes = 0;
                    hours++;
                    _itow_s(hours, str, 10);
                    SendMessageW(static_minutes, WM_SETTEXT, 60, (LPARAM)str);
                }
                if (hours == 24) {
                    hours = 0;
                    _itow_s(hours, str, 10);
                    SendMessageW(static_hours, WM_SETTEXT, 24, (LPARAM)str);
                }
            }
        }


        break;
    }
    case WM_LBUTTONDOWN:
    {
        count_click += 1;
        _itow_s(count_click, str, 10);
        SendMessageW(static_clicks, WM_SETTEXT, 100, (LPARAM)str);

        SendMessageW(progress, PBM_SETSTEP, -1, 0);
        SendMessageW(progress, PBM_STEPIT, 0, 0);
        
        int pos = SendMessageW(progress, PBM_GETPOS, 0, 0);
        break;
    }
    case WM_COMMAND:
        {
            
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case CMD_BUTTON_START_TIMER:
                SetTimer(hWnd, TIMER, 1000, NULL);
                break;
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
