#include <stdio.h>
#include <windows.h>
#include "gameplay.h"
#include "main.h"
#include "./3.hw/header/gl.h"
#include "./3.hw/header/glu.h"
#include "./3.hw/header/glut.h"
#include <stdbool.h>
#include <stdlib.h>  // rand, srand
#include <time.h>    // time
#include "hud.h"
#include "automated.h"

#include "food.h" // Elma ile ilgili fonksiyonlar
//tick kullanılıyor zaman için

int TimerCounter;
int Minutes, Seconds;
int Color;



ULONGLONG lastMoveTime = 0;
#define MOVE_INTERVAL_MS 100
void automatedmovement(void);
void automove(int val);
void idleFunc();
void linearscan();
void mousescan(void);








int besttiletogo[2] = {1, 1};

bool tilecontrol(int besttiletogo[2]);
bool isturnsafe(Direction turnvektor);


void idleFunc() {
    if (currentState == STATE_GAMEUI) {
        switch (currentdireciton) {
            case BASICAUTOMATED:
                automatedmovement();
                break;

            case LINEARSCAN:
                linearscan();
                break;
            case USERINPUT:
                // Kullanıcı girişi bekleniyor, hiçbir şey yapma
                break;
            case MOUSEMOVE:
            mousescan();
                // Fare girişi bekleniyor, hiçbir şey yapma
                break;
            case ALLRANDOM:
                break;
                
        }
         
        // --- Sabit hızla ileri gitmek isteniyorsa ---
        if (isspeed) {
            automove(0); // val parametresi zaten kullanılmıyor
        }
    }

    glutPostRedisplay();
}



int cubbeupdatedpos[2];
void automationmanager(void) {
	

cubbeupdatedpos[0] = (int)(cubbbepos[0] / 40);  // X tile index
cubbeupdatedpos[1] = (int)(cubbbepos[1] / 40);  // Y tile index
   
        glutIdleFunc(idleFunc);
    

  
}


bool  isinrealtime(void);
void automatedTimer(int val) {
    if (isautomated) {
        automatedmovement();
    }

    
}

void TimerFunction(int val){
	TimerCounter++;
	if(TimerCounter >= 10){
		TimerCounter = 0;
		Seconds++;
		if(Seconds >= 60){
			Seconds = 0;
			Minutes++;
			if(Minutes >= 60)
				Minutes = 0;
		}
		//fprintf(stdout,"Time->%02d:%02d\n", Minutes, Seconds);
		Color++;
		if(Color > 2)
			Color = 1;
	}
	glutTimerFunc(TIMER_INTERVAL, TimerFunction, 0);
	glutPostRedisplay();}



int moveid= 0;

void automatedmovement(void) {
  //  fprintf(stdout, "Automated movement started at: \n");
    if (currentState != STATE_GAMEUI) return;

    ULONGLONG currentTime = GetTickCount64();
    
    if (currentTime - lastMoveTime < 100) return;
    lastMoveTime = currentTime;

    int dx = foodPosition[0] - cubbbepos[0];
    int dy = foodPosition[1] - cubbbepos[1];

    // Öncelik Y: Yukarı-Aşağı
    if (abs(dy) > abs(dx)) {
        if (dy > 0 && snakevector != DOWN && isturnsafe(UP)) {
            MoveCube(GLUT_KEY_UP);
            return;
        } else if (dy < 0 && snakevector != UP && isturnsafe(DOWN)) {
            MoveCube(GLUT_KEY_DOWN);
            return;
        }
    }

    // Öncelik X: Sağa-Sola
    if (dx > 0 && snakevector != LEFT && isturnsafe(RIGHT)) {
        MoveCube(GLUT_KEY_RIGHT);
        return;
    } else if (dx < 0 && snakevector != RIGHT && isturnsafe(LEFT)) {
        MoveCube(GLUT_KEY_LEFT);
        return;
    }

    // Alternatif yön (örneğin tıkanmışsa)
    if (snakevector != DOWN && isturnsafe(UP)) {
        MoveCube(GLUT_KEY_UP);
    } else if (snakevector != UP && isturnsafe(DOWN)) {
        MoveCube(GLUT_KEY_DOWN);
    } else if (snakevector != RIGHT && isturnsafe(LEFT)) {
        MoveCube(GLUT_KEY_LEFT);
    } else if (snakevector != LEFT && isturnsafe(RIGHT)) {
        MoveCube(GLUT_KEY_RIGHT);
    }

	

}

 


bool isinrealtime(void){
	if (currentState == STATE_GAMEUI){
		return true;
	}

	else
		return false;
	
}

