
#include <stdlib.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"

#include "renderHandler.h"
#include "uiHandler.h"

#include "componentDefs.h"

#define matrixScale(x, y, z) (Matrix){ x, 0.0f, 0.0f, 0.0f, 0.0f, y, 0.0f, 0.0f, 0.0f, 0.0f, z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f }
/*
typedef struct {
	MeshData md;
	UIobject* ui;
} SpawnerData;

void drawSpawner(Object* object) {
	Mesh mesh = (*(Mesh*)(((ComponentDef*)(object->def))->data));
	vec3 pos = vec3subPP(object->pos, camPos);

	Matrix translation = MatrixTranslate(pos.x, pos.y, pos.z);
	Matrix rotation = QuaternionToMatrix(QuaternionFromOrientationToOrientation((orientation) { 0, 0, 1, 0, 1, 0 }, object->orientation));
	Matrix move = MatrixMultiply(rotation, translation);

	MeshData data = (*(SpawnerData*)(object->data)).md;

	pushElement((RenderElement) {
		mesh,
		data.mat,
		MatrixMultiply(MatrixScale(data.part.scale.x, data.part.scale.y, data.part.scale.z), move),
		true
	});
}

void drawMeshOpaque(Object* object) {
	Mesh mesh = (*(Mesh*)(((ComponentDef*)(object->def))->data));
	vec3 pos = vec3subPP(object->pos, camPos);
	
	Matrix translation = MatrixTranslate(pos.x, pos.y, pos.z);
	Matrix rotation = QuaternionToMatrix(QuaternionFromOrientationToOrientation((orientation) { 0, 0, 1, 0, 1, 0 }, object->orientation));
	Matrix move = MatrixMultiply(rotation, translation);

	MeshData data = (*(MeshData*)(object->data));

	data.mat.maps[MATERIAL_MAP_TRANSMIT].color = (object == controlled) ? GRAY : WHITE;
	
	pushElement((RenderElement) {
		mesh,
		data.mat,
		MatrixMultiply(MatrixScale(data.part.scale.x, data.part.scale.y, data.part.scale.z), move),
		object == controlled
	});
}

void drawParticleOpaque(Object* object) {
	Mesh* mesh = ((ComponentDef*)(object->def))->data;
	vec3 pos = vec3subPP(object->pos, camPos);

	Matrix translation = MatrixTranslate(pos.x, pos.y, pos.z);
	Matrix rotation = QuaternionToMatrix(QuaternionFromOrientationToOrientation((orientation) { 0, 0, 1, 0, 1, 0 }, object->orientation));
	Matrix move = MatrixMultiply(rotation, translation);

	ParticleData* data = object->data;
	MeshData md = data->md;

	pushElement((RenderElement) {
		*mesh,
		md.mat,
		MatrixMultiply(MatrixScale(md.part.scale.x, md.part.scale.y, md.part.scale.z), move),
		object == controlled
	});
}

void initEmerald(Object* object) {
	SpawnerData* sd = malloc(sizeof(SpawnerData));
	MeshData data = sd->md;
	data.mat = createTransmitMaterial();
	data.mat.shader = transmission;
	updateMaterial(data);
	sd->md = data;
	sd->ui = malloc(sizeof(UIobject));
	*(sd->ui) = (UIobject){
		.func = p,
		.obj = object,
	};
	object->data = sd;
};
*/




typedef struct ObjectRefNode ObjectRefNode;
struct ObjectRefNode {
	Object* obj;
	ObjectRefNode* next;
};


void updateMaterial(MeshData data) {
	data.mat.maps[MATERIAL_MAP_DIFFUSE].color = data.part.col;
	data.mat.maps[MATERIAL_MAP_TRANSMIT].color = data.part.absorption;
};


void cannonInput(Component* comp, dataChannel channel, cstr data) {
	Object* object = comp->obj;
	switch (channel) {
	case interact:
		if (data.length >= 2 && (*(UIinteraction*)data.data) & ui_DLMB) {
			Object* obj = createObject(object->pos, (cstr) { 0 });
			*(float*)(addComponent(obj, &definitions[lifetime])->data) = 1.0;
			((VelocityData*)(addComponent(obj, &definitions[velocity])->data))->velocity = vec3fromv(Vector3Scale(object->orientation.forth, 64.0));
			MeshData* md = (MeshData*)addComponent(obj, &definitions[drawMesh])->data;
			md->mesh = brickMesh;
			md->part.col = GREEN;
			md->part.absorption = WHITE;
			md->part.scale = (Vector3){ 0.125, 0.125, 0.125 };
			obj->orientation = object->orientation;
			updateMaterial(*md);
		}
		break;
	case moveTo:
		if (data.length < 24) break;
		object->pos = *(pos3*)(data.data);
		break;
	case orient:
		if (data.length < 24) break;
		object->orientation = *(orientation*)(data.data);
		break;
	}
}


