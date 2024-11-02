# RobinPlayer

在控制台播放《使一颗心免于哀伤》

## 项目结构

**`main.c`** : 最终程序 RobinPlayer.exe 的代码  

**`res`** : 项目使用到的资源文件

**`Generator/bmp_to_binary.c`** : 将 彩色BMP图片 转换为 二值图 的程序代码  

**`Generator/files_to_base64.c`** : 将 文件 转换为 base64字符串 的程序代码  

**`Generator/base64_to_img.c`** : 将 base64字符串 转换为 字符画代码 的程序代码  

**`win.h` 和 `unix.h`** : 平台相关的代码，用于处理不同平台的终端控制、音频播放和时间获取

## 如何运行本程序

### Windows 10 及以上版本

#### 从可执行文件

1. 从 [Release](https://github.com/SyrieYume/RobinPlayer/releases) 下载 `RobinPlayer.exe`。
2. 双击运行程序。
3. 将终端窗口调整到合适大小。
4. 尽情欣赏这首《使一颗心免于哀伤》吧！

#### 从源代码

使用的编译器为 **MinGW** (gcc version 14.2.0)

```bash
gcc -o RobinPlayer.exe main.c
```

⚠ **编译注意事项**

1. 使用C语言编译器，而不是C++编译器 (**Visual Studio** 中需要将源文件后缀名改为 `.c` 而不是 `.cpp`)。
2. 在 Visual Studio 上编译的时候，需要在 `"项目"->"属性"->"C/C++"->"命令行"->"其他选项"` 中加入`/utf-8`,否则可能会出现乱码。
3. 不要直接复制粘贴代码到 Visual Studio，否则排版可能会乱，并且可能会出现编码问题。
4. 如果出现编译错误`C2001:常量中有换行符`，需要在Visual Studio的 **高级保存选项** 中将编码改为`Unicode(UTF-8 带签名)-代码页65001`。
   
   打开 **高级保存选项** 的方法：
   工具 -> 自定义 -> 命令，在"菜单栏"项选择"文件"，点击"添加命令"，在"类别栏"中选择"文件"，在"命令栏"中找到"高级保存选项"，单击"确定"保存。在 Viusal Studio 主界面工具栏的"文件"中即可找到高级保存选项。

### Unix/Linux

1. 安装依赖

   ```bash
   sudo apt-get update && sudo apt-get install libsdl2-dev libsdl2-mixer-dev
   ```

2. 下载并编译

   ```bash
   git clone https://github.com/SyrieYume/RobinPlayer.git
   gcc -o RobinPlayer main.c -lSDL2 -lSDL2_mixer
   ```

3. 运行

   ```bash
   ./RobinPlayer
   ```
