/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "IL/il.h"
#include "flx_resource_texture.h"
#include "flx_resource_manager.h"
#include "../flx_renderer.h"
#include "../flx_application.h"
#pragma comment (lib, "DevIL.lib")



bool ResourceTexture::s_bGenerateMipMaps = true;

ResourceTexture::ResourceTexture()
{
	m_nHandle = NULL;
}

ResourceTexture::~ResourceTexture()
{
	destroy();
}

bool ResourceTexture::loadFromFile(GLenum _target, const std::string& _res_name)
{
	unsigned int w, h, d;
	w = h = d = 0;

	static bool first = true;

	if(first)
	{
		first = false;

		ilInit();
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
		ilEnable(IL_ORIGIN_SET);
		ilEnable(IL_TYPE_SET);
		ilTypeFunc(IL_UNSIGNED_BYTE);
	}

	ILuint ilTexture;
	ilGenImages(1, &ilTexture);
	ilBindImage(ilTexture);

	if(!ilLoadImage(_res_name.c_str()))
	{
		FLX_LOGGER->Write(LOG_DEFAULT, "[FAILED]\n");
		return false;
	}

	w = ilGetInteger(IL_IMAGE_WIDTH);
	h = ilGetInteger(IL_IMAGE_HEIGHT);
	d = ilGetInteger(IL_IMAGE_BPP);

	if(d == 4)
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
	

	const unsigned char* pixels = ilGetData();

	GLubyte* img = new GLubyte[(size_t)(w) * (size_t)(h) * (size_t)(d)];
	memcpy(img, pixels, (size_t)(w) * (size_t)(h) * (size_t)(d));


	

	ilBindImage(0);
	ilDeleteImages(1, &ilTexture);

	loadFromData(_target, img, w, h, d);
	
	FLX_LOGGER->Write(LOG_DEFAULT, "[DONE]\n");

	return true;
}

void ResourceTexture::loadFromData(GLenum _target, GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d)
{
	glTexImage2D(_target, 0, d == 3 ? GL_RGB:GL_RGBA, w, h, 0, d == 3 ? GL_RGB:GL_RGBA, GL_UNSIGNED_BYTE, ptr);
}

void ResourceTexture::gen()
{
	destroy();
	glGenTextures(1, &m_nHandle);
	//ilGenImages(1, &m_nHandle);
	GLenum errorNo = glGetError(); 
	//const unsigned char* errString;
	//errString = gluErrorString(errorNo);
	//FLX_LOGGER->Write(LOG_DEFAULT, "%s\n", errString);
}

bool ResourceTexture::load(const std::string& _res_name)
{
	gen();

	if(m_nHandle == 0)
	{
		FLX_LOGGER->Write(LOG_ERROR, "No texture handle!\n");
		//return false;
	}

	return true;
}

void ResourceTexture::destroy()
{
	if(m_nHandle != NULL)
		//ilDeleteImages(1, &m_nHandle);
		glDeleteTextures(1, &m_nHandle);
}

void ResourceTexture::bind() const
{
	glBindTexture(getTextureType(), m_nHandle);
}

void ResourceTexture::bind(GLuint _slot) const
{
	//hmm maybe needless 
	glActiveTexture(GL_TEXTURE0+_slot); 

	glEnable(GL_TEXTURE_2D);
	if(m_nHandle != NULL)
		glBindTexture(getTextureType(), m_nHandle);
}

void ResourceTexture::unbind() const
{
	glBindTexture(getTextureType(), 0);
}

void ResourceTexture::unbind(GLuint _slot) const
{
	glActiveTexture(GL_TEXTURE0+_slot);
	glClientActiveTexture(GL_TEXTURE0_ARB+_slot);
	glBindTexture(getTextureType(), 0);
}