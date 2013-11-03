#include "V4.h"

V4::V4() {
	xyzw[0] = 0.0f;
	xyzw[1] = 0.0f;
	xyzw[2] = 0.0f;
	xyzw[3] = 0.0f;
}

V4::V4(float x, float y, float z, float w) {
	xyzw[0] = x;
	xyzw[1] = y;
	xyzw[2] = z;
	xyzw[3] = w;
}

float& V4::operator[](int i) {
	return xyzw[i];
}

float V4::operator*(const V4& v1) const {
	float ret = xyzw[0]*v1.x() + xyzw[1]*v1.y() + xyzw[2]*v1.z() + xyzw[3]*v1.w();
	return ret;
}

float V4::x() const {
	return xyzw[0];
}

float V4::y() const {
	return xyzw[1];
}

float V4::z() const {
	return xyzw[2];
}

float V4::w() const {
	return xyzw[3];
}