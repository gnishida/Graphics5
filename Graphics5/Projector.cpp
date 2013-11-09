#include "Projector.h"
#include "PPC.h"
#include "FrameBuffer.h"
#include "TMesh.h"

/**
 * Construct a project.
 *
 * @param ppc			the camera to capture the scene
 * @param fb0			the frame buffer for an object which will be projected
 * @param fb1			the frame buffer for a background to check the Z buffer
 */
Projector::Projector(PPC* ppc, FrameBuffer* fb0, FrameBuffer* fb1) {
	this->ppc = ppc;
	this->fb0 = fb0;
	this->fb1 = fb1;
}

/**
 * Capture the scene so that the frame buffer is ready for projection.
 */
void Projector::Capture() {
	/*
	fb0->Set(WHITE);
	fb0->SetZB(0.0f);
	for (int i = 0; i < foremeshes.size(); i++) {
		foremeshes[i]->Render(fb0, ppc);
	}

	fb1->Set(WHITE);
	fb1->SetZB(0.0f);
	for (int i = 0; i < backmeshes.size(); i++) {
		backmeshes[i]->Render(fb1, ppc);
	}
	*/
}
