// Copyright (c) 2018 Pixel Frame Dev.

// DES++Dlg.cpp: 实现文件
//

#include "stdafx.h"
#include "DES++.h"
#include "DES++Dlg.h"
#include "afxdialogex.h"

#include "DES_Convert.h"
#include "DES_Main.h"
#include "DES_Base64.h"

#include <random>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDESDlg 对话框



CDESDlg::CDESDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DES_DIALOG, pParent)
	, mActionMode(0)
	, mBBase64(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDESDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_INPUT_TYPE, mCBIuputType);
	DDX_Control(pDX, IDC_EDIT_INPUT, mETInput);
	DDX_Control(pDX, IDC_EDIT_KEY, mETKey);
	DDX_Control(pDX, IDC_COMBO_MODE, mCBMode);
	DDX_Control(pDX, IDC_EDIT_VECTOR, mETVector);
	DDX_Radio(pDX, IDC_RADIO_EN, mActionMode);
	DDX_Check(pDX, IDC_CHECK_BASE64, mBBase64);
	DDX_Control(pDX, IDC_COMBO_OUTPUT, mCBOutputType);
}

BEGIN_MESSAGE_MAP(CDESDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ACTION, &CDESDlg::OnBnClickedButtonAction)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUT_TYPE, &CDESDlg::OnSelchangeComboInputType)
	ON_EN_CHANGE(IDC_EDIT_INPUT, &CDESDlg::OnChangeEditInput)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CDESDlg::OnBnClickedButtonAbout)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CDESDlg::OnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON_RND_KEY, &CDESDlg::OnBnClickedButtonRndKey)
	ON_BN_CLICKED(IDC_BUTTON_RND_VEC, &CDESDlg::OnBnClickedButtonRndVec)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CDESDlg::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDESDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_RADIO_EN, &CDESDlg::OnClickedRadioGroupAction)
	ON_BN_CLICKED(IDC_RADIO_DE, &CDESDlg::OnClickedRadioGroupAction)
	ON_BN_CLICKED(IDC_CHECK_BASE64, &CDESDlg::OnClickedCheckBase64)
END_MESSAGE_MAP()


// CDESDlg 消息处理程序

