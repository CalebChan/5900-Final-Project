#include "VisibleTexture.h"

#include <assert.h>
#include "Constant.h"
#include <stdio.h>

VisibleTexture::VisibleTexture(){
	createTexture();
}

void VisibleTexture::bindTexture(){
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);
}

void VisibleTexture::unbindTexture(){
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void VisibleTexture::createTexture(){
	buffer = 0;
	glGenFramebuffers(1, &buffer);
	glBindFramebuffer(GL_FRAMEBUFFER, buffer);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

	GLenum frameStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (frameStatus != GL_FRAMEBUFFER_COMPLETE){
		printf("GL_FRAMEBUFFER_COMPLETE for visible texture failed, CANNOT use FBO\n");
		assert(false);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}