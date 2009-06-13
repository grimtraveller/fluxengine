/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef RESOURCETEXTURE_H
#define RESOURCETEXTURE_H

#include <windows.h>

#include <GL/glew.h>
#include <string>
#include "../../Core/flx_core.h"
#include "../flx_logger.h"


class ENGINE_API ResourceTexture
{
public:
	ResourceTexture();
	virtual ~ResourceTexture();

	virtual GLenum getTextureType() const = 0;

	void			gen();
	virtual bool	load(const std::string& _res_name);
	void			destroy();

	void bind(GLuint _slot) const;
	void unbind(GLuint _slot) const;

	GLuint getHandle() const;


	void bind() const;
	void unbind() const;

	bool loadFromFile(GLenum _target, const std::string& _res_name);
	void loadFromData(GLenum _target, GLubyte* ptr, unsigned int w, unsigned int h, unsigned int d);


//protected:
	GLuint m_nHandle;
	static bool s_bGenerateMipMaps;
};

#endif