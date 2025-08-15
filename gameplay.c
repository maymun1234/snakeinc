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
#include "food.h" // Elma ile ilgili fonksiyonlar
#include "automated.h"
#include "gameplay.h"
#include "stats.h"

void updategameplay(void);
void displaysnake(void);
void MoveCube(int key);
void createsnake(void);
void restartgame(void); // Oyun yeniden başlatma fonksiyonu
void painttile(void);
bool ismovepossible(int x, int y); // Yılanın hareketinin mümkün olup olmadığını kontrol eden fonksiyon
void issnakecrashed(int x, int y); // Yılanın çarpışma durumunu kontrol eden fonksiyon


extern int score; // Skor değişkeni, hud.h'de tanımlı
extern int cubbbepos[2];  // foodPosition[0] = x, foodPosition[1] = y

int TimerCounter;
int Minutes, Seconds;
int Color;
int lastattempts[5] = {0, 0, 0, 0, 0}; // Son 5 denemeyi saklamak için

int cubbbepos[2] = {1, 1};

 int foodtypescore[3]={0,0,0};
Direction snakevector = UP; // Yılanın başlangıç yönü
// Küpün pozisyon ve boyut bilgileri
int cubeX = 400;
int cubeY = 240;


bool issnakemoving = 0;


int snakelenght = 1; // Yılanın uzunluğu

bool isfoodexists = true; // Elmanın varlığı kontrolü






void updategameplay(void) {
    
    glutPostRedisplay();
   glutIdleFunc(idleFunc);

    drawfood(); // Elmayı çiz
    painttile();
   cubbbepos[0]= cubeX;
   cubbbepos[1]= cubeY;
 //   fprintf(stdout, "kuppexxt: (%d)\n", cubbbex);
   
    displaysnake(); 

    // Yılanın uzunluğunu ve elmanın varlığını kontrol et
 
    if (!isfoodexists) {
        createfood(); // Elmayı oluştur
        isfoodexists = true; // Elma artık var
        fprintf(stdout, "jndvnjdfnj\n");
        fprintf(stdout, "Food created at: (%d, %d)\n", foodPosition[0], foodPosition[1]);
    }

    drawhud(score, lastattempts); // HUD'u çiz
    checkWallCollision();
    automationmanager();
}



int tileSize = 40;
int tileCols = 20;  // 800 / 40 = 20
int tileRows = 12; // 480 / 40 = 12

int cubeTileX = 10; // Başlangıçta hangi tile'da olduğunu gösterir
int cubeTileY = 6;
int cubeSize = 40;  // Aynı tile boyutu

 int cubeX, cubeY; // Piksel pozisyonları
extern int foodPosition[2];  // foodPosition[0] = x, foodPosition[1] = y

int lasttiles[90][2];
bool iswallsexists = false; // Duvarların varlığı kontrolü
int walls[20][2] = {
    {0, 0},{0,5}, 
   
}; // Örnek duvar koordinatları


void drawwalls(void) {
    if (!iswallsexists)
    {
        return;
    }
    
    glColor3ub(151, 176, 203); // Duvar rengi
    for (int i = 0; i < 20; i++) {
        int x = walls[i][0] * tileSize;
        int y = walls[i][1] * tileSize;
     //   fprintf(stdout, "Wall at tile: (%d, %d)\n", walls[i][0], walls[i][1]);
        glBegin(GL_QUADS);
            glVertex2i(x, y);
            glVertex2i(x + tileSize, y);
            glVertex2i(x + tileSize, y + tileSize);
            glVertex2i(x, y + tileSize);
        glEnd();
    }
}
void displaysnake(void) {
    drawwalls(); // Duvarları çiz
    glColor3ub(67, 112, 87);
    glBegin(GL_QUADS);
        glVertex2i(cubeX, cubeY);
        glVertex2i(cubeX + cubeSize, cubeY);
        glVertex2i(cubeX + cubeSize, cubeY + cubeSize);
        glVertex2i(cubeX, cubeY + cubeSize);
     
    glEnd();

    //fprintf(stdout, "Cube moved to tile: (%d, %d)\n", cubeTileX, cubeTileY);

    // Yem yendiyse kontrol et
    if (cubeX == foodPosition[0] && cubeY == foodPosition[1]) {
        fprintf(stdout, "Food eaten at (%d, %d)\n", foodPosition[0], foodPosition[1]);
        foodeatdurationms(1);
        printf("foodid = %d\n", foodid);

        score += foodid;
        printf("%lx\n", (unsigned long)&score);  // Adres: küçük harfli hex, 0x yok

        foodtypescore[foodid-1] ++;
        createfood();
        
       
        snakelenght++;
        lasttiles[snakelenght - 1][0] = lasttiles[snakelenght - 2][0];
    lasttiles[snakelenght - 1][1] = lasttiles[snakelenght - 2][1];

         fprintf(stdout, "cjnvsjkvk %d\n", score);
    }


    // boyalıtile ile çakışma var mı(ilk tile ı pas geç)
    issnakecrashed(cubeTileX, cubeTileY);

    glutPostRedisplay();
}

