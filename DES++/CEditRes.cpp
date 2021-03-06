// Copyright (c) 2018 Pixel Frame Dev.

// CEditRes.cpp: 实现文件
//

#include "stdafx.h"
#include "DES++.h"
#include "CEditRes.h"


// CEditRes

IMPLEMENT_DYNAMIC(CEditRes, CEdit)

CEditRes::CEditRes()
{

}

CEditRes::~CEditRes()
{
}


BEGIN_MESSAGE_MAP(CEditRes, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CEditRes 消息处理程序




void CEditRes::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (type)
	{
	case 0: //BIN
		if ((nChar == '0') || (nChar == '1') || (0x08 == nChar) || (0x2E == nChar))
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		break;
	case 1: //HEX
		if ((nChar >= '0' && nChar <= '9') || (nChar >= 'a' && nChar <= 'f') || (nChar >= 'A' && nChar <= 'F') || (0x08 == nChar) || (0x2E == nChar))
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		break;
	/*case 2: //DEC
		if ((nChar >= '0' && nChar <= '9') || (0x08 == nChar) || (0x2E == nChar))
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		break;
		*/
	case 3: //BASE64
		if ((nChar == '=') || (nChar >= '0' && nChar <= '9') || (nChar >= 'a' && nChar <= 'z') || (nChar >= 'A' && nChar <= 'Z') || (0x08 == nChar) || (0x2E == nChar))
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		break;
	default:
		CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CEditRes::SetType(int type)
{
	this->type = type;
}