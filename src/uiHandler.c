
#include "uiHandler.h"
#include "object.h"
#include "renderHandler.h"
#include "rlgl.h"

RenderTexture2D UIRSpace0, UIRSpace1; // stores pointers to UIobjects as --rgbRGB
RenderTexture2D UIRSpace2; // stores fine UI interaction info as R: UV x, G: UV y, B: depth
Image UIESpace0, UIESpace1;
Image UIESpace2;
Material M0, M1;
Material M2;

void setControlled(Object* obj) {
	controlled = toRef(obj);
	if (obj) {
		camOri = projectOrientation(camOri, obj->orientation.up);
		camPos = obj->pos;
	}
}

void pushUIobj(Mesh mesh, Matrix transform, UIobject* obj) {
	size_t ID = (size_t)obj;
	Color ID0 = { ID & 0xff, ID >> 8 & 0xff, ID >> 16 & 0xff, 0xff };
	Color ID1 = { ID >> 24 & 0xff, ID >> 32 & 0xff, ID >> 40 & 0xff, 0xff };
	M0.maps->color = ID0;
	M1.maps->color = ID1;
	Camera3D focus = mainCam;
	focus.fovy = 0.001;

	BeginTextureMode(UIRSpace0);
	BeginMode3D(focus);
	DrawMesh(mesh, M0, transform);
	EndMode3D();
	EndTextureMode();

	BeginTextureMode(UIRSpace1);
	BeginMode3D(focus);
	DrawMesh(mesh, M1, transform);
	EndMode3D();
	EndTextureMode();

	BeginTextureMode(UIRSpace2);
	BeginMode3D(focus);
	DrawMesh(mesh, M2, transform);
	EndMode3D();
	EndTextureMode();
}

int offset;

void UIInit() {
	offset = winXCenter + winWidth * winYCenter;
	UIRSpace0 = LoadRenderTexture(1, 1);
	UIRSpace1 = LoadRenderTexture(1, 1);
	UIRSpace2 = LoadRenderTexture(256, 256);
	M0 = LoadMaterialDefault();
	M1 = LoadMaterialDefault();
	M2 = LoadMaterialDefault();
	M2.shader = LoadShader("uishader.vs", "uishader.fs");
}

void UIMain() {
	UnloadImage(UIESpace0);
	BeginTextureMode(UIRSpace0);
	ClearBackground(BLACK);
	EndTextureMode();

	UnloadImage(UIESpace1);
	BeginTextureMode(UIRSpace1);
	ClearBackground(BLACK);
	EndTextureMode();

	UnloadImage(UIESpace2);
	BeginTextureMode(UIRSpace2);
	ClearBackground(BLACK);
	EndTextureMode();

	UIFrom(mainEnv.root);

	UIESpace0 = LoadImageFromTexture(UIRSpace0.texture);
	UIESpace1 = LoadImageFromTexture(UIRSpace1.texture);
	UIESpace2 = LoadImageFromTexture(UIRSpace2.texture);
}

void UIFin() {
	UnloadMaterial(M0);
	UnloadMaterial(M1);
	UnloadMaterial(M2);
	UnloadRenderTexture(UIRSpace0);
	UnloadRenderTexture(UIRSpace1);
	UnloadRenderTexture(UIRSpace2);
}

UIobject* getPointed() {
	Color ID0 = GetPixelColor((int*)UIESpace0.data, UIESpace0.format);
	Color ID1 = GetPixelColor((int*)UIESpace1.data, UIESpace1.format);
	return (UIobject*)((size_t)ID0.r | (size_t)ID0.g << 8 | (size_t)ID0.b << 16 | (size_t)ID1.r << 24 | (size_t)ID1.g << 32 | (size_t)ID1.b << 40);
}

void send(UIinteraction event) {
	UIobject* ID = getPointed();
	printf("%p\n", ID);
	if (ID) sendSignal(ID->obj, click, click_(event, ID->type));
}
