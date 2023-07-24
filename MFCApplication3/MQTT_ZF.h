#pragma once
#include "afxdialogex.h"


// MQTT_ZF 对话框

class MQTT_ZF : public CDialogEx
{
	DECLARE_DYNAMIC(MQTT_ZF)

public:
	MQTT_ZF(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MQTT_ZF();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MQTT_zf };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
