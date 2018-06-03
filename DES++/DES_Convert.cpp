#include "stdafx.h"
#include "DES_Convert.h"


DES_Convert::DES_Convert()
{
}


DES_Convert::~DES_Convert()
{
}

QWORD DES_Convert::CSW2BIN(CString str)
{
	QWORD output = 0;
	for (int i = 0; i < str.GetLength(); ++i)
	{
		if (str[i] == L'1') output ^= ((QWORD)0x1 << (63 - i));
	}
	return output;
}

QWORD DES_Convert::CSW2HEX(CString str)
{
	QWORD output = 0;
	for (int i = 0; i < str.GetLength(); ++i)
	{
		if (str[i] == L'1')			output ^= ((QWORD)0x1 << ((15 - i) * 4));
		else if (str[i] == L'2')	output ^= ((QWORD)0x2 << ((15 - i) * 4));
		else if (str[i] == L'3')	output ^= ((QWORD)0x3 << ((15 - i) * 4));
		else if (str[i] == L'4')	output ^= ((QWORD)0x4 << ((15 - i) * 4));
		else if (str[i] == L'5')	output ^= ((QWORD)0x5 << ((15 - i) * 4));
		else if (str[i] == L'6')	output ^= ((QWORD)0x6 << ((15 - i) * 4));
		else if (str[i] == L'7')	output ^= ((QWORD)0x7 << ((15 - i) * 4));
		else if (str[i] == L'8')	output ^= ((QWORD)0x8 << ((15 - i) * 4));
		else if (str[i] == L'9')	output ^= ((QWORD)0x9 << ((15 - i) * 4));
		else if (str[i] == L'A' || str[i] == L'a')	output ^= ((QWORD)0xa << ((15 - i) * 4));
		else if (str[i] == L'B' || str[i] == L'b')	output ^= ((QWORD)0xb << ((15 - i) * 4));
		else if (str[i] == L'C' || str[i] == L'c')	output ^= ((QWORD)0xc << ((15 - i) * 4));
		else if (str[i] == L'D' || str[i] == L'd')	output ^= ((QWORD)0xd << ((15 - i) * 4));
		else if (str[i] == L'E' || str[i] == L'e')	output ^= ((QWORD)0xe << ((15 - i) * 4));
		else if (str[i] == L'F' || str[i] == L'f')	output ^= ((QWORD)0xf << ((15 - i) * 4));
	}
	return output;
}

QWORD DES_Convert::CSW2ANSI(CString str)
{
	QWORD output = 0;
	int len = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, FALSE);
	char *buf = new char[len + 1];
	memset(buf, 0, len);
	int nRet = WideCharToMultiByte(CP_OEMCP, 0, str, -1, buf, len, NULL, FALSE);
	for (int i = 0; i < len - 1; ++i)
	{
		output ^= (QWORD)str[i] << ((7 - i) * 8);
	}
	delete[]buf;
	return output;
}

QWORD DES_Convert::CSW2Unicode(CString str)
{
	QWORD output = 0;
	for (int i = 0; i < str.GetLength(); ++i)
	{
		output ^= (QWORD)str[i] << ((3 - i) * 16);
	}
	return output;
}
