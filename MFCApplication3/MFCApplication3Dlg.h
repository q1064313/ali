
// MFCApplication3Dlg.h: 头文件
//
#include "c1.h"
#include"kong.h"
#include "MQTT_zf.h"
#include "caiji.h"
#pragma once


// CMFCApplication3Dlg 对话框
class CMFCApplication3Dlg : public CDialogEx
{
// 构造
public:
	CDialogEx* pChildDialog;
	CMFCApplication3Dlg(CWnd* pParent = nullptr);	// 标准构造函数
	CMenu m_menu;
	CMenu menu2;
	c1* pDlg;
	MQTT_ZF* mqttdlg;
	caiji* caijidlg;

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION3_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	CTreeCtrl m_tree;
	CString g_sSelectStr;

	HTREEITEM CMFCApplication3Dlg::FindItem(HTREEITEM item, CString strText);
	BOOL CMFCApplication3Dlg::PreTranslateMessage(MSG* pMsg);
	afx_msg void OnAdd();
	afx_msg void OnDelete();

	afx_msg void OnNMRClickTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnBeginlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnEndlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void add_com();
	afx_msg void delete_com();
	afx_msg void aboutinfo();
	CStatic m_statuchild;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSaveXml();
	void CMFCApplication3Dlg::LoadXml();
	void CMFCApplication3Dlg::LoadXmlchange();
};