BOOL CDESDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	mCBIuputType.AddString(L"Num (Bin)");
	mCBIuputType.AddString(L"Num (Hex)");
	mCBIuputType.AddString(L"Text (UTF-8)");
	mCBIuputType.AddString(L"Text (Unicode)");
	mCBIuputType.AddString(L"File");
	mCBIuputType.SetCurSel(index);
	mETKey.SetType(0);

	mCBMode.AddString(L"ECB");
	mCBMode.AddString(L"CBC");
	mCBMode.AddString(L"CFB");
	mCBMode.AddString(L"OFB");
	mCBMode.SetCurSel(0);
	mETKey.SetType(1);
	mETVector.SetType(1);

	mCBOutputType.AddString(L"Hex");
	mCBOutputType.AddString(L"Bin");
	mCBOutputType.AddString(L"UTF-8");
	mCBOutputType.AddString(L"Unicode");
	mCBOutputType.SetCurSel(0);

	LoadFont();
	mFont.CreatePointFont(120, L"Inziu Iosevka SC");
	GetDlgItem(IDC_EDIT_INPUT)->SetFont(&mFont);
	GetDlgItem(IDC_EDIT_INPUT)->ShowScrollBar(SB_VERT, TRUE);
	GetDlgItem(IDC_EDIT_OUTPUT)->SetFont(&mFont);
	GetDlgItem(IDC_EDIT_OUTPUT)->ShowScrollBar(SB_VERT, TRUE);

	GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(true);
	GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(false);
	CString info = (L"输入视为二进制数，每64位一次分割（CBF模式下8位），除ECB模式外可指定初始向量IV");
	GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(info);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDESDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDESDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDESDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDESDlg::OnBnClickedButtonAction()
{
	// TODO: 在此添加控件通知处理程序代码
	CString input, key, vector;
	GetDlgItem(IDC_EDIT_INPUT)->GetWindowTextW(input);
	GetDlgItem(IDC_EDIT_KEY)->GetWindowTextW(key);
	GetDlgItem(IDC_EDIT_VECTOR)->GetWindowTextW(vector);
	int mode = mCBMode.GetCurSel();

	std::vector<QWORD> res;
	CString output;
	outputType = mCBOutputType.GetCurSel();
	if (mActionMode == 0)
	{
		int len = WideCharToMultiByte(CP_REGION, 0, input, -1, NULL, 0, NULL, FALSE) - 1;
		switch (index)
		{
		case 0:
			if (input.GetLength() % 64 != 0)
			{
				MessageBoxExW(NULL, L"Input Will Be Lengthened to Multiples of 64 with \'0\'", L"Attention", MB_OK, 0);
			}
			res = DES_Main::encryptBin(input, DES_Convert::CSW2HEX(key), mode, DES_Convert::CSW2HEX(vector));
			break;
		case 1:
			if (input.GetLength() % 16 != 0)
			{
				MessageBoxExW(NULL, L"Input Will Be Lengthened to Multiples of 16 with \'0\'", L"Attention", MB_OK, 0);
			}
			res = DES_Main::encryptHex(input, DES_Convert::CSW2HEX(key), mode, DES_Convert::CSW2HEX(vector));
			break;
		case 2:
			if (len % 8 != 0)
			{
				MessageBoxExW(NULL, L"Input Will Be Lengthened to Multiples of 8 with \'0\'", L"Attention", MB_OK, 0);
			}
			res = DES_Main::encryptANSI(input, DES_Convert::CSW2HEX(key), mode, DES_Convert::CSW2HEX(vector));
			break;
		case 3:
			if (input.GetLength() % 4 != 0)
			{
				MessageBoxExW(NULL, L"Input Will Be Lengthened to Multiples of 4 with \'0\'", L"Attention", MB_OK, 0);
			}
			res = DES_Main::encryptUnicode(input, DES_Convert::CSW2HEX(key), mode, DES_Convert::CSW2HEX(vector));
			break;
		case 4:
			res = DES_Main::encryptFile(input, DES_Convert::CSW2HEX(key), mode, DES_Convert::CSW2HEX(vector));
			break;
		}
		if (!mBBase64)
		{
			int total = res.size() * 8;
			char* sBuff = new char[total];
			memset(sBuff, 0, total);
			for (std::vector<QWORD>::const_iterator iter = res.cbegin(); iter != res.cend(); ++iter)
			{
				QWORD mask = 0x8000000000000000;
				wchar_t wtoken;
				switch (outputType)
				{
				case 0:
					output.AppendFormat(L"Part %d:\r\n", iter - res.cbegin());
					output.AppendFormat(L"%08X", *iter >> 32);
					output.AppendFormat(L"%08X\r\n", *iter);
					break;
				case 1:
					output.AppendFormat(L"Part %d:\r\n", iter - res.cbegin());
					for (int x = 0; x < 64; ++x)
					{
						if (*iter & mask) output.Append(L"1");
						else output.Append(L"0");
						mask = mask >> 1;
					}
					output.Append(L"\r\n");
					break;
				case 2:
					for (int x = 0; x < 8; ++x)
					{
						sBuff[(iter - res.cbegin()) * 8 + x] = (*iter) >> (56 - x * 8);
					}
					break;
				case 3:
					for (int x = 0; x < 4; ++x)
					{
						wtoken = (*iter) >> (48 - x * 16);
						output.AppendChar(wtoken);
					}
					break;
				}
			}
			if (outputType == 2)
			{
				DWORD dBufSize = MultiByteToWideChar(CP_REGION, 0, sBuff, total, NULL, 0);
				wchar_t* dBuf = new wchar_t[dBufSize];
				wmemset(dBuf, 0, dBufSize);
				MultiByteToWideChar(CP_REGION, 0, sBuff, total, dBuf, dBufSize);
				output.Append(dBuf);
				delete(dBuf);
			}
			delete[]sBuff;
		}
		else
		{
			for (std::vector<QWORD>::const_iterator iter = res.cbegin(); iter != res.cend(); ++iter)
			{
				output.AppendFormat(L"%08X", *iter >> 32);
				output.AppendFormat(L"%08X", *iter);
			}
			int length = WideCharToMultiByte(CP_REGION, 0, output, -1, NULL, 0, NULL, FALSE);
			char *buf = new char[length + 1];
			memset(buf, 0, length);
			int nRet = WideCharToMultiByte(CP_REGION, 0, output, -1, buf, length, NULL, FALSE);
			output = DES_Base64::encrypt((unsigned char*)buf, nRet);
			delete[]buf;
		}
	}
	else
	{
		if (index==0) res = DES_Main::decryptHex(input, DES_Convert::CSW2HEX(key), mode, DES_Convert::CSW2HEX(vector));
		else 
		{
			int length = WideCharToMultiByte(CP_REGION, 0, input, -1, NULL, 0, NULL, FALSE);
			char *buf = new char[length + 1];
			memset(buf, 0, length);
			int nRet = WideCharToMultiByte(CP_REGION, 0, input, -1, buf, length, NULL, FALSE);
			res = DES_Main::decryptHex(DES_Base64::decrypt(buf,strlen(buf)), DES_Convert::CSW2HEX(key), mode, DES_Convert::CSW2HEX(vector));
			delete[]buf;
		}
		int total = res.size() * 8;
		char* sBuff = new char[total];
		memset(sBuff, 0, total);
		for (std::vector<QWORD>::const_iterator iter = res.cbegin(); iter != res.cend(); ++iter)
		{
			QWORD mask = 0x8000000000000000;
			wchar_t wtoken;
			switch (outputType)
			{
			case 0:
				output.AppendFormat(L"Part %d:\r\n", iter - res.cbegin());
				output.AppendFormat(L"%08X", *iter >> 32);
				output.AppendFormat(L"%08X\r\n", *iter);
				break;
			case 1:
				output.AppendFormat(L"Part %d:\r\n", iter - res.cbegin());
				for (int x = 0; x < 64; ++x)
				{
					if (*iter & mask) output.Append(L"1");
					else output.Append(L"0");
					mask = mask >> 1;
				}
				output.Append(L"\r\n");
				break;
			case 2:
				for (int x = 0; x < 8; ++x)
				{
					sBuff[(iter - res.cbegin()) * 8 + x] = (*iter) >> (56 - x * 8);
				}
				break;
			case 3:
				for (int x = 0; x < 4; ++x)
				{
					wtoken = (*iter) >> (48 - x * 16);
					output.AppendChar(wtoken);
				}
				break;
			}
		}
		if (outputType==2)
		{
			DWORD dBufSize = MultiByteToWideChar(CP_REGION, 0, sBuff, total, NULL, 0);
			wchar_t* dBuf = new wchar_t[dBufSize];
			wmemset(dBuf, 0, dBufSize);
			MultiByteToWideChar(CP_REGION, 0, sBuff, total, dBuf, dBufSize);
			output.Append(dBuf);
			delete(dBuf);
		}
		delete[]sBuff;
	}

	GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(output);
}

