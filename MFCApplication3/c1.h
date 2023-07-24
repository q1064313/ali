#pragma once
#include "afxdialogex.h"
#include<vector>

// c1 对话框

class c1 : public CDialogEx
{
	DECLARE_DYNAMIC(c1)

public:
	c1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~c1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_mfcc1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_open;
	CButton m_send;
	CButton m_close;
	CString m_edit_recv;
	CString m_edit_send;
	void initSport();
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonClear1();
	void c1::EnumerateSerialPorts(CUIntArray& porta, CUIntArray& portse, CUIntArray& portsu);
	USHORT c1::AddCom(CComboBox* pvect);
	afx_msg void OnDestroy();
};
