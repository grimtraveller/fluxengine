/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef _RESOURCETEXTURE2D_H_
#define _RESOURCETEXTURE2D_H_

#include "flx_resource_texture.h"

class ResourceTexture2D : public ResourceTexture
{
public:
	virtual GLenum getTextureType() const;
	bool load(const std::string& _res_name);
	bool load(GLubyte* ptr, unsigned int w, unsigned int h,
		unsigned int d);

	ResourceTexture2D();

};

#endif