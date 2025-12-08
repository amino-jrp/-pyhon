#include <windows.h>
#include <cstdlib>
#include <ctime>
int main(){
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

    return 0;
},NULL,0,NULL);

MessageBoxA(NULL,"内容","标题",MB_YESNO|MB_SYSTEMMODAL|MB_TOPMOST);
return 0;
}
