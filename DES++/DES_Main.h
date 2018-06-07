// Copyright (c) 2018 Pixel Frame Dev.
#pragma once

#include <vector>

class DES_Main
{
public:
	DES_Main();
	~DES_Main();
	static QWORD encrypt(QWORD m, QWORD key);
	static QWORD decrypt(QWORD c, QWORD key);
	static std::vector<QWORD> encryptBin(CString text, QWORD key, int mode, QWORD VECTOR);
	static std::vector<QWORD> encryptHex(CString text, QWORD key, int mode, QWORD VECTOR);
	static std::vector<QWORD> encryptANSI(CString text, QWORD key, int mode, QWORD VECTOR);
	static std::vector<QWORD> encryptUnicode(CString text, QWORD key, int mode, QWORD VECTOR);
	static std::vector<QWORD> encryptFile(CString strFile, QWORD key, int mode, QWORD VECTOR);
	static std::vector<QWORD> decryptHex(CString text, QWORD key, int mode, QWORD VECTOR);

	static const int MODE_ECB;
	static const int MODE_CBC;
	static const int MODE_CFB;
	static const int MODE_OFB;

private:
	static DWORD rsl28(DWORD, short);
	static int enLength(CString&, int);
};