bool fjfrf = true;
ULONGLONG lastAutoMoveTime = 100000; // otomatik hareket için ayrı zamanlayıcı

void automove(int val) {
    if (!(isspeed && currentState == STATE_GAMEUI)) return;

    ULONGLONG currentTime = GetTickCount64();

    // Sadece belli aralıklarla çalışsın (örnek: 500 ms)
    if (currentTime - lastAutoMoveTime < 2000000) return;
    lastAutoMoveTime = currentTime;

    fprintf(stdout, "Yılanın yönü: %s\n", 
        (snakevector == UP) ? "UP" : 
        (snakevector == DOWN) ? "DOWN" : 
        (snakevector == LEFT) ? "LEFT" : 
        (snakevector == RIGHT) ? "RIGHT" : "UNKNOWN");

    switch (snakevector) {
        case UP:    MoveCube(GLUT_KEY_UP);    break;
        case DOWN:  MoveCube(GLUT_KEY_DOWN);  break;
        case LEFT:  MoveCube(GLUT_KEY_LEFT);  break;
        case RIGHT: MoveCube(GLUT_KEY_RIGHT); break;
    }

    fprintf(stdout, "Sabit yönle hareket.\n");
}



bool isturnsafe(Direction turnvektor){
    // Hedef koordinatı belirle
	

	    switch(turnvektor){
        case UP:
            besttiletogo[0] = cubbeupdatedpos[0];
            besttiletogo[1] = cubbeupdatedpos[1] + 1;
            break;
        case DOWN:
            besttiletogo[0] = cubbeupdatedpos[0];
            besttiletogo[1] = cubbeupdatedpos[1] - 1;
            break;
        case LEFT:
            besttiletogo[0] = cubbeupdatedpos[0]-1;
            besttiletogo[1] = cubbeupdatedpos[1];
            break;
        case RIGHT:
            besttiletogo[0] = cubbeupdatedpos[0]+1;
            besttiletogo[1] = cubbeupdatedpos[1];
            break;
        default:
            return false;
    }

    // Çakışma kontrolü
    if(tilecontrol(besttiletogo)){
       // fprintf(stdout, "yon guvenli: [%d, %d]\n", besttiletogo[0], besttiletogo[1]);
        return true;
    } else {
    //    fprintf(stdout, "yon tehlikeli: [%d, %d]\n", besttiletogo[0], besttiletogo[1]);
        return false;
    }
}




bool tilecontrol(int besttiletogo[2]) {
//	fprintf(stdout, "kontrol ediliyor0\n");


	if(besttiletogo[0]<0  || besttiletogo[1]<0){
		return false;
	} else if(besttiletogo[0]>19  || besttiletogo[1]>11){
	return false;
	}
    for (int i = 0; i < 20; i++) {
        if (lasttiles[i][0] == besttiletogo[0] && lasttiles[i][1] == besttiletogo[1]) {
			fprintf(stdout, "cakismavar\n");
            return false; // Çakışma var
        }

		if(lasttiles[snakelenght - 1][0] ==besttiletogo[0] &&lasttiles[snakelenght - 1][1]  ==besttiletogo[1]){
			fprintf(stdout, "cakismavar\n");
            return false; // Çakışma var
		}
    }
	//fprintf(stdout, "devmaeidliyor\n");
    return true; // Güvenli
}

Direction linearscanvector;
Direction linearscanvector2;


