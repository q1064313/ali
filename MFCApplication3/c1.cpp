﻿// c1.cpp: 实现文件
//
#include "pch.h"
#include "MFCApplication3.h"
#include "afxdialogex.h"
#include "c1.h"
#include<string>
#include <tchar.h>
#include <winspool.h>
#include <vector>
#include <sys/types.h>
#define MAX_SCAN_CNT 16
#define SWAPBYTE(a)  (((a & 0x00ff) << 8) | ((a & 0xff00) >> 8))
#define SWAPINT(a)   (((a & 0x000000ff) << 24) | ((a & 0x0000ff00) << 8) | ((a & 0xff000000) >> 24) | ((a & 0x00ff0000) >> 8) )

CUIntArray ports;	//所有存在的串口
CUIntArray portse;	//可用的串口
CUIntArray portsu;	//已占用的串口
int close_flag = 0;
extern CString m_csbaud;
extern CString m_csds;
extern CString m_cssp;
extern CString m_csparity;

HANDLE hthread = NULL;
HANDLE hCom = INVALID_HANDLE_VALUE;

DWORD WINAPI ThreadProc1(LPVOID lpParam);

static uint16_t CRC16_TABLE[] =
{
	0x0000,0xC0C1,0xC181,0x0140,0xC301,0x03C0,0x0280,0xC241,
	0xC601,0x06C0,0x0780,0xC741,0x0500,0xC5C1,0xC481,0x0440,
	0xCC01,0x0CC0,0x0D80,0xCD41,0x0F00,0xCFC1,0xCE81,0X0E40,
	0x0A00,0xCAC1,0xCB81,0x0B40,0xC901,0x09C0,0x0880,0xC841,
	0xD801,0x18C0,0x1980,0xD941,0x1B00,0xDBC1,0xDA81,0x1A40,
	0x1E00,0xDEC1,0xDF81,0x1F40,0xDD01,0x1DC0,0x1C80,0xDC41,
	0x1400,0xD4C1,0xD581,0x1540,0xD701,0x17C0,0x1680,0xD641,
	0xD201,0x12C0,0x1380,0xD341,0x1100,0xD1C1,0xD081,0x1040,
	0xF001,0x30C0,0x3180,0xF141,0x3300,0xF3C1,0xF281,0x3240,
	0x3600,0xF6C1,0xF781,0x3740,0xF501,0x35C0,0x3480,0xF441,
	0x3C00,0xFCC1,0xFD81,0x3D40,0xFF01,0x3FC0,0x3E80,0xFE41,
	0xFA01,0x3AC0,0x3B80,0xFB41,0x3900,0xF9C1,0xF881,0x3840,
	0x2800,0xE8C1,0xE981,0x2940,0xEB01,0x2BC0,0x2A80,0xEA41,
	0xEE01,0x2EC0,0x2F80,0xEF41,0x2D00,0xEDC1,0xEC81,0x2C40,
	0xE401,0x24C0,0x2580,0xE541,0x2700,0xE7C1,0xE681,0x2640,
	0x2200,0xE2C1,0xE381,0x2340,0xE101,0x21C0,0x2080,0xE041,
	0xA001,0x60C0,0x6180,0xA141,0x6300,0xA3C1,0xA281,0x6240,
	0x6600,0xA6C1,0xA781,0x6740,0xA501,0x65C0,0x6480,0xA441,
	0x6C00,0xACC1,0xAD81,0x6D40,0xAF01,0x6FC0,0x6E80,0xAE41,
	0xAA01,0x6AC0,0x6B80,0xAB41,0x6900,0xA9C1,0xA881,0x6840,
	0x7800,0xB8C1,0xB981,0x7940,0xBB01,0x7BC0,0x7A80,0xBA41,
	0xBE01,0x7EC0,0x7F80,0xBF41,0x7D00,0xBDC1,0xBC81,0x7C40,
	0xB401,0x74C0,0x7580,0xB541,0x7700,0xB7C1,0xB681,0x7640,
	0x7200,0xB2C1,0xB381,0x7340,0xB101,0x71C0,0x7080,0xB041,
	0x5000,0x90C1,0x9181,0x5140,0x9301,0x53C0,0x5280,0x9241,
	0x9601,0x56C0,0x5780,0x9741,0x5500,0x95C1,0x9481,0x5440,
	0x9C01,0x5CC0,0x5D80,0x9D41,0x5F00,0x9FC1,0x9E81,0x5E40,
	0X5A00,0x9AC1,0x9B81,0x5B40,0x9901,0x59C0,0x5880,0x9841,
	0x8801,0x48C0,0x4980,0x8941,0x4B00,0x8BC1,0x8A81,0x4A40,
	0x4E00,0x8EC1,0x8F81,0x4F40,0x8D01,0x4DC0,0x4C80,0x8C41,
	0x4400,0x84C1,0x8581,0x4540,0x8701,0x47C0,0x4680,0x8641,
	0x8201,0x42C0,0x4380,0x8341,0x4100,0x81C1,0x8081,0x4040,
};

