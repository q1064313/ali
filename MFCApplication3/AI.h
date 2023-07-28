#pragma once
#include "afxdialogex.h"


// AI 对话框

class AI : public CDialogEx
{
	DECLARE_DYNAMIC(AI)

public:
	AI(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~AI();
	void AI::Loadxml(char* ainame,CString cs);
	void AI::getval(char* ch, CString source);
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AI };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_AI;
	virtual BOOL OnInitDialog();
};
