#ifndef MAIN_H
#define MAIN_H

#include "clickmanager.h"  // ClickablePoint struct burada tanımlıysa

#define SCREENWIDTH 800
#define SCREENHEIGHT 480
#define TIMER_INTERVAL      100

// Game state'leri enum ile tanımla
typedef enum {
    STATE_MAIN_MENU,
    STATE_GAMEUI,
    STATE_GAMEOVER,
    STATE_SETTINGS,
    // ... diğer state'ler buraya eklenebilir
} GameState;

// Enum'u temsil eden bir değişken
extern GameState currentState;

// Clickable butonlar
extern ClickablePoint mainMenuButtons[];
extern ClickablePoint gameUIButtons[];
extern ClickablePoint gameoverButtons[];

#endif // MAIN_H
