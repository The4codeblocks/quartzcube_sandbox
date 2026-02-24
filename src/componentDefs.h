
#ifndef objectdefsheader
#define objectdefsheader

#include "raylib.h"
#include "object.h"
#include "controls.h"
#include "suppmath.h"

typedef struct { // data for a rendered primitive
	Color col;
	Color transmission;
	Vector3 scale;
} PartData;

typedef struct {
	PartData part;
	Material mat;
	Mesh mesh;
} MeshData;

typedef struct {
	Vector3 scale;
	Mesh mesh;
} Scalable;

typedef struct {
	UIobject UIobj;
	Scalable scl;
} UIscalable;

typedef struct {
	vec3 velocity;
	vec3 acceleration;
} VelocityData;

typedef struct {
	ObjectRef from;
	ObjectRef to;
} ObjectPair;

typedef struct {
	Color col;
	unsigned char selected;

	Material matR;
	Material matG;
	Material matB;
	Material matA;

	Material mat;
} PainterData;


typedef enum {
	drawMesh,       // data: MeshData
	drawMeshOpaque, // data: MeshData
	drawEmerald,    // data: MeshData
	UImesh,         // data: UIscalable.scl

	velocity,       // data: VelocityData
	lifetime,       // data: float
	copyOnClick,    // data: NULL
	controllable,   // data: NULL

	particleCannon, // data: NULL
	physics,        // data: <this is not done>
	wireable,       // data: LinkedList<ObjectRef>
	sendOnClick,    // data: NULL

	blinker,        // data: Color
	avatar,         // data: ObjectRef
	equippable,     // data: NULL
	spawnTool,      // data: NULL

	wiring,         // data: ObjectPair
	sendOnSignal,   // data: NULL
	painter,        // data: PainterData
	remover,        // data: NULL

} Components;


ComponentDef definitions[20];

void initDefs();
void updateMaterial(MeshData);
void wireConnect(Object* from, Object* to);
Component* findFirstComponent(Object*, ComponentDef*);

#endif
