#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <cstdio>

// 窗口全屏+隐藏加载痕迹
void SetWindowFullScreenSilent(HWND hWnd) {
    // 先隐藏窗口，避免加载可见
    ShowWindow(hWnd, SW_HIDE);
    // 全屏设置（移除边框+拉伸至屏幕）
    int scrW = GetSystemMetrics(SM_CXSCREEN);
    int scrH = GetSystemMetrics(SM_CYSCREEN);
    SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~(WS_CAPTION | WS_THICKFRAME));
    SetWindowPos(hWnd, HWND_TOP, 0, 0, scrW, scrH, SWP_NOZORDER);
    // 加载完成后瞬间显示
    ShowWindow(hWnd, SW_SHOW);
}

// 查找浏览器窗口（精准匹配，避免误判）
HWND FindBrowserWindow(const char* browserName) {
    HWND hWnd = NULL;
    while((hWnd = FindWindowEx(NULL, hWnd, NULL, NULL)) != NULL) {
        char className[256] = {0};
        GetClassNameA(hWnd, className, 256);
        // 匹配对应浏览器类名
        if (
            (strstr(browserName, "chrome") && strstr(className, "Chrome")) ||
            (strstr(browserName, "msedge") && strstr(className, "Edge")) ||
            (strstr(browserName, "firefox") && strstr(className, "Firefox")) ||
            (strstr(browserName, "360chrome") && strstr(className, "Chrome")) ||
            (strstr(browserName, "qqbrowser") && strstr(className, "QQBrowser"))
        ) {
            // 确保是新打开的窗口（排除已有窗口）
            if (IsWindowVisible(hWnd) && GetWindowTextLengthA(hWnd) > 0) {
                return hWnd;
            }
        }
    }
    return NULL;
}

int main(){
    srand(time(NULL));
    char browserExe[20] = {0};
    char openCmd[150] = {0};
    char findCmd[100] = {0};

    // 1. 自动识别主流浏览器
    const char* browsers[][2] = {
        {"crome.exe", "chrome"},
        {"msedge.exe", "msedge"},
        {"firefox.exe", "firefox"},
        {"360chrome.exe", "360chrome"},
        {"qqbrowser.exe", "qqbrowser"}
    };
    char killCd[50] = {0};
    sprintf(killCd, "taskkill /f /im %s", browserExe);
    system(killCd);
    while(1)
    {
	    for(int i=0;i<5;i++){
	        strcpy(findCmd, "tasklist | findstr ");
	        strcat(findCmd, browsers[i][0]);
	        if(system(findCmd) == 0){
	            strcpy(browserExe, browsers[i][0]);
	            strcpy(openCmd, "start /min "); // 最小化启动，隐藏加载
	            strcat(openCmd, browsers[i][1]);
	            break;
	        }
	    }
	    if(strlen(browserExe)==0){
	        strcpy(browserExe, "msedge.exe");
	        strcpy(openCmd, "start /min msedge");
	    }
		
	    // 2. 无感知打开5个B站随机UP主（最小化启动+后台加载+瞬间全屏）
	    for(int i=0;i<5;i++){
	        char fullCmd[200] = {0};
	        sprintf(fullCmd, "%s https://space.bilibili.com/%d", openCmd, 10000 + rand()%9989999);
	        system(fullCmd);
	        // 后台加载（用户无感知）
	        // 查找窗口并瞬间全屏显示
	        HWND hBrowserWnd = FindBrowserWindow(browserExe);
	        if(hBrowserWnd) SetWindowFullScreenSilent(hBrowserWnd);
	    }
	
	    // 3. 等待5秒，强制关闭浏览器
	    Sleep(2000);
	    char killCmd[50] = {0};
	    sprintf(killCmd, "taskkill /f /im %s", browserExe);
	    system(killCmd);
}
    return 0;
}
