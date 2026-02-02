
#include "spawntool.h"
#include "uiHandler.h"
#include "componentDefs.h"
#include "renderHandler.h"

Object* spawner(pos3 pos, Vector3 size) {
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

Object* button(pos3 pos, Vector3 size) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = size;
	addComponent(obj, &definitions[sendOnClick]);
	addComponent(obj, &definitions[wireable]);
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = RED;
	md->part.absorption = WHITE;
	md->part.scale = size;
	updateMaterial(*md);
	return obj;
}

Object* relay(pos3 pos, Vector3 size) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = size; // (Vector3) { 0.125, 0.125, 0.125 };
	addComponent(obj, &definitions[sendOnSignal]);
	addComponent(obj, &definitions[wireable]);
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = ORANGE;
	md->part.absorption = WHITE;
	md->part.scale = size; // (Vector3) { 0.125, 0.125, 0.125 };
	updateMaterial(*md);
	return obj;
}

Object* toggleLamp(pos3 pos, Vector3 size) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = size;
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = GRAY;
	md->part.absorption = WHITE;
	md->part.scale = size;
	updateMaterial(*md);
	*(Color*)addComponent(obj, &definitions[blinker])->data = (Color){ 0,255,0,255 };
	return obj;
}

Object* wireTool(pos3 pos, Vector3 size) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	addComponent(obj, &definitions[wiring]);
	addComponent(obj, &definitions[equippable]);
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = size;
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = spoolMesh;
	md->part.col = LIGHTGRAY;
	md->part.absorption = WHITE;
	md->part.scale = size;
	updateMaterial(*md);
	return obj;
}

Object* paintTool(pos3 pos, Vector3 size) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	addComponent(obj, &definitions[painter]);
	addComponent(obj, &definitions[equippable]);
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = size;
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = BLUE;
	md->part.absorption = WHITE;
	md->part.scale = size;
	updateMaterial(*md);
	return obj;
}

void ghostWireframeBox(pos3 pos, Vector3 size, orientation ori) {
	Vector3 rpos = vec3tov(vec3subPP(pos, camPos));

	Quaternion rotation = QuaternionFromOrientationToOrientation((orientation) { 0, 0, 1, 0, 1, 0 }, ori);

	Vector3 basisX = Vector3RotateByQuaternion((Vector3) { size.x/2.0, 0, 0 }, rotation);
	Vector3 basisY = Vector3RotateByQuaternion((Vector3) { 0, size.y/2.0, 0 }, rotation);
	Vector3 basisZ = Vector3RotateByQuaternion((Vector3) { 0, 0, size.z/2.0 }, rotation);

	Vector3 XYZ = Vector3Add(Vector3Add(basisX, basisY), basisZ);
	Vector3 XYz = Vector3Subtract(Vector3Add(basisX, basisY), basisZ);
	Vector3 XyZ = Vector3Add(Vector3Subtract(basisX, basisY), basisZ);
	Vector3 Xyz = Vector3Subtract(Vector3Subtract(basisX, basisY), basisZ);
	Vector3 xyz = Vector3Negate(XYZ);
	Vector3 xyZ = Vector3Negate(XYz);
	Vector3 xYz = Vector3Negate(XyZ);
	Vector3 xYZ = Vector3Negate(Xyz);

	XYZ = Vector3Add(rpos, XYZ);
	XYz = Vector3Add(rpos, XYz);
	XyZ = Vector3Add(rpos, XyZ);
	Xyz = Vector3Add(rpos, Xyz);
	xyz = Vector3Add(rpos, xyz);
	xyZ = Vector3Add(rpos, xyZ);
	xYz = Vector3Add(rpos, xYz);
	xYZ = Vector3Add(rpos, xYZ);

	DrawLine3D(XYZ, XYz, WHITE);
	DrawLine3D(xYZ, xYz, WHITE);
	DrawLine3D(XyZ, Xyz, WHITE);
	DrawLine3D(xyZ, xyz, WHITE);

	DrawLine3D(XYZ, XyZ, WHITE);
	DrawLine3D(xYZ, xyZ, WHITE);
	DrawLine3D(XYz, Xyz, WHITE);
	DrawLine3D(xYz, xyz, WHITE);

	DrawLine3D(XYZ, xYZ, WHITE);
	DrawLine3D(XyZ, xyZ, WHITE);
	DrawLine3D(XYz, xYz, WHITE);
	DrawLine3D(Xyz, xyz, WHITE);
}

Element elements[elementCount] = {
	{ button,     .name = "button", ghostWireframeBox },
	{ toggleLamp, .name = "light" , ghostWireframeBox },
	{ wireTool,   .name = "wiring", ghostWireframeBox },
	{ relay,      .name = "relay" , ghostWireframeBox },
	{ spawner,    .name = "cannon", ghostWireframeBox },
	{ paintTool,  .name = "paint" , ghostWireframeBox },
};

Object* spawnElement(pos3 pos, Vector3 size, orientation ori, int index) {
	Object* obj = elements[index].cons(pos, size);
	obj->orientation = ori;
	return obj;
}

void    ghostElement(pos3 pos, Vector3 size, orientation ori, int index) {
	elements[index].ghost(pos, size, ori);
}
