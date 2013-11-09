#pragma once

#include <vector>

class PPC;
class FrameBuffer;
class TMesh;

class Projector {
public:
	PPC* ppc;

	/** The frame buffer for an object which will be projected */
	FrameBuffer* fb0;

	/** The frame buffer for a background to check the Z buffer */
	FrameBuffer* fb1;

public:
	Projector(PPC* ppc, FrameBuffer* fb0, FrameBuffer* fb1);
	void Capture();
};

