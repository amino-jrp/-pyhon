#include <windows.h>
#include <cstdlib>
#include <ctime>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    srand(time(0));
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = 200;
    int windowHeight = 100;
    while (true) {
        int x = rand() % (screenWidth - windowWidth);
        int y = rand() % (screenHeight - windowHeight);
        HWND hwnd = CreateWindowEx(0, "STATIC", "\n\n\n\n黑奴嗯嗯----一起来拉屎", WS_POPUP, 
            x, y, windowWidth, windowHeight, 
            NULL, NULL, hInstance, NULL);
        ShowWindow(hwnd, nCmdShow);
        UpdateWindow(hwnd);
        Sleep(1000); // 每秒弹出一个窗口
    }
    return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
