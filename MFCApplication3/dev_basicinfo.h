#pragma once
#include "afxdialogex.h"


// dev_basicinfo 对话框

class dev_basicinfo : public CDialogEx
{
	DECLARE_DYNAMIC(dev_basicinfo)

public:
	dev_basicinfo(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~dev_basicinfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_basicinfo };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
