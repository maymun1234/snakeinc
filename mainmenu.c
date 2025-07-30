#include <windows.h>
#include "./3.hw/header/gl.h"
#include "./3.hw/header/glu.h"
#include "./3.hw/header/glut.h"
#include "gameplay.h"
#include "shapelib.h"
#include <stdio.h>
#include <stdbool.h>
#include "main.h"
#include "hud.h"
#include "automated.h"
//#include "./3.hw/glfw-3.4/include/GLFW/glfw3.h"

//arayüz sayfalarının metodları

#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

GLuint snakemainmenuTexture;   // food.png
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

void loadMainMenuTexture() {
    int width, height, nrChannels;
    unsigned char *data;
stbi_set_flip_vertically_on_load(true);
    // base
    data = stbi_load("source/resources/snakemainmenu.png", &width, &height, &nrChannels, 0);
    if (!data) {
    fprintf(stderr, "Texture YÜKLENEMEDİ! Dosya bulunamadı: source/resources/snakemainmenu.png\n");
} else {
    printf("Texture Yüklendi: %dx%d, Kanal: %d\n", width, height, nrChannels);
}

    if (data) {
        glGenTextures(1, &snakemainmenuTexture);
        glBindTexture(GL_TEXTURE_2D, snakemainmenuTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        stbi_image_free(data);
    }
    

    // overlay
  

}


extern int Color; // main.c'deki global değişken
void DrawGameoverscreen(void);

void DrawScreennn(){

    if(currentState != STATE_GAMEUI) {
       
        return; // Eğer oyun durumu GAMEUI değilse, ekranı çizme
    }
   // fprintf(stdout, "[INFO] DrawScreennn fonksiyonu yuklendi.\n");
    static bool gameStarted = false;
glClearColor(0.925f, 0.929f, 0.69f, 1.0f);
//arka plan 236 237 176 olmalı

    glClear(GL_COLOR_BUFFER_BIT);
    glColor3ub(255, 205, 0);
    glBegin(GL_QUADS);
        glVertex2i(800, 400);
        glVertex2i(800, 500);
        glVertex2i(650, 400);
        glVertex2i(650, 500);
    glEnd();
    
    
    updategameplay();

    if (!gameStarted) {
        startgame();
        gameStarted = true;
        restartgame();
    }
   
     dialoguehud(60);
    glColor3f(0.2f, 0.6f, 0.8f); // Renk
   // drawRoundedRect(100, 100, 200, 100, 5); //, y, genişlik, yükseklik, yarıçap
}

void DrawMainMenu() {
    glClearColor(0.925f, 0.929f, 0.69f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, snakemainmenuTexture);

    int imageWidth = 400;
    int imageHeight = 300;

    int x_center = 800 / 2;
    int y_top = 600 - 100;

    int left   = x_center - (imageWidth / 2);
    int right  = x_center + (imageWidth / 2);
    int top    = y_top;
    int bottom = y_top - imageHeight;

    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2i(left, top);
        glTexCoord2f(1, 1); glVertex2i(right, top);
        glTexCoord2f(1, 0); glVertex2i(right, bottom);
        glTexCoord2f(0, 0); glVertex2i(left, bottom);
    glEnd();

    loadMainMenuTexture();
    glDisable(GL_TEXTURE_2D);

    // --- Buton 1: "Oyuna Başla" ---
    int buttonWidth = 200;
    int buttonHeight = 50;
    int x1 = (SCREENWIDTH - buttonWidth) / 2;
    int y1 = 150;  // ↓↓↓ daha aşağıya çekildi

    glColor3ub(255, 205, 0);  // Sarı renk
    glBegin(GL_QUADS);
        glVertex2i(x1, y1);
        glVertex2i(x1 + buttonWidth, y1);
        glVertex2i(x1 + buttonWidth, y1 + buttonHeight);
        glVertex2i(x1, y1 + buttonHeight);
    glEnd();

    // --- Buton 2: "Ayarlar" ---
    int x2 = x1;
    int y2 = y1 + 80;  // Diğer butonla arası 80 piksel

    glColor3ub(255, 180, 0);  // Farklı bir sarı tonu
    glBegin(GL_QUADS);
        glVertex2i(x2, y2);
        glVertex2i(x2 + buttonWidth, y2);
        glVertex2i(x2 + buttonWidth, y2 + buttonHeight);
        glVertex2i(x2, y2 + buttonHeight);
    glEnd();
}





void DrawGameoverscreen() {
    if (currentState != STATE_GAMEOVER) {
        return; // Eğer oyun durumu GAMEOVER değilse, ekranı çizme
    }
    
    

     youlosttext(score);
    glClearColor(0.184f, 0.322f, 0.286f, 1.0f); // #2F5249


    
   
}

void DrawSettingsscreen() {
    if (currentState != STATE_GAMEOVER) {
        return; // Eğer oyun durumu GAMEOVER değilse, ekranı çizme
    }
    
    
isautomated= false;
     youlosttext(score);
    glClearColor(0.184f, 0.322f, 0.286f, 1.0f); // #2F5249


    
   
}