typedef struct fucReadREG03H
{
	
	uint8_t     SlaveAddr;      // 从机地址
	uint8_t     Cmd;            // 03H 读开关量
	uint16_t    wRegAddr;        // 寄存器地址
	uint16_t    wRegNum;         // 寄存器个数

	int32_t GetPacketSize()
	{
		return sizeof(fucReadREG03H);
	}

} *Read03H;

uint16_t CalcCRC16(uint8_t* pchData, uint32_t dwLength, uint16_t wCrc, uint32_t CrcType)
{
	uint32_t i;
	uint16_t temp;
	for (i = 0; i < dwLength; i++)
	{
		wCrc = CRC16_TABLE[(wCrc & 0xff) ^ pchData[i]] ^ (wCrc >> 8);
	}
	if (CrcType == 1)
	{
		temp = ((wCrc & 0xff) << 8) | ((wCrc >> 8) & 0xff);
		wCrc = temp;
	}
	return wCrc;
}


int32_t Packet03H(uint8_t* pBuff, uint8_t Address, uint16_t wRegAddr, uint16_t wRegNum, uint32_t CrcType)
{
	uint16_t* pCrc;
	Read03H  pReadReg;

	pReadReg = (Read03H)pBuff;

	pReadReg->SlaveAddr = Address;
	pReadReg->Cmd = 0x03;
	pReadReg->wRegAddr = SWAPBYTE(wRegAddr);
	pReadReg->wRegNum = SWAPBYTE(wRegNum);

	pCrc = (uint16_t*)(pBuff + pReadReg->GetPacketSize());
	*pCrc = CalcCRC16(&pReadReg->SlaveAddr, pReadReg->GetPacketSize(), 0xffff, CrcType);

	return pReadReg->GetPacketSize() + sizeof(uint16_t);
}



// c1 对话框


IMPLEMENT_DYNAMIC(c1, CDialogEx)

c1::c1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_mfcc1, pParent)
	, m_edit_recv(_T(""))
	, m_edit_send(_T(""))
	
{

}

c1::~c1()
{
}

void c1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_OPEN, m_open);
	DDX_Control(pDX, IDC_BUTTON_SEND, m_send);
	DDX_Control(pDX, IDC_BUTTON_CLOSE, m_close);
	DDX_Text(pDX, IDC_EDIT_RECEIVE, m_edit_recv);
	DDX_Text(pDX, IDC_EDIT_SEND, m_edit_send);
	//  DDX_CBString(pDX, IDC_COMBO_BAUDRATE, m_c_baud);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_c_baud);
	//  DDX_Control(pDX, IDC_COMBO_BITS, m);
	DDX_Control(pDX, IDC_COMBO_BITS, m_c_bit);
	DDX_Control(pDX, IDC_COMBO_STOPBITS, m_c_stp);
	DDX_Control(pDX, IDC_COMBO_CHECK, m_c_parity);
}


BEGIN_MESSAGE_MAP(c1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &c1::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &c1::OnBnClickedButtonOpen)
	ON_CBN_SELCHANGE(IDC_COMBO_CHECK, &c1::OnCbnSelchangeComboCheck)
	ON_CBN_SELCHANGE(IDC_COMBO_STOPBITS, &c1::OnCbnSelchangeComboStopbits)
	ON_CBN_SELCHANGE(IDC_COMBO_BITS, &c1::OnCbnSelchangeComboBits)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &c1::OnCbnSelchangeComboBaudrate)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &c1::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &c1::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR1, &c1::OnBnClickedButtonClear1)
	ON_COMMAND(ID_32774, &c1::Onsave)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// c1 消息处理程序


