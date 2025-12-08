#include <windows.h>
#include <shlobj.h>
#include <windows.h>
#include <shlobj.h>
#include <string>
#include <cwchar>

// 匹配"阿米诺+数字+."格式的文件夹（兼容C++98）
bool IsTargetFolder(const std::wstring& folderName) {
    const wchar_t* prefix = L"\u963F\u7C73\u8BFA"; // 阿米诺的Unicode编码
    size_t prefixLen = wcslen(prefix);
    size_t nameLen = folderName.length();

    // 长度校验（前缀3 + 数字至少1 + 点1 = 最小长度5）
    if (nameLen < 5) return false;
    // 前缀匹配
    if (folderName.compare(0, prefixLen, prefix) != 0) return false;
    // 最后一位是点（替换back()，兼容C++98）
    if (folderName[nameLen - 1] != L'.') return false;
    // 中间全是数字
    for (size_t i = prefixLen; i < nameLen - 1; ++i) {
        if (!iswdigit(folderName[i])) return false;
    }
    return true;
}

int main() {
    // 1. 获取桌面路径
    WCHAR desktop[MAX_PATH] = {0};
    if (FAILED(SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, desktop))) {
        return 1;
    }

    // 2. 遍历桌面所有项
    std::wstring searchPath = L"\\\\?\\";
    searchPath += desktop;
    searchPath += L"\\*";
    WIN32_FIND_DATAW findData = {0};
    HANDLE hFind = FindFirstFileW(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE) {
        return 1;
    }

    // 3. 清理目标文件夹
    do {
        // 跳过.和..，仅处理文件夹
        if (wcscmp(findData.cFileName, L".") != 0 && wcscmp(findData.cFileName, L"..") != 0) {
            if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                std::wstring folderName = findData.cFileName;
                if (IsTargetFolder(folderName)) {
                    // 拼接完整路径并删除
                    std::wstring fullPath = L"\\\\?\\";
                    fullPath += desktop;
                    fullPath += L"\\";
                    fullPath += folderName;
                    RemoveDirectoryW(fullPath.c_str());
                }
            }
        }
    } while (FindNextFileW(hFind, &findData));

    // 4. 释放资源
    FindClose(hFind);
    return 0;
}
