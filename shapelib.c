

#include <windows.h>
#include <stdio.h>
#include "./3.hw/header/gl.h"
#include "./3.hw/header/glu.h"
#include "./3.hw/header/glut.h"

#include "mainmenu.h"
#include "gameplay.h"
#include "clickmanager.h"
#include "clickmanager.h"
#include <math.h>

#define PI 3.14159265
int TimerCounter;
int Minutes, Seconds;
int Color;
ULONGLONG lastMoveTime = 0;
#define MOVE_INTERVAL_MS 100

void drawRoundedRect(float x, float y, float width, float height, float radius) {
    int segments = 20; // Daha fazla segment, daha düzgün daire

    // Orta dikdörtgen (tam ortada)
    glBegin(GL_QUADS);
        glVertex2f(x + radius, y + radius);
        glVertex2f(x + width - radius, y + radius);
        glVertex2f(x + width - radius, y + height - radius);
        glVertex2f(x + radius, y + height - radius);
    glEnd();

    // Dört kenar (üst, alt, sol, sağ)
    // Üst kenar
    glBegin(GL_QUADS);
        glVertex2f(x + radius, y + height - radius);
        glVertex2f(x + width - radius, y + height - radius);
        glVertex2f(x + width - radius, y + height);
        glVertex2f(x + radius, y + height);
    glEnd();

    // Alt kenar
    glBegin(GL_QUADS);
        glVertex2f(x + radius, y);
        glVertex2f(x + width - radius, y);
        glVertex2f(x + width - radius, y + radius);
        glVertex2f(x + radius, y + radius);
    glEnd();

    // Sol kenar
    glBegin(GL_QUADS);
        glVertex2f(x, y + radius);
        glVertex2f(x + radius, y + radius);
        glVertex2f(x + radius, y + height - radius);
        glVertex2f(x, y + height - radius);
    glEnd();

    // Sağ kenar
    glBegin(GL_QUADS);
        glVertex2f(x + width - radius, y + radius);
        glVertex2f(x + width, y + radius);
        glVertex2f(x + width, y + height - radius);
        glVertex2f(x + width - radius, y + height - radius);
    glEnd();

    // 4 köşeye çeyrek daireler
    for (int i = 0; i < 4; i++) {
        float cx = x + (i % 2 ? width - radius : radius);
        float cy = y + (i / 2 ? height - radius : radius);
        float startAngle = i * 90;

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(cx, cy);
        for (int j = 0; j <= segments; j++) {
            float angle = (startAngle + j * 90.0f / segments) * PI / 180.0f;
            glVertex2f(cx + cos(angle) * radius, cy + sin(angle) * radius);
        }
        glEnd();
    }


    

}

float animX = 0;  // Başlangıç X konumu
float animY = 0;  // Başlangıç Y konumu
float targetX = 500;  // Hedef X
float targetY = 300;  // Hedef Y
float speed = 2.0f;   // Her karede kaç birim ilerlesin

void animateobjects(int duration) {
    ULONGLONG currentTime = GetTickCount64();
    if (currentTime - lastMoveTime < duration) return;
    lastMoveTime = currentTime;

    // X yönü
    if (fabs(targetX - animX) > 0.1f) {
        animX += (targetX > animX) ? speed : -speed;
    }

    // Y yönü
    if (fabs(targetY - animY) > 0.1f) {
        animY += (targetY > animY) ? speed : -speed;
    }

    glColor3ub(0, 0, 0);
    glBegin(GL_QUADS);
        glVertex2f(animX, animY);
        glVertex2f(animX + 100, animY);
        glVertex2f(animX + 100, animY + 100);
        glVertex2f(animX, animY + 100);
    glEnd();

    fprintf(stdout, "X=%.2f Y=%.2f\n", animX, animY);
    glPointSize(50.0f);
    glBegin(GL_POINTS);
        glColor3ub(255, 255, 0);
        glVertex2i(400, 240);
    glEnd();
}
