#include <io.h>
#include <windows.h>

// 因为使用了 pause，所以这个宏也是平台相关的
#define error(message)        \
    {                         \
        printf(message "\n"); \
        fflush(stdout);       \
        system("pause");      \
        exit(-1);             \
    }

HANDLE hConsole; // 控制台句柄

// 时钟相关，转为 Windows 下的 Sleep 函数
#define usleep(ns) Sleep(ns / 1000000)
#define sleep(s) Sleep(s)

// 开启Windows的虚拟终端序列支持
BOOLEAN enableVTMode()
{
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x4
#endif
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE)
        return FALSE;

    DWORD dwMode = 0;
    if (!GetConsoleMode(hOut, &dwMode))
        return FALSE;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!SetConsoleMode(hOut, dwMode))
        return FALSE;

    return TRUE;
}

// 设置控制台的光标到 (x, y)
void setCursorPos(int x, int y)
{
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(hConsole, coord);
}

// 调用Windows的API播放mid音频 (ps: 这个API用来播放mp3音乐也是可以的~)
BOOLEAN playMusic(const char *filePath)
{
    HMODULE module = LoadLibraryA("winmm.dll");

    typedef MCIERROR(WINAPI * MciSendStringT)(LPCSTR lpstrCommand, LPSTR lpstrReturnString, UINT uReturnLength, HWND hwndCallback);

    MciSendStringT func_mciSendStringA = (MciSendStringT)GetProcAddress(module, "mciSendStringA");
    if (func_mciSendStringA == NULL)
        return FALSE;

    char buff[255], command[100];

    sprintf_s(command, 100, "open %s alias playsound_134", filePath);
    func_mciSendStringA(command, buff, 254, NULL);

    sprintf_s(command, 100, "set playsound_134 time format milliseconds");
    func_mciSendStringA(command, buff, 254, NULL);

    sprintf_s(command, 100, "status playsound_134 length");
    func_mciSendStringA(command, buff, 254, NULL);

    sprintf_s(command, 100, "play playsound_134 from 0 to %s", buff);
    func_mciSendStringA(command, buff, 254, NULL);

    return TRUE;
}