#include <windows.h>

static LRESULT WindowProc(HWND h, UINT msg, WPARAM wp, LPARAM lp) {
    /* More info at [2] */
    switch (msg)
    {
        /*                                                              */
        /* Add a win32 push button and do something when it's clicked.  */
        /* Google will help you with the other widget types.  There are */
        /* many tutorials.                                              */
        /*                                                              */
    case WM_CREATE: {
        HWND hbutton = CreateWindow("BUTTON", "Hey There",  /* class and title */
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, /* style */
            0, 0, 100, 30,            /* position */
            h,                     /* parent */
            (HMENU)101,            /* unique (within the application) integer identifier */
            GetModuleHandle(0), 0   /* GetModuleHandle(0) gets the hinst */
        );
    } break;

    case WM_COMMAND: {
        switch (LOWORD(wp)) {
        case 101: /* the unique identifier used above. These are usually #define's */
            PostQuitMessage(0);
            break;
        default:;
        }
    } break;


        /*                                 */
        /* Minimally need the cases below: */
        /*                                 */
    case WM_CLOSE: PostQuitMessage(0); break;
    default:
        return DefWindowProc(h, msg, wp, lp);
    }
    return 0;
}

int WinMain(HINSTANCE hinst,
    HINSTANCE hprev,
    LPSTR     cmdline,
    int       show) {
    if (!hprev) {
        WNDCLASS c = { 0 };
        c.lpfnWndProc = WindowProc;
        c.hInstance = hinst;
        c.hIcon = LoadIcon(0, IDI_APPLICATION);
        c.hCursor = LoadCursor(0, IDC_ARROW);
        c.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
        c.lpszClassName = "MainWindow";
        RegisterClass(&c);
    }

    HWND h = CreateWindow("MainWindow",          /* window class name*/
        "WindowTitle",              /* title  */
        WS_OVERLAPPEDWINDOW,        /* style */
        CW_USEDEFAULT, CW_USEDEFAULT,/* position */
        CW_USEDEFAULT, CW_USEDEFAULT,/* size */
        0,                          /* parent */
        0,                          /* menu */
        hinst,
        0                           /* lparam -- crazytown */
    );
    ShowWindow(h, show);

    while (1) {  /* or while(running) */
        MSG msg;
        while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) { /* See Note 3,4 */
            if (msg.message == WM_QUIT)
                return (int)msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return 0;
}