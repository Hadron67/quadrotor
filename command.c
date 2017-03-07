#include <stdint.h>
#include <string.h>
#include "log.h"
#include "command.h"
#include "motor_control.h"

#define TAG "command"

#define CS() (dfa->state[dfa->sptr])
#define PUSHS(s) if(dfa->sptr < COMMAND_STATE_SIZE) dfa->state[++dfa->sptr] = (s);
#define POPS() if(dfa->sptr > 0) dfa->sptr--;

#define SWITCH(cond,s) if(cond) CS() = (s)
#define SWITCHTO(s) CS() = (s)
#define PUSHCHAR(c) if(dfa->bptr < COMMAND_BUF_SIZE) dfa->buf[dfa->bptr++] = (c)
#define POPCHAR() (dfa->buf[--dfa->bptr])
#define RESETCHAR() dfa->bptr = 0
#define GETCHAR(i) (dfa->buf[dfa->bptr - (i)])

#define ERR() goto err;

int CommandDFA_init(CommandDFA *dfa){
	dfa->bptr = dfa->sptr = 0;
	dfa->state[0] = 0;
	return 0;
}

static int emitSetFloatParametre(CommandDFA *dfa,uint8_t addr,float val){
	if(dfa->onSetFloatParametre){
		dfa->onSetFloatParametre(addr,val);
	}
	return 0;
}

static int emitSetMotor(CommandDFA *dfa,char c){
	if(dfa->onSetMotor){
		dfa->onSetMotor(c);
	}
	return 0;
}

static int emitSetThrottle(CommandDFA *dfa,int t){
	if(dfa->onSetThrottle){
		dfa->onSetThrottle(t);
	}
	return 0;
}

static int emitSetGyro(CommandDFA *dfa,uint8_t which,float val){
	if(dfa->onSetGyro){
		dfa->onSetGyro(which,val);
	}
	return 0;
}

static int emitGetStatus(CommandDFA *dfa){
	if(dfa->onGetStatus){
		dfa->onGetStatus();
	}
	return 0;
}



static int getRaw(CommandDFA *dfa,void *src,size_t size){
	char *t = (char *)src;
	int i;
	for(i = 0;i < size;i++){
		t[i] = POPCHAR();
	}

	return 0;
}

/*

	data formats:

	set a PID parametre: ^p<CHAR><FLOAT>$

	start/stop motor: ^m<CHAR>$

	set throttle: ^t<INT>$

	set orientation: ^g<CHAR><FLOAT>$

	get status: ^s$

 */
int CommandDFA_accept(CommandDFA *dfa,char c){
	switch(CS()){
		case 0:
			if(c == '^'){
				SWITCHTO(1);
			}
			break;
		case 1://main start
			switch(c){
				case 'p': SWITCHTO(20);break;
				case 'm': SWITCHTO(30);break;
				case 't': SWITCHTO(40);break;
				case 'g': SWITCHTO(50);break;
				case 's': SWITCHTO(60);break;
				default:ERR();
			}
			break;

	//set parametre
		case 20:
			SWITCHTO(21);
			PUSHCHAR(c);
			PUSHS(10);
			dfa->count = 4;
			break;
		case 21:
			if(c == '$'){
				float val;
				getRaw(dfa,&val,sizeof(float));
				char which = POPCHAR();
				emitSetFloatParametre(dfa,which,val);
				SWITCHTO(0);
			}
			else {
				ERR();
			}
			break;

	//start/stop motor
		case 30:
			PUSHCHAR(c);
			SWITCHTO(31);
			break;
		case 31:
			if(c == '$'){
				emitSetMotor(dfa,POPCHAR());
				SWITCHTO(0);
			}
			else {
				ERR();
			}
			break;
	
	//set throttle
		case 40:
			dfa->count = 3;
			PUSHCHAR(c);
			SWITCHTO(41);
			PUSHS(10);
			break;
		case 41:
			if(c == '$'){
				int t;
				getRaw(dfa,&t,sizeof(int));
				emitSetThrottle(dfa,t);
				SWITCHTO(0);
			}
			else {
				ERR();
			}
			break;

	//set orientation
		case 50:
			PUSHCHAR(c);
			SWITCHTO(51);
			PUSHS(10);
			dfa->count = 4;
			break;
		case 51:
			if(c == '$'){
				float val;
				getRaw(dfa,&val,sizeof(float));
				char which = POPCHAR();
				emitSetGyro(dfa,which,val);
				SWITCHTO(0);
			}
			else {
				ERR();
			}
			break;
	
	//get status
		case 60:
			if(c == '$'){
				emitGetStatus(dfa);
				SWITCHTO(0);
			}
			else {
				ERR();
			}
			break;

	//function INT(count)
		case 10:
			PUSHCHAR(c);
			if(--dfa->count <= 0){
				POPS();
			}
			break;
		default:;
	}
	return 0;
err:
	CommandDFA_init(dfa);
	return -1;
}