#pragma once

#define QWORD unsigned long long
#define DWORD unsigned long 
#define BYTE  unsigned char

class DES_LookUp
{
public:
	DES_LookUp();
	~DES_LookUp();

//Look-up Tables
private:
	static int S[8][4][16];
	static int PC1[56];
	static int PC2[48];
	static int IP[64];
	static int IP_T[64];
	static int E[48];
	static int P[32];
	
//Bit Operations
private:
	static QWORD mask64(QWORD, short);
	static DWORD mask32(DWORD, short);
	
//Public Interfaces
public:
	static QWORD IP_Swap(QWORD);
	static QWORD PC1_Swap(QWORD);
	static QWORD PC2_Swap(QWORD);
	static QWORD Expand_R(QWORD);
	static DWORD S_Box_Swap(QWORD);
	static DWORD P_Swap(DWORD);
	static QWORD IP_T_Swap(QWORD);
};

