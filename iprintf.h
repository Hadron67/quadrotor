#ifndef __IPRINTF_H_
#define __IPRINTF_H_
#include <stdarg.h>
#include <sys/cdefs.h>
#define printf iprintf

extern int ifputc(int ch);

int va_iprintf(const char *fmt,va_list va);
int iprintf(const char *fmt,...) __printflike(1,2);


#endif

