#include "stdafx.h"
#include "DES_Main.h"
#include "DES_LookUp.h"

DES_Main::DES_Main()
{
}


DES_Main::~DES_Main()
{
}

QWORD DES_Main::encrypt(QWORD m, QWORD key) {
	QWORD L0R0 = DES_LookUp::IP_Swap(m);
	QWORD T = DES_LookUp::PC1_Swap(key);
	DWORD C = (T & 0xFFFFFFF000000000) >> 32;
	DWORD D = (T & 0x0000000FFFFFFF00) >> 4 ;
	DWORD L = L0R0 >> 32;
	DWORD R = L0R0;
	short shiftTable[] =
	{
		1, 1, 2, 2,
		2, 2, 2, 2, 
		1, 2, 2, 2,
		2, 2, 2, 1
	};
	for (int round = 0; round < 16; ++round)
	{
		C = rsl28(C, shiftTable[round]);
		D = rsl28(D, shiftTable[round]);
		T = ((QWORD)(C & 0xFFFFFFF0) << 32) | ((QWORD)D << 4);
		QWORD Ki = DES_LookUp::PC2_Swap(T);
		DWORD temp = R;
		R = DES_LookUp::P_Swap(DES_LookUp::S_Box_Swap(DES_LookUp::Expand_R(R) ^ Ki)) ^ L;
		L = temp;
	}
	QWORD R16L16 = ((QWORD)R << 32) | (QWORD)L;
	QWORD RES = DES_LookUp::IP_T_Swap(R16L16);
	return RES;
}

DWORD DES_Main::rsl28(DWORD CD,short shift)
{
	DWORD res = CD << shift;
	res |= CD >> (28 - shift);
	return res;
}