﻿// MQTT_ZF.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication3.h"
#include "afxdialogex.h"
#include "MQTT_ZF.h"


// MQTT_ZF 对话框

IMPLEMENT_DYNAMIC(MQTT_ZF, CDialogEx)

MQTT_ZF::MQTT_ZF(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MQTT_zf, pParent)
{

}

MQTT_ZF::~MQTT_ZF()
{
}

void MQTT_ZF::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_mqtt, m_mqttab);
}


BEGIN_MESSAGE_MAP(MQTT_ZF, CDialogEx)
END_MESSAGE_MAP()


// MQTT_ZF 消息处理程序


BOOL MQTT_ZF::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_mqttab.InsertItem(0, _T("MQTT平台")); //添加参数一选项卡 
	m_mqttab.InsertItem(1, _T("筛选点")); //添加参数二选项卡

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
