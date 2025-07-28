#include "clickmanager.h"
#include <stdio.h>
#include <stddef.h>
#include <windows.h>
#include <stdio.h>
#include "./3.hw/header/gl.h"
#include "./3.hw/header/glu.h"
#include "./3.hw/header/glut.h"
#include "mainmenu.h"
#include "gameplay.h"
#include "clickmanager.h"
#include <stdbool.h>
#include "main.h"
#include "settings.h"
#include "hud.h"



static ClickablePoint* g_points = NULL;
static int g_pointCount = 0;

void RegisterClickablePoints(ClickablePoint* points, int count){
    g_points = points;
    g_pointCount = count;
}


ClickablePoint mainMenuButtons[] = {
    {400, 275, 100, GoToGameUI},        // Buton 1 merkezi (x, y)
    {400, 375, 100, GoTosettingsUI},    // Buton 2 merkezi
};





ClickablePoint gameoverButtons[] = {
    {0, 0, 10000, GoToGameUI},
};




ClickablePoint gameUIButtons[] = {
    {650, 400, 150, GoToMainMenu},
};


void HandleClick(int x, int y){
    for(int i = 0; i < g_pointCount; i++){
        ClickablePoint* p = &g_points[i];
        int dx = x - p->x;
        int dy = y - p->y;
        int distSquared = dx*dx + dy*dy;
        if(distSquared <= p->radius * p->radius){
            if(p->callback)
                p->callback();
            break;
        }
    }
}

void MouseFunction(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        int oglY = SCREENHEIGHT - y; // GLUT penceresinde y ekseni ters olduğu için düzeltme

        if(currentState == STATE_MAIN_MENU){
            RegisterClickablePoints(mainMenuButtons, 1);
            HandleClick(x, oglY);
        }
        else if(currentState == STATE_GAMEUI){
            RegisterClickablePoints(gameUIButtons, 1);
            HandleClick(x, oglY);
        }
        else if(currentState == STATE_GAMEOVER){
            RegisterClickablePoints(gameoverButtons, 1);
            HandleClick(x, oglY);
            // GAMEOVER durumunda tıklama işlemleri
            // Örneğin, yeniden
        // Başka durumlar varsa onları da buraya ekleyebilirsin
    }
}
}



void SpecialKeyFunction(int key, int x, int y){
    if(currentState == STATE_GAMEUI){
        MoveCube(key);           // gameplay.c'deki fonksiyon
        glutPostRedisplay();     // Ekranı güncelle
    }
	fprintf(stdout, "Special Key Pressed: %d\n", key);
	
}
