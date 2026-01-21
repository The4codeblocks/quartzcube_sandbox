
#ifndef channelsheader
#define channelsheader

#include "suppmath.h"

/*----/
simple channel system:
a hook is present
takes (dataChannel, cstr) as channel type and data
does whatever, DOES NOT STORE DATA INPUT AS-IS
/----*/

// b = byte, bytes expressed in string order
typedef enum {
	arbitrary = 0, // expect whatever
	moveTo, // move to a position relative to origin, takes x:bbbbbbbb, y:bbbbbbbb, z:bbbbbbbb
	orient, // orient relative to space, takes fx:bbbb, fy:bbbb, fz:bbbb, ux:bbbb, uy:bbbb, uz:bbbb
	interact0, // player interaction input, normally bound to LMB
	interact1, // player interaction input, normally bound to RMB
} dataChannel;

pos3 moveTo_temp;
orientation orient_temp;

inline cstr moveTo_(pos3 pos) {
	moveTo_temp = pos;
	return (cstr) { .data = &moveTo_temp, 24 };
}

inline cstr orient_(orientation ori) {
	orient_temp = ori;
	return (cstr) { .data = &orient_temp, 24 };
}

#endif
