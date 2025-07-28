#ifndef FOOD_H
#define FOOD_H

void createfood(void); // Yiyecek oluşturma fonksiyonu
void drawfood(void);   // Yiyeceği çizme fonksiyonu
//foodposition arrayını ekle
// food.h içinde sadece bu olmalı:
extern int foodPosition[2];  // Tanım DEĞİL, sadece bildirim
int foodid;
bool isFoodOnSnake(int x, int y);

#endif