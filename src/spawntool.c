
#include "spawntool.h"
#include "uiHandler.h"
#include "componentDefs.h"
#include "renderHandler.h"

Object* spawner(pos3 pos, Vector3 size, orientation ori) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	obj->orientation = ori;
	addComponent(obj, &definitions[controllable]);
	addComponent(obj, &definitions[particleCannon]);
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = RAYWHITE;
	md->part.transmission = WHITE;
	md->part.scale = (Vector3){ 1.0, 1.0, 2.0 };
	updateMaterial(*md);
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = (Vector3){ 1.0, 1.0, 2.0 };
	return obj;
}

Object* button(pos3 pos, Vector3 size, orientation ori) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	obj->orientation = ori;
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = size;
	addComponent(obj, &definitions[sendOnClick]);
	addComponent(obj, &definitions[wireable]);
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = RED;
	md->part.transmission = WHITE;
	md->part.scale = size;
	updateMaterial(*md);
	return obj;
}

Object* relay(pos3 pos, Vector3 size, orientation ori) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	obj->orientation = ori;
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = size; // (Vector3) { 0.125, 0.125, 0.125 };
	addComponent(obj, &definitions[sendOnSignal]);
	addComponent(obj, &definitions[wireable]);
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = ORANGE;
	md->part.transmission = WHITE;
	md->part.scale = size; // (Vector3) { 0.125, 0.125, 0.125 };
	updateMaterial(*md);
	return obj;
}

Object* toggleLamp(pos3 pos, Vector3 size, orientation ori) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	obj->orientation = ori;
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = size;
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = GRAY;
	md->part.transmission = WHITE;
	md->part.scale = size;
	updateMaterial(*md);
	*(Color*)addComponent(obj, &definitions[blinker])->data = (Color){ 0,255,0,255 };
	return obj;
}

Object* wireTool(pos3 pos, Vector3 size, orientation ori) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	obj->orientation = ori;
	addComponent(obj, &definitions[wiring]);
	addComponent(obj, &definitions[equippable]);
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = size;
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = spoolMesh;
	md->part.col = LIGHTGRAY;
	md->part.transmission = WHITE;
	md->part.scale = size;
	updateMaterial(*md);
	return obj;
}

Object* paintTool(pos3 pos, Vector3 size, orientation ori) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	obj->orientation = ori;
	addComponent(obj, &definitions[painter]);
	addComponent(obj, &definitions[equippable]);
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = size;
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = BLUE;
	md->part.transmission = WHITE;
	md->part.scale = size;
	updateMaterial(*md);
	return obj;
}

Object* remove(pos3 pos, Vector3 size, orientation ori) {
	Object* obj = createObject(pos, (cstr) { NULL, 0 });
	obj->orientation = ori;
	addComponent(obj, &definitions[remover]);
	addComponent(obj, &definitions[equippable]);
	UIscalable* ui = addComponent(obj, &definitions[UImesh])->data;
	ui->scl.mesh = brickMesh;
	ui->scl.scale = size;
	MeshData* md = addComponent(obj, &definitions[drawMesh])->data;
	md->mesh = brickMesh;
	md->part.col = (Color){255, 0, 0, 255};
	md->part.transmission = (Color){128, 255, 255, 255};
	md->part.scale = size;
	updateMaterial(*md);
	return obj;
}

void dispmat(pos3 pos, Vector3 size, orientation ori) {
}

void ghostWireframeBox(pos3 pos, Vector3 size, orientation ori) {
	Vector3 rpos = vec3tov(vec3subPP(pos, camPos));

	Vector3 basisX = Vector3Scale(getRight(ori), size.y / 2);
	Vector3 basisY = Vector3Scale(ori.up, size.y / 2);
	Vector3 basisZ = Vector3Scale(ori.forth, size.y / 2);

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

	DrawLine3D(rpos, Vector3Add(rpos, Vector3Add(basisZ, basisZ)), BLUE);
	DrawLine3D(rpos, Vector3Add(rpos, Vector3Add(basisY, basisY)), GREEN);
}

Element elements[elementCount] = {
	{ button,     .name = "button", ghostWireframeBox },
	{ toggleLamp, .name = "light" , ghostWireframeBox },
	{ wireTool,   .name = "wiring", ghostWireframeBox },
	{ relay,      .name = "relay" , ghostWireframeBox },
	{ spawner,    .name = "cannon", ghostWireframeBox },
	{ paintTool,  .name = "paint" , ghostWireframeBox },
	{ remove,     .name = "remove", ghostWireframeBox },
	{ dispmat,    .name = "displaymatrix", ghostWireframeBox },
};

Object* spawnElement(pos3 pos, Vector3 size, orientation ori, int index) {
	return elements[index].cons(pos, size, ori);
}

void    ghostElement(pos3 pos, Vector3 size, orientation ori, int index) {
	elements[index].ghost(pos, size, ori);
}


#define configCount 4

