// Copyright (c) 2018 Pixel Frame Dev.
#pragma once
class DES_Convert
{
public:
	DES_Convert();
	~DES_Convert();

	static QWORD CSW2BIN(CString str);
	static QWORD CSW2HEX(CString str);
	static QWORD CSW2ANSI(CString str);
	static QWORD CHAR2ANSI(char* str);
	static QWORD CSW2Unicode(CString str);

};