Component* findFirstComponent(Object* obj, ComponentDef* def) { // O(n), finds 1st component matching given definition
	Component* comp = obj->components;
	while (comp) {
		ComponentDef* ndef = comp->def;
		if (ndef == def) return comp;
		comp = comp->next;
	}
}

void wireConnect(Object* from, Object* to) {
	Component* comp = findFirstComponent(from, &definitions[wireable]);
	if (!comp) return;
	ObjectRefNode* node = malloc(sizeof(ObjectRefNode));
	node->next = comp->data;
	comp->data = node;
	node->obj = to;
}

void wireSendAll(Object* from, cstr data) {
	Component* comp = findFirstComponent(from, &definitions[wireable]);
	if (!comp) return;
	ObjectRefNode* node = comp->data;
	while (node) {
		ObjectRefNode* next = node->next;
		sendSignal(node->obj, wire, data);
		node = next;
	}
}

void wireable_elim(Component* comp) {
	ObjectRefNode* node = comp->data;
	while (node) {
		ObjectRefNode* next = node->next;
		free(node);
		node = next;
	}
}


void copyOnClick_UIaction(Component* comp, dataChannel channel, cstr data) {
	if (data.length >= 3 & channel == click) {
		ClickAction act = *(ClickAction*)data.data;
		if (act.id == solid)
		if (act.action & ui_DLMB) {
			Object* source = comp->obj;
			Object* object = createObject(vec3addPv(source->pos, source->orientation.up), (cstr) { NULL, 0 });
			copyComponents(object, source);
			object->orientation = source->orientation;
		}
	}
};

void paint(Component* comp, Color col) {
	MeshData* md = comp->data;
	md->part.col = col;
	updateMaterial(*md);
}

void controllable_UIaction(Component* comp, dataChannel channel, cstr data) {
	if (data.length >= 3 & channel == click) {
		ClickAction act = *(ClickAction*)data.data;
		if (act.id == solid)
		if (act.action & ui_DLMB) setControlled(comp->obj);
	}
}

void sendOnClick_UIaction(Component* comp, dataChannel channel, cstr data) {
	if (data.length >= 3 & channel == click) {
		ClickAction act = *(ClickAction*)data.data;
		if (act.id == solid)
		if (act.action & ui_DLMB) wireSendAll(comp->obj, (cstr) { 0 });
	}
}

void blinker_recieve(Component* comp, dataChannel channel, cstr data) {
	if (channel == wire) {
		Component* meshC = findFirstComponent(comp->obj, &definitions[drawMesh]);
		Color interm = ((MeshData*)(meshC->data))->part.col;
		paint(meshC, *(Color*)comp->data);
		*(Color*)comp->data = interm;
	}
}


void initFloat(Component* comp) {
	comp->data = malloc(sizeof(float));
}

void initColor(Component* comp) {
	comp->data = malloc(sizeof(Color));
}


void lifetime_tick(Component* comp, float dt) {
	float* data = comp->data;
	*data -= dt;
	if ((*data) < 0) eliminateObject(comp->obj);
}


void velocity_init(Component* comp) {
	comp->data = calloc(1, sizeof(VelocityData));
}

void velocity_tick(Component* comp, float dt) {
	Object* object = comp->obj;
	VelocityData* data = comp->data;
	object->pos = vec3addPV(object->pos, vec3scale(data->velocity, dt));
	data->velocity = vec3addVV(data->velocity, vec3scale(data->acceleration, dt));
}


void initScalable(Component* comp) {
	comp->data = malloc(sizeof(Scalable));
}


void UImesh_init(Component* comp) {
	UIscalable* data = malloc(sizeof(UIscalable));
	data->UIobj = (UIobject){
		.type = solid,
		.obj = comp->obj,
	};
	comp->data = data;
}

