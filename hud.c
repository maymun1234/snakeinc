// çizim ve buton tıklamaları için gerekli

#include <stdio.h>
#include <windows.h>
#include "gameplay.h"
#include "main.h"
#include "./3.hw/header/gl.h"
#include "./3.hw/header/glu.h"
#include "./3.hw/header/glut.h"
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <string.h> // strlen için

#define SCORE

int score = 0;
int gameoverscore = 5;
void drawString(int x, int y, const char* string, float r, float g, float b, float scale);

void drawhud(int score1,int array[]);
void GoToMainMenu(void);

// === Font Yapısı ===
unsigned char ttf_buffer[1 << 20];        // 1MB
unsigned char font_bitmap[512 * 512];
stbtt_bakedchar cdata[96];                // ASCII 32..126
GLuint fontTex;






//aşağıdaki metodda txt dosyası olacak her saniye bir satır. ve metod çağrıldıdddğında saıtrıın souna aaa yazacak
ULONGLONG beginduration = 0;


void saveStats(int startstop) {
   //düz debug verince sorun yok ama zamanlayıcı olunca fps düşüyor:()

    if (startstop == 0) {
        
        beginduration = GetTickCount64();
        fprintf(stdout, "Baslangic zamani\n");
    } else if (startstop != 0) {
        
        // Bitirme zamanı ve süre hesaplanır
        ULONGLONG duration = GetTickCount64() - beginduration;
    // fprintf(stdout, " %llu ms\n", GetTickCount64());
        //daurationu konsola yazdır dosyaya değii lkosnola
      fprintf(stdout, "%d Sure: %llu ms\n", startstop, duration);
    } 


}


void initFont() {
     
     
    FILE* f = fopen("source/resources/Roboto-Regular.ttf", "rb");
    if (!f) {
        printf("[HATA] Font dosyası açılamadı! Yol: resources/Roboto-Regular.ttf\n");
        exit(1);
    }
    size_t read = fread(ttf_buffer, 1, 1 << 20, f);
    fclose(f);
    //printf("[OK] Font dosyası başarıyla yüklendi (%zu bayt).\n", read);
    
    int bakeResult = stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0, font_bitmap, 512, 512, 32, 96, cdata);
    
    if (bakeResult <= 0) {
        printf("[HATA] Font bake işlemi başarısız oldu!\n");
        exit(1);
    }
    //printf("[OK] Font bake işlemi başarılı. Toplam karakter: %d\n", bakeResult);

    glGenTextures(1, &fontTex);
    glBindTexture(GL_TEXTURE_2D, fontTex);

    // Alternatif: GL_LUMINANCE ile daha geniş destek
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, font_bitmap);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //printf("[OK] Font OpenGL dokusu başarıyla oluşturuldu (ID: %u).\n", fontTex);
}

void drawString(int x, int y, const char* string, float r, float g, float b, float scale) {
    if (!string || strlen(string) == 0) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fontTex);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glColor3f(r, g, b);

    float fx = (float)x;
    float fy = (float)(SCREENHEIGHT - y);

    glBegin(GL_QUADS);
    for (size_t i = 0; i < strlen(string); i++) {
        unsigned char ch = (unsigned char)string[i];
        if (ch >= 32 && ch < 128) {
            stbtt_aligned_quad q;
            stbtt_GetBakedQuad(cdata, 512, 512, ch - 32, &fx, &fy, &q, 1);

            // Scale ve ters çevirme:
            float x0 = q.x0 * scale;
            float x1 = q.x1 * scale;
            float y0 = SCREENHEIGHT - (q.y0 * scale);
            float y1 = SCREENHEIGHT - (q.y1 * scale);

            float cx = (x0 + x1) / 2.0f;
            float cy = (y0 + y1) / 2.0f;
            float w = (x1 - x0) / 2.0f;
            float h = (y1 - y0) / 2.0f;

            glTexCoord2f(q.s0, q.t0); glVertex2f(cx - w, cy - h);
            glTexCoord2f(q.s1, q.t0); glVertex2f(cx + w, cy - h);
            glTexCoord2f(q.s1, q.t1); glVertex2f(cx + w, cy + h);
            glTexCoord2f(q.s0, q.t1); glVertex2f(cx - w, cy + h);
        }
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}


int jfjfjf = 0;
int fpsfark = 0;
int currentscond;

