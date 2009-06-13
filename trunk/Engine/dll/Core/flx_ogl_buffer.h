/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef _flx_ogl_buffer_h
#define _flx_ogl_buffer_h

#include <map>
#include <vector>
#include "GL/glew.h"

template <class T> 
class flx_ogl_buffer
{
public:
	flx_ogl_buffer()
	{
		m_Count				= 0;
		m_VBOID				= 0;
		m_Data				= 0;
		m_ReservedElements	= 0;
		m_externalList		= false;
	}

	bool build(unsigned int _buffer_type, unsigned int _buffer_data_type)
	{
		m_BufferType		= _buffer_type;
		m_BufferDataType	= _buffer_data_type;

		if(GLEW_VERSION_1_5)
		{
			glGenBuffers(1, &m_VBOID);
			glBindBuffer(m_BufferType, m_VBOID);
			glBufferData(m_BufferType, m_Count * sizeof(T), m_Data, GL_STATIC_DRAW);
		}
		else
		{
			glGenBuffersARB(1, &m_VBOID);
			glBindBufferARB(m_BufferType, m_VBOID);
			glBufferDataARB(m_BufferType, m_Count * sizeof(T), m_Data, GL_STATIC_DRAW_ARB);
		}

		if(m_VBOID != 0)
			return true;

		return false;
	}

	void update(T *_list, unsigned int _elementCount)
	{
		setElementList(_list, _elementCount);

		if(GLEW_VERSION_1_5)
		{
			glBindBuffer(m_BufferType, m_VBOID);
			glBufferData(m_BufferType, m_Count * sizeof(T), m_Data, GL_DYNAMIC_DRAW);
		}
		else
		{
			glBindBufferARB(m_BufferType, m_VBOID);
			glBufferDataARB(m_BufferType, m_Count * sizeof(T), m_Data, GL_DYNAMIC_DRAW_ARB);
		}
	}

	void addElement(const T _element)
	{
		if(!m_externalList)
		{
			if(m_ReservedElements < m_Count)
			{
				T* pOldDataPtr		= m_Data;
				m_ReservedElements += 3;
				m_Data				= new T[m_ReservedElements];
				memcpy(m_Data, pOldDataPtr, m_Count* sizeof(T));

				delete pOldDataPtr;
			}
			m_Data[m_Count] = _element;

			m_Count++;
		}
	}

	void cleanup()
	{
		m_Count = 0;

		if(m_externalList)
		{
			m_Data = 0;
			m_externalList = false;
		}
		else
			delete m_Data;
	}

	void setElementList(T *_list, unsigned int _elementCount)
	{
		m_Data			= _list;
		m_Count			= _elementCount;
		m_externalList	= true;
	}


	void useElementList(T *_list, unsigned int _elementCount)
	{
		m_Data			= _list;
		m_Count			= _elementCount;
		m_externalList	= true;
	}

	inline void bind(unsigned int _texture_slot = 0)
	{
		if(m_BufferType == GL_ELEMENT_ARRAY_BUFFER)
		{
			if(GLEW_VERSION_1_5)
				glBindBuffer(m_BufferType, m_VBOID);
			else
				glBindBufferARB(m_BufferType, m_VBOID);
		}
		else
		{
			glClientActiveTexture(GL_TEXTURE0_ARB+_texture_slot);
			glEnableClientState(m_BufferDataType);

			if(GLEW_VERSION_1_5)
				glBindBuffer(m_BufferType, m_VBOID);
			else
				glBindBufferARB(m_BufferType, m_VBOID);

			switch (m_BufferDataType)
			{
			case GL_VERTEX_ARRAY:		glVertexPointer(3, GL_FLOAT, 0, (char*) NULL); break;
				
			case GL_NORMAL_ARRAY:		glNormalPointer(GL_FLOAT, 0,(char*) NULL); break;
			case GL_COLOR_ARRAY:		glColorPointer(4, GL_FLOAT, 0, (char*) NULL); break;
			case GL_TEXTURE_COORD_ARRAY:glTexCoordPointer(2, GL_FLOAT, 0, NULL); break;
			}
		}
	}

	inline void unbind()
	{
		if(m_BufferType != GL_ELEMENT_ARRAY_BUFFER)
			glDisableClientState(m_BufferDataType);

		if(GLEW_VERSION_1_5)
			glBindBuffer(m_BufferType, 0);
		else
			glBindBufferARB(m_BufferType, 0);

		glActiveTexture(GL_TEXTURE0);
	}

	inline unsigned int getLength()
	{
		return m_Count;
	}

	T* get(unsigned int _index)
	{
		if(_index < m_Count)
			return &m_Data[_index];
	}

//protected!
public:
	T* m_Data;

	unsigned int m_Count;
	unsigned int m_ReservedElements;
	unsigned int m_VBOID;
	unsigned int m_BufferType;
	unsigned int m_BufferDataType;

	bool m_externalList;
};

#endif