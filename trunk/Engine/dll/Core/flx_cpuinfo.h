/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#ifndef _CPU_INF_
#define _CPU_INF_

#include <iostream>

typedef struct CPU_INFO
{
	bool B_SUPPORTS_SSE;
	bool B_SUPPORTS_SSE2;
	bool B_SUPPORTS_SSE3;
	bool B_SUPPORTS_3DNOW;
	bool B_SUPPORTS_MMX;
	bool B_SUPPORTS_MMX_EX;
	bool B_SUPPORTS_3DNOW_EX;
	bool B_SUPPORTS_HTT;
	bool B_SUPPORTS_FPU;

	char S_MODEL_NAME[49];
	char S_VENDOR_ID[13];

	int CPU_STEPPING;
	int CPU_MODEL;
	int CPU_FAMILY;
	int CPU_TYPE;
	int CPU_EXT_MODEL;
	int CPU_EXT_FAMILY;
	int CPU_L1_CACHE;
	int CPU_L2_CACHE;

	bool B_SSE_USEABLE;
	bool B_SSE2_USEABLE;
	bool B_SSE3_USEABLE;

} CPUINFO;

CPUINFO GetCPUInfo();


#endif