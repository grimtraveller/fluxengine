/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

//OLD! Not used actually...

#include "flx_octree.h"
#include "flx_node_transform.h"
#include "flx_scene.h"


Box boxes;

char test2[70];


struct remove_element
{
	bool operator() (Object*& elem)
	{
		if(elem->remove)
		{
			//delete elem;
			return true;
		}
		return false;
	}
};

Octree::Octree()
: _objCount(0), _objects(NULL), _radius(0.0f)
{
	memset(_child, 0, sizeof(_child));

	box_offset_tbl[0] = Vector3(-0.5f, -0.5f, -0.5f);
	box_offset_tbl[1] = Vector3(0.5f, -0.5f, -0.5f);
	box_offset_tbl[2] = Vector3(-0.5f, 0.5f, -0.5f); 
	box_offset_tbl[3] = Vector3(0.5f, 0.5f, -0.5f);
	box_offset_tbl[4] = Vector3(-0.5f, -0.5f, 0.5f);
	box_offset_tbl[5] = Vector3(0.5f, -0.5f, 0.5f);
	box_offset_tbl[6] = Vector3(-0.5f, 0.5f, 0.5f);
	box_offset_tbl[7] = Vector3(0.5f, 0.5f, 0.5f);

	m_bIsInFrustum = true;
}

Octree::~Octree()
{
	//delete[] _objects;
}

const bool Octree::build(std::vector<Object*> objects, unsigned int count, unsigned int threshold, unsigned int max_depth, const Box &boxes, unsigned int current_depth)
{
	if(count < threshold || current_depth > max_depth)
	{
		_objCount = count;
		return true;
	}

	m_box = boxes;


	for(unsigned int i = 0; i < 8; i++)
	{
		child_obj_counts[i] = 0;
	}

	for(unsigned int i = 0; i < count; i++)
	{
		if(!(objects[i])) return true;
		Object &obj = *objects[i];

		const Vector3 &c = boxes.center;

		obj.code = 0;
		if(obj.m_vPosition.x > c.x) obj.code |= 1;
		if(obj.m_vPosition.y > c.y) obj.code |= 2;
		if(obj.m_vPosition.z > c.z) obj.code |= 4;

		child_obj_counts[obj.code]++;
	}

	for(unsigned int i = 0; i < 8; i++)
	{
		std::vector<Object*> newList_tmp;


		if(child_obj_counts[i] <= 0) continue;

		_child[i] = new Octree;

		for(unsigned int j = 0; j < count; j++)
		{
			if(objects[j]->code == i)
			{
				newList_tmp.push_back(objects[j]);
			}
		}

		/*//todo remove duplicate Ns
		int new_count = 0;
		for (unsigned int j = 0; j < child_obj_counts[i]; j++)
		{
		if(!newList_tmp[j] || !objects[j]) continue;
		if(objects[j] == newList_tmp[j])
		{
		//newList_tmp[j]->remove = true;
		} else
		{
		//newList.push_back(newList_tmp[j]);
		}
		}
		//newList_tmp.erase(remove_if(newList_tmp.begin(), newList_tmp.end(), remove_element()), newList_tmp.end());
		*/

		Vector3 offset = box_offset_tbl[i] * boxes.radius;

		Box new_boxes;
		new_boxes.radius = boxes.radius * 0.5f;
		new_boxes.center = boxes.center + offset;

		//recursive call
		_objCount = newList_tmp.size();
		_child[i]->build(newList_tmp, newList_tmp.size(), threshold, max_depth, new_boxes, current_depth+1);
		newList_tmp.clear();
	}

	return true;
}

const Box Octree::calc_boxes(std::vector<Object*> objects, unsigned int count)
{
	Box b;

	Object _min = *objects[0];
	Object _max = *objects[0];

	for(unsigned int i = 1; i < count; i++)
	{
		const Object &o = *objects[i];
		if(o.m_vPosition.x < _min.m_vPosition.x) _min.m_vPosition.x = o.m_vPosition.x;
		if(o.m_vPosition.y < _min.m_vPosition.y) _min.m_vPosition.y = o.m_vPosition.y;
		if(o.m_vPosition.z < _min.m_vPosition.z) _min.m_vPosition.z = o.m_vPosition.z;
		if(o.m_vPosition.x > _max.m_vPosition.x) _max.m_vPosition.x = o.m_vPosition.x;
		if(o.m_vPosition.y > _max.m_vPosition.y) _max.m_vPosition.y = o.m_vPosition.y;
		if(o.m_vPosition.z > _max.m_vPosition.z) _max.m_vPosition.z = o.m_vPosition.z;
	}

	Vector3 radius	= _max.m_vPosition - _min.m_vPosition;
	radius.x *= 0.5f;
	radius.y *= 0.5f;
	radius.z *= 0.5f;
	b.center	= _min.m_vPosition + radius ;

	b.radius = radius.x;
	if(b.radius < radius.y) b.radius = radius.y;
	if(b.radius < radius.z) b.radius = radius.z;

	return b;
}

