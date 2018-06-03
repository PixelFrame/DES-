#pragma once
#include "DES_LookUp.h"
class DES_Main
{
public:
	DES_Main();
	~DES_Main();
	static QWORD encrypt(QWORD m, QWORD key);

private:
	static DWORD rsl28(DWORD, short);
};

