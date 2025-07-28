//başlatıcı ve ekrana çizici


#include <windows.h>
#include <stdio.h>
#include "./3.hw/header/gl.h"
#include "./3.hw/header/glu.h"
#include "./3.hw/header/glut.h"
#include "mainmenu.h"
#include "gameplay.h"
#include "clickmanager.h"
#include "clickmanager.h"

#define SCREENWIDTH         800
#define SCREENHEIGHT        480
#define TIMER_INTERVAL      1000


#define KEY_ESC             27

void Init_Globals(void);
void MouseFunction(int button, int state, int x, int y);
void KeyboardFunction(unsigned char key, int x, int y);
void TimerFunction(int val);
void DisplayFunction(void);

int TimerCounter;
int Minutes, Seconds;
int Color;
void InitRandom() {
    srand(time(NULL));  // Rastgelelik için başlangıç
}
typedef enum {
    STATE_MAIN_MENU,
    STATE_GAMEUI,
	STATE_GAMEOVER,
    STATE_SETTINGS,
    // ... başka state'ler eklersen buraya
} GameState;

GameState currentState = STATE_GAMEUI; // Başlangıç durumu

void SpecialKeyFunction(int key, int x, int y);


int main(int argc, char **argv){
    Init_Globals();
    
InitRandom();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - SCREENWIDTH) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - SCREENHEIGHT) / 2);
    glutCreateWindow("HMI Simulation with OpenGL");
	
    glShadeModel(GL_FLAT);
    glPointSize(1.0f);

    // ✅ Projection ayarları eklendi
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0f, (float)SCREENWIDTH, 0.0f, (float)SCREENHEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    fprintf(stdout, "Simulation started with OpenGL version: %s\n", glGetString(GL_VERSION));
    glutMouseFunc(MouseFunction);
    glutKeyboardFunc(KeyboardFunction);
    glutTimerFunc(TIMER_INTERVAL, TimerFunction, 0);
    glutDisplayFunc(DisplayFunction);
	glutSpecialFunc(SpecialKeyFunction); // Ok tuşları için

    glutMainLoop();

    exit(EXIT_SUCCESS);
    return 0;
}


void Init_Globals(void){
	TimerCounter = 0;
	Minutes = Seconds = 0;
	Color = 0;
}





void KeyboardFunction(unsigned char key, int x, int y){
	//fprintf(stdout,"Key Pressed: 0x%02x -> %03d\n", key, key);
	switch(key){
		case KEY_ESC:
			fprintf(stdout,"Exiting simulation\n");
			exit(EXIT_SUCCESS);
			break;
	}
}

//runs every 100ms

#include "mainmenu.h"
#include "gameplay.h"

int redrawRequired = 1; // Sadece state değişince yeniden çizim yapılsın


void DisplayFunction(void){
    if (!redrawRequired)// Eğer çizim gerekli değilse çık

    glClear(GL_COLOR_BUFFER_BIT);

    switch(currentState){
        case STATE_MAIN_MENU:
            DrawMainMenu();
          
			
            break;
        case STATE_GAMEUI:
		
            DrawScreennn();

           
            break;

		case STATE_GAMEOVER:
			DrawGameoverscreen();
			
		    break;
        case STATE_SETTINGS:
			DrawSettingsscreen();
			
		break;
        

    }
    //fpsi debug ver
  //zamanda saliseden daha hassas debug ver
 

    glFlush();
    glutSwapBuffers();

    redrawRequired = 0; // Çizim yapıldı
	

  
		
	

}













