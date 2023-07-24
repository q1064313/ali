#pragma once
#include "afxdialogex.h"


// caiji 对话框

class caiji : public CDialogEx
{
	DECLARE_DYNAMIC(caiji)

public:
	caiji(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~caiji();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_caiji };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
