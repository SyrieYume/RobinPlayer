# RobinPlayer

在控制台播放《使一颗心免于哀伤》

这个分支是 Unix 版本，Winows 版本请查看 main 分支

## 如何使用

1. 安装依赖

   ```bash
   sudo apt-get update && sudo apt-get install libsdl2-dev libsdl2-mixer-dev
   ```

2. 下载并编译

   ```bash
   git clone https://github.com/SyrieYume/RobinPlayer.git
   cd RobinPlayer
   git checkout unix
   gcc -o main main.c -lSDL2 -lSDL2_mixer
   ```

3. 运行

   ```bash
   ./main
   ```
