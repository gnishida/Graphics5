#pragma once

#include "PPC.h"
#include "Texture.h"

class ProjectiveTexture {
public:
	PPC* ppc;
	Texture* texture;

public:
	ProjectiveTexture(PPC* ppc, Texture* texture);
};

