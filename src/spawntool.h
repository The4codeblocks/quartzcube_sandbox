
#ifndef spawntoolheader
#define spawntoolheader

#include "object.h"
#include "suppmath.h"

#define elementCount 5

typedef struct {
	Object* (*cons)(pos3);
	char* name;
} Element;

Element elements[elementCount];

Object* spawnElement(pos3, orientation, int);

#endif
