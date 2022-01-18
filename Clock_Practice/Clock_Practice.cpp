
#include "framework.h"
#include "Clock_Practice.h"
#include <Windowsx.h>
#include <Windows.h>
#include <CommCtrl.h>
#include <string.h>
#include <iostream>
#include <sstream>


#define _GLIBCXX_USE_C99 1
#define MAX_LOADSTRING 100
#define TIMER 2000
#define CMD_BUTTON_START_TIMER 2001
#define CMD_BUTTON_EASY_LVL 2002
#define CMD_BUTTON_HARD_LVL 2003
#define CMD_BUTTON_IMPOSIBLE_LVL 2004


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND static_seconds;
HWND static_minutes;
HWND static_hours;
HWND static_clicks;
HWND static_s_res;
HWND static_m_res;
HWND static_h_res;
HWND static_s_res_temp;
HWND static_m_res_temp;
HWND static_h_res_temp;
HWND static_s_r_res;
HWND static_m_r_res;
HWND static_h_r_res;
HWND static_click_res;
HWND start_timer;
HWND progress;
HWND hwndRadioEasy;
HWND hwndRadioHard;
HWND hwndRadioImposible;
WCHAR str[100];
UINT seconds = 0;
UINT minutes = 0;
UINT hours = 0;
UINT count_click = 0;
bool easy_lvl = false;
bool hard_lvl = false;
bool imposible_lvl = false;
bool game_end = false;
bool level_of_difficulty = false;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
DWORD   CALLBACK    CreateUI(LPVOID);  
DWORD   CALLBACK    Clock(LPVOID);


