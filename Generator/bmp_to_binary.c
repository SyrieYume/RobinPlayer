// 将彩色BMP图片 Robin1.bmp 转换为二值图 Robin_binary.bmp
// 需要让编译后的程序和 Robin1.bmp 处于同一个目录下

#include <windows.h>
#include <stdio.h>

// 图片中像素颜色的值(R, G, B值)
#pragma pack(1)
typedef struct {
    unsigned char B,G,R;
} Pixel, ColorBGR;


int main() {
    FILE *fp = fopen("Robin1.bmp", "rb");
    if(fp == NULL)
        return FALSE;

    // 读取Bitmap的文件头部分
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;

    // 读取Bitmap图片的数据
    Pixel *pixels = (Pixel*)malloc(sizeof(Pixel) * width * height);
    fread(pixels, sizeof(Pixel), width*height, fp);

    fclose(fp);

    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {
            int i = x + y * width;

            // 最简单的灰度计算方法
            // unsigned char gray = ((int)pixels[i].R + (int)pixels[i].G + (int)pixels[i].B) / 3;

            // 更好的灰度计算方法
            unsigned char gray = (unsigned char)(pixels[i].R * 0.299f + pixels[i].G * 0.587f + (int)pixels[i].B * 0.114f);

            // 根据阈值将颜色设置为黑色或者白色
            if(gray > 195) {
                pixels[i].R = 255;
                pixels[i].G = 255;
                pixels[i].B = 255;
            }

            else {
                pixels[i].R = 0;
                pixels[i].G = 0;
                pixels[i].B = 0;
            }
        }
    }

    // 写入输出文件
    FILE *fp_output = fopen("Robin_binary.bmp", "wb");
    fwrite(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fp_output);
    fwrite(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fp_output);
    fwrite(pixels, sizeof(Pixel), width * height, fp_output);
    fflush(fp_output);
    fclose(fp_output);

    return 0;
}