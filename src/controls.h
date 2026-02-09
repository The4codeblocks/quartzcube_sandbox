
#ifndef controlsheader
#define controlsheader

#include "raylib.h"
#include "renderHandler.h"
#include "suppmath.h"

typedef struct {
	double current;
	double increment;
} NumberInput;

Vector3 getMovement();

directionOrtho getScrollDir();

void parseNumberInputControl(NumberInput*, directionOrtho);

#endif
