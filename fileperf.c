#include <stdio.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    // if(argc!=4){
    //     printf("輸入參數不到3個\n");
    // }
    FILE* input;
    printf("argv[1]=%s\r\n",argv[1]);
    input = fopen(argv[1],"r");//開啟第一個輸入變數 source_file 
    if(input==NULL)
    {
        printf("file %s not exist\r\n",argv[1]);
        printf( "open failure\r\n" );
    }else{
        printf("開檔成功\r\n");
        fclose(input);//若前面fopen（input）失敗 則後面fclose(input);時 會出現 程式記憶體區段錯誤 (核心已傾印)
    }
    
	return 0;
}
