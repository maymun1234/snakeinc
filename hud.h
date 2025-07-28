#ifndef HUD_H
#define HUD_H

#include <stdbool.h>
int score;
int gameoverscore;

void drawhud(int score1, int arr[]);
void youlosttext(int score);
void GoToGameUI(void);
void GoToMainMenu(void);
void GoTosettingsUI(void);
void dialoguehud(int dial);

#endif
