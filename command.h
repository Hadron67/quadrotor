#ifndef __COMMAND_H
#define __COMMAND_H
#include <stdint.h>
#include "loopqueue.h"

typedef struct _CommandDFA CommandDFA;

#define COMMAND_BUF_SIZE 20
#define COMMAND_STATE_SIZE 20

struct _CommandDFA{
    
    char buf[COMMAND_BUF_SIZE];
    int bptr;

    char state[COMMAND_STATE_SIZE];
    int sptr;

    int count;

    void (*onSetFloatParametre)(uint8_t addr,float val);
    void (*onSetMotor)(char c);
    void (*onSetThrottle)(int t);
    void (*onSetGyro)(uint8_t which,float val);
    void (*onGetStatus)();
};

int CommandDFA_init(CommandDFA *dfa);
int CommandDFA_accept(CommandDFA *dfa,char c);

#endif
