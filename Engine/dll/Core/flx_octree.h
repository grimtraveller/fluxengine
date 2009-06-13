/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

// OLD playground! Actually not used in the engine for now...

#ifndef OCTREE_H
#define OCTREE_H

#include "../MathLib/flx_vector3.h"

#include <vector>
#include <algorithm>

const int maxobjects = 500;


class Octree;
typedef bool (*callback)(const Octree &o, void *data);

class Object {
public:
	Object()
	{
		code = 0;
		mass = 0.0f;
		m_vPosition = Vector3(0.0f,0.0f,0.0f);
		m_vVelocity = Vector3(0.0f,0.0f,0.0f);
		remove = false;
	}
	Vector3			m_vPosition;
	Vector3			m_vVelocity;
	float			mass;
	float			force;
	unsigned int	code;
	bool			remove;
	double			id;
};

// Bounding box
typedef struct
{
	Vector3 center;
	float	radius;
} Box;

class Octree
{
public:
			Octree();
	virtual ~Octree();

	virtual const bool build(std::vector<Object*> objects, unsigned int count, 
	unsigned int threshold, unsigned int max_depth, const Box &boxes,
	unsigned int current_depth = 0);
	void cleanup(unsigned int count);

	static const Box	calc_boxes(std::vector<Object*> objects ,unsigned int count);
	virtual const bool	traverse(callback proc, void *data) const;
	void draw_cube();
	void update(double fDeltaTime, std::vector<Object*> objects);

private:

public:
	Octree			*_child[8];
	unsigned int	_objCount;
	Object			**_objects;
	Object			_center;
	double			_radius;
	Box				m_box;
	Vector3 box_offset_tbl[8];
	bool			m_bIsInFrustum;
	unsigned int child_obj_counts[8];
	std::vector<Object*> objList;
};

#endif