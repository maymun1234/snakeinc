#include <stdio.h>
#include <windows.h>
#include "gameplay.h"
#include "main.h"
#include "./3.hw/header/gl.h"
#include "./3.hw/header/glu.h"
#include "./3.hw/header/glut.h"
#include <stdbool.h>
#include <stdlib.h>  // rand, srand
#include<time.h>
#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



int foodPosition[2] = {0, 0};
extern int foodid;
void createfood(void);  
void drawfood(void);
int getRandomGridPositionX(void);
int getRandomGridPositionY(void);
bool isFoodOnSnake(int x, int y);
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif


GLuint foodBaseTexture;   // food.png
GLuint foodOverlayTexture; // food_overlay.png gibi


void loadFoodTextures() {
    int width, height, nrChannels;
    unsigned char *data;

    // base
    data = stbi_load("source/resources/food1.png", &width, &height, &nrChannels, 0);
    if (data) {
        glGenTextures(1, &foodBaseTexture);
        glBindTexture(GL_TEXTURE_2D, foodBaseTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }

    // overlay
   data = stbi_load("source/resources/food1.png", &width, &height, &nrChannels, 4);  // 4 kanal al


if (data) {
    // foodid'ye göre renk seçimi
    unsigned char r = 255, g = 255, b = 255;
    if (foodid == 1) {        // turuncu
        r = 246; g = 133; b = 55;
    } else if (foodid == 2) { // yeşil
        r = 120; g = 200; b = 100;
    } else if (foodid == 3) { // mor
        r = 170; g = 80;  b = 255;
    }

    // Boyama: sadece alpha değeri yüksek olan pikselleri boya (örnek: sadece "fff" olan yerler)
    for (int i = 0; i < width * height * 4; i += 4) {
        if (data[i + 3] > 128) {  // alpha kanalı: görünür mü?
            data[i] = r;     // R
            data[i + 1] = g; // G
            data[i + 2] = b; // B
            // A aynen kalır
        }
    }

    glGenTextures(1, &foodOverlayTexture);
    glBindTexture(GL_TEXTURE_2D, foodOverlayTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);
}

}


typedef struct {
    char foodname[32];

    int type;        // 0: normal, 1: bonus, 2: zehirli, vs.
    int point;       // Skora etkisi
   
} Food;
#include "stats.h"

void createfood(void) {
      loadFoodTextures();
    foodeatdurationms(0);
    foodPosition[0] = getRandomGridPositionX();
    foodPosition[1] = getRandomGridPositionY();
    if(foodPosition[0]%40 != 0 || foodPosition[1]%40 != 0 || isFoodOnSnake(foodPosition[0], foodPosition[1])) {
       int r = rand() % 100; // 0-99 arasında sayı

if (r < 45) {
    foodid = 1;  // %45 olasılık
} else if (r < 90) {
    foodid = 2;  // %45 olasılık
} else {
    foodid = 3;  // %10 olasılık
}
        createfood();
    }
    if (1) {
      
    }
    //food hangi tilede onu yaz
    fprintf(stdout, "Food created at tile: (%d, %d)\n", foodPosition[0] / 40, foodPosition[1] / 40);
  
   
}


bool isFoodOnSnake(int x, int y) {
    for (int i = 0; i < snakelenght; i++) {
        if (lasttiles[i][0] * tileSize == x && lasttiles[i][1] * tileSize == y) {
            return true; // Yem yılanın üstünde
            
        }
    }
    return false; // Çakışma yok
}

void drawfood(void) {
    int size = 40;
    int foodX = foodPosition[0];
    int foodY = foodPosition[1];

    // Ekran sınır kontrolü
    if (foodX + size > SCREENWIDTH) foodX = SCREENWIDTH - size;
    if (foodY + size > SCREENHEIGHT) foodY = SCREENHEIGHT - size;

    foodPosition[0] = foodX;
    foodPosition[1] = foodY;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    // 1. Baz texture (örnek: food.png)
    glBindTexture(GL_TEXTURE_2D, foodBaseTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(foodX, foodY);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(foodX + size, foodY);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(foodX + size, foodY + size);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(foodX, foodY + size);
    glEnd();

    // 2. Üstte gösterilecek efekt (örnek: boyama, ışık, seviye gibi)
    glBindTexture(GL_TEXTURE_2D, foodOverlayTexture);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2i(foodX, foodY);
        glTexCoord2f(1.0f, 0.0f); glVertex2i(foodX + size, foodY);
        glTexCoord2f(1.0f, 1.0f); glVertex2i(foodX + size, foodY + size);
        glTexCoord2f(0.0f, 1.0f); glVertex2i(foodX, foodY + size);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glutPostRedisplay();
}


// 20'nin katı olan random pozisyon üreti
int getRandomGridPositionX() {
    
    return (rand() % GRID_WIDTH) * 20;
    
}

int getRandomGridPositionY() {
    return (rand() % GRID_HEIGHT) * 20;
}