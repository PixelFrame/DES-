
// DES++Dlg.cpp: 实现文件
//

#include "stdafx.h"
#include "DES++.h"
#include "DES++Dlg.h"
#include "afxdialogex.h"

#include "DES_Convert.h"
#include "DES_Main.h"

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
}

BEGIN_MESSAGE_MAP(CDESDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ENCRPT, &CDESDlg::OnBnClickedButtonEncrpt)
	ON_CBN_SELCHANGE(IDC_COMBO_INPUT_TYPE, &CDESDlg::OnSelchangeComboInputType)
	ON_EN_CHANGE(IDC_EDIT_INPUT, &CDESDlg::OnChangeEditInput)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CDESDlg::OnBnClickedButtonAbout)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, &CDESDlg::OnSelchangeComboMode)
	ON_BN_CLICKED(IDC_BUTTON_RND_KEY, &CDESDlg::OnBnClickedButtonRndKey)
	ON_BN_CLICKED(IDC_BUTTON_RND_VEC, &CDESDlg::OnBnClickedButtonRndVec)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CDESDlg::OnBnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CDESDlg::OnBnClickedButtonSave)
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
	mCBIuputType.AddString(L"Text (ANSI)");
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



void CDESDlg::OnBnClickedButtonEncrpt()
{
	// TODO: 在此添加控件通知处理程序代码
	CString input, key, vector;
	GetDlgItem(IDC_EDIT_INPUT)->GetWindowTextW(input);
	GetDlgItem(IDC_EDIT_KEY)->GetWindowTextW(key);
	GetDlgItem(IDC_EDIT_VECTOR)->GetWindowTextW(vector);
	int mode = mCBMode.GetCurSel();

	std::vector<QWORD> cipher;
	switch(index) 
	{
	case 0:
		if (input.GetLength() % 64 != 0)
		{
			MessageBoxExW(NULL, L"Input Will Be Lengthened to Multiples of 64 with \'0\'", L"Attention", MB_OK, 0);
		}
		cipher = DES_Main::encryptBin(input, DES_Convert::CSW2HEX(key), mode, DES_Convert::CSW2HEX(vector));
		break;
	case 1:
		if (input.GetLength() % 16 != 0)
		{
			MessageBoxExW(NULL, L"Input Will Be Lengthened to Multiples of 16 with \'0\'", L"Attention", MB_OK, 0);
		}
		cipher = DES_Main::encryptHex(input, DES_Convert::CSW2HEX(key), mode, DES_Convert::CSW2HEX(vector));
		break;
	case 2:
		if (input.GetLength() % 8 != 0)
		{
			MessageBoxExW(NULL, L"Input Will Be Lengthened to Multiples of 8 with \'0\'", L"Attention", MB_OK, 0);
		}
		cipher = DES_Main::encryptANSI(input, DES_Convert::CSW2HEX(key), mode, DES_Convert::CSW2HEX(vector));
		break;
	case 3:
		if (input.GetLength() % 4 != 0)
		{
			MessageBoxExW(NULL, L"Input Will Be Lengthened to Multiples of 4 with \'0\'", L"Attention", MB_OK, 0);
		}
		cipher = DES_Main::encryptUnicode(input, DES_Convert::CSW2HEX(key), mode, DES_Convert::CSW2HEX(vector));
		break;
	case 4:
		cipher = DES_Main::encryptFile(input, DES_Convert::CSW2HEX(key), mode, DES_Convert::CSW2HEX(vector));
		break;
	}
	
	CString output;
	for (std::vector<QWORD>::const_iterator iter = cipher.cbegin(); iter != cipher.cend(); ++iter)
	{
		output.AppendFormat(L"Part %d:\r\n%08X", iter - cipher.cbegin(), *iter >> 32);
		output.AppendFormat(L"%08X\r\n", *iter);
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

	if (index == 2)
	{
		GetDlgItem(IDC_CHECK_BASE64)->EnableWindow(true);
	}
	else
	{
		GetDlgItem(IDC_CHECK_BASE64)->EnableWindow(false);
	}

	CString info;
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
		info.Append(L"输入视为ASCII码，每8位一次分割（CBF模式下1位），除ECB模式外可指定初始向量IV\r\n框选BASE64将转换为BASE64编码后加密\r\n\r\n！！！！！！！！！！！！！！！！！\r\n！勿输入中文，可能导致未知的结果！\r\n！！！！！！！！！！！！！！！！！");
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
	GetDlgItem(IDC_EDIT_INPUT)->SetWindowTextW(L"");
	mETInput.SetType(index);
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
	}
	else
	{

		GetDlgItem(IDC_BUTTON_RND_VEC)->EnableWindow(true);
		GetDlgItem(IDC_EDIT_VECTOR)->EnableWindow(true);
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
		const int UNICODE_TXT_FLG = 0xFEFF;
		fileName = openFileDlg.GetPathName();
		fileName.Replace(L"\\", L"/");
		CFile file;
		file.Open(fileName, CFile::modeCreate | CFile::modeWrite);
		file.Write(&UNICODE_TXT_FLG, 2);
		file.Write(text, text.GetLength() * 2);
	}
	SetFocus();
}
