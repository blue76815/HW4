#include <stdio.h>
#include <stdlib.h>
#include <string.h>//引進 memset
//http://c.biancheng.net/cpp/html/258.html
// getc() 【返回值】讀到文件尾而無數據時便返回EOF
int main(int argc, char **argv)
{
    // if(argc!=4){
    //     printf("輸入參數不到3個\n");
    // }    
    FILE* input= fopen(argv[1],"r");//開啟第一個輸入變數 source_file 
    FILE* output = fopen(argv[2],"w+");//開啟第二個輸入變數 dest_file_1
    char wordBuf[80]={0x00};
    int bufLen=0;  // bufLen為還沒讀到空白鍵 先暫存到wordBuf[bufLen]內的 未完整英文單字
    int linePos=0; // linePos為字的位置
//	setvbuf(input);  //新增這行
//	setvbuf(output); //新增這行 

    if(input==NULL){
        printf("file %s not exist\r\n",argv[1]);
        printf( "open failure\r\n" );
        return 0;
    }else{
        printf("開檔成功\r\n");
    }
	while (1) {
		int inputInt = getc(input); // int getc(FILE *stream)// 指定的流stream獲取下一個字符（一個無符號字符），並把位置標識符往前移動。
		printf("inputInt=%c\r\n",inputInt);
        /*讀到 EOF（-1）代表已讀完檔案裡全部資料*/
        if (inputInt == EOF){ 
            printf("EOF\r\n");
            ///wordBuf[bufLen++] = inputChar; //EOF為（-1）值 為不可見字 不能寫入到檔案中
            fprintf(output, wordBuf, bufLen);
            memset(wordBuf,0x00,sizeof(wordBuf));
            bufLen=0;
            break;
        } 
		//將輸入轉成char來處理
		unsigned char inputChar =(unsigned char)inputInt;
		//因為重新排版，因此忽略換行
        
        /*讀到最後一個字'\n' （ascii值10）的時候*/
		if (inputChar == '\n') 
        {
            wordBuf[bufLen++] = inputChar;
            printf("結束 讀到最後一個字（ascii值10）\r\n");
            continue;
        }
		//『空白』為單字的結束
		if (inputChar == ' ') 
        {
            printf("讀到空格\n");
			bufLen++;
            printf("linePos:%d bufLen:%d \r\n",linePos,bufLen);
            /*該行已經容納不下，需要換行了*/
            if (linePos + bufLen > 10) {
                printf("該行已經容納不下字寬 得先提前換行\r\n");
                fprintf(output,"\n"); // fprintf（）輸出時 已經自帶檔案寫入到 output檔案路徑指標內
             //去掉開頭的空白
                bufLen--;//扣掉前面多讀到的' '字元
                linePos = 0;
            }else{
                linePos++;
                wordBuf[bufLen++] = inputChar;//在字寬內，則也要把' '也要存入wordBuf[]內
            }
            fprintf(output, wordBuf, bufLen);
            bufLen=0;
            memset(wordBuf,0x00,sizeof(wordBuf));
            linePos=linePos + bufLen;
            continue;

		}//end if (inputChar == ' ')
		//因為只有空白可以換行
		//因此把『.』算成單字的一部分
		wordBuf[bufLen++] = inputChar;
	}
    fclose(input);//若前面fopen（input）失敗 則後面fclose(input);時 會出現 程式記憶體區段錯誤 (核心已傾印)
    fclose(output);    
	return 0;
}