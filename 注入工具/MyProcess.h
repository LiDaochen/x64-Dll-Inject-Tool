#pragma once
#include "afxcmn.h"
#include <TlHelp32.h>
#include <Psapi.h>


// CMyProcess 对话框

class CMyProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CMyProcess)

public:
	CMyProcess(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMyProcess();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	CString m_exeName;

private:
	void EnumProcess();
	CBitmap m_subBmp[2];
	BOOL Is64BitOS();
	BOOL Is64BitPorcess(DWORD dwProcessID);
public:
	afx_msg void OnRefresh();
	afx_msg void OnTerminateProcess();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