const bool Octree::traverse(callback proc, void *data) const
{
	if(!proc(*this, data)) return false;

	if(!_objCount)
	{
		for(unsigned int i = 0; i < 8; i++)
		{
			if(!_child[i]) continue;
			if(!_child[i]->traverse(proc, data)) return false;
		}
	}

	return false;
}

void Octree::update(double fDeltaTime, std::vector<Object*> objects)
{
	boxes = calc_boxes(objects, maxobjects);
	build(objects, maxobjects, 32, 32, boxes, 0);
}

void Octree::draw_cube()
{
	float m_fDepth  = this->m_box.radius;
	float m_fHeight = this->m_box.radius;
	float m_fWidth  = this->m_box.radius;
	/*dynamic_cast<NodeTransform*>(octree_box_t)->m_v3Translate = Vector3(this->m_box.center.x,this->m_box.center.y,this->m_box.center.z);
	dynamic_cast<NodeTransform*>(octree_box_t)->RenderOGL(false);
	*/

	if(this->m_box.center.x == 0.0f && this->m_box.center.y == 0.0f) return;

	glDisable(GL_TEXTURE_2D);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glTranslatef(this->m_box.center.x, this->m_box.center.y,this->m_box.center.z);
	glBegin(GL_QUADS);

	//front
	glNormal3f(0, 0, 1.0f);
	glVertex3f(-m_fWidth ,	-m_fHeight, m_fDepth);
	glVertex3f(m_fWidth,	-m_fHeight, m_fDepth);
	glVertex3f(m_fWidth,	m_fHeight, m_fDepth);
	glVertex3f(-m_fWidth,	m_fHeight, m_fDepth);

	//right
	glNormal3f(1.0f, 0, 0.0f);
	glVertex3f(m_fWidth ,	-m_fHeight,	m_fDepth);
	glVertex3f(m_fWidth,	-m_fHeight,	-m_fDepth);
	glVertex3f(m_fWidth,	m_fHeight,	-m_fDepth);
	glVertex3f(m_fWidth,	m_fHeight,	m_fDepth);

	//back
	glNormal3f(0.0f, 0, -1.0f);
	glVertex3f(m_fWidth ,	-m_fHeight,	-m_fDepth);
	glVertex3f(-m_fWidth,	-m_fHeight,	-m_fDepth);
	glVertex3f(-m_fWidth,	m_fHeight,	-m_fDepth);
	glVertex3f(m_fWidth,	m_fHeight,	-m_fDepth);

	//left
	glNormal3f(-1.0f, 0, 0.0f);
	glVertex3f(-m_fWidth ,			-m_fHeight,	-m_fDepth);
	glVertex3f(-m_fWidth,			-m_fHeight,	m_fDepth);
	glVertex3f(-m_fWidth,			m_fHeight,	m_fDepth);
	glVertex3f(-m_fWidth,			m_fHeight,	-m_fDepth);

	//top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-m_fWidth ,			m_fHeight,	m_fDepth);
	glVertex3f(m_fWidth,			m_fHeight,	m_fDepth);
	glVertex3f(m_fWidth,			m_fHeight,	-m_fDepth);
	glVertex3f(-m_fWidth,			m_fHeight,	-m_fDepth);

	//bottom
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-m_fWidth ,		m_fHeight,	-m_fDepth);
	glVertex3f(m_fWidth,		m_fHeight,	-m_fDepth);
	glVertex3f(m_fWidth,		m_fHeight,	m_fDepth);
	glVertex3f(-m_fWidth,		m_fHeight,	m_fDepth);
	glEnd();
	glTranslatef(-this->m_box.center.x, -this->m_box.center.y,-this->m_box.center.z);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_TEXTURE_2D);

	for(int i = 0; i < 8; i++)
	{
		if(this->_child[i])
				this->_child[i]->draw_cube();
	}
}

void Octree::cleanup(unsigned int count)
{

	for(int j = 0; j < 8; j++)
	{
		if(!child_obj_counts[j]) continue;
		if(_child[j])
		{
			_child[j]->cleanup(count);
			delete  _child[j];
		}
	}

}