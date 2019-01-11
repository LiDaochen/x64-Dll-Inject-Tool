// MyProcess.cpp : 实现文件
//

#include "stdafx.h"
#include "注入工具.h"
#include "MyProcess.h"
#include "afxdialogex.h"


// CMyProcess 对话框

IMPLEMENT_DYNAMIC(CMyProcess, CDialogEx)

CMyProcess::CMyProcess(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyProcess::IDD, pParent)
{

}

CMyProcess::~CMyProcess()
{
}

void CMyProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}


BEGIN_MESSAGE_MAP(CMyProcess, CDialogEx)
	ON_COMMAND(ID_32773, &CMyProcess::OnRefresh)
	ON_COMMAND(ID_32774, &CMyProcess::OnTerminateProcess)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CMyProcess::OnNMRClickList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CMyProcess::OnNMDblclkList1)
END_MESSAGE_MAP()


// CMyProcess 消息处理程序

// 重写虚函数PreTranslateMessage
BOOL CMyProcess::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类

	if (pMsg->message == WM_KEYDOWN)
	{
		int nKey = (int)pMsg->wParam;
		if (nKey == VK_RETURN || nKey == VK_ESCAPE)
		{
			return true;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

// 重写虚函数OnInitDialog
BOOL CMyProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// list列表初始化
	// 设置扩展风格：正行选中
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 添加标题
	m_list.InsertColumn(0, _T("Index"), LVCFMT_LEFT, 45);
	m_list.InsertColumn(1, _T("程序名称"), LVCFMT_LEFT, 176);
	m_list.InsertColumn(3, _T("PID"), LVCFMT_LEFT, 50);

	// 右键菜单加载位图
	m_subBmp[0].LoadBitmap(IDB_REFRESH);
	m_subBmp[1].LoadBitmap(IDB_TERMINATE);

	// 调用遍历进程函数 
	EnumProcess();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

// 遍历进程的函数
void CMyProcess::EnumProcess()
{
	// 删除所有节点
	m_list.DeleteAllItems();
	// 定义一些局部变量
	PROCESSENTRY32 pe = { sizeof(PROCESSENTRY32) };
	DWORD dwIndex = 0;
	CString CIndex;
	CString CPID;
	// 创建快照
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	// 得到第一个进程的信息
	BOOL bSuccess = Process32First(hSnap, &pe);
	// 循环遍历所有进程的信息
	if (bSuccess == TRUE)
	{
		do
		{
			if (Is64BitPorcess(pe.th32ProcessID))
			{
				// 先插入一个item到列表
				m_list.InsertItem(dwIndex, L"");
				// 将索引插入到item 第0列
				CIndex.Format(_T("%d"), dwIndex);
				m_list.SetItemText(dwIndex, 0, CIndex);
				// 将进程名插入到item 第1列
				m_list.SetItemText(dwIndex, 1, pe.szExeFile);
				// 将进程PID插入到item 第2列
				CPID.Format(_T("%d"), pe.th32ProcessID);
				m_list.SetItemText(dwIndex, 2, CPID);
				++dwIndex;
			}
		} while (Process32Next(hSnap, &pe));
	}
}

// 右键菜单刷新进程
void CMyProcess::OnRefresh()
{
	// TODO:  在此添加命令处理程序代码
	EnumProcess();
}

// 右键菜单结束进程
void CMyProcess::OnTerminateProcess()
{
	// 进程ID 保存在第2列
	int nIndex = 0;
	POSITION pos = { 0 };
	// 获取被选中节点的开始位置
	pos = m_list.GetFirstSelectedItemPosition();
	TCHAR buff[MAX_PATH];
	while ((nIndex = m_list.GetNextSelectedItem(pos)) != -1)
	{
		// 获取第nIndex行的第2列的文本
		m_list.GetItemText(nIndex, 2, buff, MAX_PATH);
	}
	// 将TCHAR类型转换成DWORD
	DWORD lg;
	swscanf_s(buff, L"%d", &lg);
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, lg);
	TerminateProcess(hProcess, 0);
	EnumProcess();
}

// 修改字体颜色
HBRUSH CMyProcess::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	if (pWnd->GetDlgCtrlID() == IDC_STATIC)
	{
		pDC->SetBkColor(RGB(0, 0, 0));
		pDC->SetTextColor(RGB(0, 255, 0));
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

// 右键单击处理函数
void CMyProcess::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;

	// 定义下面要用到的cmenu对象 
	CMenu menu, *pSubMenu;
	// 装载自定义的右键菜单 
	menu.LoadMenu(IDR_MENU2);
	// 获取子菜单项
	pSubMenu = menu.GetSubMenu(0);
	// 实现右键菜单前面的图标
	pSubMenu->SetMenuItemBitmaps(0, MF_BYPOSITION, &m_subBmp[0], &m_subBmp[0]);
	pSubMenu->SetMenuItemBitmaps(1, MF_BYPOSITION, &m_subBmp[1], &m_subBmp[1]);
	// 代表没有列表项被选中，则禁用结束进程的功能
	if (pNMItemActivate->iItem == -1)
	{
		pSubMenu->EnableMenuItem(ID_32774, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	}

	// 获取第一个弹出菜单，所以第一个菜单必须有子菜单
	// 定义一个用于确定光标位置的位置 
	CPoint oPoint;
	//获取当前光标的位置，以便使得菜单可以跟随光标
	GetCursorPos(&oPoint);
	// 弹出子菜单
	pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, oPoint.x, oPoint.y, this);
}

// 列表双击事件
void CMyProcess::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码

	if (m_list.GetSelectedCount() == 1)
	{
		POSITION pos_s;
		pos_s = m_list.GetFirstSelectedItemPosition();
		int item = m_list.GetNextSelectedItem(pos_s);
		m_exeName = m_list.GetItemText(item, 1);
	}
	else
	{
		MessageBox(L"请选择程序！");
		return;
	}
	SendMessage(WM_CLOSE);

	*pResult = 0;
}

// 判断是不是64位操作系统
BOOL CMyProcess::Is64BitOS()
{
	typedef VOID(WINAPI *LPFN_GetNativeSystemInfo)(__out LPSYSTEM_INFO lpSystemInfo);
	LPFN_GetNativeSystemInfo fnGetNativeSystemInfo = (LPFN_GetNativeSystemInfo)GetProcAddress(GetModuleHandleW(L"kernel32"), "GetNativeSystemInfo");
	if (fnGetNativeSystemInfo)
	{
		SYSTEM_INFO stInfo = { 0 };
		fnGetNativeSystemInfo(&stInfo);
		if (stInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64
			|| stInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		{
			return TRUE;
		}
	}
	return FALSE;
}

// 判断是不是64位程序
BOOL CMyProcess::Is64BitPorcess(DWORD dwProcessID)
{
	if (!Is64BitOS())
	{
		return FALSE;
	}
	else
	{
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessID);
		if (hProcess)
		{
			typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
			LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandleW(L"kernel32"), "IsWow64Process");
			if (NULL != fnIsWow64Process)
			{
				BOOL bIsWow64 = FALSE;
				fnIsWow64Process(hProcess, &bIsWow64);
				CloseHandle(hProcess);
				if (bIsWow64)
				{
					return FALSE;
				}
				else
				{
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}