void UImesh_pushUI(Component* comp) {
	Object* object = comp->obj;
	vec3 pos = vec3subPP(object->pos, camPos);

	Matrix translation = MatrixTranslate((float)pos.x, (float)pos.y, (float)pos.z);
	Matrix rotation = QuaternionToMatrix(QuaternionFromOrientationToOrientation((orientation) { 0, 0, 1, 0, 1, 0 }, object->orientation));
	Matrix move = MatrixMultiply(rotation, translation);

	UIscalable* data = comp->data;

	Vector3 scale = data->scl.scale;

	if (controlled != object) pushUIobj(data->scl.mesh, MatrixMultiply(MatrixScale(scale.x, scale.y, scale.z), move), &data->UIobj);
}

void freeData(Component* comp) {
	free(comp->data);
}


void drawEmerald_init(Component* comp) {
	MeshData* data = malloc(sizeof(MeshData));
	data->mat = createTransmitMaterial();
	data->mat.shader = transmission;
	PartData* part = &data->part;
	part->scale = (Vector3){ 1.0, 0.25, 0.5 };
	//part->col = (Color){ rand() & 255, rand() & 255, rand() & 255, 255 };
	part->col = BLUE;
	part->absorption = (Color){ rand() & 255, rand() & 255, rand() & 255, 255 };
	updateMaterial(*data);
	srand(rand());
	comp->data = data;
}


void drawMesh_init(Component* comp) {
	MeshData* data = malloc(sizeof(MeshData));
	data->mat = createTransmitMaterial();
	data->mat.shader = transmission;
	data->part.scale = (Vector3){1.0, 1.0, 1.0};
	comp->data = data;
}

void drawMesh_draw(Component* comp) {
	Object* object = comp->obj;
	vec3 pos = vec3subPP(object->pos, camPos);

	Matrix translation = MatrixTranslate(pos.x, pos.y, pos.z);
	Matrix rotation = QuaternionToMatrix(QuaternionFromOrientationToOrientation((orientation) { 0, 0, 1, 0, 1, 0 }, object->orientation));
	Matrix move = MatrixMultiply(rotation, translation);

	MeshData data = (*(MeshData*)(comp->data));

	pushElement((RenderElement) {
		data.mesh,
		data.mat,
		MatrixMultiply(MatrixScale(data.part.scale.x, data.part.scale.y, data.part.scale.z), move),
		.isTransparent = true
	});
}

void drawMesh_elim(Component* comp) {
	MeshData* data = comp->data;
	unloadTransmitMaterial(data->mat);
	free(data);
}


void wireable_draw(Component* comp) {
	ObjectRefNode* node = comp->data;
	while (node) {
		Vector3 posF = vec3tov(vec3subPP(comp->obj->pos, camPos));
		Vector3 posT = vec3tov(vec3subPP(node->obj->pos, camPos));
		Vector3 posM = Vector3Scale(Vector3Add(posF, posT), 0.5);
		DrawLine3D(posF, posM, ORANGE);
		DrawLine3D(posM, posT, YELLOW);
		node = node->next;
	}
}


void initDefs() {
	
	definitions[drawMesh] = (ComponentDef){
		.init = drawMesh_init,
		.draw = drawMesh_draw,
		.elim = drawMesh_elim,
	};

	definitions[drawEmerald] = (ComponentDef){
		.init = drawEmerald_init,
		.draw = drawMesh_draw,
		.elim = drawMesh_elim,
	};

	definitions[UImesh] = (ComponentDef){
		.init = UImesh_init,
		.pushUI = UImesh_pushUI,
		.elim = freeData,
	};

	definitions[velocity] = (ComponentDef){
		.init = velocity_init,
		.tick = velocity_tick,
		.elim = freeData,
	};

	definitions[lifetime] = (ComponentDef){
		.init = initFloat,
		.tick = lifetime_tick,
		.elim = freeData,
	};

	definitions[copyOnClick] = (ComponentDef){
		.recieve = copyOnClick_UIaction
	};

	definitions[controllable] = (ComponentDef){
		.recieve = controllable_UIaction
	};

	definitions[particleCannon] = (ComponentDef){
		.recieve = cannonInput
	};

	definitions[wireable] = (ComponentDef){
		.draw = wireable_draw,
		.elim = wireable_elim,
	};

	definitions[sendOnClick] = (ComponentDef){
		.recieve = sendOnClick_UIaction
	};

	definitions[blinker] = (ComponentDef){
		.init = initColor,
		.recieve = blinker_recieve,
		.elim = freeData,
	};

}
