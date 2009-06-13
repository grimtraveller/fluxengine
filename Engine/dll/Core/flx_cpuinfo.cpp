/*---------------------------------------------------------------------------
This source file is part of the FluxEngine.

Copyright (c) 2008 - 2009 Marvin K. (starvinmarvin)

This program is free software.
---------------------------------------------------------------------------*/

#include "flx_cpuinfo.h"
#include <string.h>

CPUINFO GetCPUInfo()
{
	CPUINFO t_cpu_info;
	memset((void*)&t_cpu_info, 0, sizeof(CPUINFO));

	unsigned int cpu_feature_eax = 0;
	unsigned int cpu_feature_ecx = 0;
	unsigned int cpu_feature_edx = 0;
	unsigned int cpu_feature_ext_edx = 0;
	unsigned int cpu_high_feature_eax = 0;
	unsigned int cpu_high_feature_ecx = 0;
	unsigned int cpu_high_feature_edx = 0;

	__asm
	{
		mov eax, 0x00000000
			CPUID
			mov DWORD PTR [t_cpu_info.S_VENDOR_ID + 0], ebx		//get vendor
			mov DWORD PTR [t_cpu_info.S_VENDOR_ID + 4], edx
			mov DWORD PTR [t_cpu_info.S_VENDOR_ID + 8], ecx

			//if eax == 0 cpu has no features
			test eax, eax
			jz _EXIT

			mov eax, 0x00000001		//get extended infos
			CPUID
			mov [cpu_feature_eax], eax
			mov [cpu_feature_edx], edx
			mov [cpu_feature_ecx], ecx

			mov eax, 0x80000000		//get first extended function
			CPUID

			cmp eax, 0x80000001
			jb	_EXIT
			cmp eax, 0x00000004
			jb	_EXT_FEATURES_ONLY

			//get brand name
			mov eax, 0x80000002
			CPUID
			mov DWORD PTR [t_cpu_info.S_MODEL_NAME + 0], eax
			mov DWORD PTR [t_cpu_info.S_MODEL_NAME + 4], ebx
			mov DWORD PTR [t_cpu_info.S_MODEL_NAME + 8], ecx
			mov DWORD PTR [t_cpu_info.S_MODEL_NAME + 12], edx

			mov eax, 0x80000003
			CPUID
			mov DWORD PTR [t_cpu_info.S_MODEL_NAME + 16], eax
			mov DWORD PTR [t_cpu_info.S_MODEL_NAME + 20], ebx
			mov DWORD PTR [t_cpu_info.S_MODEL_NAME + 24], ecx
			mov DWORD PTR [t_cpu_info.S_MODEL_NAME + 28], edx

			mov eax, 0x80000004
			CPUID
			mov DWORD PTR [t_cpu_info.S_MODEL_NAME + 32], eax
			mov DWORD PTR [t_cpu_info.S_MODEL_NAME + 36], ebx
			mov DWORD PTR [t_cpu_info.S_MODEL_NAME + 40], ecx
			mov DWORD PTR [t_cpu_info.S_MODEL_NAME + 44], edx

			mov eax, 0x00000006		//get highest feature
			CPUID
			mov [cpu_high_feature_eax], eax
			mov [cpu_high_feature_edx], edx
			mov [cpu_high_feature_ecx], ecx

_EXT_FEATURES_ONLY:
		mov eax, 0x80000001
			CPUID
			mov [cpu_feature_ext_edx], edx

_EXIT:
	}

	t_cpu_info.CPU_STEPPING		= (cpu_feature_eax) & 0xF;
	t_cpu_info.CPU_MODEL		= (cpu_feature_eax >> 4) & 0xF;
	t_cpu_info.CPU_FAMILY		= (cpu_feature_eax >> 8) & 0xF;
	t_cpu_info.CPU_TYPE			= (cpu_feature_eax >> 12) & 0x3;
	t_cpu_info.CPU_EXT_MODEL	= (cpu_feature_eax >> 16) & 0xF;
	t_cpu_info.CPU_EXT_FAMILY	= (cpu_feature_eax >> 20) & 0xFF;

	t_cpu_info.B_SUPPORTS_FPU	= (cpu_feature_edx >> 0) & 0x1;
	t_cpu_info.B_SUPPORTS_MMX	= (cpu_feature_edx >> 23) & 0x1;
	t_cpu_info.B_SUPPORTS_SSE	= (cpu_feature_edx >> 25) & 0x1;
	t_cpu_info.B_SUPPORTS_SSE2	= (cpu_feature_edx >> 26) & 0x1;
	t_cpu_info.B_SUPPORTS_HTT	= (cpu_feature_edx >> 26) & 0x1;

	t_cpu_info.B_SUPPORTS_SSE3	= cpu_feature_ecx & 0x1;

	t_cpu_info.B_SUPPORTS_MMX_EX	= (cpu_feature_ext_edx >> 22) & 0x1;
	t_cpu_info.B_SUPPORTS_3DNOW		= (cpu_feature_ext_edx >> 31) & 0x1;
	t_cpu_info.B_SUPPORTS_3DNOW_EX	= (cpu_feature_ext_edx >> 30) & 0x1;

	return t_cpu_info;
}