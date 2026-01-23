
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include "suppmath.h"

#include "object.h"
#include "componentDefs.h"

#include "renderHandler.h"
#include "uiHandler.h"

#include "channels.h"
#include "controls.h"

#include "gametime.h"


int main()
{
	srand(time(NULL));

	drawInit();
	UIInit();

	float pitch = 0;

	updateCamControl(&mainCam, camOri, pitch, &camR, &camFace);

	initDefs();

	float sensitivity = DEG2RAD * 0.5;
	float speed = 4.0;

	bool mouseLocked = false;

	SetExitKey(-1);
	SetMousePosition(winXCenter, winYCenter);

	{
		Object* object = createObject((pos3) { 4, 0, 0 }, (cstr) { NULL, 0 });
		addComponent(object, &definitions[spawnTool]);
		addComponent(object, &definitions[equippable]);
		UIscalable* ui = addComponent(object, &definitions[UImesh])->data;
		ui->scl.mesh = brickMesh;
		ui->scl.scale = (Vector3){ 0.25, 0.25, 0.25 };
		MeshData* md = addComponent(object, &definitions[drawMesh])->data;
		md->mesh = brickMesh;
		md->part.col = LIGHTGRAY;
		md->part.absorption = WHITE;
		md->part.scale = (Vector3){ 0.25, 0.25, 0.25 };
		updateMaterial(*md);
	}

	{
		Object* object = createObject((pos3) { 2, 0, 0 }, (cstr) { NULL, 0 });
		addComponent(object, &definitions[avatar]);
		UIscalable* ui = addComponent(object, &definitions[UImesh])->data;
		ui->scl.mesh = brickMesh;
		ui->scl.scale = (Vector3){ 1.0, 1.0, 1.0 };
		addComponent(object, &definitions[controllable]);
		MeshData* md = addComponent(object, &definitions[drawMeshOpaque])->data;
		md->mesh = brickMesh;
		md->part.col = WHITE;
		md->part.absorption = WHITE;
		md->part.scale = (Vector3){ 1.0, 1.0, 1.0 };
		updateMaterial(*md);
	}

	{
		Object* source;
		Object* dest;

		{
			source = createObject((pos3) { -4, 4, 0 }, (cstr) { NULL, 0 });
			UIscalable* ui = addComponent(source, &definitions[UImesh])->data;
			ui->scl.mesh = brickMesh;
			ui->scl.scale = (Vector3){ 0.25, 0.25, 0.25 };
			addComponent(source, &definitions[sendOnClick]);
			addComponent(source, &definitions[wireable]);
			MeshData* md = addComponent(source, &definitions[drawMesh])->data;
			md->mesh = brickMesh;
			md->part.col = RED;
			md->part.absorption = WHITE;
			md->part.scale = (Vector3){ 0.25, 0.25, 0.25 };
			updateMaterial(*md);
		}

		{
			dest = createObject((pos3) { -4, 0, 0 }, (cstr) { NULL, 0 });
			MeshData* md = addComponent(dest, &definitions[drawMesh])->data;
			md->mesh = brickMesh;
			md->part.col = GRAY;
			md->part.absorption = WHITE;
			md->part.scale = (Vector3){ 0.25, 0.25, 0.25 };
			updateMaterial(*md);
			UIscalable* ui = addComponent(dest, &definitions[UImesh])->data;
			ui->scl.mesh = brickMesh;
			ui->scl.scale = (Vector3){ 0.25, 0.25, 0.25 };
			*(Color*)addComponent(dest, &definitions[blinker])->data = (Color){0,255,0,255};
		}

		wireConnect(source, dest);
	}

	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses the Close button on the window
	{
		updateTime();

		BeginDrawing(); // CLIENT

		// control
		if (IsKeyPressed(KEY_ESCAPE)) {
			mouseLocked = false;
			ShowCursor();
		}
		
		if (IsKeyPressed(KEY_TAB)) {
			if (IsKeyDown(KEY_LEFT_CONTROL)) controlled = NULL;
			if (controlled && IsKeyDown(KEY_LEFT_SHIFT)) {
				Component* avatarComp = findFirstComponent(controlled, &definitions[avatar]);
				if (controlled && avatarComp) avatarComp->data = NULL;
			}
		}
		
		if (!IsWindowFocused()) {
			mouseLocked = false;
			ShowCursor();
		}
		
		if (mouseLocked) {
			camOri = rotateOrientationAxisAngle(camOri, camOri.up, (GetMouseX() - winXCenter) * sensitivity);
			pitch = fmin(fmax(pitch + (GetMouseY() - winYCenter) * sensitivity, -5.0*PI/8.0), 5.0*PI/8.0);
			SetMousePosition(winXCenter, winYCenter);
			updateCamControl(&mainCam, camOri, pitch, &camR, &camFace);
		}

		orientation targetOri = camOri;
		bool update = false;
		if (IsKeyDown(KEY_R)) {
			targetOri = rotateOrientationAxisAngle(targetOri, targetOri.forth, -dt);
			update = true;
		}
		if (IsKeyDown(KEY_F)) {
			targetOri = rotateOrientationAxisAngle(targetOri, targetOri.forth, dt);
			update = true;
		}
		if (!controlled) {
			if (update) {
				camOri = targetOri;
				updateCamControl(&mainCam, camOri, pitch, &camR, &camFace);
			}
		}

		float displacement = dt * speed;
		Vector3 movement = Vector3Scale(getMovement(), displacement);

		if (mouseLocked)
		if (controlled) {
			Component* comp = controlled->components;
			while (comp) {
				ComponentDef* def = comp->def;
				if (def->recieve) {
					def->recieve(comp, moveTo, moveTo_(vec3addPv(camPos, movement)));
					def->recieve(comp, orient, orient_(projectOrientation(targetOri, comp->obj->orientation.up)));
				}
				comp = comp->next;
			}
			camPos = controlled->pos;
			camOri.up = controlled->orientation.up;
			camOri = rotateOrientationAxisAngle(camOri, camOri.up, (GetMouseX() - winXCenter) * sensitivity);
			updateCamControl(&mainCam, camOri, pitch, &camR, &camFace);
			mainCam.position = Vector3Scale(camFace.forth, -4.0);
			mainCam.target = (Vector3){ 0 };
		} else {
			camPos = vec3addPv(camPos, movement);
			updateCamControl(&mainCam, camOri, pitch, &camR, &camFace);
			mainCam.position = (Vector3){ 0 };
		}
		/*
		if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
			Object* object = createObject(vec3addPv(camPos, Vector3Scale(camFace.forth, 4.0)), (cstr) { NULL, 0 });
			object->orientation = camFace;
			((MeshData*)(addComponent(object, &definitions[drawEmerald])->data))->mesh = brickMesh;
			addComponent(object, &definitions[copyOnClick]);
			UIscalable* ui = (UIscalable*)addComponent(object, &definitions[UImesh])->data;
			ui->scl.mesh = brickMesh;
			ui->scl.scale = (Vector3){ 1.0, 0.25, 0.5 };
		}
		*/

		drawMain();
		UIMain();
		if (mouseLocked) {
			unsigned short action = 0;
			if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) action = action | ui_DLMB;
			if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) action = action | ui_DRMB;
			if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE)) action = action | ui_DMMB;
			if (IsMouseButtonPressed(MOUSE_BUTTON_SIDE)) action = action | ui_DSMB;
			if (IsMouseButtonPressed(MOUSE_BUTTON_EXTRA)) action = action | ui_DEMB;
			if (IsMouseButtonPressed(MOUSE_BUTTON_FORWARD)) action = action | ui_DFMB;
			if (IsMouseButtonPressed(MOUSE_BUTTON_BACK)) action = action | ui_DBMB;
			if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) action = action | ui_ULMB;
			if (IsMouseButtonReleased(MOUSE_BUTTON_RIGHT)) action = action | ui_URMB;
			if (IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE)) action = action | ui_UMMB;
			if (IsMouseButtonReleased(MOUSE_BUTTON_SIDE)) action = action | ui_USMB;
			if (IsMouseButtonReleased(MOUSE_BUTTON_EXTRA)) action = action | ui_UEMB;
			if (IsMouseButtonReleased(MOUSE_BUTTON_FORWARD)) action = action | ui_UFMB;
			if (IsMouseButtonReleased(MOUSE_BUTTON_BACK)) action = action | ui_UBMB;

			directionOrtho scrollDir = getScrollDir();
			if (scrollDir) {
				if (controlled) sendSignal(controlled, paginate, paginate_(scrollDir));
				UIobject* ID = getPointed();
				if (ID) sendSignal(ID->obj, scroll, scroll_(scrollDir, ID->type));
			}

			if (controlled && action)
				sendSignal(controlled, interact, interact_(action));
			if (action) send(action);
		}

		if (!mouseLocked & IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
			mouseLocked = true;
			HideCursor();
			SetMousePosition(winXCenter, winYCenter);
		}
		//if (!IsWindowFocused()) mouseLocked = false;

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();

		// SERVER

		ObjectNode* node = rootObjectNode;
		while (node) {
			Component* comp = node->object.components;
			ObjectNode* next = node->next;
			while (comp) {
				ComponentDef* def = comp->def;
				ObjectNode* node = comp->obj->node;
				size_t* delP = &(node->next);
				if (def->tick) def->tick(comp, dt);
				if ((*delP) == 1) {
					free(node);
					break;
				}
				comp = comp->next;
			}
			node = next;
		}
	}

	drawFin();

	return 0;
}
