
#include "raylib.h"

#include "gametime.h"

void updateTime() {
	dt = GetFrameTime();
	t += dt;
}
