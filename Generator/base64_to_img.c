// 将 base64.txt 的内容转换为字符画代码 img.c
// 需要让编译后的程序和 base64.txt, 以及二值图 Robin.bmp 处于同一个目录下

#include <stdio.h>
#include <windows.h>


// 图片中像素颜色的值(R, G, B值)
#pragma pack(1)
typedef struct {
    unsigned char B,G,R;
} Pixel, ColorBGR;


typedef struct {
    int width, height;
    Pixel *pixels;
} BitmapImage;


// 读取Bitmap图片
BOOLEAN readBitmap(const char* filePath, BitmapImage* image) {
    FILE *file = fopen(filePath, "rb");
    if(file == NULL)
        return FALSE;

    // 读取Bitmap的文件头部分
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, file);
    fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, file);
    int width = infoHeader.biWidth;
    int height = infoHeader.biHeight;

    // 读取Bitmap图片的数据
    Pixel *pixels = (Pixel*)malloc(sizeof(Pixel) * width * height);
    fread(pixels, sizeof(Pixel), width*height, file);

    // Bitmap图片的行顺序是上下颠倒的，这里把它颠倒回来
    Pixel* temp = (Pixel*)malloc(sizeof(Pixel) * width * height);
    for (int y = 0; y < height; y++)
        memcpy(temp+y*width, pixels+(height-y-1)*width, width*sizeof(Pixel));
    free(pixels);
    pixels = temp;

    image->width = width;
    image->height = height;
    image->pixels = pixels;

    fclose(file);
    return TRUE;
}


int main() {
    FILE *fp_output = fopen("img.c", "w");
    FILE *fp_base64 = fopen("base64.txt", "r");

    // 获取文件大小
    fseek(fp_base64, 0, SEEK_END);
    int base64Size = ftell(fp_base64);
    rewind(fp_base64);

    // 获取base64的数据
    char *base64Data = (char*)malloc(sizeof(char) * (base64Size + 1));
    fread(base64Data, sizeof(char), base64Size, fp_base64);
    base64Data[base64Size] = '\0';

    fclose(fp_base64);

    // 读取 Robin.bmp 的图片数据
    BitmapImage image;
    readBitmap("Robin.bmp", &image);

    fprintf(fp_output, "char s[] = \n");


    int state = 0; // state = 0 表示上一个像素的颜色是白色， state = 1 表示上一个像素的颜色是黑色
    int i = 0; 

    for(int y = 0; y < image.height; y++) {
        for(int x = 0; x < image.width; x++) {
            ColorBGR color = image.pixels[x + y * image.width];

            // 黑色像素
            if(color.R == 0) {

                // 如果上一个像素是白色，就写入一个引号
                if(state == 0) {
                    fputc('"', fp_output);
                    state = 1;
                }

                // 如果上一个像素是黑色，就写入base64字符串的数据
                else {
                    fputc(base64Data[i++], fp_output);
                }
            }

            // 白色像素
            else {

                // 如果上一个像素是白色，就写入空格
                if(state == 0) {
                    fputc(' ', fp_output);
                }

                // 如果上一个像素是黑色，就写入一个引号
                else {
                    fputc('"', fp_output);
                    state = 0;
                }
            }

            // 提前读取完了所有base64字符串的数据，就跳出循环
            if(i >= base64Size) {
                if(state == 1)
                    fputc('"', fp_output);
                goto end;
            }
        }

        // 在每行的末尾了，判断一下要不要添加一个引号
        if(state == 1) {
            fputc('"', fp_output);
            state = 0;
        }

        fputc('\n', fp_output);
    }

// 遍历完图片数据后，base64字符串还有多，就把剩下的base64字符串全部写入
if(i < (base64Size - 1)) {
    fputc('"', fp_output);
    fputs(base64Data + i, fp_output);
    fputc('"', fp_output);
}

end:
    fputc(';', fp_output);
    fflush(fp_output);

    fclose(fp_output);
    fclose(fp_base64);

    free(base64Data);
    free(image.pixels);

    return 0;
}