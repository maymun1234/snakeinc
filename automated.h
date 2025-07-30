#ifndef AUTOMATED_H
#define AUTOMATED_H
#include <stdbool.h>

void  automationmanager();
void idleFunc();
void automatedTimer(int val);
bool isspeed =true;
bool isautomated = false;
typedef enum {
    LINEARSCAN,
	BASICAUTOMATED,
    USERINPUT,
    MOUSEMOVE,
    ALLRANDOM,
    // ... başka state'ler eklersen buraya
} AutonomDirection;

AutonomDirection currentdireciton = MOUSEMOVE; // Başlangıç yönü
const char* getDirectionName(AutonomDirection dir);

#endif