#pragma once
#include "afxdialogex.h"


// kong 对话框

class kong : public CDialogEx
{
	DECLARE_DYNAMIC(kong)

public:
	kong(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~kong();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
