
#include <stdlib.h>

#include "renderHandler.h"
#include "object.h"

RenderSortNode* renderedRoot = NULL;
RenderSortNode* pooledRoot = NULL;

RenderSortNode* obtainPooledRenderNode() {
	if (!pooledRoot) pooledRoot = calloc(1, sizeof(RenderSortNode));
	RenderSortNode* out = pooledRoot;
	pooledRoot = out->right;
	out->left = NULL;
	out->right = NULL;
	out->up = NULL;
	return out;
};

void drawElement(RenderElement element) {
	SetShaderValueTexture(transmission, GetShaderLocation(transmission, "zFrame"), rndt.texture);
	DrawMesh(element.mesh, element.material, element.transform);
}

void pushElement(RenderElement element) {
	if (element.isTransparent) {
		RenderSortNode* node = obtainPooledRenderNode();
		node->element = element;
		float curdist = Vector3DistanceSqr(getTranslation(element.transform), mainCam.position);
		node->distance = curdist;
		if (!renderedRoot) {
			renderedRoot = node;
			return;
		}
		RenderSortNode* current = renderedRoot;
		RenderSortNode* previous;
		while (current) {
			previous = current;
			current = (curdist > current->distance) ? current->right : current->left;
		}
		if (curdist > previous->distance) {
			previous->right = node;
		}
		else {
			previous->left = node;
		}
		node->up = previous;
	} else drawElement(element);
};

void clearAll() {
	bintreeOperation lastOp = down;
	RenderSortNode* current = renderedRoot;
	renderedRoot = NULL;
	while (current) {
		switch (lastOp) {
		case down:
			if (current->right) {
				current = current->right;
			} else if (current->left) {
				current = current->left;
			} else {
				RenderSortNode* next = current->up;
				current->up = NULL;
				current->right = pooledRoot;
				pooledRoot = current;
				if (!next) {
					current = next;
					return;
				} else if (next->right == current) {
					lastOp = unright;
				} else {
					lastOp = unleft;
				}
				current = next;
			}
			break;
		case unright:
			if (current->left) {
				current = current->left;
			} else {
				RenderSortNode* next = current->up;
				current->up = NULL;
				current->right = pooledRoot;
				pooledRoot = current;
				if (!next) {
					current = next;
					return;
				} else if (next->right == current) {
					lastOp = unright;
				} else {
					lastOp = unleft;
				}
				current = next;
			}
			break;
		case unleft:
			RenderSortNode* next = current->up;
			current->up = NULL;
			current->right = pooledRoot;
			pooledRoot = current;
			if (!next) {
				current = next;
				return;
			} else if (next->right == current) {
				lastOp = unright;
			} else {
				lastOp = unleft;
			}
			current = next;
			break;
		}
	}
};

void drawAll() {
	bintreeOperation lastOp = down;
	RenderSortNode* current = renderedRoot;
	while (current) {
		switch (lastOp) {
		case down:
			if (current->right) {
				current = current->right;
			} else if (current->left) {
				drawElement(current->element);
				current = current->left;
			} else {
				drawElement(current->element);
				RenderSortNode* next = current->up;
				if (!next) {
					current = next;
					return;
				} else if (next->right == current) {
					lastOp = unright;
				} else {
					lastOp = unleft;
				}
				current = next;
			}
			break;
		case unright:
			drawElement(current->element);
			if (current->left) {
				current = current->left;
				lastOp = down;
			} else {
				RenderSortNode* next = current->up;
				if (!next) {
					current = next;
					return;
				} else if (next->right == current) {
					lastOp = unright;
				} else {
					lastOp = unleft;
				}
				current = next;
			}
			break;
		case unleft:
			RenderSortNode* next = current->up;
			if (!next) {
				current = next;
				return;
			} else if (next->right == current) {
				lastOp = unright;
			} else {
				lastOp = unleft;
			}
			current = next;
			break;
		}
	}
};

Material createTransmitMaterial() {
	Material material = LoadMaterialDefault();
	material.shader = transmission;
	material.maps[MATERIAL_MAP_VIEWPORT].texture = rndt.texture;
	return material;
}

