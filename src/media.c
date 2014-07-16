#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdio.h>
#include <string.h>

#define ENABLE_DEBUG_LOGGING 0

HINSTANCE hInst;
TCHAR szTitle[] = "ATKMEDIA";
TCHAR szWindowClass[] = "ATKMEDIA";

typedef struct {
    char quickCommand[4092];
    char calcCommand[4092];
} Commands;

Commands* gCommands;

ATOM MRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
void loadConfig();
void debugLog(char *what);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
    MRegisterClass(hInstance);

    if (!InitInstance(hInstance, nCmdShow)) {
        return FALSE;
    }

    Commands cmd = {
        .quickCommand = "start cmd.exe",
        .calcCommand = "start calc.exe"
    };
    gCommands = &cmd;

    loadConfig();

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int) msg.wParam;
}

ATOM MRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WindowProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = 0;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = 0;
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = 0;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

    return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    HWND hWnd;

    hInst = hInstance; // Store instance handle in our global variable

    hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

    if (!hWnd) {
        return FALSE;
    }

    return TRUE;
}

void RunProc(LPSTR path) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    CreateProcess(NULL, path, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

#define ATKMEDIA_MESSAGE    0x0917
#define ATKMEDIA_QUICK      0x0001
#define ATKMEDIA_PLAY       0x0002
#define ATKMEDIA_STOP       0x0003
#define ATKMEDIA_NEXT       0x0004
#define ATKMEDIA_PREV       0x0005
#define ATKMEDIA_CALC       0x002B

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int wmId, wmEvent;
    BYTE vKeyCode = 0;

    switch (message) {
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);

        if (ENABLE_DEBUG_LOGGING) {
            char debugMessage[512];
            snprintf(debugMessage, 512, "loword: %d, hiword: %d", wmId, wmEvent);
            debugLog(debugMessage);
        }

        switch (wmId) {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case ATKMEDIA_MESSAGE:
            switch (wmEvent) {
            case ATKMEDIA_QUICK:
                RunProc(gCommands->quickCommand);
                break;
            case ATKMEDIA_PLAY:
                vKeyCode = VK_MEDIA_PLAY_PAUSE;
                break;
            case ATKMEDIA_STOP:
                vKeyCode = VK_MEDIA_STOP;
                break;
            case ATKMEDIA_NEXT:
                vKeyCode = VK_MEDIA_NEXT_TRACK;
                break;
            case ATKMEDIA_PREV:
                vKeyCode = VK_MEDIA_PREV_TRACK;
                break;
            case ATKMEDIA_CALC:
                RunProc(gCommands->calcCommand);
                break;
            }
            keybd_event(vKeyCode, 0,               0, 0); //Key down
            keybd_event(vKeyCode, 0, KEYEVENTF_KEYUP, 0); //Key up (quite useless)

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
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

void loadConfig() {
    TCHAR modulePath[4096];
    TCHAR confFilePath[4096];
    ZeroMemory(modulePath, 4096);
    ZeroMemory(confFilePath, 4096);
    
    GetModuleFileName(NULL /*current process*/, modulePath, 4096);

    char * c = strrchr(modulePath, '\\') + 1;
    *c= '\0';

    strcat(confFilePath, modulePath);
    strcat(confFilePath, "settings.conf");

    FILE *config = fopen(confFilePath, "r");

    if (config) {
        char line[4096];

        while (fgets(line, 4095, config) != NULL) {
            int len = strlen(line);
            line[len - 1] = 0; // it's a newline otherwise

            if (strstr(line, "quick") && strlen(line) > 6) {
                char command[4092];
                strncpy(command, line + 6, 4092 - 6);
                command[4091] = 0;
                strncpy(gCommands->quickCommand, command, 4092);
            } else if (strstr(line, "calc") && strlen(line) > 5) {
                char command[4092];
                strncpy(command, line + 5, 4092 - 5);
                command[4091] = 0;
                strncpy(gCommands->calcCommand, command, 4092);
            }
        }
    }
    fclose(config);
}

void debugLog(char *what) {
    FILE *log = fopen("log.txt", "a");
    fprintf(log, "%s\n", what);
    fclose(log);
}
