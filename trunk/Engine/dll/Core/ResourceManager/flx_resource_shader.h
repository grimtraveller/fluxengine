/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef _RESOURCESHADER_H
#define _RESOURCESHADER_H

#include <iostream>
#include "../../Core/flx_core.h"
#include "../../MathLib/flx_vector3.h"
#include "flx_resource_texture.h"

class ENGINE_API ResourceShader
{
public:
	ResourceShader();

	static bool init();
	bool load(const std::string& _res_name, const std::string& _extensions = "");
	bool Activate();
	static void Deactivate();

	static ResourceShader* getActivatedShader();
	static bool isExtensionSupported(const std::string& _ext);
	
	void Uniform(const std::string& ext, GLint value);
	void Uniform(const std::string& ext, GLfloat value);
	void Uniform(const std::string& ext, GLfloat* value);
	void Uniform(const std::string& ext, float f1, float f2);

	void UniformTexture(const std::string& ext, GLint slot);

private:
	static GLhandleARB loadShader(const std::string& _filename);
	static bool compileShader(GLhandleARB _object);
	static GLhandleARB linkShaders(GLhandleARB* object, const unsigned int& nb);

private:
	static bool				s_bInitialized;
	static ResourceShader*	s_pCurrent;
	std::string				m_strName;
	GLhandleARB				m_nProgram;
	
};

#endif