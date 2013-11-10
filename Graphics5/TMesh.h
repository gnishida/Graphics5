#pragma once

#include "V3.h"
#include "PPC.h"
#include "Texture.h"
#include "ProjectiveTexture.h"

class FrameBuffer;

typedef struct {
	V3 v;
	V3 pv;
	V3 c;
	V3 n;
	float t[2];
} Vertex;

class TMesh {
public:
	/** the numver of vertices */
	int vertsN;

	/** the list of vertices */
	Vertex* verts;

	/** the number of triangles */
	int trisN;

	/** the list of triangles */
	unsigned int* tris;

	Texture* texture;
	ProjectiveTexture* projTexture;

public:
	TMesh();
	~TMesh();

	void SetRaytracing();
	void Load(char *filename);
	void ComputeAABB(AABB &aabb);
	void Translate(const V3 &v);
	void Scale(float t);
	void Scale(const V3 &centroid, const V3 &size);

	void Clear();
	void RotateAbout(const V3 &axis, float angle);
	void RotateAbout(const V3 &axis, float angle, const V3 &orig);
	V3 GetCentroid();

	bool isInside2D(const V3 &p0, const V3 &p1, const V3 &p2, const V3 p) const;
	void SetTexture(const char* filename);
	void SetProjectiveTexture(ProjectiveTexture* projTexture);

	void Render(FrameBuffer *fb, PPC *ppc);
	void RenderHW();
};