void CDESDlg::OnSelchangeComboInputType()
{
	// TODO: 在此添加控件通知处理程序代码
	index = mCBIuputType.GetCurSel();
	if (index == 4)
	{
		GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(false);
	}

	CString info;
	if (mActionMode == 0)
	{
		switch (index)
		{
		case 0:
			info.Append(L"输入视为二进制数，每64位一次分割（CBF模式下8位），除ECB模式外可指定初始向量IV");
			GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(info);
			break;
		case 1:
			info.Append(L"输入视为十六进制数，每16位一次分割（CBF模式下2位），除ECB模式外可指定初始向量IV");
			GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(info);
			break;
		case 2:
			info.Append(L"输入视为UTF-8编码，每8位一次分割（CBF模式下1位），除ECB模式外可指定初始向量IV\r\n\r\n以运行主机代码页为准进行转换，仅在中文环境下经过测试，其他环境不保证准确");
			GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(info);
			break;
		case 3:
			info.Append(L"输入视为Unicode(UTF-16 LE)编码，在Windows环境下UTF-8使用16位wchar_t存储，因此每4位一次分割（CBF模式下1位），除ECB模式外可指定初始向量IV");
			GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(info);
			break;
		case 4:
			info.Append(L"输入文件以二进制方式打开进行加密，除ECB模式外可指定初始向量IV\r\n最大支持1MB文件，加密过程中程序可能未响应");
			GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(info);
			break;
		}
		mETInput.SetType(index);
	}
	else
	{
		switch (index)
		{
		case 0:
			info.Append(L"输入视为十六进制数，每16位一次分割（CBF模式下2位），除ECB模式外可指定初始向量IV");
			GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(info);
			mETInput.SetType(1);
			break;
		case 1:
			info.Append(L"输入为十六进制数的BASE64加密，先进行BASE64解密再按十六进制数进行DES解密");
			GetDlgItem(IDC_EDIT_OUTPUT)->SetWindowTextW(info);
			mETInput.SetType(3);
			break;
		}
	}
	GetDlgItem(IDC_EDIT_INPUT)->SetWindowTextW(L"");
}

