
#ifndef spawntoolheader
#define spawntoolheader

#include "object.h"
#include "suppmath.h"

#define elementCount 6

typedef struct {
	Object* (*cons)(pos3, Vector3);
	char* name;
	void (*ghost)(pos3, Vector3, orientation ori);
} Element;

Element elements[elementCount];

Object* spawnElement(pos3, Vector3, orientation, int);
   void ghostElement(pos3, Vector3, orientation, int);

#endif