bool isPositionInLastTiles(int x, int y) {
    for (int i = 0; i < snakelenght; i++) {
        if (lasttiles[i][0] == x && lasttiles[i][1] == y) {
            return true;  // Aynı pozisyon bulundu
        }
    }
    return false;
}

// Küpü hareket ettiren fonksiyon
void MoveCube(int key) {
     if (!isPositionInLastTiles(cubeTileX, cubeTileY)) {
        // Eğer yoksa diziyi kaydır ve yeni pozisyonu başa ekle
        for (int i = snakelenght - 1; i > 0; i--) {
            lasttiles[i][0] = lasttiles[i - 1][0];
            lasttiles[i][1] = lasttiles[i - 1][1];
        }
        lasttiles[0][0] = cubeTileX;
        lasttiles[0][1] = cubeTileY;
    } else {
        // Pozisyon zaten var, istersen buraya özel işlem koyabilirsin
        // Örneğin hata mesajı, oyun sonu, vs.
     //   printf("Bu pozisyon zaten yılanın içinde!\n");
    }
    if(1) {
      
    
    switch (key) {
        case GLUT_KEY_UP:
            if (cubeTileY < tileRows - 1 && snakevector != DOWN){ cubeTileY++;
            snakevector = UP;} // Yılan yukarı hareket ediyor
            break;
        case GLUT_KEY_DOWN:
            if (cubeTileY > 0 && snakevector != UP){ cubeTileY--;
            snakevector = DOWN;} // Yılan aşağı hareket ediyor
            break;
        case GLUT_KEY_LEFT:
            if (cubeTileX > 0 && snakevector != RIGHT) {cubeTileX--;
            snakevector = LEFT;} // Yılan sola hareket ediyor
            break;
        case GLUT_KEY_RIGHT:
            if (cubeTileX < tileCols - 1 && snakevector != LEFT){ cubeTileX++;
            snakevector = RIGHT;} // Yılan sağa hareket ediyor
            break;
    }
    }
    //fprintf(stdout, "Cube moved to tile: (%d, %d)\n", cubeTileX, cubeTileY);
    // Yeni piksel pozisyonunu tile'a göre güncelle
    cubeX = cubeTileX * tileSize;
    cubeY = cubeTileY * tileSize;



    
    //geçilen tilenin koordoinatı kaydet
    

    glutPostRedisplay(); // Ekranı tekrar çiz
}


// Ok tuşlarını dinleyen GLUT fonksiyonu (main.c'den çağrılmalı)
void SpecialKeys(int key, int x, int y) {
    MoveCube(key);
}

// Yılan oluşturma fonksiyonu
void createsnake(void) {
    fprintf(stdout, "Snake created!\n");
    
    
    cubeX = 420;
    cubeY = 260;
    displaysnake(); // Yılanı ekranda göster
    glutPostRedisplay(); // Yılan başlarken konum güncellensin
    
}





#define GRID_WIDTH 40    // 800 px / 20 = 40
#define GRID_HEIGHT 30   // 600 px / 20 = 30

//random metodları



char djjdjd[5]={
    'A', 'B', 'C', 'D', 'E'
}; // Elma türleri için örnek karakterler

int burcuuuuu;

 char mrwkmfkrkm[] ="kmdsdvskmsmklv";


