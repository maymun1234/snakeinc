#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <stdbool.h>
void displaysnake(void);         // display fonksiyonu
void MoveCube(int key);     // hareket fonksiyonu
void SpecialKeys(int key, int x, int y); // tuş yakalama
void createsnake(void);     // yılan oluşturma fonksiyonu
void DrawCube(void);        // <<< EKLENMESİ GEREKEN FONKSİYON
void updategameplay(void);  // oyun güncelleme fonksiyonu
void startgame(void);     // oyunu başlatma fonksiyonu
void restartgame(void);
void checkWallCollision(void);
extern int foodtypescore[3];
#define GRID_WIDTH 40    // 800 px / 20 = 40
#define GRID_HEIGHT 30   // 600 px / 20 = 30
int cubeTileX; // Başlangıçta hangi tile'da olduğunu gösterir
int cubeTileY;
int cubbbex, cubbbey;
extern int tileSize;
extern int tileCols;  // 800 / 40 = 20
extern int tileRows; // 480 / 40 = 12
extern int snakelenght;
extern int lasttiles[90][2];
extern int walls[20][2];
extern int cubbbepos[2];
bool ismovepossible(int x, int y) ;
 int cubeX, cubeY; // Piksel pozisyonları
 typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    // ... başka state'ler eklersen buraya
} Direction;
extern Direction snakevector; // Yılanın başlangıç yönü
#endif
