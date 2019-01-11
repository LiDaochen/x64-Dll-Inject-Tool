
// 注入工具Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "注入工具.h"
#include "注入工具Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// C注入工具Dlg 对话框



C注入工具Dlg::C注入工具Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(C注入工具Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void C注入工具Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_Edit2);
	DDX_Control(pDX, IDC_EDIT1, m_Edit1);
	DDX_Control(pDX, IDC_LIST5, m_List);
}

BEGIN_MESSAGE_MAP(C注入工具Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &C注入工具Dlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &C注入工具Dlg::OnBnClickedButton2)
	ON_COMMAND(ID_32772, &C注入工具Dlg::OnAbout)
	ON_COMMAND(ID_32771, &C注入工具Dlg::OnExit)
	ON_BN_CLICKED(IDC_BUTTON4, &C注入工具Dlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &C注入工具Dlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON3, &C注入工具Dlg::OnBnClickedButton3)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// C注入工具Dlg 消息处理程序

//************************************************************
// 函数名称:	OnInitDialog
// 函数说明: 重写OnInitDialog函数，数据初始化函数
// 作	者: 李道臣
// 时	间: 2018/12/20
//************************************************************
BOOL C注入工具Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	// 加载菜单
	m_Menu.LoadMenuW(IDR_MENU1);
	// 实现主菜单的子菜单前面的图标
	m_Bmp[0].LoadBitmap(IDB_EXIT);
	m_Bmp[1].LoadBitmap(IDB_ABOUT);
	GetMenu()->GetSubMenu(0)->SetMenuItemBitmaps(0, MF_BYPOSITION, &m_Bmp[0], &m_Bmp[0]);
	GetMenu()->GetSubMenu(1)->SetMenuItemBitmaps(0, MF_BYPOSITION, &m_Bmp[1], &m_Bmp[1]);

	// list列表初始化
	// 设置扩展风格：正行选中
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 添加标题
	m_List.InsertColumn(0, _T("模块名称"), LVCFMT_LEFT, 150);
	m_List.InsertColumn(1, _T("加载基址"), LVCFMT_LEFT, 120);
	m_List.InsertColumn(3, _T("模块路径"), LVCFMT_LEFT, 350);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void C注入工具Dlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR C注入工具Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//************************************************************
// 函数名称:	PreTranslateMessage
// 函数说明: 重写PreTranslateMessage函数，让程序主窗口的Enter键和Esc键失效
// 作	者: 李道臣
// 时	间: 2018/12/20
//************************************************************
BOOL C注入工具Dlg::PreTranslateMessage(MSG* PMsg)
{
	if (PMsg->message == WM_KEYDOWN)
	{
		int nKey = (int)PMsg->wParam;
		if (nKey == VK_RETURN || nKey == VK_ESCAPE)
		{
			return true;
		}
	}
	return CDialog::PreTranslateMessage(PMsg);
}

//************************************************************
// 函数名称:	EnumProcess
// 函数说明: 根据进程名称获取进程PID
// 作	者: 李道臣
// 时	间: 2018/12/20
//************************************************************
DWORD C注入工具Dlg::EnumProcess(CString ProcessName)
{
	// 遍历进程
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	// 创建快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// 得到第一个进程的信息
	BOOL bSuccess = Process32First(hSnap, &pe);
	DWORD dwIndex = 0;
	CString str;
	// 循环遍历所有进程的信息
	if (bSuccess)
	{
		do
		{
			if (wcscmp(pe.szExeFile, ProcessName) == 0)
			{
				return pe.th32ProcessID;
			}
			// dwIndex加1
			++dwIndex;
		} while (Process32Next(hSnap, &pe));
	}
	return 0;
}

//************************************************************
// 函数名称:	LoadDll
// 函数说明: 根据进程PID和dll路径，远程线程注入DLL
// 作	者: 李道臣
// 时	间: 2018/12/20
//************************************************************
// 加载DLL函数
BOOL C注入工具Dlg::LoadDll(DWORD dwProcessId, char* szDllPathname)
{
	BOOL bRet;
	HANDLE hProcess;
	HANDLE hThread;
	size_t dwLength;
	DWORD dwLoadAddr;
	LPVOID lpAllocAddr;
	//HMODULE hMoudule;

	bRet = 0;
	dwLoadAddr = 0;
	// 1:获取进程句柄
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	// 2:计算DLL路径名字长度，并且要加上0结尾的长度
	dwLength = strlen(szDllPathname);
	// 3:在目标进程分配内存
	lpAllocAddr = VirtualAllocEx(hProcess, NULL, dwLength, MEM_COMMIT, PAGE_READWRITE);
	// 4:拷贝DLL路径名字到目标进程的内存
	bRet = WriteProcessMemory(hProcess, lpAllocAddr, szDllPathname, dwLength, NULL);
	// 5: 创建远程线程，加载DLL
	hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, lpAllocAddr, 0, NULL);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	return TRUE;
}