void linearscan(void) {
    ULONGLONG currentTime = GetTickCount64();

    if (currentTime - lastMoveTime < 30) return;
    lastMoveTime = currentTime;
    fprintf(stdout, "Lineer tarama yönü: %s\n", 
        (linearscanvector == UP) ? "UP" : 
        (linearscanvector == DOWN) ? "DOWN" : 
        (linearscanvector == LEFT) ? "LEFT" : 
        (linearscanvector == RIGHT) ? "RIGHT" : "UNKNOWN");
    // Eğer hedefe ulaşıldıysa fonksiyonu durdur



    if (cubbbepos[0]/40 == 0 && cubbbepos[1]/40 == 11) {
        fprintf(stdout, "Hedefe ulaşıldı, tarama durduruldu.\n");
        MoveCube(GLUT_KEY_RIGHT); // Hedefe ulaşıldığında sola dön
        linearscanvector = DOWN; // Tarama yönünü yukarı olarak ayarla
        linearscanvector2 = RIGHT; // İkinci tarama yönünü aşağı olarak ayarla
       
    }
    if (cubbbepos[0]/40 == 19 && cubbbepos[1]/40 == 0) {
        fprintf(stdout, "Hedefe ulaşıldı, tarama durduruldu.\n");
        MoveCube(GLUT_KEY_LEFT); // Hedefe ulaşıldığında sola dön
        linearscanvector = UP; // Tarama yönünü yukarı olarak ayarla
        linearscanvector2 = LEFT; // İkinci tarama yönünü aşağı olarak ayarla
       
    }
    if (cubbbepos[0] == 0 && cubbbepos[1] == 10) {
        return;
        fprintf(stdout, "Hedefe ulaşıldı, tarama durduruldu.\n");
    }

    if (linearscanvector == UP) {
     //   fprintf(stdout, "Tarama yönü: UP\n");
        
        // Yukarı doğru hareket
        if (cubbbepos[1]/40 < 11) {
            MoveCube(GLUT_KEY_UP);
            fprintf(stdout, "Yukarı doğru hareket ediliyor\n");
            
        } else if (cubbbepos[1]/40 == 11) {
            if(linearscanvector2 == RIGHT){
                MoveCube(GLUT_KEY_RIGHT);
                fprintf(stdout, "Yukarı tarama tamamlandı, sağa dönülüyor\n");
            } else {
                MoveCube(GLUT_KEY_LEFT);
                fprintf(stdout, "Yukarı tarama tamamlandı, sola dönülüyor\n");
            }
         
		//	fprintf(stdout, "Yukarı tarama tamamlandı, sola dönülüyor\n");
            linearscanvector = DOWN;
        }
    } 
    
    else if (linearscanvector == DOWN) {
        // Aşağı doğru hareket
        if (cubbbepos[1]/40 > 0) {
            MoveCube(GLUT_KEY_DOWN);
        } else if (cubbbepos[1] == 0) {
            if(linearscanvector2 == RIGHT){
                MoveCube(GLUT_KEY_RIGHT);
                fprintf(stdout, "Aşağı tarama tamamlandı, sağa dönülüyor\n");
            
            } else {
                MoveCube(GLUT_KEY_LEFT);
                fprintf(stdout, "Aşağı tarama tamamlandı, sola dönülüyor\n");
         
            }
          
            linearscanvector = UP;
        }
    }

   // fprintf(stdout, "Tarama devam ediyor\n");
   // fprintf(stdout, "Son konum: [%d, %d]\n", cubbbepos[0]/40, cubbbepos[1]/40);
    // Diğer yönler için de benzer mantık eklenebilir
         
}


void mousescan(void) {
    POINT P;
    ULONGLONG currentTime = GetTickCount64();

    if (currentTime - lastMoveTime < 30) return;
    lastMoveTime = currentTime;

    glutMouseFunc(MouseFunction);
    

   
    int mouseX = P.x;
    int mouseY = P.y;
fprintf(stdout, "Mouse Position: [%d, %d]\n", mouseX, mouseY);
        // Tile koordinatlarını hesapla
        int tileWidth = 40;
        int tileHeight = 40;
        int mouseTileX = mouseX / tileWidth;
        int mouseTileY = mouseY / tileHeight;

   //mouse nerdeyse hangi tilede olduğunu hesapla
   

    fprintf(stdout, "Mouse Tile: [%d, %d]\n", mouseTileX, mouseTileY);

    // Eğer hedefe ulaşıldıysa fonksiyonu durdur
    if (mouseTileX == cubbeupdatedpos[0] && mouseTileY == cubbeupdatedpos[1]) {
        fprintf(stdout, "Hedefe ulaşıldı, tarama durduruldu.\n");
        return;
    }

    // Hedefe doğru hareket et
    if (mouseTileX > cubbeupdatedpos[0]) {
        MoveCube(GLUT_KEY_RIGHT);
    } else if (mouseTileX < cubbeupdatedpos[0]) {
        MoveCube(GLUT_KEY_LEFT);
    } else if (mouseTileY > cubbeupdatedpos[1]) {
        MoveCube(GLUT_KEY_UP);
    } else if (mouseTileY < cubbeupdatedpos[1]) {
        MoveCube(GLUT_KEY_DOWN);
    }
}









const char* getDirectionName(AutonomDirection dir) {
    
    switch (dir) {
        case LINEARSCAN: return "LINEARSCAN";
        case BASICAUTOMATED: return "BASICAUTOMATED";
        case USERINPUT: return "USERINPUT";
        case MOUSEMOVE: return "MOUSEMOVE";
        default: return "UNKNOWN";
    }

  
}