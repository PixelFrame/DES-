// Copyright (c) 2018 Pixel Frame Dev.
#pragma once
class DES_Base64
{
private:
	const static CString base64Table;
	const static char base64Pad = '=';
public:
	DES_Base64();
	~DES_Base64();
	static CString encrypt(const unsigned char * str, int bytes);
	static CString decrypt(const char *str, int length);
};

