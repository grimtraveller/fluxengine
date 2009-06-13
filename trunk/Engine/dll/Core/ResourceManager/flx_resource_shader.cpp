/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../flx_logger.h"
#include "flx_resource_shader.h"

const int MAX_LOG_STRING = 1024;
char logstring[MAX_LOG_STRING];

bool ResourceShader::s_bInitialized = false;
ResourceShader* ResourceShader::s_pCurrent     = NULL;

ResourceShader::ResourceShader()
{
	m_nProgram = 0; 
	m_strName  = "undefined";
}

ResourceShader* ResourceShader::getActivatedShader()	
{
	return s_pCurrent;
}

GLhandleARB ResourceShader::loadShader(const std::string &_filename)
{
	bool VertexShader	= false;
	bool FragmentShader = false;
	bool GeometryShader = false;

	if(_filename.find(".vert") != std::string::npos)
	{
		VertexShader = true;
	}
	else
	{
		if(_filename.find(".frag") != std::string::npos)
			FragmentShader = true;
	}

	if(!VertexShader && !FragmentShader)
		return 0;

	std::ifstream file(_filename.c_str());
	if(!file)
		return 0;

	std::stringstream buffer;

	buffer << file.rdbuf();
	file.close();

	std::string s = buffer.str();

	GLcharARB* source = new GLcharARB[4*(buffer.str().size()/4+1)];

	if (source == 0)
		return 0;

	unsigned int i;
	for(i=0; i < buffer.str().size(); ++i)
	{
		source[i] = s[i];
	}
	source[i] = '\0';

	GLhandleARB so = 0;
	if(VertexShader)
		so = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
	if(FragmentShader)
		so = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

	GLenum errorNo = glGetError(); 
	const unsigned char* errString;
	errString = gluErrorString(errorNo);
	FLX_LOGGER->Write(LOG_DEFAULT, "%s\n", errString);

	glShaderSourceARB(so, 1, (const GLcharARB**)&source, 0);


	return so;
}
	
bool ResourceShader::compileShader(GLhandleARB _object)
{
	if(_object == 0)
		return false;

	glCompileShaderARB(_object);

	int status;

	glGetObjectParameterivARB(_object, GL_OBJECT_COMPILE_STATUS_ARB, &status);
	if(status == 0)
	{
		int length = 0;
		glGetObjectParameterivARB(_object, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);

		if(length > 0)
		{
			GLsizei minlength = min(MAX_LOG_STRING, length);
			glGetInfoLogARB(_object, minlength, 0, logstring);
			FLX_LOGGER->Write(LOG_DEFAULT, logstring);
		}
		return false;
	}
	FLX_LOGGER->Write(LOG_DEFAULT, "Successfully compiled shaders\n");
	return true;
}

GLhandleARB ResourceShader::linkShaders(GLhandleARB *object, const unsigned int &nb)
{
	if(object == 0)
		return 0;

	GLhandleARB po = 0;
	po = glCreateProgramObjectARB();

	for(unsigned int i = 0; i < nb; ++i)
	{
		if(object[i] > 0)
			glAttachObjectARB(po, object[i]);
	}

	glLinkProgramARB(po);

	int status;
	glGetObjectParameterivARB(po, GL_OBJECT_LINK_STATUS_ARB, &status);
	if(status==0)
	{
		int length = 0;
		glGetObjectParameterivARB(po, GL_OBJECT_INFO_LOG_LENGTH_ARB, &length);
		if(length>0){
			GLsizei minlength = min(MAX_LOG_STRING,length);
			glGetInfoLogARB(po, minlength, 0, logstring);
			FLX_LOGGER->Write(LOG_DEFAULT, logstring);
		}
		return 0;
	}


	FLX_LOGGER->Write(LOG_DEFAULT, "Successfully linked shaders\n");
	return po;
}

