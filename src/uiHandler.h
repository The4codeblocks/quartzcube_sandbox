
#ifndef uihandlerheader
#define uihandlerheader

#include "raylib.h"
#include "object.h"
#include "componentDefs.h"

#define ui_DLMB 1
#define ui_ULMB 2
#define ui_DRMB 4
#define ui_URMB 8
#define ui_DMMB 16
#define ui_UMMB 32
#define ui_DSMB 64
#define ui_USMB 128
#define ui_DEMB 256
#define ui_UEMB 512
#define ui_DFMB 1024
#define ui_UFMB 2048
#define ui_DBMB 4096
#define ui_UBMB 8192
#define ui_on   16384
#define ui_off  32768

Object* controlled;

void setControlled(Object* obj);

void pushUIobj(Mesh mesh, Matrix transform, UIobject*);

void UIInit();
void UIMain();

void send(UIinteraction);

#endif
