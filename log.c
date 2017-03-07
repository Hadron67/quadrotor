#include "iprintf.h"
#include "log.h"
#define TAG "log"

static const char *levelName[] = { "ERR","WARN","INFO" };
int __stm32_print_log(int level,const char *tag,const char *fmt,...){
    int ret;
    iprintf("[%s/%s]: ",tag,levelName[level]);
    va_list arg_ptr;
    va_start(arg_ptr,fmt);
    ret = va_iprintf(fmt,arg_ptr);
    va_end(arg_ptr);
    iprintf("\n");
    return ret;
}

void assert_failed(char *file,int line){
    LOGE(TAG,"assertion failed,at %s:%d",file,line);
}