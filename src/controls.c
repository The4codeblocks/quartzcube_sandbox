
#include "controls.h"

Vector3 getMovement() {
	Vector3 movementP = IsKeyDown(KEY_SPACE) ? camOri.up : (Vector3) { 0 };
	Vector3 movementN = (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) ? camOri.up : (Vector3) { 0 };
	if (IsKeyDown(KEY_W)) movementP = Vector3Add(movementP, camFace.forth);
	if (IsKeyDown(KEY_S)) movementN = Vector3Add(movementN, camFace.forth);
	if (IsKeyDown(KEY_D)) movementP = Vector3Add(movementP, camR);
	if (IsKeyDown(KEY_A)) movementN = Vector3Add(movementN, camR);
	if (IsKeyDown(KEY_E)) movementP = Vector3Add(movementP, camFace.up);
	if (IsKeyDown(KEY_Q)) movementN = Vector3Add(movementN, camFace.up);
	return Vector3Subtract(movementP, movementN);
}
