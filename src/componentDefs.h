
#ifndef objectdefsheader
#define objectdefsheader

#include "raylib.h"
#include "object.h"
#include "suppmath.h"

typedef struct { // data for a rendered primitive
	Color col;
	Color absorption;
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


typedef enum {
	drawMesh,       // data: MeshData
	drawEmerald,    // data: MeshData
	UImesh,         // data: UIscalable.scl
	velocity,       // data: VelocityData
	lifetime,       // data: float
	copyOnClick,    // data: NULL
	controllable,   // data: NULL
	particleCannon, // data: NULL
	physics,        // data: <this is not done>
	wireable,       // data: LinkedList<Object*>
	sendOnClick,    // data: NULL
	blinker,        // data: Color

} Components;


ComponentDef definitions[13];

void initDefs();
void updateMaterial(MeshData);
void wireConnect(Object* from, Object* to);

#endif
