#include <windows.h>
#include <urlmon.h>
#include <cstdio>

// 手动加载DLL解决库链接问题（无需编译参数）
int main(){
    // 加载urlmon.dll并获取下载函数
    HMODULE hUrlmon = LoadLibraryA("urlmon.dll");
    typedef HRESULT (*URLFunc)(LPUNKNOWN, LPCWSTR, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK);
    URLFunc URLDownloadToFileW = (URLFunc)GetProcAddress(hUrlmon, "URLDownloadToFileW");
    
    // 下载图片+设为壁纸（强制类型转换修复报错）
    URLDownloadToFileW(NULL,L"https://img2.baidu.com/it/u=3210685500,3239719857&fm=253&fmt=auto&app=138&f=JPEG?w=722&h=829",L"C:\\wall.jpg",0,NULL);
    SystemParametersInfoW(SPI_SETDESKWALLPAPER,0,(void*)L"C:\\wall.jpg",SPIF_UPDATEINIFILE|SPIF_SENDCHANGE);
    
    FreeLibrary(hUrlmon); // 释放库
    return 0;
}
