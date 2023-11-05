#pragma once
#include "Globals.h"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	void Create_FrameBuffer(int width, int height);
	void Rescale_FrameBuffer(float width, float height);

	void Bind_FrameBuffer();
	void Unbind_FrameBuffer();

	GLuint GetTextureID() {
		return texture_id;
	}

private:
	GLuint FBO;
	GLuint RBO;
	GLuint texture_id;
};