//************************************************************
// 函数名称: EnumMoule
// 函数说明: 根据进程PID和dll路径，获取模块的基址
// 作	者: 李道臣
// 时	间: 2018/12/20
//************************************************************
HMODULE C注入工具Dlg::EnumMoudle(CString ProcessName, DWORD dwPid)
{
	/*开始遍历模块*/
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };
	// 创建一个模块相关的快照句柄
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPid);
	Module32First(hModuleSnap, &me32);
	// 循环获取模块信息
	do
	{
		if (wcscmp(ProcessName, me32.szExePath) == 0)
		{
			return me32.hModule;
		}
	} while (Module32Next(hModuleSnap, &me32));
	CloseHandle(hModuleSnap);
	MessageBox(L"未找到注入DLL！");
	return 0;
}

//************************************************************
// 函数名称:	FreeDll
// 函数说明: 根据进程PID和dll路径，远程线程卸载DLL
// 作	者: 李道臣
// 时	间: 2018/12/20
//************************************************************
BOOL C注入工具Dlg::FreeDll(DWORD dwProcessId, CString szDllPathname)
{
	BOOL bRet;
	HANDLE hProcess;
	HANDLE hThread;
	size_t dwFreeAddr;
	HMODULE hMoudule;
	HMODULE hMouduleInjectDll;
	bRet = 0;
	dwFreeAddr = 0;
	// 1:获取进程句柄
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
	// 2:获取模块句柄
	hMouduleInjectDll = EnumMoudle(szDllPathname, dwProcessId);
	if (hMouduleInjectDll)
	{
		// 3:获取模块地址
		hMoudule = GetModuleHandle(L"kernel32.dll");
		// 4:获取LoadLibraryA 函数地址
		dwFreeAddr = (size_t)GetProcAddress(hMoudule, "FreeLibrary");
		// 5: 创建远程线程，加载DLL
		size_t nNum = (size_t)hMouduleInjectDll;
		hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)dwFreeAddr, (LPVOID)nNum, 0, NULL);
		if (hThread)
		{
			MessageBox(L"卸载成功!");
		}
		CloseHandle(hThread);
		CloseHandle(hProcess);
		return TRUE;
	}
	return FALSE;
}

//************************************************************
// 函数名称:	OnBnClickedButton1
// 函数说明: 选择DLL，并将路径设置到编辑框控件
// 作	者: 李道臣
// 时	间: 2018/12/20
//************************************************************
void C注入工具Dlg::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码
	// 1、打开一个文件夹选择对话框
	CFileDialog dlg(TRUE);
	if (dlg.DoModal() == IDOK)
	{
		CString path = dlg.GetFolderPath();
		// 2、获取选择的文件的后缀名，判断是否是DLL
		CString path1 = dlg.GetFileName();
		PCTSTR FileType = PathFindExtension(path1);
		if (wcscmp(FileType, L".dll") != 0)
		{
			MessageBox(L"您选择的文件有误，请重新选择！");
			return;
		}
		// 3、将文件路径显示到编辑框
		m_Edit2.SetWindowText(path + L"\\" + path1);
	}
}

//************************************************************
// 函数名称:	OnBnClickedButton2
// 函数说明: 实现一键注入DLL
// 作	者: 李道臣
// 时	间: 2018/12/20
//************************************************************
void C注入工具Dlg::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	// 1、获取要注入的程序名称
	CString ProcessName;
	m_Edit1.GetWindowTextW(ProcessName);
	// 2、获取要注入的程序名称的PID
	DWORD PID = EnumProcess(ProcessName);
	if (PID == 0)
	{
		MessageBox(L"您选择的程序有误，请重新选择！");
		return;
	}
	// 3、获取要注入的DLL的路径
	CString DllPath;
	m_Edit2.GetWindowTextW(DllPath);
	if (wcscmp(DllPath, L"点击按钮选择您要注入/卸载的DLL...") == 0 || wcscmp(DllPath, L"") == 0)
	{
		MessageBox(L"请选择DLL！");
		return;
	}
	// 4、char* 类型的DLL 路径
	USES_CONVERSION;
	char* CharDllPath = W2A(DllPath);
	// 5、注入DLL
	LoadDll(PID, CharDllPath);
}

