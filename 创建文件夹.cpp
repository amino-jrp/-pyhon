#include <windows.h>
#include <shlobj.h>
#include <string>
#include <sstream>

int main() {
    WCHAR desktop[MAX_PATH];
    SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, desktop);
    
    for (int i = 1; i <= 89; ++i) {
        std::wostringstream oss;
        // 用Unicode转义序列表示"阿米诺"，避免中文编码问题
        oss << L"\u963F\u7C73\u8BFA" << i << L".";
        std::wstring path = L"\\\\?\\" + std::wstring(desktop) + L"\\" + oss.str();
        CreateDirectoryW(path.c_str(), NULL);
    }
    
    return 0;
}
