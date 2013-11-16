#include "CubeMap.h"
#include <libtiff/tiffio.h>

CubeMap::CubeMap(const char* filename) {
	TIFF* tiff = TIFFOpen(filename, "r");
	if (tiff == NULL) throw "File is not accessible.";

	int w, h;

	TIFFGetField(tiff, TIFFTAG_IMAGEWIDTH, &w);
	TIFFGetField(tiff, TIFFTAG_IMAGELENGTH, &h);

	unsigned int* image = (unsigned int*)_TIFFmalloc(sizeof(unsigned int) * w * h);
	if (!TIFFReadRGBAImage(tiff, w, h, image, 0)) {
		delete [] image;
		image = NULL;
		TIFFClose(tiff);
		return;
	}

	TIFFClose(tiff);

	int width, height;
	if (w > h) {
		width = w / 4;
		height = h / 3;
	} else {
		width = w / 3;
		height / h / 4;
	}

	for (int i = 0; i < 6; i++) {
		textures.push_back(new Texture(width, height));
	}

	// create 6 textures
	for (int v = 0; v < height; v++) {
		for (int u = 0; u < width; u++) {
			textures[0]->image[v * width + u] = image[(v + h - height * 2) * w + width * 2 + u];
		}
	}



	binded = false;
}

CubeMap::~CubeMap() {
	for (int i = 0; i < textures.size(); i++) {
		delete textures[i];
	}
}

GLuint CubeMap::Bind() {
	if (binded) return texture_id;

	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);    

	GLenum  cube[6] = {GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                     GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                     GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                     GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                     GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                     GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

	for (int i = 0; i < 6; i++) {        
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage2D(cube[i], 0, GL_RGBA, textures[i]->width, textures[i]->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textures[i]->image);
	}

	binded = true;

	return texture_id;
}