BOOL c1::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
//	CComboBox* pRecvStrNum = (CComboBox*)GetDlgItem(IDC_COMBO_COM);
//	pRecvStrNum->SetCurSel(0);
	
	///////////////////////////////////////////////////////////////
	CComboBox* pRecvStrNum = (CComboBox*)GetDlgItem(IDC_COMBO_COM);
	unsigned short com_cnt;
	while (pRecvStrNum->DeleteString(0) >= 0);
	com_cnt = AddCom(pRecvStrNum);
	if (com_cnt)
	{
		pRecvStrNum->DeleteString(com_cnt + 1);
		pRecvStrNum->SetCurSel(0);
	}
	//////////////////////////////////////////////////////////////////获取可用串口
	
	CComboBox* pRecvStrRate = (CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE);
	pRecvStrRate->SetCurSel(0);
	CComboBox* pRecvStrBit = (CComboBox*)GetDlgItem(IDC_COMBO_BITS);
	pRecvStrBit->SetCurSel(0);

	CComboBox* pRecvStrSbit = (CComboBox*)GetDlgItem(IDC_COMBO_STOPBITS);
	pRecvStrSbit->SetCurSel(0);

	CComboBox* pRecvStrCheck = (CComboBox*)GetDlgItem(IDC_COMBO_CHECK);
	pRecvStrCheck->SetCurSel(0);
	if (m_csbaud.IsEmpty() || m_csds.IsEmpty() || m_csparity.IsEmpty() || m_cssp.IsEmpty()) {
		m_c_baud.GetWindowText(m_csbaud);
		m_c_bit.GetWindowText(m_csds);
		m_c_stp.GetWindowText(m_cssp);
		m_c_parity.GetWindowText(m_csparity);
	}
	else
	{
		pRecvStrRate->SelectString(0, m_csbaud);
		pRecvStrBit->SelectString(0, m_csds);
		pRecvStrSbit->SelectString(0, m_cssp);
		pRecvStrCheck->SelectString(0, m_csparity);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void c1::initSport()
{
	// TODO: 在此处添加实现代码.
	CString strtemp = NULL;
	char a[1024] = {0};
	CComboBox* precvStrNum = (CComboBox*)GetDlgItem(IDC_COMBO_COM);
	precvStrNum->GetWindowText(strtemp);
	hCom = CreateFile(strtemp, GENERIC_WRITE | GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hCom == INVALID_HANDLE_VALUE) {
		MessageBox(_T("打开串口失败"));
	}
	else {
		MessageBox(_T("打开串口成功"));
	}
	SetupComm(hCom, 100, 100);
	COMMTIMEOUTS Timeouts;
	Timeouts.ReadIntervalTimeout = 50;
	Timeouts.ReadTotalTimeoutMultiplier = 50;
	Timeouts.ReadTotalTimeoutConstant = 50;
	Timeouts.WriteTotalTimeoutMultiplier = 100;
	Timeouts.WriteTotalTimeoutConstant = 50;
	SetCommTimeouts(hCom, &Timeouts);
	
	DCB dcb;
	GetCommState(hCom, &dcb);
	CComboBox* precvStrRate = (CComboBox*)GetDlgItem(IDC_COMBO_BAUDRATE);
	precvStrRate->GetWindowText(strtemp);
	for (int i = 0; i <= strtemp.GetLength(); i++) {
		a[i] = strtemp[i];
	}
	dcb.BaudRate = atoi(a);

	CComboBox* precvStrBit = (CComboBox*)GetDlgItem(IDC_COMBO_BITS);
	precvStrBit->GetWindowText(strtemp);
	for (int i = 0; i <= strtemp.GetLength(); i++) {
		a[i] = strtemp[i];
	}
	dcb.ByteSize = atoi(a);

	CComboBox* precvStrSbit = (CComboBox*)GetDlgItem(IDC_COMBO_STOPBITS);
	precvStrSbit->GetWindowText(strtemp);
	for (int i = 0; i <= strtemp.GetLength(); i++) {
		a[i] = strtemp[i]; 

	}
	if (0 == strcmp(a, "1")) {
		dcb.StopBits = ONESTOPBIT;
	}
	if (0 == strcmp(a, "2")) {
		dcb.StopBits = TWOSTOPBITS;
	}

	CComboBox* precvStrCheck = (CComboBox*)GetDlgItem(IDC_COMBO_CHECK);
	precvStrCheck->GetWindowText(strtemp);
	for (int i = 0; i <= strtemp.GetLength(); i++) {
		a[i] = strtemp[i];
		

	}
	if (0 == strcmp(a, "none")) {
		dcb.Parity = NOPARITY;
	}
	if (0 == strcmp(a, "odd")) {
		dcb.Parity = ODDPARITY;
	}
	if (0 == strcmp(a, "even")) {
		dcb.Parity = EVENPARITY;
	}
	SetCommState(hCom, &dcb);
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
}


void c1::OnBnClickedButtonSend()
{
	// TODO: 在此添加控件通知处理程序代码
	if (hCom == NULL) {
		AfxMessageBox(_T("串口没有打开"));
		return;
	}
	CEdit* pRecvStrEdit = (CEdit*)GetDlgItem(IDC_EDIT_SEND);
	pRecvStrEdit->GetWindowText(m_edit_send);


	int sendSize = m_edit_send.GetLength();

	char* lpOutBuffer = new char[1024] {0};

	for (int i = 0; i <= sendSize; i++) {
		lpOutBuffer[i] = m_edit_send[i];
	}
	
	int lenl = strlen(lpOutBuffer);
	DWORD dwbyteswrite = 0;
	COMSTAT Comstat;
	DWORD dwErrorFlags = 0;

	BOOL bwritestat;
	TerminateThread(hthread, 0);
	ClearCommError(hCom, &dwErrorFlags, &Comstat);
	bwritestat = WriteFile(hCom, lpOutBuffer, lenl, &dwbyteswrite, NULL);
	hthread = CreateThread(NULL, 0, ThreadProc1, this, 0, NULL);
	if (!bwritestat) {
		MessageBox(_T("发送失败"));
	}
	delete[] lpOutBuffer;
}


void c1::OnBnClickedButtonOpen()
{
	// TODO: 在此添加控件通知处理程序代码

	initSport();
	if (hCom == INVALID_HANDLE_VALUE) {
		return;
	}
//	m_c_baud.GetWindowText(m_csbaud);
//	m_c_bit.GetWindowText(m_csds);
//	m_c_stp.GetWindowText(m_cssp);
//	m_c_parity.GetWindowText(m_csparity);



	hthread = CreateThread(NULL, 0, ThreadProc1, (LPVOID)(this), 0, NULL);
	m_send.EnableWindow(1);
	m_close.EnableWindow(1);
	m_open.EnableWindow(0);
	close_flag = 0;
	if (hthread == NULL) {
		MessageBox(_T("读取失败！"));
	}
}






void c1::OnBnClickedButtonClose()
{
	// TODO: 在此添加控件通知处理程序代码
	if (hCom != INVALID_HANDLE_VALUE) {
		TerminateThread(hthread, 0);
		Sleep(5);
		CloseHandle(hCom);
		close_flag = 1;
		m_send.EnableWindow(0);
		m_close.EnableWindow(0);
		m_open.EnableWindow(1);
		MessageBox(_T("串口已关闭"));

	}
}




void c1::OnBnClickedButtonClear()
{
	CEdit* precvStrEdit = (CEdit*)GetDlgItem(IDC_EDIT_RECEIVE);
	precvStrEdit->SetWindowText(_T(""));
	
	// TODO: 在此添加控件通知处理程序代码
}


void c1::OnBnClickedButtonClear1()
{
	// TODO: 在此添加控件通知处理程序代码
	CEdit* precvStrEdit = (CEdit*)GetDlgItem(IDC_EDIT_SEND);
	precvStrEdit->SetWindowText(_T(""));
}

DWORD WINAPI ThreadProc1(LPVOID lpParam) {
	DWORD temp = 0;
	while (true) {
		char str[100] = {0};
		DWORD wcount = 100;
		BOOL breadstat;
		c1* cr = (c1*)lpParam;
		memset(str, 0, sizeof(str));

		breadstat = ReadFile(hCom, str, wcount, &wcount, NULL);
		if (!breadstat) {
			AfxMessageBox(_T("读取串口失败"));
			exit(0);
		}

		PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

		if (strlen(str) != 0) {
			cr->m_edit_recv = str;

			CEdit* precvStrEdit = (CEdit*)cr->GetDlgItem(IDC_EDIT_RECEIVE);
			precvStrEdit->SetSel(-1, -1);
			precvStrEdit->ReplaceSel(cr->m_edit_recv);
		}
		Sleep(5);

	}
	return 1;

}
void c1::EnumerateSerialPorts(CUIntArray& porta, CUIntArray& portse, CUIntArray& portsu)
{
	//清除串口数组内容 
	porta.RemoveAll();
	portse.RemoveAll();
	portsu.RemoveAll();
	//依次检查各串口是否存在                                                   
	//如果能打开某一串口，或打开串口不成功，但返回的是 ERROR_ACCESS_DENIED错误信息，都认为串口存在，只不过后者表明串口已经被占用                                                      
	//否则串口不存在
	for (int i = 1; i < MAX_SCAN_CNT; i++)
	{
		//形成原始设备名称
		CString sPort;
		sPort.Format(_T("\\\\.\\COM%d"), i);
		//尝试打开该端口
		BOOL bSuccess = FALSE;
		HANDLE hPort = ::CreateFile(sPort, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
		if (hPort == INVALID_HANDLE_VALUE)
		{
			DWORD dwError = GetLastError();

			if (dwError == ERROR_ACCESS_DENIED)
			{
				bSuccess = TRUE;
				portsu.Add(i);//已占用的串口
			}
		}
		else
		{
			//该端口已成功打开
			bSuccess = TRUE;
			portse.Add(i);//可用串口
			//不要忘记关闭端口，因为我们不会对它做任何事情
			CloseHandle(hPort);
		}
		//将端口号添加到将要返回的数组中
		if (bSuccess)
			porta.Add(i);//所有存在的串口
	}
}

USHORT c1::AddCom(CComboBox* pvect)
{
	USHORT uicounter; //串口数量
	USHORT uisetcom;
	CString str;

	EnumerateSerialPorts(ports, portse, portsu);  //获得可用串口数
	//获取可用串口个数  
	uicounter = portse.GetSize();
	//如果个数大于0  
	if (uicounter > 0)
	{
		//初始化串口列表框
		for (int i = 0; i < uicounter; i++)
		{
			uisetcom = portse.ElementAt(i);
			str.Format(_T("COM%d "), uisetcom);
			pvect->InsertString(i, str);
		}
	}
	return uicounter;
}


void c1::OnDestroy()
{
	CDialogEx::OnDestroy();
	TerminateThread(hthread, 0);
	Sleep(5);
	if (close_flag == 0) {
		CloseHandle(hCom);
	}
	// TODO: 在此处添加消息处理程序代码
}


void c1::OnCbnSelchangeComboBaudrate()
{
	// TODO: 在此添加控件通知处理程序代码
	m_c_baud.GetWindowText(m_csbaud);
	
}


void c1::OnCbnSelchangeComboBits()
{

	m_c_bit.GetWindowText(m_csds);
	// TODO: 在此添加控件通知处理程序代码
}


void c1::OnCbnSelchangeComboStopbits()
{
	m_c_stp.GetWindowText(m_cssp);
	// TODO: 在此添加控件通知处理程序代码
}


void c1::OnCbnSelchangeComboCheck()
{
	m_c_parity.GetWindowText(m_csparity);
	// TODO: 在此添加控件通知处理程序代码
}


void c1::Onsave()
{
	// TODO: 在此添加命令处理程序代码
	m_c_baud.GetWindowText(m_csbaud);
	m_c_bit.GetWindowText(m_csds);
	m_c_stp.GetWindowText(m_cssp);
	m_c_parity.GetWindowText(m_csparity);
}
