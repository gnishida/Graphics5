#include "M44.h"

M44::M44() {
	rows[0] = V4(1.0f, 0.0f, 0.0f, 0.0f);
	rows[1] = V4(0.0f, 1.0f, 0.0f, 0.0f);
	rows[2] = V4(0.0f, 0.0f, 1.0f, 0.0f);
	rows[3] = V4(0.0f, 0.0f, 0.0f, 1.0f);
}

M44::M44(const V4& v1, const V4& v2, const V4& v3, const V4& v4) {
	rows[0] = v1;
	rows[1] = v2;
	rows[2] = v3;
	rows[3] = v4;
}

V4& M44::operator[](int i) {
	return rows[i];
}

V4 M44::GetColumn(int i) const {
	V4 ret(rows[0].xyzw[i], rows[1].xyzw[i], rows[2].xyzw[i], rows[3].xyzw[i]);
	return ret;
}

void M44::SetColumn(int i, const V4& col) {
	rows[0][i] = col.x();
	rows[1][i] = col.y();
	rows[2][i] = col.z();
	rows[3][i] = col.w();
}

V4 M44::operator*(const V4& v) const {
	return V4(rows[0]*v, rows[1]*v, rows[2]*v, rows[3]*v);
}

M44 M44::operator*(const M44& m1) const {
	const M44 &m0 = *this;
	M44 ret;
	ret.SetColumn(0, m0*m1.GetColumn(0));
	ret.SetColumn(1, m0*m1.GetColumn(1));
	ret.SetColumn(2, m0*m1.GetColumn(2));
	ret.SetColumn(3, m0*m1.GetColumn(3));
	return ret;
}