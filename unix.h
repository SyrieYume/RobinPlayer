#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#define __USE_XOPEN_EXTENDED
#include <unistd.h>
#include <signal.h>

#define error(message)        \
    {                         \
        printf(message "\n"); \
        fflush(stdout);       \
        getchar();            \
        exit(-1);             \
    }

// 一些 Windows 独有的数据类型
typedef unsigned char BYTE;
typedef int BOOLEAN;
#define FALSE 0
#define TRUE 1

// 停止信号, 用来响应 ^C （SIGINT）信号
volatile sig_atomic_t stop = 0;
void handle_sigint(int sig)
{
    stop = 1;
}

// 用于响应停止信号的宏
#define RESPONSE_STOP \
    if (stop)         \
        break;

// 使用 SDL_mixer 播放音乐
// 需要在编译时链接 -lSDL2 -lSDL2_mixer
// 需要在主程序退出前调用 Mix_CloseAudio() 和 SDL_Quit() 来清理资源
BOOLEAN playMusic(const char *filePath)
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        fprintf(stderr, "无法初始化 SDL: %s\n", SDL_GetError());
        return 0;
    }

    // 初始化 SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "无法初始化 SDL_mixer: %s\n", Mix_GetError());
        SDL_Quit();
        return 0;
    }

    // 加载 MP3 文件
    Mix_Music *music = Mix_LoadMUS(filePath);
    if (music == NULL)
    {
        fprintf(stderr, "无法加载音频文件: %s\n", Mix_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 0;
    }

    // 播放音乐
    if (Mix_PlayMusic(music, 1) == -1)
    {
        fprintf(stderr, "无法播放音频: %s\n", Mix_GetError());
        Mix_FreeMusic(music);
        Mix_CloseAudio();
        SDL_Quit();
        return 0;
    }

    return 1;
}

// 设置控制台的光标到 (x, y)
void setCursorPos(int x, int y)
{
    printf("\033[%d;%dH", y + 1, x + 1);
    fflush(stdout);
}