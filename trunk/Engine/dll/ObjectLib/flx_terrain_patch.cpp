/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_terrain_patch.h"
#include "../Core/flx_renderer.h"
#include "../Core/ResourceManager/flx_resource_manager.h"

TerrainPatch::TerrainPatch()
{ }

TerrainPatch::~TerrainPatch()
{
}

void TerrainPatch::render()
{
	//Some quick hack to discard all patches which are further than 250 units from cam pos
	float dist_to_eye = (m_aabb.vecMin - Flux::Player::getInstance()->getPosition()).magnitude_sq();
	bool render_textures = true;
	
	if(dist_to_eye > 400*400)
		return;
	if(dist_to_eye > 300*300)
		render_textures = false;

	//Discard all patches if not in frustum
	if(!Flux::Renderer::getInstance()->GetCurrentScene()->GetFrustum().IsBoxInFrustum(m_aabb))
		return;

	//m_aabb.render();

	//Binding the buffers...
	VertexBuffer.bind();
	

	//wow, this sucks! binding the buffers 4 times...

	if(render_textures)
	{
		NormalBuffer.bind();
		ResourceManager::getInstance()->getTexture2D("resources/textures/alpha.tga")->bind(0);
		//	ResourceManager::getInstance()->getTexture2D("resources/textures/alpha1.tga")->bind(1);
		ResourceManager::getInstance()->getTexture2D("resources/textures/lightmap.tga")->bind(2);
		TexCoordBuffer.bind(2);
		ResourceManager::getInstance()->getTexture2D("resources/textures/ground.png")->bind(3);
		//ResourceManager::getInstance()->getTexture2D("resources/textures/grass.png")->bind(4);
		ResourceManager::getInstance()->getTexture2D("resources/textures/MullgoreBaseGrass.tga")->bind(4);
		ResourceManager::getInstance()->getTexture2D("resources/textures/rock.png")->bind(5);
		ResourceManager::getInstance()->getTexture2D("resources/textures/tile_rock0.png")->bind(6);
		TexCoordBuffer.bind();
	}
	//Ready! GO!
	glDrawElements(GL_TRIANGLE_STRIP, (GLsizei)m_Indices[0].size(), GL_UNSIGNED_INT, &m_Indices[0][0] );

	//Unbind the buffers
	if(render_textures) 
	{
		TexCoordBuffer.unbind();
		NormalBuffer.unbind();
	}
	VertexBuffer.unbind();

	Flux::Renderer::getInstance()->mNumVisibleVertices += VertexBuffer.getLength();
}