
#ifndef controlsheader
#define controlsheader

#include "raylib.h"
#include "renderHandler.h"
#include "suppmath.h"

typedef struct {
	double value;
	double digit;
} NumberInput;

Vector3 getMovement();

directionOrtho getScrollDir();

void parseNumberInputControl(NumberInput*, directionOrtho);
void parseRotateInputControl(NumberInput*, directionOrtho);

#endif
