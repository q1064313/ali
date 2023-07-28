#pragma once
#include "afxdialogex.h"
#include"dev_basicinfo.h"
#include"AI.h"
// Devtab 对话框

class Devtab : public CDialogEx
{
	DECLARE_DYNAMIC(Devtab)

public:
	Devtab(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Devtab();
	dev_basicinfo devdlg;
	AI aidlg;
	AI accdlg;
	AI didlg;
	AI aodlg;
	AI dodlg;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Kong };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_devtab;
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangedevtab(NMHDR* pNMHDR, LRESULT* pResult);
};
