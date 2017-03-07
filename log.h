#ifndef __LOG_H__
#define __LOG_H__
#include <sys/cdefs.h>
#define A_LOG_INFO   2
#define A_LOG_WARN   1
#define A_LOG_ERR    0

int __stm32_print_log(int level,const char *tag,const char *fmt,...) __printflike(3,4);

#define LOGI(tag,...) ((void)__stm32_print_log(A_LOG_INFO,tag,__VA_ARGS__))
#define LOGW(tag,...) ((void)__stm32_print_log(A_LOG_WARN,tag,__VA_ARGS__))
#define LOGE(tag,...) ((void)__stm32_print_log(A_LOG_ERR,tag,__VA_ARGS__))

#endif