bool                Victory(LPVOID);
bool                Defeat(LPVOID);
bool                Record(LPVOID);
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
    wcex.hbrBackground  = (HBRUSH)CreateSolidBrush(RGB(73, 15, 117));
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
       CreateUI(&hWnd);
        break;
    }
    
    case WM_TIMER:
    {
           Clock(&hWnd);
           int posD = SendMessageW(progress, PBM_GETPOS, 0, 0);
           if (posD <= 0) {
               Defeat(&hWnd);
               SendMessageW(progress, PBM_SETPOS, 10, 0);
           }
        break;
    }
    case WM_LBUTTONDOWN:
    {

        count_click += 1;
        _itow_s(count_click, str, 10);
        SendMessageW(static_clicks, WM_SETTEXT, 100, (LPARAM)str);

        SendMessageW(progress, PBM_SETSTEP, 1, 0);
        SendMessageW(progress, PBM_STEPIT, 0, 0);
        int posV = SendMessageW(progress, PBM_GETPOS, 0, 0);
        if (posV >= 99) {
            Victory(&hWnd);
            SendMessageW(progress, PBM_SETPOS, 10, 0);
        }

        break;
    }
    
    case WM_COMMAND:
        {
            
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case CMD_BUTTON_START_TIMER:
                
                if (level_of_difficulty == false) {
                    MessageBox(hWnd, L"Please choise level of difficulty", L"Game", NULL);
                }
                else{
                    SetTimer(hWnd, TIMER, 1000, NULL);
                    
                }
             
                
                
                break;
            case CMD_BUTTON_EASY_LVL:
            {
                level_of_difficulty = true;
                LRESULT state = SendMessageW(hwndRadioEasy, BM_GETCHECK, 0, 0);
                if (!state == BST_CHECKED){
                    easy_lvl = true;
                    hard_lvl = false;
                    imposible_lvl = false;
                    SendMessage(hwndRadioEasy, BM_SETCHECK, BST_CHECKED, 0);
                    SendMessage(hwndRadioHard, BM_SETCHECK, BST_UNCHECKED, 0);
                    SendMessage(hwndRadioImposible, BM_SETCHECK, BST_UNCHECKED, 0);
                    
                    
                }
                else
                    SendMessage(hwndRadioEasy, BM_SETCHECK, BST_CHECKED, 0);
            
                SendMessageW(hWnd, WM_KILLFOCUS, 0, 0);
                break;
            }

            case CMD_BUTTON_HARD_LVL:
            {
                level_of_difficulty = true;
                LRESULT state = SendMessageW(hwndRadioHard, BM_GETCHECK, 0, 0);
                if (!state == BST_CHECKED) {
                    
                    easy_lvl = false;
                    hard_lvl = true;
                    imposible_lvl = false;
                    SendMessage(hwndRadioEasy, BM_SETCHECK, BST_UNCHECKED, 0);
                    SendMessage(hwndRadioHard, BM_SETCHECK, BST_CHECKED, 0);
                    SendMessage(hwndRadioImposible, BM_SETCHECK, BST_UNCHECKED, 0);
                }
                else
                    SendMessage(hwndRadioHard, BM_SETCHECK, BST_CHECKED, 0);

                SendMessageW(hWnd, WM_KILLFOCUS, 0, 0);
                break;
            }

            case CMD_BUTTON_IMPOSIBLE_LVL:
            {
                level_of_difficulty = true;
                LRESULT state = SendMessageW(hwndRadioImposible, BM_GETCHECK, 0, 0);
                if (!state == BST_CHECKED) {
                    easy_lvl = false;
                    hard_lvl = false;
                    imposible_lvl = true;
                    SendMessage(hwndRadioEasy, BM_SETCHECK, BST_UNCHECKED, 0);
                    SendMessage(hwndRadioHard, BM_SETCHECK, BST_UNCHECKED, 0);
                    SendMessage(hwndRadioImposible, BM_SETCHECK, BST_CHECKED, 0);
                }
                else
                    SendMessage(hwndRadioImposible, BM_SETCHECK, BST_CHECKED, 0);

                SendMessageW(hWnd, WM_KILLFOCUS, 0, 0);
                break;
            }
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

DWORD CALLBACK CreateUI(LPVOID params) {
    HWND hWnd = *((HWND*)params);
    /* static1 = CreateWindowW(L"Static", L"",
            WS_CHILD | WS_VISIBLE | SS_CENTER,
            10, 10, 50, 15, hWnd, 0, hInst, NULL);*/
    hwndRadioEasy = CreateWindowW(L"Button", L"Easy", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_CENTER,
        10, 250, 100, 23, hWnd, (HMENU)CMD_BUTTON_EASY_LVL, hInst, 0);
    hwndRadioHard = CreateWindowW(L"Button", L"Hard", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_CENTER,
        10, 280, 100, 23, hWnd, (HMENU)CMD_BUTTON_HARD_LVL, hInst, 0);
    hwndRadioImposible = CreateWindowW(L"Button", L"Imposible", WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON | BS_CENTER,
        10, 310, 100, 23, hWnd, (HMENU)CMD_BUTTON_IMPOSIBLE_LVL, hInst, 0);
    static_seconds = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        110, 10, 40, 15, hWnd, 0, hInst, NULL);
    static_minutes = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        60, 10, 40, 15, hWnd, 0, hInst, NULL);
    static_hours = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        10, 10, 40, 15, hWnd, 0, hInst, NULL);
    start_timer = CreateWindowW(L"Button", L"START",
        WS_CHILD | WS_VISIBLE | SS_CENTER ,
        160, 10, 70, 20, hWnd, (HMENU)CMD_BUTTON_START_TIMER, hInst, NULL);
    static_clicks = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        10, 100, 40, 15, hWnd, 0, hInst, NULL);
    progress = CreateWindowW(PROGRESS_CLASSW, L"",
        WS_CHILD | WS_VISIBLE | PBS_SMOOTH | PBS_VERTICAL,
        350, 0, 20, 340, hWnd, 0, hInst, NULL);
    SendMessageW(progress, PBM_SETRANGE, 0, MAKELPARAM(0, 100));
    SendMessageW(progress, PBM_SETBARCOLOR, 0, RGB(140, 62, 136));
    SendMessageW(static_seconds, PBM_SETPOS, 50, 0);
    SendMessageW(progress, PBM_DELTAPOS, 10, 0);

    CreateWindowW(L"Static", L"Last round",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        240, 10, 90, 15, hWnd, 0, hInst, NULL);

    static_h_res = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        240, 30, 30, 15, hWnd, 0, hInst, NULL);
    static_m_res = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        270, 30, 30, 15, hWnd, 0, hInst, NULL);
    static_s_res = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        300, 30, 30, 15, hWnd, 0, hInst, NULL);
    static_click_res = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        240, 50, 30, 15, hWnd, 0, hInst, NULL);

    static_h_res_temp = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        10, 30, 30, 15, hWnd, 0, hInst, NULL);
    static_m_res_temp = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        40, 30, 30, 15, hWnd, 0, hInst, NULL);
    static_s_res_temp = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        70, 30, 30, 15, hWnd, 0, hInst, NULL);
    

    CreateWindowW(L"Static", L"Record time",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        240, 90, 90, 15, hWnd, 0, hInst, NULL);

    static_h_r_res = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        240, 110, 30, 15, hWnd, 0, hInst, NULL);
    static_m_r_res = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        270, 110, 30, 15, hWnd, 0, hInst, NULL);
    static_s_r_res = CreateWindowW(L"Static", L"",
        WS_CHILD | WS_VISIBLE | SS_CENTER,
        300, 110, 30, 15, hWnd, 0, hInst, NULL);


    SetWindowPos(hWnd, HWND_TOP, 400,400, 400, 400, SWP_SHOWWINDOW);
    return 0;
}
DWORD CALLBACK Clock(LPVOID params) {
    HWND hWnd = *((HWND*)params);
    WPARAM wParam = TIMER;

    if (wParam == TIMER) {
        if (easy_lvl == true) {

            SendMessageW(static_seconds, PBM_DELTAPOS, 1, 0);
            SendMessageW(progress, PBM_DELTAPOS, -1, 0);
        }
        else if (hard_lvl == true) {

            SendMessageW(static_seconds, PBM_DELTAPOS, 1, 0);
            SendMessageW(progress, PBM_DELTAPOS, -2, 0);
        }
        else if (imposible_lvl == true) {

            SendMessageW(static_seconds, PBM_DELTAPOS, 1, 0);
            SendMessageW(progress, PBM_DELTAPOS, -3, 0);
        }
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
    
   

    return 0;
}



bool Defeat(LPVOID params)
{
    HWND hWnd = *((HWND*)params);
    
   

    _itow_s(seconds, str, 10);
    SendMessageW(static_s_res, WM_SETTEXT, 100, (LPARAM)str);
    _itow_s(minutes, str, 10);
    SendMessageW(static_m_res, WM_SETTEXT, 100, (LPARAM)str);
    _itow_s(hours, str, 10);
    SendMessageW(static_h_res, WM_SETTEXT, 100, (LPARAM)str);
   
    
    
    
    

    
    _itow_s(count_click, str, 10);
    SendMessageW(static_click_res, WM_SETTEXT, 100, (LPARAM)str);



   
    
   
    KillTimer(hWnd, TIMER);


   
    

    int mb = MessageBox(hWnd, L"Хотите попробовать ещё раз?", L"О нет! :(", MB_YESNO);
    if (mb == IDYES) {
        
        
        
    }
    if (mb == IDNO) exit(0);
   
    
    _itow_s(seconds, str, 10);
    SendMessageW(static_seconds, WM_SETTEXT, 60, (LPARAM)str);
    _itow_s(minutes, str, 10);
    SendMessageW(static_minutes, WM_SETTEXT, 60, (LPARAM)str);
    _itow_s(hours, str, 10);
    SendMessageW(static_hours, WM_SETTEXT, 24, (LPARAM)str);

    count_click = 0;
    seconds = 0;
    minutes = 0;
    hours = 0;
    
       
    

    

    
    return true;
}

bool Victory(LPVOID params) {
    HWND hWnd = *((HWND*)params);
   
    _itow_s(count_click, str, 10);
    SendMessageW(static_click_res, WM_SETTEXT, 100, (LPARAM)str);
    _itow_s(seconds, str, 10);
    SendMessageW(static_s_res, WM_SETTEXT, 100, (LPARAM)str);
    _itow_s(minutes, str, 10);
    SendMessageW(static_m_res, WM_SETTEXT, 100, (LPARAM)str);
    _itow_s(hours, str, 10);
    SendMessageW(static_h_res, WM_SETTEXT, 100, (LPARAM)str);
    KillTimer(hWnd, TIMER);

    count_click = 0;
    seconds = 0;
    minutes = 0;
    hours = 0;
    _itow_s(count_click, str, 10);
    SendMessageW(static_clicks, WM_SETTEXT, 100, (LPARAM)str);

    int mb = MessageBox(hWnd, L"Хотите попробовать ещё раз?", L"Замечательно :3", MB_YESNO);
    if (mb == IDNO) exit(0);
    else  Record(&hWnd);
    

    _itow_s(seconds, str, 10);
    SendMessageW(static_seconds, WM_SETTEXT, 60, (LPARAM)str);
    _itow_s(minutes, str, 10);
    SendMessageW(static_minutes, WM_SETTEXT, 60, (LPARAM)str);
    _itow_s(hours, str, 10);
    SendMessageW(static_hours, WM_SETTEXT, 24, (LPARAM)str);

    
   
    return true;
}


bool Record(LPVOID params) {
    HWND hWnd = *((HWND*)params);
  
    
    
   
    

    
    
    return 0;
    
    
}