//************************************************************
// 函数名称:	OnAbout
// 函数说明: 菜单关于按钮事件处理函数
// 作	者: 李道臣
// 时	间: 2018/12/20
//************************************************************
void C注入工具Dlg::OnAbout()
{
	// TODO:  在此添加命令处理程序代码
	CAbout dlg(this);
	dlg.DoModal();
}

//************************************************************
// 函数名称:	OnExit
// 函数说明: 菜单退出按钮事件处理函数
// 作	者: 李道臣
// 时	间: 2018/12/20
//************************************************************
void C注入工具Dlg::OnExit()
{
	// TODO:  在此添加命令处理程序代码
	SendMessage(WM_CLOSE);
}

//************************************************************
// 函数名称:	OnBnClickedButton4
// 函数说明: 实现一键卸载DLL
// 作	者: 李道臣
// 时	间: 2018/12/20
//************************************************************
void C注入工具Dlg::OnBnClickedButton4()
{
	// TODO:  在此添加控件通知处理程序代码
	// 1、获取要卸载的程序名称
	CString ProcessName;
	m_Edit1.GetWindowTextW(ProcessName);
	// 2、获取要卸载的程序名称的PID
	DWORD PID = EnumProcess(ProcessName);
	if (PID == 0)
	{
		MessageBox(L"您选择的程序有误，请重新选择！");
		return;
	}
	// 3、获取要卸载的DLL的路径
	CString DllPath;
	m_Edit2.GetWindowTextW(DllPath);
	if (wcscmp(DllPath, L"点击按钮选择您要注入/卸载的DLL...") == 0 || wcscmp(DllPath, L"") == 0)
	{
		MessageBox(L"请选择DLL！");
		return;
	}
	// 4、卸载DLL
	FreeDll(PID, DllPath);
}

//************************************************************
// 函数名称:	OnBnClickedButton5
// 函数说明: 查看模块
// 作	者: 李道臣
// 时	间: 2018/12/20
//************************************************************
void C注入工具Dlg::OnBnClickedButton5()
{
	// 1.清空列表
	m_List.DeleteAllItems();
	// 2.初始化数据
	DWORD nIndex = 0;
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32 = { sizeof(MODULEENTRY32) };

	// 3.获取进程PID
	// 3.1 获取要卸载的程序名称
	CString ProcessName;
	m_Edit1.GetWindowTextW(ProcessName);
	// 3.2 获取要卸载的程序名称的PID
	DWORD PID = EnumProcess(ProcessName);
	if (PID == 0)
	{
		MessageBox(L"您选择的程序有误，请重新选择！");
		return;
	}
	// 4.创建一个模块相关的快照句柄
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, PID);
	Module32First(hModuleSnap, &me32);
	// 5.循环获取模块信息
	do
	{
		// 5.1 先插入一个item 到列表
		m_List.InsertItem(nIndex, L"");
		// 5.2 将模块名插入到item 第0列
		m_List.SetItemText(nIndex, 0, me32.szModule);
		// 5.3 将模块加载基址插入到item 第1列
		size_t nNum = (size_t)me32.hModule;  // 将HMOUDLE转换为size_t
		CString strMoudle2;
		strMoudle2.Format(L"0x%llx", nNum);
		m_List.SetItemText(nIndex, 1, strMoudle2);
		// 5.4 将模块路径插入到item第2列
		m_List.SetItemText(nIndex, 2, me32.szExePath);
		nIndex++;
	} while (Module32Next(hModuleSnap, &me32));
	CloseHandle(hModuleSnap);
}

// 选择程序按钮处理函数
void C注入工具Dlg::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码

	CMyProcess dlg(this);
	dlg.DoModal();
	CString strExeName;
	strExeName = dlg.m_exeName;
	GetDlgItem(IDC_EDIT1)->SetWindowText(strExeName);
}

// 改变编辑框颜色
HBRUSH C注入工具Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	if (pWnd->GetDlgCtrlID() == IDC_EDIT1)
	{
		pDC->SetTextColor(RGB(136, 23, 152));
	}
	else if (pWnd->GetDlgCtrlID() == IDC_EDIT2)
	{
		pDC->SetTextColor(RGB(136, 23, 152));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
