/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef SCENE_H
#define SCENE_H

#include "flx_core.h"
#include <vector>
#include "flx_node.h"
#include "../ObjectLib/flx_geometry.h"
#include "../ObjectLib/flx_geometry_grid.h"
#include "../ObjectLib/flx_geometry_terrain.h"
#include "../ObjectLib/flx_geometry_mesh.h"

#include "flx_node_transform.h"
#include "flx_node_shape.h"
#include "flx_texture_transform.h"
#include "flx_material.h"
#include "flx_frustum.h"
#include "flx_light.h"
#include "flx_light_point.h"
#include "flx_light_spot.h"
#include "flx_light_directional.h"
#include "flx_player.h"
#include "flx_octree.h"
#include "flx_logger.h"
#include "ResourceManager/flx_resource_manager.h"

namespace Flux
{
	class ENGINE_API Scene
	{
	public:
		Scene();
		virtual ~Scene();

		virtual bool Initialize() = 0;
		static void RunThread(void* instance);

		virtual void Update() = 0;
		virtual void Render() = 0;
		void Update(float deltaTime);
		void HUD();

		void	PlayerUpdate(float fTime);
		void	ToggleHeadLight();

		unsigned int	AddNode(Node* pNode, unsigned int iNestedID);
		unsigned int	SaveGeometry(Geometry* pGeometry);
		void			DeleteAllNodes();
		void			DeleteAllGeometries();
		unsigned int	SaveMaterial(Material* pMaterial);
		void			DeleteAllMaterials();
		void			DeleteAllAlphaObjects();

		//Materials
		bool			AssignMaterial(unsigned int iShapeID, unsigned int iMaterialID);
		bool			AssignTexture(unsigned int iShapeID, const std::string& TextureIdentifier, int tex_id);
		bool			AssignShader(unsigned int iShapeID, ResourceShader* shader);

		unsigned int	SaveTextureTransform(TextureTransform* pTextureTransform);
		void			DeleteAllTextureTransforms();
		bool			AssignTextureTransform(unsigned int iShapeID, unsigned iTextureTransformID);

		//Lighting Stuff
		int				AddLight(Light* pLight);
		void			DeleteAllLights();
		Light*			GetLight(int iID);
		void			LightsEnabled();
		void			LightsDisabled();



		bool			ClearAll();

	public:
		Frustum GetFrustum() { return m_Frustum; }
		Frustum	m_Frustum;
		std::vector<Node*>				m_vNodes;
		std::vector<Light*> m_vLights;

		Geometry*	m_currentSelectedGeometry;
	public:
		int		m_iVisible;
		bool	m_bSceneInitialized;

	private:
		std::vector<Geometry*>			m_vGeometries;
		std::vector<TextureTransform*>	m_vTextureTransforms;
		std::vector<Material*>			m_vMaterials;
		std::vector<AlphaObject*>		m_vAlphaObjects;

	private:
		//light stuff
		
		int m_iLightCounter;

	};
}

#endif