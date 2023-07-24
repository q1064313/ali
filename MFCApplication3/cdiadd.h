#pragma once
#include "afxdialogex.h"


// cdiadd 对话框

class cdiadd : public CDialogEx
{
	DECLARE_DYNAMIC(cdiadd)

public:
	cdiadd(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~cdiadd();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEditadd();
//	CString m_add;
	CEdit m_add;
	afx_msg void OnBnClickedButtoncancel();
};
