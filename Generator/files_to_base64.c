// 将 多个文件的数据 转换为base64字符串 base64.txt
// 需要让编译后的程序 和 所有文件 处于同一个目录下
// 注意：需要根据打印出的内容修改 main.c 的 decodeDatas 函数

#include <stdio.h>
#include <stdlib.h>

// Base64编码表
char *encode_table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

// 将数据datas编码为base64字符串
char* base64_encode(char *datas, int datas_length, int *encoded_str_length){
    // 计算最终base64字符串的长度
    *encoded_str_length = (datas_length + 2) / 3 * 4;

    // 申请内存空间
    char *encoded_str = (char*)malloc(*encoded_str_length + 1);
    char *encoded_ptr = encoded_str; 

    // 首先，3个字符一组，进行编码
    for(int i = 0, n = datas_length/3; i < n; ++i){
        encoded_ptr[0] = encode_table[(datas[0] >> 2) & 0x3F];
        encoded_ptr[1] = encode_table[((datas[0] << 4) & 0x30) | ((datas[1] >> 4) & 0x0F)];
        encoded_ptr[2] = encode_table[((datas[1] << 2) & 0x3C) | ((datas[2] >> 6) & 0x03)];
        encoded_ptr[3] = encode_table[datas[2] & 0x3F];
        encoded_ptr += 4;
        datas += 3;
    }

    // 当字符串长度不是3的倍数时，对剩下的字符进行编码
    int n = datas_length % 3;
    if(n == 1){
        encoded_ptr[0] = encode_table[(datas[0] >> 2) & 0x3F];
        encoded_ptr[1] = encode_table[(datas[0] << 4) & 0x30];
        encoded_ptr[2] = encode_table[64];
        encoded_ptr[3] = encode_table[64];
        encoded_ptr += 4;
    }
    else if(n == 2){
        encoded_ptr[0] = encode_table[(datas[0] >> 2) & 0x3F];
        encoded_ptr[1] = encode_table[((datas[0] << 4) & 0x30) | ((datas[1] >> 4) & 0x0F)];
        encoded_ptr[2] = encode_table[(datas[1] << 2) & 0x3C];
        encoded_ptr[3] = encode_table[64];
        encoded_ptr += 4;
    }

    *encoded_ptr = '\0';
    return encoded_str;
}

int main() {
    
    // 所有文件的文件名
    char *files[] = { "music.mid", "lyric.lrc", "pic1.bmp", "pic2.bmp" };
    
    // 文件数量(自动计算，不需要修改)
    int fileNums = sizeof(files) / sizeof(files[0]);

    FILE *fp_output = fopen("base64.txt", "w");

    for(int i = 0; i < fileNums; i++) {
        FILE *fp = fopen(files[i], "rb");

        // 获取文件大小
        fseek(fp, 0, SEEK_END);
        int fileSize = ftell(fp);
        rewind(fp);

        // 读取文件数据
        char *fileData = (char*)malloc(sizeof(char) * fileSize);
        fread(fileData, sizeof(char), fileSize, fp);

        // 转换为base64字符串
        int base64Size;
        char* base64Str = base64_encode(fileData, fileSize, &base64Size);

        // 打印出每个文件的base64字符串的的长度，需要根据打印出的内容修改 main.c 的 decodeDatas 函数
        printf("%s: %d\n", files[i], base64Size);
        
        // 将base64字符串写入文件
        fwrite(base64Str, sizeof(char), base64Size, fp_output);

        free(fileData);
        free(base64Str);
        fclose(fp);
    }

    fclose(fp_output);

    return 0;
}