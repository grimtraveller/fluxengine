/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_resource_manager.h"

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
	destroy();
}

void ResourceManager::destroy()
{
	for(std::map<std::string, void*>::iterator it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it)
	{
		delete (*it).second;
		(*it).second = NULL;
	}
	m_ResourceMap.clear();
}

void ResourceManager::setTexturePath(const std::string &_res_path)
{
	m_texture_path = _res_path;
};

std::string ResourceManager::getTexturePath()
{
	return m_texture_path;
}

void* ResourceManager::loadResource(FLX_RESOURCE_TYPE _res_type, const std::string &_res_name)
{
	assert(_res_name != "");

	if(m_ResourceMap.find(_res_name) != m_ResourceMap.end())
		return m_ResourceMap.find(_res_name)->second;

	FLX_LOGGER->Write(LOG_DEFAULT, "Loading resource %s... ", _res_name.c_str());

	void* p = NULL;

	switch(_res_type)
	{
	case FLX_TEXTURE2D:
		{
			p = new ResourceTexture2D();
			((ResourceTexture2D*)p)->load(_res_name);

			break;
		}
	case FLX_SHADER:
		{
			p = new ResourceShader();
			if(!_res_name.empty())
				((ResourceShader*)p)->load(_res_name);

			break;
		}
	}




	if(p == NULL) 
	{
		FLX_LOGGER->Write(LOG_ERROR, "ResourceManager::loadResource::p == NULL\n");
		return NULL;
	}

	m_ResourceMap.insert(res_pair(_res_name, p));
	//m_ResourceMap[_res_name] = p;

	return p;
}

void* ResourceManager::createResource(void *_res_data, const std::string& _res_name)
{
	if(!_res_data) return NULL;

	if(m_ResourceMap.find(_res_name) != m_ResourceMap.end())
		return m_ResourceMap.find(_res_name)->second;

	m_ResourceMap.insert(res_pair(_res_name, _res_data));
	//m_ResourceMap[_res_name] = _res_data;
}

void* ResourceManager::getResource(const std::string& _res_name)	
{
	for(std::map<std::string, void*>::iterator it = m_ResourceMap.begin(); it != m_ResourceMap.end(); ++it)
	{
		if((*it).first == _res_name)
		{
			return (*it).second;
		}
	}
	return NULL;
}

ResourceTexture2D* ResourceManager::getTexture2D(const std::string& _res_name)
{
	if((ResourceTexture2D*)getResource(_res_name) == NULL)
	{
		FLX_LOGGER->Write(LOG_DEFAULT, "System requested texture %s before it was loaded. Trying to load it now...", _res_name.c_str());
		//if there's no texture named as in _res_name try to load it
		if(!loadResource(ResourceManager::FLX_TEXTURE2D, _res_name))
		{
			//file does not exist!
			//we load another texture instead to prevent crashes
			FLX_LOGGER->Write(LOG_DEFAULT, "Could not load texture %s. Loaded error texture instead.\n", _res_name.c_str());
			loadResource(ResourceManager::FLX_TEXTURE2D, "resources/textures/cursor.png");
			FLX_LOGGER->Write(LOG_DEFAULT, "[DONE}\n");
			return (ResourceTexture2D*)getResource("resources/textures/cursor.png");
		}
	}
	else
		return (ResourceTexture2D*)getResource(_res_name);
}