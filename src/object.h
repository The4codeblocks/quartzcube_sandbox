
#ifndef objectheader
#define objectheader

#include "raylib.h"

#include "channels.h"
#include "suppmath.h"

struct Component;
typedef struct Component Component; // component element

struct ObjectNode;
typedef struct ObjectNode ObjectNode; // object linked list element

struct ComponentDef;
typedef struct ComponentDef ComponentDef; // component spec, avoids component bloat

typedef struct { // in-world object
	pos3 pos;

	orientation orientation;

	Component* components; // component linked list

	ObjectNode* node; // carrier node backpointer

	void* data; // arbitrary data structure
} Object;

struct Component {
	ComponentDef* def; // associated definition
	Object* obj; // associated object
	Component* prev; // preceding element
	Component* next; // succeding element
	void* data; // arbitrary data structure
};

typedef unsigned short UIinteraction;

typedef enum {
	solid,
} UIID;

typedef struct {
	Object* obj;
	UIID type;
} UIobject;

struct ComponentDef {
	void (*init)(Component*); // initializes component
	void (*tick)(Component*, float); // ticks component
	void (*elim)(Component*); // removes component from object

	void (*draw)(Component*); // draw component
	void (*pushUI)(Component*); // push component to UI
	void (*UIaction)(Component*, UIobject, UIinteraction); // parse UI input

	void (*recieve)(Component*, dataChannel, cstr); // signals

	cstr (*serialize)(Component); // convert component to data
	void (*deserialize)(Component*, cstr); // load data to component

	void* data; // arbitrary data structure
};

struct ObjectNode {
	Object object;
	ObjectNode* next;
	ObjectNode* prev;
};

ObjectNode* rootObjectNode; // object list root node pointer, can be null

Object* createObject(pos3, cstr); // allocates memory to create an object

void eliminateObject(Object*);

Component* addComponent(Object*, ComponentDef*); // adds component to object

void eliminateComponent(Component*); // removes component from object

void copyComponents(Object*, Object*); // copies components with reinitialization

void drawFrom(ObjectNode*); // draws starting from the specified linked list element

void UIFrom(ObjectNode*); // pushes UI starting from the specified linked list element

inline void sendSignalComp(Component* comp, dataChannel channel, cstr data) {
	ComponentDef* def = comp->def;
	if (def->recieve) def->recieve(comp, channel, data);
};

inline void sendSignal(Object* object, dataChannel channel, cstr data) {
	Component* comp = object->components;
	while (comp) {
		sendSignalComp(comp, channel, data);
		comp = comp->next;
	}
};

#endif