bool ResourceShader::isExtensionSupported(const std::string &_ext)
{
	/* FIX ME
	if(!_ext.empty())
	{
		std::string extensions = (char*)glGetString(GL_EXTENSIONS);

	if(extensions.find(_ext) != std::string::npos)
		return true;
	}

	FLX_LOGGER->Write(LOG_DEFAULT, "Extension %s is not supported!\n", _ext.c_str());

	return false;
	*/

	return true;
}

bool ResourceShader::init()
{
	s_bInitialized	= true;
	s_pCurrent		= NULL;

	if(!isExtensionSupported("GL_ARB_shading_language_100"))
		s_bInitialized = false;

	return s_bInitialized;
}

bool ResourceShader::load(const std::string &_res_name, const std::string &_extensions)
{
	if(!s_bInitialized)
		if(!init())
			return false;

	if(!_extensions.empty())
	{
		std::stringstream ss(_extensions);
		std::string it;

		while(std::getline(ss, it, ' '))
		{
			if(!it.empty())
				isExtensionSupported(it);
		}
	}

	m_strName = _res_name;

	GLhandleARB so[3];
	memset(so, 0, sizeof(GLhandleARB)*3);

	std::string s1 = _res_name +  ".vert";


	so[0] = loadShader(s1);

	if(so[0] == 0)
	{
		FLX_LOGGER->Write(LOG_DEFAULT, "Could not load shader %s\n", s1.c_str());
		return false;
	}

	if(!compileShader(so[0]))
	{
		FLX_LOGGER->Write(LOG_DEFAULT, "Could not compile shader %s\n", s1.c_str());
		return false;
	}

	std::string s2 = _res_name +  ".frag";
	so[1] = loadShader(s2);

	if(so[1] == 0)
	{
		FLX_LOGGER->Write(LOG_DEFAULT, "Could not load shader %s\n", s2.c_str());
		return false;
	}
	if(!compileShader(so[1]))
	{
		FLX_LOGGER->Write(LOG_DEFAULT, "Could not compile shader %s\n", s2.c_str());
		return false;
	}


	m_nProgram = linkShaders(so, 2);
	glDeleteObjectARB(so[0]);
	glDeleteObjectARB(so[1]);

	FLX_LOGGER->Write(LOG_DEFAULT, "[DONE]\n");
	return true;
}

bool ResourceShader::Activate()
{
	if(!m_nProgram)
	{
		//FLX_LOGGER->Write(LOG_DEFAULT, "Could not activate shader %s\n", m_strName.c_str());
		return false;
	}

	s_pCurrent = this;
	glUseProgramObjectARB(m_nProgram);

	return true;
}

void ResourceShader::Deactivate()
{
	//s_pCurrent = NULL;
	glUseProgramObjectARB(0);
}

void ResourceShader::UniformTexture(const std::string &ext, GLint slot)
{
	//glActiveTexture(GL_TEXTURE0+slot);
	glClientActiveTexture(GL_TEXTURE0+slot);
	glUniform1iARB(glGetUniformLocation(m_nProgram, ext.c_str()), slot);
}

void ResourceShader::Uniform(const std::string &ext, GLint value)
{
	int id = glGetUniformLocation(m_nProgram, ext.c_str());
	glUniform1iARB(id, value);
}


void ResourceShader::Uniform(const std::string& ext, GLfloat value)
{
	int id = glGetUniformLocation(m_nProgram, ext.c_str());
	glUniform1fARB(id, value);
}

void ResourceShader::Uniform(const std::string& ext, float f1, float f2)
{
	int id = glGetUniformLocation(m_nProgram, ext.c_str());
	glUniform2fARB(id, f1, f2);
}


void ResourceShader::Uniform(const std::string &ext, GLfloat* value)
{
	int id = glGetUniformLocation(m_nProgram, ext.c_str());
	float v[4];
	v[0] = value[0];
	v[1] = value[1];
	v[2] = value[2];
	v[3] = 1.0f;
	glUniform4fvARB(id, 1, v);
}

