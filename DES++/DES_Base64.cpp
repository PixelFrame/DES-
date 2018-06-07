// Copyright (c) 2018 Pixel Frame Dev.
#include "stdafx.h"
#include "DES_Base64.h"


DES_Base64::DES_Base64()
{
}


DES_Base64::~DES_Base64()
{
}

const CString DES_Base64::base64Table = L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

CString DES_Base64::encrypt(const unsigned char * str, int bytes)
{
	int num = 0, bin = 0, i;
	CString result;
	const unsigned char * current;
	current = str;
	while (bytes > 2) {
		result += base64Table[current[0] >> 2];
		result += base64Table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
		result += base64Table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
		result += base64Table[current[2] & 0x3f];
		current += 3;
		bytes -= 3;
	}
	if (bytes > 0)
	{
		result += base64Table[current[0] >> 2];
		if (bytes % 3 == 1) {
			result += base64Table[(current[0] & 0x03) << 4];
			result += "==";
		}
		else if (bytes % 3 == 2) {
			result += base64Table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
			result += base64Table[(current[1] & 0x0f) << 2];
			result += "=";
		}
	}
	return result;
}


CString DES_Base64::decrypt(const char *str, int length)
{
	const char DecodeTable[] =
	{
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -1, -1, -2, -2, -1, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-1, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, 62, -2, -2, -2, 63,
		52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -2, -2, -2, -2, -2, -2,
		-2,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
		15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -2, -2, -2, -2, -2,
		-2, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
		41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,
		-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2
	};
	int bin = 0, i = 0, pos = 0;
	CString result;
	const char *current = str;
	char ch;
	while ((ch = *current++) != '\0' && length-- > 0)
	{
		if (ch == base64Pad) { // ��ǰһ���ַ��ǡ�=����
								/*
								��˵��һ������ڽ���ʱ��4���ַ�Ϊһ�����һ���ַ�ƥ�䡣
								����������
								1�����ĳһ��ƥ��ĵڶ����ǡ�=���ҵ������ַ����ǡ�=����˵������������ַ������Ϸ���ֱ�ӷ��ؿ�
								2�������ǰ��=�����ǵڶ����ַ����Һ�����ַ�ֻ�����հ׷�����˵�������������Ϸ������Լ�����
								*/
			if (*current != '=' && (i % 4) == 1) {
				return NULL;
			}
			continue;
		}
		ch = DecodeTable[ch];
		//�������Ҫ�������������в��Ϸ����ַ�
		if (ch < 0) { /* a space or some other separator character, we simply skip over */
			continue;
		}

		wchar_t token;
		switch (i % 4)
		{
		case 0:
			bin = ch << 2;
			break;
		case 1:
			bin |= ch >> 4;
			token = bin;
			if (token != L'\0') result.AppendChar(token);
			bin = (ch & 0x0f) << 4;
			break;
		case 2:
			bin |= ch >> 2;
			token = bin;
			if (token != L'\0') result.AppendChar(token);
			bin = (ch & 0x03) << 6;
			break;
		case 3:
			bin |= ch;
			token = bin;
			if (token != L'\0') result.AppendChar(token);
			break;
		}
		i++;
	}
	return result;
}