//başlatıcı
void startgame(void) {
    drawfood(); // Elmayı çiz
    fprintf(stdout, "Game started!\n");
    for(int i=0; i<4; i++){
      fprintf(stdout, "%p\n", &djjdjd[i]);
    }
   fprintf(stdout, "%p\n", &score);
   
    createsnake(); // Yılanı oluştur
    score = 0; // Skoru sıfırla
    for (int i = 0; i < snakelenght; i++) {
        lasttiles[i][0] = cubeTileX; // X koordinatlarını sıfırla
        lasttiles[i][1] = cubeTileY; // Y koordinatlarını sıfırla
    }
}



void painttile(void) {
     glColor3ub(151, 176, 103);
    for (int i = 0; i < snakelenght; i++) {
        int x = lasttiles[i][0] * tileSize;
        int y = lasttiles[i][1] * tileSize;
        glBegin(GL_QUADS);
            glVertex2i(x, y);
            glVertex2i(x + tileSize, y);
            glVertex2i(x + tileSize, y + tileSize);
            glVertex2i(x, y + tileSize);
        glEnd();

    }

   
}



bool ismovepossible(int x, int y) {
    if(snakelenght >= 2) {
        
        if (lasttiles[0][0] == x && lasttiles[0][1] == y) {
            
            fprintf(stdout, "Move blocked by snake at tile: (%d, %d)\n", x, y);
           currentState = STATE_GAMEOVER;
           restartgame();
            return false; // Çakışma var, hareket mümkün değil
        }
        
    }
    
    //son tile ı kontorl et çakışma var mı
    
    return true;
}

void paintsnakehead(void){
   return;
}

void restartgame(void) {
    asavescoretotext(score);
    fprintf(stdout, "Game restarted!\n");
    fprintf(stdout, "jndsnkdnds");
    isautomated = false;
    fprintf(stdout, "Game restarted!\n");
   
    // Skor ve yılan durumu sıfırlanıyor
   
    score = 0;
    snakelenght = 1;
    isfoodexists = false;

    // Başlangıç pozisyonları
    cubeTileX = 10;
    cubeTileY = 6;
    cubeX = cubeTileX * tileSize;
    cubeY = cubeTileY * tileSize;

    // lasttiles dizisini sıfırla
    for (int i = 0; i < 50; i++) {
        lasttiles[i][0] = -1; // Geçersiz pozisyon
        lasttiles[i][1] = -1;
    }

    // İlk tile'ı güncelle
    lasttiles[0][0] = cubeTileX;
    lasttiles[0][1] = cubeTileY;

    // Yönü sıfırla
    snakevector = UP;

     foodtypescore[0]= 0;
      foodtypescore[1]= 0;
       foodtypescore[2]= 0;
    
    // Yeni elma oluştur
    createfood();

    // Yılanı ekranda tekrar oluştur
    createsnake();

    glutPostRedisplay(); // Ekranı güncelle – doğru yere alındı
}

#include "mainmenu.h" // Oyun menüsüne dönmek için

void issnakecrashed(int x, int y) {
    if (snakelenght > 1) {
        for (int i = 1; i < 20; i++) { // 0'dan değil 1'den başlıyoruz
            if (lasttiles[i][0] == x && lasttiles[i][1] == y) {
                fprintf(stdout, "Kendine çarptı: (%d, %d)\n", x, y);
                currentState = STATE_GAMEOVER;
                
                
                restartgame();
                return;
            }
            else if(walls[i][0]==lasttiles[0][i] && walls[i][1]==lasttiles[i][1]){ //brada engele çarpmaya bakılacak.
                 if (!iswallsexists)
                    {
                        return;
                    }
                fprintf(stdout, "duvara çarptı: (%d, %d)\n", walls[i][0], walls[i][0]);
                currentState = STATE_GAMEOVER;
                restartgame();
                return;
            }
            

        }
    }
}










void checkWallCollision(void) {
    if (cubeTileX < 0 || cubeTileX >= tileCols || cubeTileY < 0 || cubeTileY >= tileRows) {
        fprintf(stdout, "Sınıra çarpıldı: (%d, %d)\n", cubeTileX, cubeTileY);

        currentState = STATE_GAMEOVER;

        restartgame();
    }
}


