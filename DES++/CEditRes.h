// Copyright (c) 2018 Pixel Frame Dev.
#pragma once


// CEditRes

class CEditRes : public CEdit
{
	DECLARE_DYNAMIC(CEditRes)

public:
	CEditRes();
	virtual ~CEditRes();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	void SetType(int);
private:
	int type;
};


