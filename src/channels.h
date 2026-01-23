
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
	interact, // player interaction input, takes input:bb in respect to UIinteraction
	click, // exactly like interact but for UI, takes input:bb, type:b
	paginate, // usually a player interaction input, intended for sequences, normally bound to SCROLL, takes direction:b[enum directionOrtho]
	scroll, // exactly like paginate but for UI, takes direction:b[enum directionOrtho], type:b
	wire, // mechanical input, takes type:b, data:(type?<number>:bbbbbbbb,<position>:(x:bbbbbbbb y:bbbbbbbb z:bbbbbbbb),<string>:b...), if invalid interprets as impulse
} dataChannel;

typedef unsigned short UIinteraction;

typedef struct {
	UIinteraction action;
	char id;
} ClickAction;

typedef struct {
	char action;
	char id;
} ScrollAction;

typedef enum {
	solid,
} UIID;

pos3 moveTo_temp;
orientation orient_temp;
UIinteraction interact_temp;
directionOrtho paginate_temp;
ClickAction click_temp;
ScrollAction scroll_temp;

inline cstr moveTo_(pos3 pos) {
	moveTo_temp = pos;
	return (cstr) { .data = &moveTo_temp, 24 };
}

inline cstr orient_(orientation ori) {
	orient_temp = ori;
	return (cstr) { .data = &orient_temp, 24 };
}

inline cstr interact_(UIinteraction action) {
	interact_temp = action;
	return (cstr) { .data = &interact_temp, 2 };
}

inline cstr click_(UIinteraction action, UIID ID) {
	click_temp = (ClickAction){action, ID};
	return (cstr) { .data = &click_temp, 3 };
}

inline cstr paginate_(directionOrtho dir) {
	paginate_temp = dir;
	return (cstr) { .data = &paginate_temp, 1 };
}

inline cstr scroll_(directionOrtho dir, UIID ID) {
	scroll_temp = (ScrollAction){ dir, ID };
	return (cstr) { .data = &scroll_temp, 2 };
}

#endif
