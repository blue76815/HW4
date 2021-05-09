#include <stdio.h>
#include <stdlib.h>
#include <string.h>//引進 memset
#include <assert.h>

#include <sys/file.h>
#include <sys/types.h>
#include <unistd.h>
#include <libgen.h>
#include <time.h>
//http://c.biancheng.net/cpp/html/258.html
// getc() 【返回值】讀到文件尾而無數據時便返回EOF

#define CH_WIDTH 80
int main(int argc, char **argv)
{
    if(argc!=5){
        printf("輸入參數不到4個\n");
        return 0;
    }    
    FILE* input= fopen(argv[1],"r");//開啟第一個輸入變數 source_file 
    FILE* output = fopen(argv[2],"w");//開啟第二個輸入變數 dest_file_1
    size_t  buffer_size;                //第三個參數則是設定buffer_size 
    sscanf(argv[3], "%ld", &buffer_size);//(第三個參數 可設定 0、-1、4KB、16KB、64KB、1MB、8MB)

    char wordBuf[280]={0x00};
    int bufLen=0;  // bufLen為還沒讀到空白鍵 先暫存到wordBuf[bufLen]內的 未完整英文單字
    int linePos=0; // linePos為字的位置
    char *setv_inputBuf,*setv_outputBuf;
    
    if(input==NULL){
        printf("file %s not exist\r\n",argv[1]);
        printf( "open failure\r\n" );
        return 0;
    }else{
        printf("開檔成功\r\n");
    }
    int setv_mode;//
    sscanf(argv[4], "%d", &setv_mode); //(第四個參數 可設定setvbuf()為 1：_IONBF  2:_IOLBF  3:_IOFBF
    printf("setvbuf() setv_mode=");
    switch (setv_mode)
    {
    case 1:
        printf("1：_IONBF\r\n");
        break;
    case 2:
        printf("2：_IOLBF\r\n");
        break;
    case 3:
        printf("3：_IOFBF\r\n");
        break;            
    default:
        printf("參數輸入錯誤：%d\r\n",setv_mode);
        return 0;
    }

    setv_inputBuf=(char*)malloc(sizeof(char)*buffer_size);
    setv_outputBuf=(char*)malloc(sizeof(char)*buffer_size);
    assert(setvbuf(input,setv_inputBuf,_IOLBF,buffer_size)==0);//新增這行
    assert(setvbuf(output,setv_outputBuf,_IOLBF,buffer_size)==0);//新增這行 

	while (1) {
		int inputInt = getc(input); // int getc(FILE *stream)// 指定的流stream獲取下一個字符（一個無符號字符），並把位置標識符往前移動。
        /*讀到 EOF（-1）代表已讀完檔案裡全部資料*/
        if (inputInt == EOF){ 
            printf("EOF\r\n");
            ///wordBuf[bufLen++] = inputChar; //EOF為（-1）值 為不可見字 不能寫入到檔案中
            if (linePos + bufLen > CH_WIDTH){ 
             /* 目前游標+將來要寫入的字串 該行已經容納不下字寬 得先提前換行*/
               fprintf(output,"\n"); 
            }
            fprintf(output, wordBuf, bufLen);
            memset(wordBuf,0x00,sizeof(wordBuf));
            bufLen=0;
            break;
        } 
		//將輸入轉成char來處理
		unsigned char inputChar =(unsigned char)inputInt;       
        /*讀到最後一個字'\n' （ascii值10）的時候*/
	if (inputChar == '\n') {
            /*結束 讀到最後一個字（ascii值10）*/
            wordBuf[bufLen] = inputChar;
            if(linePos + bufLen > CH_WIDTH)
            {
              fprintf(output,"\n%s", wordBuf); 
              //fprintf(output,"%s", wordBuf); 
            }else{
              fprintf(output,"%s", wordBuf); 
            }
            memset(wordBuf,0x00,sizeof(wordBuf));
            bufLen=0;
            linePos = 0;//換行時得歸零寫入檔案時的游標
            continue;
        }

        // /*讀到最後一個字'\n' （ascii值10）的時候*/
		// if (inputChar == '\n') {
        //     /*結束 讀到最後一個字（ascii值10）*/
        //     fprintf(output, "%s\n", wordBuf);
        //     memset(wordBuf,0x00,sizeof(wordBuf));
        //     bufLen=0;
        //     linePos = 0;//換行時得歸零寫入檔案時的游標
        //     continue;
        // }        
		//『空白』為單字的結束
	if (inputChar == ' '){ 

            /*讀到空格*/      
            if (linePos + bufLen+1 > CH_WIDTH) {
                /* (目前游標+將來要寫入的字串) 該行已經容納不下字寬 得先提前換行*/  
                fprintf(output,"\n"); // fprintf（）輸出時 已經自帶檔案寫入到 output檔案路徑指標內
                linePos = 0;//換行時得歸零寫入檔案時的游標
                fprintf(output,"%s ",wordBuf); 
                linePos=linePos + bufLen+1; //更新linePos寫入檔案的游標位置+1個空格
            }else{
                fprintf(output,"%s ",wordBuf); 
                linePos=linePos + bufLen+1; //更新linePos寫入檔案的游標位置+1個空格
            }
            memset(wordBuf,0x00,sizeof(wordBuf));
            bufLen=0;

            // /*讀到空格*/      
            // if (linePos + bufLen > CH_WIDTH) {
            //     /* (目前游標+將來要寫入的字串) 該行已經容納不下字寬 得先提前換行*/  
            //     fprintf(output,"\n"); // fprintf（）輸出時 已經自帶檔案寫入到 output檔案路徑指標內
            //     linePos = 0;//換行時得歸零寫入檔案時的游標
            // }
            // fprintf(output,"%s ",wordBuf); 
            // linePos=linePos + bufLen+1; //更新linePos寫入檔案的游標位置+1個空格
            // memset(wordBuf,0x00,sizeof(wordBuf));
            // bufLen=0;

            //**fprintf獨立寫" " 執行時 會出現 segment fault**/
            // fprintf(output,wordBuf,bufLen); 
            // linePos=linePos + bufLen; //更新linePos寫入檔案的游標位置,bufLen不含空格
            // memset(wordBuf,'\0',280);
            // bufLen=0;
            // fprintf(output," ");//ASCII的空白鍵值 
            // linePos++; //寫入的linePos游標位置加入1個"空格"
            continue;

		}//end if (inputChar == ' ')

		// //『空白』為單字的結束
		// if (inputChar == ' '){   
        //     /*讀到空格*/  
        //     bufLen++;    
        //     if (linePos + bufLen > CH_WIDTH) {
        //         /* (目前游標+將來要寫入的字串) 該行已經容納不下字寬 得先提前換行*/  
        //         fprintf(output,"\n%s",wordBuf);
        //         linePos = 0;//換行時得歸零寫入檔案時的游標
        //     }else{
        //         if(linePos==0)
        //             fprintf(output,"%s",wordBuf);
        //         else
        //             fprintf(output," %s",wordBuf);
        //     }
        //     linePos=linePos + bufLen; //更新linePos寫入檔案的游標位置                             
        //     memset(wordBuf,'\0',280);
        //     bufLen=0;
        //     continue;

		// }//end if (inputChar == ' ')
        
		//因為只有空白可以換行
		//因此把『.』算成單字的一部分
		wordBuf[bufLen++] = inputChar;
	}
    free(setv_inputBuf);
    free(setv_outputBuf);    
    fclose(input);//若前面fopen（input）失敗 則後面fclose(input);時 會出現 程式記憶體區段錯誤 (核心已傾印)
    fclose(output);    
	return 0;
}
