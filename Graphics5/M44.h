#pragma once

#include "V4.h"

class M44 {
public:
	V4 rows[4];

public:
	M44();
	M44(const V4& v1, const V4& v2, const V4& v3, const V4& v4);

	V4& operator[](int i);
	V4 GetColumn(int i) const;
	void SetColumn(int i, const V4& col);
	V4 operator*(const V4& v) const;
	M44 operator*(const M44& m) const;
};

