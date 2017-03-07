
#include <math.h>
#include "iprintf.h"
#define PUTC(b) ifputc(b)

static void print_int(int i){
    char buf[20];
    char *bptr = buf;
    if(i == 0){
        PUTC('0');
        return;
    }
    if(i < 0){
        PUTC('-');
        i = -i;
    }
    while(i > 0){
        *bptr++ = i % 10 + '0';
        i /= 10;
    }
    while(bptr != buf){
        PUTC(*(--bptr));
    }
}
static void print_double(double d,int precision){
    if(d < 0){
        PUTC('-');
        d = -d;
    }
    else if(d == 0){
        PUTC('0');
        return;
    }
    double in = floor(d);
    double decimal = d - in;
    print_int(in);
    PUTC('.');
    while(decimal > 0 && precision > 0){
        decimal *= 10;
        PUTC((int)decimal + '0');
        decimal = decimal - (int) decimal;
        precision--;
    }
}
static void print_string(const char *s){
    if(s){
        while(*s){
            PUTC(*s++);
        }
    }
    else
        print_string("(null)");
}

int va_iprintf(const char *fs,va_list arg_ptr){
    while(*fs){
        if(*fs == '%'){
            fs++;
            if(*fs && *fs == 'c'){
                fs++;
                PUTC(va_arg(arg_ptr,int));
            }
            else if(*fs && *fs == 'd'){
                fs++;
                print_int(va_arg(arg_ptr,int));
            }
            else if(*fs && *fs == 'X'){
                fs++;
                //XXX: should print hex
                print_int(va_arg(arg_ptr,int));
            }
            else if(*fs && *fs == 'x'){
                fs++;
                //XXX: should print hex
                print_int(va_arg(arg_ptr,int));
            }
            else if(*fs && *fs == 'f'){
                fs++;
                print_double(va_arg(arg_ptr,double),3);
            }
            else if(*fs && *fs == 's'){
                fs++;
                print_string(va_arg(arg_ptr,const char *));
            }
            else{
                PUTC('%');
            }
        }
        else if(*fs == '\\'){
            fs++;
            if(*fs && *fs == 'n'){
                fs++;
                PUTC('\n');
            }
            else{
                PUTC('\\');
            }
        }
        else{
            PUTC(*fs);
            fs++;
        }
    }
    return 0;
}
int iprintf(const char *fmt,...){
    int ret;
    va_list arg_ptr;
    va_start(arg_ptr,fmt);
    ret = va_iprintf(fmt,arg_ptr);
    va_end(arg_ptr);
    return ret;
    
}
