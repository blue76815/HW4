#include <stdio.h>

int main(int argc, char **argv)
{
    if(argc==4){
        printf("argc=%d\n",argc);
        printf("argv[0]=%s\n",argv[0]);
        printf("argv[1]=%s\n",argv[1]);
        printf("argv[2]=%s\n",argv[2]);
        printf("argv[3]=%s\n",argv[3]);
    }else{

        printf("輸入參數不到3個\n");
    }
	return 0;
}
