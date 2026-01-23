
#include "spawntool.h"
#include "uiHandler.h"
#include "componentDefs.h"
#include "renderHandler.h"

Object* spawner(pos3 pos) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	addComponent(obj, &definitions[controllable]);
	addComponent(obj, &definitions[particleCannon]);
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = RAYWHITE;
	md->part.absorption = WHITE;
	md->part.scale = (Vector3){ 1.0, 1.0, 2.0 };
	updateMaterial(*md);
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = (Vector3){ 1.0, 1.0, 2.0 };
	return obj;
}

Object* button(pos3 pos) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = (Vector3){ 0.25, 0.25, 0.25 };
	addComponent(obj, &definitions[sendOnClick]);
	addComponent(obj, &definitions[wireable]);
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = RED;
	md->part.absorption = WHITE;
	md->part.scale = (Vector3){ 0.25, 0.25, 0.25 };
	updateMaterial(*md);
	return obj;
}

Object* relay(pos3 pos) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = (Vector3){ 0.125, 0.125, 0.125 };
	addComponent(obj, &definitions[sendOnSignal]);
	addComponent(obj, &definitions[wireable]);
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = ORANGE;
	md->part.absorption = WHITE;
	md->part.scale = (Vector3){ 0.125, 0.125, 0.125 };
	updateMaterial(*md);
	return obj;
}

Object* toggleLamp(pos3 pos) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = (Vector3){ 0.25, 0.25, 0.25 };
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = GRAY;
	md->part.absorption = WHITE;
	md->part.scale = (Vector3){ 0.25, 0.25, 0.25 };
	updateMaterial(*md);
	*(Color*)addComponent(obj, &definitions[blinker])->data = (Color){ 0,255,0,255 };
	return obj;
}

Object* wireTool(pos3 pos) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	addComponent(obj, &definitions[wiring]);
	addComponent(obj, &definitions[equippable]);
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = (Vector3){ 0.25, 0.25, 0.25 };
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = spoolMesh;
	md->part.col = LIGHTGRAY;
	md->part.absorption = WHITE;
	md->part.scale = (Vector3){ 0.25, 0.25, 0.25 };
	updateMaterial(*md);
	return obj;
}

Element elements[elementCount] = {
	{ button,     .name = "button" },
	{ toggleLamp, .name = "light"  },
	{ wireTool,   .name = "wiring" },
	{ relay,      .name = "relay"  },
	{ spawner,    .name = "cannon" },
};

Object* spawnElement(pos3 pos, orientation ori, int index) {
	Object* obj = elements[index].cons(pos);
	obj->orientation = ori;
	return obj;
}