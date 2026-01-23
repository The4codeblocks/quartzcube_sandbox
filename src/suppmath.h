
#ifndef suppmathheader
#define suppmathheader

#include <stdlib.h>
#include <string.h>
#include "raymath.h"

typedef struct { // vector orientation
	Vector3 forth, up;
} orientation;

typedef struct { // doubleprecision vec3
	double x, y, z;
} vec3;

typedef struct { // doubleprecision vec3, but for positions
	double x, y, z;
} pos3;


inline pos3 vec3addPV(const pos3 p, const vec3 v) {
	return (pos3) {
		p.x + v.x,
		p.y + v.y,
		p.z + v.z,
	};
};

inline vec3 vec3addVV(const vec3 u, const vec3 v) {
	return (vec3) {
		u.x + v.x,
		u.y + v.y,
		u.z + v.z,
	};
};

inline pos3 vec3subPV(const pos3 p, const vec3 v) {
	return (pos3) {
		p.x - v.x,
		p.y - v.y,
		p.z - v.z,
	};
};

inline vec3 vec3subPP(const pos3 p, const pos3 v) {
	return (vec3) {
		p.x - v.x,
		p.y - v.y,
		p.z - v.z,
	};
};

inline vec3 vec3subVV(const vec3 u, const vec3 v) {
	return (vec3) {
		u.x - v.x,
		u.y - v.y,
		u.z - v.z,
	};
};

inline pos3 vec3addPv(const pos3 p, const Vector3 v) {
	return (pos3) {
		p.x + v.x,
		p.y + v.y,
		p.z + v.z,
	};
};

inline vec3 vec3addVv(const vec3 u, const Vector3 v) {
	return (vec3) {
		u.x + v.x,
		u.y + v.y,
		u.z + v.z,
	};
};

inline pos3 vec3subPv(const pos3 p, const Vector3 v) {
	return (pos3) {
		p.x - v.x,
		p.y - v.y,
		p.z - v.z,
	};
};

inline vec3 vec3subVv(const vec3 u, const Vector3 v) {
	return (vec3) {
		u.x - v.x,
		u.y - v.y,
		u.z - v.z,
	};
};

inline pos3 pos3fromv(const Vector3 v) { // avoid
	return (pos3) {
		v.x,
		v.y,
		v.z,
	};
};

inline vec3 vec3fromv(const Vector3 v) {
	return (vec3) {
		v.x,
		v.y,
		v.z,
	};
};

inline Vector3 vec3tov(const vec3 v) {
	return (Vector3) {
		(float)v.x,
		(float)v.y,
		(float)v.z,
	};
};

inline vec3 vec3scale(const vec3 v, const double n) {
	return (vec3) {
		v.x * n,
		v.y * n,
		v.z * n,
	};
};

inline pos3 pos3avg(const pos3 u, const pos3 v) {
	return (pos3) {
		(u.x + v.x) * 0.5,
		(u.y + v.y) * 0.5,
		(u.z + v.z) * 0.5,
	};
};

inline Vector3 getTranslation(Matrix m) {
	return (Vector3) { m.m12, m.m13, m.m14 };
};

inline orientation rotateOrientationQuaternion(orientation ori, Quaternion r) {
	return (orientation) {
		Vector3RotateByQuaternion(ori.forth, r),
		Vector3RotateByQuaternion(ori.up,    r),
	};
}

inline orientation rotateOrientationAxisAngle(orientation ori, Vector3 axis, float angle) {
	return (orientation) {
		Vector3RotateByAxisAngle(ori.forth, axis, angle),
		Vector3RotateByAxisAngle(ori.up,    axis, angle),
	};
}

inline Vector3 getRight(orientation ori) {
	return Vector3CrossProduct(ori.up, ori.forth);
}

inline Quaternion otherQuaternionFromVector3ToVector3(Vector3 from, Vector3 to) {
	Quaternion result = { 0 };

	float cos2Theta = (from.x * to.x + from.y * to.y + from.z * to.z);    // Vector3DotProduct(from, to)
	Vector3 cross = { from.y * to.z - from.z * to.y, from.z * to.x - from.x * to.z, from.x * to.y - from.y * to.x }; // Vector3CrossProduct(from, to)

	result.x = cross.x;
	result.y = cross.y;
	result.z = cross.z;
	result.w = sqrtf(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z + cos2Theta * cos2Theta) + cos2Theta;

	// QuaternionNormalize(q);
	// NOTE: Normalize to essentially nlerp the original and identity to 0.5
	Quaternion q = result;
	float length = sqrtf(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
	if (length == 0.0f) length = 1.0f;
	float ilength = 1.0f / length;

	result.x = q.x * ilength;
	result.y = q.y * ilength;
	result.z = q.z * ilength;
	result.w = q.w * ilength;

	return result;
}

inline Quaternion QuaternionFromOrientationToOrientation(orientation from, orientation to) {
	Quaternion frontTransform = otherQuaternionFromVector3ToVector3(from.forth, to.forth);
	Quaternion upError = otherQuaternionFromVector3ToVector3(Vector3RotateByQuaternion(from.up, frontTransform), to.up);
	Quaternion out = QuaternionMultiply(upError, frontTransform);
	return out;
}

inline orientation projectOrientation(orientation from, Vector3 on) {
	Vector3 projectedForth = Vector3Reject(from.forth, on);
	if (Vector3LengthSqr(projectedForth) < 0.25 * 0.25) projectedForth = Vector3Reject(Vector3Negate(from.up), on);
	return (orientation) {
		projectedForth,
		on,
	};
}

#define vec3addVP(a, b) vec3addPV(b, a)
#define vec3addvP(a, b) vec3addPv(b, a)

// [c]ounted [str]ing

typedef struct { // string with specified length instead of null termination
	char* data;
	size_t length;
} cstr;

inline cstr copyNTstring(char* NTstring) { // Copies a [n]ull-[t]erminated string into a newly allocated (strlen(NTstring) bytes) counted string
	size_t length = strlen(NTstring);
	if (length == 0) return (cstr) { 0 };

	char* data = malloc(length);
	for (size_t i = 0; i < length; i++) data[i] = NTstring[i];

	return (cstr) { data, length };
}

typedef enum {
	dirNone = 0,
	dirU, dirD,
	dirR, dirL,
	dirF, dirB,
} directionOrtho;

#endif
