/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef CORE_H
#define CORE_H

#include <list>
#include "Memory.h"
#include <assert.h>
using namespace std;
typedef unsigned long DWORD;

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete [] (p); (p) = NULL; } }
#endif

#define FLT_MAX         3.402823466e+38F
#define FLT_MIN         1.175494351e-38F

#include <windows.h>
#include <GL/glew.h>
#include <GL/glu.h>
#include <GL/gl.h>


#include "flx_dll_api.h"
#include "../MathLib/flx_vector3.h"
#include "../MathLib/flx_matrix4.h"
#include "../MathLib/flx_ray.h"
#include "../MathLib/flx_plane.h"

typedef unsigned __int64 int64;
typedef unsigned int flx_Index;

struct flx_Vertex		{ float x,y,z; };
struct flx_Normal		{ float x,y,z; };
struct flx_TexCoord		{ float u,v; };
struct flx_VertexColor	{ float r,g,b,a; };


struct AABBox
{
	AABBox();
	AABBox(Vector3& _max, Vector3& _min);

	void getCorners(Vector3 pt[8]) const;
	void render();
	bool intersectionRay(Ray ray);

	AABBox transform(const AABBox &box, Matrix4x4 &mat);

	bool ClipSegment(Vector3& A, Vector3& B);
	bool ClipSegment(float _min, float _max, float a, float b, float d, float& t0, float& t1);
	bool isInBox(const Vector3& _point);

	Matrix4x4	m_pMatrix;
	Vector3		m_boxCenter;
	Vector3		vecMin, vecMax;
};

struct BoundingSphere
{
	BoundingSphere() {
		m_sphereRadius = 1.0f;
	}

	BoundingSphere(AABBox box)
	{
		Vector3 boxCenter = (box.vecMax - box.vecMin)/2;

		m_sphereCenter = box.m_boxCenter;
		m_sphereRadius = 5.0f;
	}

	void setCenter(AABBox box)
	{
		Vector3 boxCenter = (box.vecMax - box.vecMin)/2;
		m_sphereCenter = box.m_boxCenter;
	}

	float		m_sphereRadius;
	Vector3		m_sphereCenter;
};

//create virtual key codes
#define VK_KEY0 0x30
#define VK_KEY1 0x31
#define VK_KEY2 0x32
#define VK_KEY3 0x33
#define VK_KEY4 0x34
#define VK_KEY5 0x35
#define VK_KEY6 0x36
#define VK_KEY7 0x37
#define VK_KEY8 0x38
#define VK_KEY9 0x39
#define VK_KEYA 0x41
#define VK_KEYB 0x42
#define VK_KEYC 0x43
#define VK_KEYD 0x44
#define VK_KEYE 0x45
#define VK_KEYF 0x46
#define VK_KEYG 0x47
#define VK_KEYH 0x48
#define VK_KEYI 0x49
#define VK_KEYJ 0x4A
#define VK_KEYK 0x4B
#define VK_KEYL 0x4C
#define VK_KEYM 0x4D
#define VK_KEYN 0x4E
#define VK_KEYO 0x4F
#define VK_KEYP 0x50
#define VK_KEYQ 0x51
#define VK_KEYR 0x52
#define VK_KEYS 0x53
#define VK_KEYT 0x54
#define VK_KEYU 0x55
#define VK_KEYV 0x56
#define VK_KEYW 0x57
#define VK_KEYX 0x58
#define VK_KEYY 0x59
#define VK_KEYZ 0x5A
#define VK_KEYNUMPLUS 0x6b
#define VK_KEYNUMMINUS 0x6d

enum eAction {
	MOVE_FORWARD = 0,
	MOVE_BACK,
	MOVE_LEFT,
	MOVE_RIGHT,
	LOOK_LEFT,
	LOOK_RIGHT,
	LOOK_UP,
	LOOK_DOWN,
	JUMP,
	CROUCH_DOWN,
	STAND_UP,
	MBUTTON_LEFT,
	MBUTTON_RIGHT
};


#endif