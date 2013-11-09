#include "ShadowMap.h"
#include "FrameBuffer.h"

ShadowMap::ShadowMap(PPC* ppc, int w, int h) {
	this->ppc = ppc;
	fb = new FrameBuffer(w, h);
	depthBuffer = NULL;
}

Texture* ShadowMap::getTexture() {
	if (depthBuffer != NULL) {
		delete depthBuffer;
	}

	depthBuffer = new Texture(fb);
	return depthBuffer;
}