void spawnTool_recieve(Component* comp, dataChannel channel, cstr data) {
	Object* object = comp->obj;
	SpawnToolData* cdata = comp->data;
	switch (channel) {
	case interact:
		double snap = cdata->gridsnap.value;
		if (data.length >= 2) {
			int configIdx = cdata->configIdx;
			UIinteraction interaction = *(UIinteraction*)data.data;
			if ((interaction & ui_DLMB) && (configIdx != 3)) {
				orientation savedori = cdata->savedori;
				orientation objori = Vector3Equals(savedori.forth, (Vector3) { 0 }) ? object->orientation : savedori;
				spawnElement(pos3round(vec3addPv(object->pos, Vector3Scale(object->orientation.forth, 2.0)), (pos3) { 0 }, (vec3) { snap, snap, snap }), (Vector3) { 0.25, 0.25, 0.25 }, rotateOrientationAxisAngle(objori, objori.up, cdata->diroff.value * DEG2RAD), * (int*)comp->data);
			}
			if (configIdx == 3) {
				if (interaction & ui_DLMB) cdata->savedori = object->orientation;
				if (interaction & ui_DRMB) cdata->savedori = orientationEqual(cdata->savedori, (orientation){0}) ? (orientation) { 0, 0, 1,  0, 1, 0 } : (orientation) {0};
			}
		}
		break;
	case paginate:
		if (data.length >= 1) {
			directionOrtho dir = ((directionOrtho) * (char*)data.data);
			int configIdx = cdata->configIdx;
			switch (dir) {
			case dirF:
			case dirU:
				if (configIdx == 0) {
					cdata->index = wrapInt(cdata->index - 1, 0, elementCount);
					break;
				}
			case dirB:
			case dirD:
				switch (configIdx) {
				case 0:
					cdata->index = wrapInt(cdata->index + 1, 0, elementCount);
					break;
				case 1:
					parseNumberInputControl(&(cdata->gridsnap), dir);
					cdata->gridsnap.value = max(cdata->gridsnap.value, 0);
					break;
				case 2:
					parseRotateInputControl(&(cdata->diroff), dir);
					break;
				}
				break;
			case dirL:
				cdata->configIdx = wrapInt(configIdx - 1, 0, configCount);
				cdata->gridsnap.digit = 0;
				break;
			case dirR:
				cdata->configIdx = wrapInt(configIdx + 1, 0, configCount);
				cdata->gridsnap.digit = 0;
				break;
			}
		}
		break;
	}
}

void spawnTool_init(Component* comp) {
	SpawnToolData* data = malloc(sizeof(SpawnToolData));
	data->index = 0;
	data->mat = createTransmitMaterial();
	data->textbox = LoadRenderTexture(128, 32);
	data->mat.maps[MATERIAL_MAP_DIFFUSE].texture = data->textbox.texture;
	data->configIdx = 0;

	data->gridsnap.value = 0.25;
	data->gridsnap.digit = 0.0;

	data->diroff.value = 0.0;
	data->diroff.digit = 0.0;
	
	data->savedori = (orientation){0};

	comp->data = data;
}

void spawnTool_draw(Component* comp) {
	Object* object = comp->obj;
	SpawnToolData* data = comp->data;

	EndMode3D();
	EndTextureMode();
	BeginTextureMode(data->textbox);
	ClearBackground(BLACK);

	char* text;
	switch (data->configIdx) {
	case 0:
		text = elements[data->index].name;
		break;
	case 1:
		text = TextFormat("grid: %.4f", data->gridsnap.value);
		break;
	case 2:
		text = TextFormat("rotation: %.1f", data->diroff.value); // unsupported degree symbol: °
		break;
	case 3:
		text = orientationEqual(data->savedori, (orientation){0}) ? "free orient" : (orientationEqual(data->savedori, (orientation) { 0, 0, 1,  0, 1, 0 }) ? "fixed orient" : "locked orient");
		break;
	default:
		text = "";
	}

	DrawTextPro(GetFontDefault(), text, (Vector2) { 0.0, 0.0 }, (Vector2) { 0.0, 0.0 }, 0.0, 16.0, 2.0, WHITE);
	EndTextureMode();
	BeginTextureMode(rndt);
	BeginMode3D(mainCam);

	Vector3 pos = vec3tov(vec3subPP(object->pos, camPos));

	Matrix translation = MatrixTranslate(pos.x, pos.y, pos.z);
	Matrix rotation = QuaternionToMatrix(QuaternionFromOrientationToOrientation((orientation) { 0, 0, 1, 0, 1, 0 }, object->orientation));
	Matrix move = MatrixMultiply(MatrixMultiply(MatrixScale(1.0, 0.25, 0.25), MatrixRotateX(-PI / 2.0)), MatrixMultiply(MatrixMultiply(MatrixTranslate(0.75, 0, 0), rotation), translation));

	pushElement((RenderElement) {
		planeMesh,
		data->mat,
		move,
		true,
	});

	double snap = data->gridsnap.value;
	orientation savedori = data->savedori;
	orientation objori = Vector3Equals(savedori.forth, (Vector3){0}) ? object->orientation : savedori;
	ghostElement(pos3round(vec3addPv(object->pos, Vector3Scale(object->orientation.forth, 2.0)), (pos3) { 0 }, (vec3) { snap, snap, snap }), (Vector3) { 0.25, 0.25, 0.25 }, rotateOrientationAxisAngle(objori, objori.up, data->diroff.value * DEG2RAD), *(int*)comp->data);
}
