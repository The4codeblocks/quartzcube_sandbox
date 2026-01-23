
#ifndef renderhandlerheader
#define renderhandlerheader

#include "raylib.h"
#include "suppmath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#include "gametime.h"

#define MATERIAL_MAP_TRANSMIT MATERIAL_MAP_SPECULAR
#define SHADER_LOC_VIEWPORT SHADER_LOC_MAP_ROUGHNESS
#define MATERIAL_MAP_VIEWPORT MATERIAL_MAP_ROUGHNESS

typedef enum {
	unleft,
	unright,
	down
} bintreeOperation;

typedef struct {
	Mesh mesh;
	Material material;
	Matrix transform;
	bool isTransparent;
} RenderElement;

struct RenderSortNode;
typedef struct RenderSortNode RenderSortNode; // object linked list element

struct RenderSortNode { // binary tree node containing a RenderElement
	RenderElement element;
	float distance;

	RenderSortNode* left;
	RenderSortNode* up;
	RenderSortNode* right;
};

RenderSortNode* obtainPooledRenderNode();

void pushElement(RenderElement element);

Material defaultMat;

Material createTransmitMaterial();
inline void unloadTransmitMaterial(Material mat) {
	mat.shader = (Shader){0};
	mat.maps[MATERIAL_MAP_VIEWPORT].texture.id = -1;
	UnloadMaterial(mat);
}

void clearAll();

void drawAll();

pos3 camPos;
Camera3D mainCam;

orientation camOri;

Vector3 camR;
orientation camFace;

RenderTexture2D rndt;

Texture wabbit;
Shader transmission;
Mesh brickMesh;
Mesh planeMesh;
Model wireModel;
Mesh spoolMesh;

int winWidth;
int winHeight;

int winXCenter;
int winYCenter;

void updateCamControl(Camera3D*, orientation, float, Vector3*, orientation*);

void drawInit();
void drawMain();
void drawFin();

#endif