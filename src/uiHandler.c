
#include "uiHandler.h"
#include "object.h"
#include "renderHandler.h"
#include "rlgl.h"

RenderTexture2D UIRSpace0;
RenderTexture2D UIRSpace1;
Image UIESpace0;
Image UIESpace1;
Image depthbuf;
Material M0;
Material M1;

void setControlled(Object* obj) {
	controlled = obj;
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
	focus.fovy = 0.0000000000001;
	BeginTextureMode(UIRSpace0);
	BeginMode3D(mainCam);
	DrawMesh(mesh, M0, transform);
	EndMode3D();
	EndTextureMode();
	BeginTextureMode(UIRSpace1);
	BeginMode3D(mainCam);
	DrawMesh(mesh, M1, transform);
	EndMode3D();
	EndTextureMode();
}

int offset;

void UIInit() {
	offset = winXCenter + winWidth * winYCenter;
	UIRSpace0 = LoadRenderTexture(1, 1);
	UIRSpace1 = LoadRenderTexture(1, 1);
	M0 = LoadMaterialDefault();
	M1 = LoadMaterialDefault();
}

void UIMain() {
	UnloadImage(UIESpace0);
	UnloadImage(UIESpace1);
	BeginTextureMode(UIRSpace0);
	ClearBackground(BLACK);
	EndTextureMode();
	BeginTextureMode(UIRSpace1);
	ClearBackground(BLACK);
	EndTextureMode();
	UIFrom(rootObjectNode);
	UIESpace0 = LoadImageFromTexture(UIRSpace0.texture);
	UIESpace1 = LoadImageFromTexture(UIRSpace1.texture);

}

UIobject* getPointed() {
	Color ID0 = GetPixelColor((int*)UIESpace0.data, UIESpace0.format);
	Color ID1 = GetPixelColor((int*)UIESpace1.data, UIESpace1.format);
	float depth = GetPixelColor((int*)depthbuf.data, depthbuf.format).r;
	return (UIobject*)((size_t)ID0.r | (size_t)ID0.g << 8 | (size_t)ID0.b << 16 | (size_t)ID1.r << 24 | (size_t)ID1.g << 32 | (size_t)ID1.b << 40);
}

void send(UIinteraction event) {
	UIobject* ID = getPointed();
	if (ID) {
		UIobject obj = *(UIobject*)ID;
		Component* comp = obj.obj->components;
		while (comp) {
			ComponentDef* def = comp->def;
			if (def->recieve) def->recieve(comp, click, click_(event, ID->type));
			comp = comp->next;
		}
	}
}
