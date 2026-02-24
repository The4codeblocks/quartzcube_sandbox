
#ifndef spawntoolheader
#define spawntoolheader

#include "object.h"
#include "suppmath.h"
#include "controls.h"

#define elementCount 8

typedef struct {
	int index;

	int configIdx;
	NumberInput gridsnap;
	NumberInput diroff;
	orientation savedori;

	Material mat;
	RenderTexture2D textbox;
} SpawnToolData;

typedef struct {
	Object* (*cons)(pos3, Vector3, orientation);
	char* name;
	void (*ghost)(pos3, Vector3, orientation);
} Element;

Element elements[elementCount];

Object* spawnElement(pos3, Vector3, orientation, int);
   void ghostElement(pos3, Vector3, orientation, int);

void spawnTool_recieve(Component* comp, dataChannel channel, cstr data);
void spawnTool_init   (Component* comp);
void spawnTool_draw   (Component* comp);

#endif
