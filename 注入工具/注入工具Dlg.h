
// 注入工具Dlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include <Psapi.h>
#include <TlHelp32.h>
#include "About.h"
#include "afxcmn.h"
#include "MyProcess.h"


// C注入工具Dlg 对话框
class C注入工具Dlg : public CDialogEx
{
// 构造
public:
	C注入工具Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DWORD EnumProcess(CString ProcessName);
	HMODULE EnumMoudle(CString ProcessName, DWORD dwPid);
	BOOL LoadDll(DWORD dwProcessId, char* szDllPathname);
	BOOL FreeDll(DWORD dwProcessId, CString szDllPathname);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_Edit2;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CEdit m_Edit1;
	CMenu m_Menu;
	CBitmap m_Bmp[2];
	afx_msg void OnAbout();
	afx_msg void OnExit();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	CListCtrl m_List;
	afx_msg void OnBnClickedButton3();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
