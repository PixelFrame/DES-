#include "stdafx.h"
#include "DES_Main.h"
#include "DES_LookUp.h"
#include "DES_Convert.h"

DES_Main::DES_Main()
{
}

DES_Main::~DES_Main()
{
}

const int DES_Main::MODE_ECB = 0;
const int DES_Main::MODE_CBC = 1;
const int DES_Main::MODE_CFB = 2;
const int DES_Main::MODE_OFB = 3;

QWORD DES_Main::encrypt(QWORD m, QWORD key) {
	QWORD L0R0 = DES_LookUp::IP_Swap(m);
	QWORD T = DES_LookUp::PC1_Swap(key);
	DWORD C = (T & 0xFFFFFFF000000000) >> 32;
	DWORD D = (T & 0x0000000FFFFFFF00) >> 4;
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

std::vector<QWORD> DES_Main::encryptBin(CString text, QWORD key, int mode, QWORD VECTOR)
{
	std::vector<QWORD> cipher;
	int part;
	switch(mode)
	{
	case MODE_ECB:
		part = enLength(text, 64);
		for (int i = 0; i < part; ++i)
		{
			CString token = text.Mid(i * 64, 64);
			cipher.push_back(encrypt(DES_Convert::CSW2BIN(token), key));
		}
		break;
	case MODE_CBC:
		part = enLength(text, 64);
		for (int i = 0; i < part; ++i)
		{
			CString token = text.Mid(i * 64, 64);
			QWORD tokenQ = DES_Convert::CSW2BIN(token) ^ VECTOR;
			VECTOR = encrypt(tokenQ, key);
			cipher.push_back(VECTOR);
		}
		break;
	case MODE_CFB:
		part = enLength(text, 8);
		for (int i = 0; i < part; ++i)
		{
			QWORD enc = encrypt(VECTOR, key);
			CString token = text.Mid(i * 8, 8);
			QWORD ci = (enc ^ DES_Convert::CSW2BIN(token)) >> 56;
			VECTOR = (VECTOR << 8) ^ ci;
			cipher.push_back(ci);
		}
		break;
	case MODE_OFB:
		part = enLength(text, 64);
		for (int i = 0; i < part; ++i)
		{
			VECTOR = encrypt(VECTOR, key);
			CString token = text.Mid(i * 64, 64);
			QWORD ci = VECTOR ^ DES_Convert::CSW2BIN(token);
			cipher.push_back(ci);
		}
		break;
	}
	return cipher;
}

std::vector<QWORD> DES_Main::encryptHex(CString text, QWORD key, int mode, QWORD VECTOR)
{
	std::vector<QWORD> cipher;
	int part;
	switch (mode)
	{
	case MODE_ECB:
		part = enLength(text, 16);
		for (int i = 0; i < part; ++i)
		{
			CString token = text.Mid(i * 16, 16);
			cipher.push_back(encrypt(DES_Convert::CSW2HEX(token), key));
		}
		break;
	case MODE_CBC:
		part = enLength(text, 16);
		for (int i = 0; i < part; ++i)
		{
			CString token = text.Mid(i * 16, 16);
			QWORD tokenQ = DES_Convert::CSW2HEX(token) ^ VECTOR;
			VECTOR = encrypt(tokenQ, key);
			cipher.push_back(VECTOR);
		}
		break;
	case MODE_CFB:
		part = enLength(text, 2);
		for (int i = 0; i < part; ++i)
		{
			QWORD enc = encrypt(VECTOR, key);
			CString token = text.Mid(i * 2, 2);
			QWORD ci = (enc ^ DES_Convert::CSW2HEX(token)) >> 56;
			VECTOR = (VECTOR << 8) ^ ci;
			cipher.push_back(ci);
		}
		break;
	case MODE_OFB:
		part = enLength(text, 16);
		for (int i = 0; i < part; ++i)
		{
			VECTOR = encrypt(VECTOR, key);
			CString token = text.Mid(i * 16, 16);
			QWORD ci = VECTOR ^ DES_Convert::CSW2HEX(token);
			cipher.push_back(ci);
		}
		break;
	}
	return cipher;
}

std::vector<QWORD> DES_Main::encryptANSI(CString text, QWORD key, int mode, QWORD VECTOR)
{
	std::vector<QWORD> cipher;
	int part;
	switch (mode)
	{
	case MODE_ECB:
		part = enLength(text, 8);
		for (int i = 0; i < part; ++i)
		{
			CString token = text.Mid(i * 8, 8);
			cipher.push_back(encrypt(DES_Convert::CSW2ANSI(token), key));
		}
		break;
	case MODE_CBC:
		part = enLength(text, 8);
		for (int i = 0; i < part; ++i)
		{
			CString token = text.Mid(i * 8, 8);
			QWORD tokenQ = DES_Convert::CSW2ANSI(token) ^ VECTOR;
			VECTOR = encrypt(tokenQ, key);
			cipher.push_back(VECTOR);
		}
		break;
	case MODE_CFB:
		part = enLength(text, 1);
		for (int i = 0; i < part; ++i)
		{
			QWORD enc = encrypt(VECTOR, key);
			CString token = text.Mid(i * 1, 1);
			QWORD ci = (enc ^ DES_Convert::CSW2ANSI(token)) >> 56;
			VECTOR = (VECTOR << 8) ^ ci;
			cipher.push_back(ci);
		}
		break;
	case MODE_OFB:
		part = enLength(text, 8);
		for (int i = 0; i < part; ++i)
		{
			VECTOR = encrypt(VECTOR, key);
			CString token = text.Mid(i * 8, 8);
			QWORD ci = VECTOR ^ DES_Convert::CSW2ANSI(token);
			cipher.push_back(ci);
		}
		break;
	}
	return cipher;
}

std::vector<QWORD> DES_Main::encryptUnicode(CString text, QWORD key, int mode, QWORD VECTOR)
{
	std::vector<QWORD> cipher;
	int part;
	switch (mode)
	{
	case MODE_ECB:
		part = enLength(text, 4);
		for (int i = 0; i < part; ++i)
		{
			CString token = text.Mid(i * 4, 4);
			cipher.push_back(encrypt(DES_Convert::CSW2Unicode(token), key));
		}
		break;
	case MODE_CBC:
		part = enLength(text, 4);
		for (int i = 0; i < part; ++i)
		{
			CString token = text.Mid(i * 4, 4);
			QWORD tokenQ = DES_Convert::CSW2Unicode(token) ^ VECTOR;
			VECTOR = encrypt(tokenQ, key);
			cipher.push_back(VECTOR);
		}
		break;
	case MODE_CFB:
		part = enLength(text, 1);
		for (int i = 0; i < part; ++i)
		{
			QWORD enc = encrypt(VECTOR, key);
			CString token = text.Mid(i * 1, 1);
			QWORD ci = (enc ^ DES_Convert::CSW2Unicode(token)) >> 48;
			VECTOR = (VECTOR << 16) ^ ci;
			cipher.push_back(ci);
		}
		break;
	case MODE_OFB:
		part = enLength(text, 4);
		for (int i = 0; i < part; ++i)
		{
			VECTOR = encrypt(VECTOR, key);
			CString token = text.Mid(i * 4, 4);
			QWORD ci = VECTOR ^ DES_Convert::CSW2Unicode(token);
			cipher.push_back(ci);
		}
		break;
	}
	return cipher;
	return std::vector<QWORD>();
}

std::vector<QWORD> DES_Main::encryptFile(CString strFile, QWORD key, int mode, QWORD VECTOR)
{
	std::vector<QWORD> cipher;
	CFile file;
	if (file.Open(strFile, CFile::modeRead | CFile::typeBinary) == 0)
	{
		AfxMessageBox(L"Open File Failed");
		return cipher;
	}
	int iLen = file.GetLength();
	if (iLen >= 1048576)
	{
		AfxMessageBox(L"Only Support File < 1MB");
		file.Close();
		return cipher;
	}
	int part = iLen % 8 == 0 ? (iLen / 8) : (iLen / 8 + 1);
	int add = iLen % 8 == 0 ? 0 : 8 - iLen % 8;
	BYTE* pFile = new BYTE[iLen + add];
	BYTE* pData = pFile;
	file.Read(pData, iLen);
	file.Close();

	switch (mode)
	{
	case MODE_ECB:
		for (int i = 0; i < part; ++i)
		{
			QWORD token = 0;
			for (int j = 0; j < 8; ++j)
			{
				token ^= (QWORD)*pData << ((7 - j) * 8);
				++pData;
			}
			cipher.push_back(encrypt(token, key));
		}
		break;
	case MODE_CBC:
		for (int i = 0; i < part; ++i)
		{
			QWORD token = 0;
			for (int j = 0; j < 8; ++j)
			{
				token ^= (QWORD)*pData << ((7 - j) * 8);
				++pData;
			}
			token = token ^ VECTOR;
			VECTOR = encrypt(token, key);
			cipher.push_back(VECTOR);
		}
		break;
	case MODE_CFB:
		for (int i = 0; i < part; ++i)
		{
			QWORD enc = encrypt(VECTOR, key);
			QWORD ci = (enc >> 56) ^ *pData;
			++pData;
			VECTOR = (VECTOR << 8) ^ ci;
			cipher.push_back(ci);
		}
		break;
	case MODE_OFB:
		for (int i = 0; i < part; ++i)
		{
			VECTOR = encrypt(VECTOR, key);
			QWORD token = 0;
			for (int j = 0; j < 8; ++j)
			{
				token ^= (QWORD)*pData << ((7 - j) * 8);
				++pData;
			}
			QWORD ci = VECTOR ^ token;
			cipher.push_back(ci);
		}
		break;
	}
	delete[] pFile;
	return cipher;
}


DWORD DES_Main::rsl28(DWORD CD, short shift)
{
	DWORD res = CD << shift;
	res |= CD >> (28 - shift);
	return res;
}

int DES_Main::enLength(CString& text, int ciplen)
{
	int len = text.GetLength();
	int part = len % ciplen == 0 ? (len / ciplen) : (len / ciplen + 1);
	int add = len % ciplen == 0? 0 : ciplen - len % ciplen;
	for (int i = 0; i < add; ++i)
	{
		text.Append(L"0");
	}
	return part;
}
