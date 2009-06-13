/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include <string>
#include <map>
#include "../flx_logger.h"
#include "../flx_singleton.h"
#include "flx_resource_texture2D.h"
#include "flx_resource_shader.h"
#include <assert.h>


typedef std::pair <std::string, void*> res_pair;

class ENGINE_API ResourceManager : public Singleton<ResourceManager>
{
public:
	ResourceManager();
	virtual ~ResourceManager();

	enum FLX_RESOURCE_TYPE {
		FLX_TEXTURE2D,
		FLX_SHADER
	};

	void setTexturePath(const std::string &_res_path);
	std::string getTexturePath();

	void* loadResource(FLX_RESOURCE_TYPE _res_type, const std::string& _res_name);
	void* createResource(void* _res_data, const std::string& _res_name);

	void*		getResource(const std::string& _res_name);
	ResourceTexture2D*	getTexture2D(const std::string& _res_name);
	//inline Shader*		getShader(const std::string& _res_name);

	void destroy();
	
protected:


private:
	std::map<std::string, void*> m_ResourceMap;
	std::string m_texture_path;

};

#endif