/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

// Crappy! Not used often

#ifndef COLOROGL_H
#define COLOROGL_H

class ColorOGL
{
public:
	inline ColorOGL() {
		m_fRed		= 0.0f;
		m_fGreen	= 0.0f;
		m_fBlue		= 0.0f;
		m_fAlpha	= 1.0f;
	}

	inline void Set(float fRed, float fGreen, float fBlue, float fAlpha = 255)
	{
		if (fBlue > 1.0f)
			m_fRed = 1.0f;
		else if (fRed <= 0.0f)
			m_fRed = 0.0f;
		else
			m_fRed = fRed;

		if (fGreen > 1.0f)
			m_fGreen = 1.0f;
		else if (fGreen <= 0.0f)
			m_fGreen = 0.0f;
		else
			m_fRed = fRed;

		if (fBlue > 1.0f)
			m_fBlue = 1.0f;
		else if (fBlue <= 0.0f)
			m_fBlue = 0.0f;
		else
			m_fBlue = fBlue;

		if(fAlpha  >= 1.0f) {
			m_fAlpha = 1.0f;
		}
		else if(fAlpha <= 0.0f)
		{
			m_fAlpha = 0.0f;
		}
		else
			m_fAlpha = fAlpha;
	}

	inline void Set(int iRed, int iGreen, int iBlue, int iAlpha = 255)
	{
		if (iRed <= 0)
			m_fRed = 0.0f;
		else if (iRed >= 255)
			m_fRed = 1.0f;
		else
			m_fRed = iRed/255.0f;

		if (iGreen <= 0)
			m_fGreen = 0.0f;
		else if (iGreen >= 255)
			m_fGreen = 1.0f;
		else
			m_fGreen = iGreen/255.0f;

		if (iBlue <= 0)
			m_fBlue = 0.0f;
		else if (iBlue > 255)
			m_fBlue = 1.0f;
		else
			m_fBlue = iBlue/255.0f;

		if(iAlpha  <= 0) {
			m_fAlpha = 0.0f;
		}
		else if(iAlpha >= 255)
		{
			m_fAlpha = 1.0f;
		}
		else
			m_fAlpha = iAlpha/255.0f;
	}

	inline ColorOGL(int iRed, int iGreen, int iBlue, int fAlpha = 255) {
		Set(iRed, iGreen, iBlue, fAlpha);
	}

	inline ColorOGL(float fRed, float fGreen, float fBlue, float fAlpha = 1.0f) {
		Set(fRed, fGreen, fBlue, fAlpha);
	}

	virtual ~ColorOGL() {}

	inline ColorOGL operator*(float &fValue) const {
		float fColor[3];

		fColor[0] = m_fRed * fValue;
		if(fColor[0] <= 0) {
			fColor[0] = 0.0f;
		}
		else if (fColor[0] >= 1.0f) {
			fColor[0] = 1.0f;
		}

		fColor[1] = m_fGreen * fValue;
		if(fColor[1] <= 0) {
			fColor[1] = 0.0f;
		}
		else if (fColor[1] >= 1.0f) {
			fColor[1] = 1.0f;
		}

		fColor[2] = m_fBlue * fValue;
		if(fColor[2] <= 0) {
			fColor[2] = 0.0f;
		}
		else if (fColor[2] >= 1.0f) {
			fColor[2] = 1.0f;
		}

		return ColorOGL(fColor[0], fColor[1], fColor[2], m_fAlpha);
	}

	public:
	float m_fRed;
	float m_fGreen;
	float m_fBlue;
	float m_fAlpha;

};

#endif