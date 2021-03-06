// Copyright (c) 2018 Pixel Frame Dev.

// DES++Dlg.h: 头文件
//

#pragma once

#include "CEditRes.h"

// CDESDlg 对话框
class CDESDlg : public CDialog
{
// 构造
public:
	CDESDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DES_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CFont mFont;
	HANDLE mHFont;
	CComboBox mCBIuputType;
	CComboBox mCBMode;
	CEditRes mETInput;
	CEditRes mETVector;
	CEditRes mETKey;
	afx_msg void OnChangeEditInput();
	afx_msg void OnBnClickedButtonAbout();
	afx_msg void OnSelchangeComboMode();
	afx_msg void OnBnClickedButtonAction();
	afx_msg void OnSelchangeComboInputType();
	afx_msg void OnBnClickedButtonRndKey();
	afx_msg void OnBnClickedButtonRndVec();
	afx_msg void OnBnClickedButtonFile();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnClickedRadioGroupAction();

private:
	int index = 0;
	int outputType = 0;
	void LoadFont();
	bool dispWarning = true;
	int mActionMode = 0;
	BOOL mBBase64;
	void LoadInputType();
public:
	afx_msg void OnClickedCheckBase64();
	CComboBox mCBOutputType;
};