void CDESDlg::OnChangeEditInput()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CString str;
	GetDlgItemTextW(IDC_EDIT_INPUT, str);
	CString length;
	length.Format(L"Text Length: %d", str.GetLength());
	GetDlgItem(IDC_STATIC_LENGTH)->SetWindowText(length);
}

void CDESDlg::OnBnClickedButtonAbout()
{
	// TODO: 在此添加控件通知处理程序代码
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CDESDlg::OnSelchangeComboMode()
{
	// TODO: 在此添加控件通知处理程序代码
	int mode = mCBMode.GetCurSel();
	if (mode == 0)
	{
		GetDlgItem(IDC_BUTTON_RND_VEC)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_VECTOR)->EnableWindow(false);
		GetDlgItem(IDC_CHECK_BASE64)->EnableWindow(true);
	}
	else if (mode == 2)
	{
		MessageBoxW(L"CFB模式每个分组的结果依然使用QWORD(64bit)返回，因此暂不能使用BASE64加密", L"Attention", MB_OK);
		GetDlgItem(IDC_BUTTON_RND_VEC)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_VECTOR)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_BASE64)->EnableWindow(false);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_RND_VEC)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_VECTOR)->EnableWindow(true);
		GetDlgItem(IDC_CHECK_BASE64)->EnableWindow(true);
	}
}

void CDESDlg::LoadFont()
{
	// TODO: 在此处添加实现代码.
	HINSTANCE hResInstance = AfxGetResourceHandle();
	HRSRC res = FindResource(hResInstance, MAKEINTRESOURCE(IDR_FONT_INZIU), RT_FONT);//_T("Font"));
	if (res)
	{
		HGLOBAL mem = LoadResource(hResInstance, res);
		void *data = LockResource(mem);
		DWORD len = SizeofResource(hResInstance, res);
		DWORD nFonts = 0;
		mHFont = AddFontMemResourceEx(data, len, NULL, &nFonts);
		if (!mHFont)
			AfxMessageBox(_T("字体加载失败！"));
	}
	else
	{
		AfxMessageBox(_T("字体加载失败！"));
	}
}

void CDESDlg::OnBnClickedButtonRndKey()
{
	// TODO: 在此添加控件通知处理程序代码
	if (dispWarning == true)
	{
		MessageBox(L"此程序的随机数发生器是C++11标准库梅森旋转算法引擎\r\n随机种子采用随机数生成设备random_device生成\r\n由于Windows下random_device的实现并非调用真实随机数发生器\r\n对于加密可能无法保证安全！", L"CRITICAL WARNING", MB_ICONWARNING);
		dispWarning = false;
	}
	std::random_device rd;
	std::mt19937 gen(rd());
	CString keyDisp;
	keyDisp.Format(L"%08X%08X", gen(), gen());
	GetDlgItem(IDC_EDIT_KEY)->SetWindowTextW(keyDisp);
}

