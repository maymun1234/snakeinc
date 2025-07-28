#ifndef CLICKMANAGER_H
#define CLICKMANAGER_H

typedef struct {
    int x, y;
    int radius;
    void (*callback)(void);
} ClickablePoint;



void RegisterClickablePoints(ClickablePoint* points, int count);
void HandleClick(int x, int y);


void MouseFunction(int button, int state, int x, int y);
void SpecialKeyFunction(int key, int x, int y);

#endif