void drawhud(int score1, int arr[]) {
 //  saveStats(0); // Oyun başladığında istatistikleri kaydet
 //  saveStats(1);
    initFont();
   // saveStats(1); // Oyun başladığında istatistikleri kaydet

    glMatrixMode(GL_PROJECTION);

glLoadIdentity();
 
gluOrtho2D(0, SCREENWIDTH, 0, SCREENHEIGHT);  // Ekran koordinatları

glMatrixMode(GL_MODELVIEW);

glLoadIdentity();

    char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", score1);
    if(score1 > 0){
        gameoverscore = score1;
    }
     int boxWidth = 130;
    int boxHeight = 40;
    int boxX = SCREENWIDTH - boxWidth - 10; // sağdan 10 piksel içeri
    int boxY = SCREENHEIGHT - boxHeight - 10; // yukarıdan 10 piksel içeri
   
    glColor3ub(255, 255, 255); // Beyaz renk
    glBegin(GL_QUADS);
        glVertex2i(boxX, boxY);
        glVertex2i(boxX + boxWidth, boxY);
        glVertex2i(boxX + boxWidth, boxY + boxHeight);
        glVertex2i(boxX, boxY + boxHeight);
    glEnd();

    glColor3ub(246, 133, 55); // Yazı rengi
   for (int i = 0; i <= 2; i++) {
    int posX = SCREENWIDTH - 60 - (2 - i) * 40;  // i'nin pozisyonunu ters çeviriyoruz
    char str[12];
    sprintf(str, "/%d", foodtypescore[i]);
    drawString(posX, SCREENHEIGHT -40, str, 0.263f, 0.439f, 0.341f, 1.0f);
    
 
}

//saveStats(1); // Oyun başladığında istatistikleri kaydet



    //zamanı  ölçüp yaz
    char str[12];
    sprintf(str, "%d", fpsfark); // jfjfjf bir tamsayı olmalı
    drawString(boxX + 90, boxY - 20, str, 0.863f, 0.439f, 0.341f, 1.0f);

    SYSTEMTIME currentTime;
    GetSystemTime(&currentTime); // UTC zaman verir



        if(currentTime.wSecond != currentscond){
            fpsfark = jfjfjf; // FPS farkını kaydet
            jfjfjf = 0; // Saniye değiştiğinde jfjfjf sıfırlanır
          
        }
       
        currentscond = currentTime.wSecond;
        jfjfjf++;

    
    drawString(10, SCREENHEIGHT - 30, scoreText, 0.263f, 0.439f, 0.341f, 1.0f);
//saveStats(2); // Oyun başladığında istatistikleri kaydet
}


void drawgameovertext(void){
    
}

//string arrayı oluştur
char sozler[10];







void youlosttext(int score1) {
    for(int i = 0; i < 10; i++) {
      //  jjffj[i] = '\0'; // Diziyi sıfırla
     //   jjffj[i] = 'a' + (rand() % 26); // Rastgele harf atama
      //  printf("%c\n", jjffj[i]); // Diziyi yazdır
    }
    //drawString(SCREENWIDTH / 2 - 100, SCREENHEIGHT / 2 - 30, jjffj[1], 1.0f, 0.0f, 0.0f, 1.0f);
    
    glutPostRedisplay();
    glColor3ub(255, 0, 0); // Kırmızı
    //  printf("[DEBUG] Skor değeri: %d\n", gameoverscore);

    //fpriintf(stdout, "[INFO] Oyun kaybedildi! Ekrana mesaj yazılıyor...\n");
    glClearColor(0.184f, 0.322f, 0.286f, 1.0f); // #2f5249ff
    drawString(SCREENWIDTH / 3.7, SCREENHEIGHT - 100, "You Lost!", 1.0f, 1.0f, 0.0f, 1.5f);  // Sarı, 1.5x büyük
drawString(SCREENWIDTH / 2 - 100, SCREENHEIGHT / 2 - 30, "Press R to Restart", 1.0f, 0.0f, 0.0f, 1.0f);  // Kırmızı, normal boyut
char scoreText[32];
    snprintf(scoreText, sizeof(scoreText), "Score: %d", gameoverscore);  // Global 'score' değişkeni
    drawString(SCREENWIDTH / 2 - 50, SCREENHEIGHT / 2 - 80, scoreText, 1.0f, 1.0f, 1.0f, 1.0f);  // Beyaz


}

// === Butonlar ve Ekran geçişi ===

extern int redrawRequired; // main.c'den gelen değişken

void GoToGameUI() {
    printf("[INFO] Game UI'ya geçiliyor...\n");
    currentState = STATE_GAMEUI;
    redrawRequired = 1;
    glutPostRedisplay();
}

void GoTosettingsUI() {
    printf("[INFO] Game UI'ya geçiliyor...\n");
    currentState = STATE_SETTINGS;
    redrawRequired = 1;
    glutPostRedisplay();
}

void GoToMainMenu() {
    printf("[INFO] Ana menüye dönülüyor...\n");
    currentState = STATE_MAIN_MENU;
    redrawRequired = 1;
    glutPostRedisplay();
}
ULONGLONG lastMoveTime = 0;
#define MOVE_INTERVAL_MS 100

bool printEnabled = true;  // Durum takibi: Yazma açık mı?

void dialoguehud(int dial, bool istimed, char dialogue_name[]) {
    if(istimed){
        ULONGLONG currentTime = GetTickCount64();

    if (currentTime - lastMoveTime >= 3000) {
        lastMoveTime = currentTime;
        printEnabled = !printEnabled;  // Yazma durumunu tersine çevir
    }
    }
    

    if (printEnabled) {
         glPointSize(50.0f);
    
       glColor3ub(255, 0, 0);
        glBegin(GL_QUADS);
            glVertex2i(20, 20);   // Sol alt
            glVertex2i(120, 20);  // Sağ alt
            glVertex2i(120, 70);  // Sağ üst
            glVertex2i(20, 70);   // Sol üst
        glEnd();
         drawString(50, -400, "Welcome game!", 0.5f, 1.0f, 0.5f, 0.50f);


         

    }
}