void CDESDlg::OnBnClickedButtonRndVec()
{
	// TODO: 在此添加控件通知处理程序代码
	std::random_device rd;
	std::mt19937 gen(rd());
	CString keyDisp;
	keyDisp.Format(L"%08X%08X", gen(), gen());
	GetDlgItem(IDC_EDIT_VECTOR)->SetWindowTextW(keyDisp);
}

void CDESDlg::OnBnClickedButtonFile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog openFileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"所有文件(*.*)|*.*||", this, NULL);
	//openFileDlg.m_ofn.lpstrInitialDir = L"%USERPROFILE%";
	INT_PTR result = openFileDlg.DoModal();
	CString fileName;
	if (result == IDOK)
	{
		fileName = openFileDlg.GetPathName();
		fileName.Replace(L"\\", L"/");
		GetDlgItem(IDC_EDIT_INPUT)->SetWindowTextW(fileName);
	}
	SetFocus();
}

void CDESDlg::OnBnClickedButtonSave()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog openFileDlg(FALSE, L"txt", L"DES_Crypt.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, L"文本文档(*.txt)|*.txt||", AfxGetMainWnd(), NULL);
	INT_PTR result = openFileDlg.DoModal();
	CString fileName;
	CString text;
	GetDlgItem(IDC_EDIT_OUTPUT)->GetWindowTextW(text);
	if (result == IDOK)
	{
		fileName = openFileDlg.GetPathName();
		fileName.Replace(L"\\", L"/");
		CFile file;
		file.Open(fileName, CFile::modeCreate | CFile::modeWrite);

		int len = WideCharToMultiByte(CP_REGION, 0, text, -1, NULL, 0, NULL, FALSE);
		char *buf = new char[len];
		memset(buf, 0, len);
		int nRet = WideCharToMultiByte(CP_REGION, 0, text, -1, buf, len, NULL, FALSE);
		
		file.Write(buf, len-1);
		delete[]buf;
		file.Close();
	}
	SetFocus();
}

void CDESDlg::OnClickedRadioGroupAction()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (mActionMode == 1)
	{
		GetDlgItem(IDC_BUTTON_ACTION)->SetWindowTextW(L"Decrypt");
		GetDlgItem(IDC_CHECK_BASE64)->EnableWindow(false);
		mCBOutputType.EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_BUTTON_ACTION)->SetWindowTextW(L"Encrypt");
		GetDlgItem(IDC_CHECK_BASE64)->EnableWindow(true);
		((CButton*)GetDlgItem(IDC_CHECK_BASE64))->SetCheck(false);
	}
	LoadInputType();
}

void CDESDlg::LoadInputType()
{
	if (mActionMode == 0)
	{
		mCBIuputType.ResetContent();
		mCBIuputType.AddString(L"Num (Bin)");
		mCBIuputType.AddString(L"Num (Hex)");
		mCBIuputType.AddString(L"Text (UTF-8)");
		mCBIuputType.AddString(L"Text (Unicode)");
		mCBIuputType.AddString(L"File");
		mCBIuputType.SetCurSel(0);
		OnSelchangeComboInputType();
	}
	else
	{
		mCBIuputType.ResetContent();
		mCBIuputType.AddString(L"Num (Hex)");
		mCBIuputType.AddString(L"BASE64");
		mCBIuputType.SetCurSel(0);
		OnSelchangeComboInputType();
	}
}

void CDESDlg::OnClickedCheckBase64()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	if (mBBase64)
	{
		mCBOutputType.EnableWindow(false);
	}
	else
	{
		mCBOutputType.EnableWindow(true);
	}
}