void updateCamControl(Camera3D* cam, orientation facing, float pitch, Vector3* right, orientation* heading) {
	Vector3 side = Vector3CrossProduct(facing.up, facing.forth);
	orientation ori = rotateOrientationAxisAngle(facing, side, pitch);
	cam->target = Vector3Add(cam->position, ori.forth);
	cam->up = ori.up;

	*right = side;
	*heading = ori;
}

Material mat;
int viewportShaderLocation;

void drawInit() {
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	
	camOri.forth = Vector3Normalize((Vector3) { 0, -1, 1 });
	camOri.up = Vector3Normalize((Vector3) { 0, 1, 1 });
	mainCam = (Camera3D){ {0,0,0}, camOri.forth, camOri.up, 90.0, CAMERA_PERSPECTIVE };
	
	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	winWidth = GetScreenWidth();
	winHeight = GetScreenHeight();

	winXCenter = winWidth / 2;
	winYCenter = winHeight / 2;

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Load a texture from the resources directory
	wabbit = LoadTexture("wabbit_alpha.png");
	brickMesh = GenMeshCube(1.0, 1.0, 1.0);

	defaultMat = LoadMaterialDefault();

	transmission = LoadShader("transmission.vs", "transmission.fs");
	transmission.locs[SHADER_LOC_COLOR_SPECULAR] = GetShaderLocation(transmission, "colTransmit");
	transmission.locs[SHADER_LOC_VIEWPORT] = GetShaderLocation(transmission, "viewport");

	rndt = LoadRenderTexture(winWidth, winHeight);

	mat = createTransmitMaterial();
	mat.maps[MATERIAL_MAP_TRANSMIT].color = MAGENTA;
}

void drawMain() {

	// drawing
	BeginTextureMode(rndt);
	BeginMode3D(mainCam);

	// Setup the back buffer for drawing (clear color and depth buffers)
	ClearBackground(BLACK);

	//DrawCubeV((Vector3) { 2.0, 0, 4.5 }, (Vector3) { 1, 1, 1 }, RED);
	//DrawSphere((Vector3) { 0.0, 0, 4.5 }, 0.5, GREEN);

	//DrawBillboard(mainCam, wabbit, (Vector3) { 0.0, -2.0, 6.0 }, 1.0, WHITE);
	//rlDrawRenderBatchActive();

	float x = fmod(t * 2.0, 8.0) - 4.0;
	Matrix m = MatrixMultiply(MatrixScale(1.0, 8.0, 1.0), MatrixTranslate(x, 0.0, 4.0));

	//pushElement((RenderElement) { *(unitcube.meshes), mat, m });

	drawFrom(rootObjectNode);

	drawAll();
	clearAll();

	DrawLine3D((Vector3) { 0, -4, 4 }, Vector3Add(Vector3Scale(camOri.forth, 4.0), (Vector3) { 0, -4, 4 }), RED);
	DrawLine3D((Vector3) { 0, -4, 4 }, Vector3Add(camOri.up, (Vector3){ 0, -4, 4 }), GREEN);
	orientation exp = rotateOrientationQuaternion((orientation) { 0, 0, 1, 0, 1, 0 }, QuaternionFromOrientationToOrientation((orientation) { 0, 0, 1, 0, 1, 0 }, camOri));
	printf("\n%.2f\n", (Vector3Distance(Vector3RotateByQuaternion((Vector3) { 0, 0, 1 }, QuaternionFromVector3ToVector3((Vector3) { 0, 0, 1 }, camOri.forth)), camOri.forth)));
	DrawLine3D((Vector3) { 0.1, -4, 4 }, Vector3Add(Vector3Scale(exp.forth, 4.0), (Vector3) { 0.1, -4, 4 }), PURPLE);
	DrawLine3D((Vector3) { 0.1, -4, 4 }, Vector3Add(exp.up, (Vector3){ 0.1, -4, 4 }), WHITE);

	EndMode3D();
	EndTextureMode();
	DrawTextureEx(rndt.texture, (Vector2) { winWidth, winHeight }, 180.0, 1.0, WHITE);
	DrawLine(winXCenter - 10, winYCenter - 10, winXCenter + 10, winYCenter + 10, WHITE);
	DrawLine(winXCenter - 10, winYCenter + 10, winXCenter + 10, winYCenter - 10, WHITE);

}

void drawFin() {

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(wabbit);
	UnloadRenderTexture(rndt);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
}
