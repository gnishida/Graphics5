#pragma once

class V4 {
friend class M44;

private:
	float xyzw[4];

public:
	V4();
	V4(float x, float y, float z, float w);
	float& operator[](int i);
	float operator*(const V4& v1) const;

	float x() const;
	float y() const;
	float z() const;
	float w() const;
};

