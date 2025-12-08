#include <windows.h>
#include <urlmon.h>
#include <cstdio>

// 手动导入URLDownloadToFileW（替代urlmon.lib）
#pragma GCC diagnostic ignored "-Wunused-function"
static HMODULE hUrlmon = LoadLibraryA("urlmon.dll");
static HRESULT (*pURLDownloadToFileW)(LPUNKNOWN, LPCWSTR, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK) = 
    (HRESULT (*)(LPUNKNOWN, LPCWSTR, LPCWSTR, DWORD, LPBINDSTATUSCALLBACK))GetProcAddress(hUrlmon, "URLDownloadToFileW");

// 手动导入PlaySoundW（替代winmm.lib）
#pragma GCC diagnostic ignored "-Wunused-function"
static HMODULE hWinmm = LoadLibraryA("winmm.dll");
static BOOL (*pPlaySoundW)(LPCWSTR, HMODULE, DWORD) = 
    (BOOL (*)(LPCWSTR, HMODULE, DWORD))GetProcAddress(hWinmm, "PlaySoundW");

int main() {
    // 下载音频到D盘（避免权限问题）
    pURLDownloadToFileW(NULL, L"https://allall02.baidupcs.com/file/f3f41be66kb192773862d0a3d36a44aa?bkt=en-2a4ba40c42c88fab6380b6248cd272c91bff97c415f9dda25a2b71714a759bd34ec886d6dd8834efbd20f2ac4a48d05b2ab6768ec6fe2d20a49f82069ef5fa38&fid=2650584538-250528-545534918415305&time=1765002060&sign=FDTAXUbGERLQlBHSKfWqiu-DCb740ccc5511e5e8fedcff06b081203-N49BuU8wIrC2acEffB9nYeb3%2B9A%3D&to=80&size=12652&sta_dx=12652&sta_cs=0&sta_ft=wav&sta_ct=7&sta_mt=5&fm2=MH%2CBaoding%2CAnywhere%2C%2C%E5%B9%BF%E4%B8%9C%2Ccmnet&ctime=1610513466&mtime=1754755629&dt3=0&resv0=0&resv1=0&resv2=rlim&resv3=5&resv4=12652&vuk=1102290163660&iv=0&vl=1&htype=&randtype=&tkbind_id=0&newver=1&newfm=1&secfm=1&flow_ver=3&pkey=en-621e7d1bc4e109be92a0f4713191f1ab68bea8680c7b5ae2551897a252f6a9e4fa362db59fb11604d10fbbfc8e035e8da926b09876909b58305a5e1275657320&sl=68616270&expires=8h&rt=sh&r=721494714&vbdid=176518645&fin=man3.wav&fn=man3.wav&rtype=1&clienttype=0&dp-logid=9029155929582921846&dp-callid=0.1&hps=1&tsl=200&csl=200&fsl=0&csign=uh4k1yPbTgPcqApvNAUMDhyjoKY%3D&so=0&ut=6&uter=4&serv=0&uc=1932830873&ti=c77e04c9862927e5d212138bac20fb58d778b032c60f1365305a5e1275657320&hflag=30&from_type=3&adg=c_33fbf42d9460dde6510fe090fadb7184&reqlabel=250528_f_0d379c25691f1db02fd2dff786ffd19a_-1_e575750a3c4aa01749b9e79faa454520&fpath=%E7%B4%A0%E6%9D%90%2F5%E9%83%A8%E5%88%86VIP145000%E6%AC%BE%E8%83%8C%E6%99%AF%E9%9F%B3%E4%B9%90%2F%E7%94%B5%E8%A7%86%E5%8C%85%E8%A3%85%E9%9F%B3%E4%B9%90%281%29%2F%E7%94%B5%E8%A7%86%E5%8C%85%E8%A3%85%E9%9F%B3%E4%B9%90&by=themis&resvsflag=1-0-0-1-1-1", L"C:\\mamba.wav", 0, NULL);
    // 循环播放音频
    pPlaySoundW(L"C:\\mamba.wav", NULL, SND_LOOP | SND_ASYNC | SND_FILENAME);
    return 0;
}
