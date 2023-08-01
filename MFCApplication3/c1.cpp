// c1.cpp: 实现文件
//
#include "pch.h"
#include "MFCApplication3.h"
#include "afxdialogex.h"
#include "c1.h"
#include<string>
#include <tchar.h>
#include <winspool.h>

#include <sys/types.h>
#include"tinyxml2.h"
#include <iostream> //读写io c++标准库
#include <fstream> //读写文件 c++标准库
#include <string> //字符串类 c++标准库
#include <sstream> //字符串流 c++标准库
#include "json/json.h" //jsoncpp的头文件
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
extern CTreeCtrl* m_aitree;
HANDLE hthread = NULL;
HANDLE hCom = INVALID_HANDLE_VALUE;

DWORD WINAPI ThreadProc1(LPVOID lpParam);
char* U2G1(const char* utf8)
{

	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);

	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);

	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);

	char* str = new char[len + 1];
	memset(str, 0, len + 1);

	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);

	if (wstr)
	{
		delete[] wstr;
		wstr = NULL;
	}
	return str;
}
void StringtoHex(BYTE* GB, int glen, BYTE* SB, int* slen);
int hex_to_str(BYTE* ptr, int& len, char* dtr);
unsigned short CRC16_MODBUS(BYTE* puchMsg, unsigned int usDataLen);
void writeFileFromString(const std::string& filename, const std::string& body);
void readregbuff(LPVOID lpParam, int bitfield, CString nodeid, CString nodename, CString nodeflag, CString nodetime, CString nodeype);
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
void cstrtch(char* ch, CString source);

int send03H(BYTE slaveaddr, BYTE* regaddr, BYTE* regnum, BYTE* SendBuf) {
	SendBuf[0] = slaveaddr;
	SendBuf[1] = 0x03;
	SendBuf[2] = regaddr[0];
	SendBuf[3] = regaddr[1];
	SendBuf[4] = regnum[0];
	SendBuf[5] = regnum[1];
	unsigned short crc = CRC16_MODBUS(SendBuf, 6);
	BYTE crcl = (BYTE)(crc & 0xff);//低八位
	BYTE crch = (BYTE)(crc >> 8);//高八位
	SendBuf[6] = crcl;
	SendBuf[7] = crch;
	return sizeof(SendBuf);
}
void package03H(BYTE* lpOutBuffer, int sendSize, BYTE* regaddr, BYTE* regnum) {
	BYTE tembuff[1024] = { 0 };
	int temlen = 0;
	StringtoHex(lpOutBuffer, sendSize, tembuff, &temlen);
	regaddr[0] = tembuff[0];
	regaddr[1] = tembuff[1];
	regnum[0] = tembuff[2];
	regnum[1] = tembuff[3];

}
int send10H(BYTE slaveaddr, BYTE* lpOutBuffer, int sendSize, BYTE* SendBuf, int* SendLen) {
	SendBuf[0] = slaveaddr;
	SendBuf[1] = 0x10;
	(*SendLen)++;
	(*SendLen)++;
	int x = *SendLen;
	StringtoHex(lpOutBuffer, sendSize, SendBuf, &x);
	*SendLen = x;
	unsigned short crc = CRC16_MODBUS(SendBuf, *SendLen);
	BYTE crcl = (BYTE)(crc & 0xff);//低八位
	BYTE crch = (BYTE)(crc >> 8);//高八位
	SendBuf[*SendLen] = crcl;
	SendBuf[(*SendLen) + 1] = crch;
	(*SendLen) += 2;
	return sizeof(SendBuf);
}
unsigned short CRC16_MODBUS(BYTE* puchMsg, unsigned int usDataLen)
{
	unsigned short usCRC = 0xFFFF;
	uint32_t i;
	uint16_t temp;
	for (i = 0; i < usDataLen; i++)
	{
		usCRC = CRC16_TABLE[(usCRC & 0xff) ^ puchMsg[i]] ^ (usCRC >> 8);
	}
	return usCRC;
}
BOOL Retpisvalid(int len, BYTE* str) {
	if (len >= 4) {
		unsigned short crc = CRC16_MODBUS(str, len - 2);
		BYTE crcl = (BYTE)(crc & 0xff);//低八位
		BYTE crch = (BYTE)(crc >> 8);//高八位
		if (crcl == str[len - 2] && crch == str[len - 1]) {
			return TRUE;
		}
	}
	return FALSE;
}
void unpackage03H(BYTE* str, char* addr, char* Funcode, char* data, int len, int bitfield,char &bit) {
	char str1[1024] = { 0 };
	if (len != 0) {
		len = len - 2;
		int lenstr =  hex_to_str(str, len, str1);
		addr[0] = str1[0];
		addr[1] = str1[1];
		Funcode[0] = str1[2];
		Funcode[1] = str1[3];
		for (int i = 0; i < lenstr - 4; i++) {
			data[i] = str1[i + 6];
		}

		BYTE n[16] = { 0 };
		n[7] = (str[3] & 0x01) == 0x01 ? 1 : 0;	//等于0
		n[6] = (str[3] & 0x02) == 0x02 ? 1 : 0;	//等于1
		n[5] = (str[3] & 0x04) == 0x04 ? 1 : 0;	//等于0
		n[4] = (str[3] & 0x08) == 0x08 ? 1 : 0;	//等于0
		n[3] = (str[3] & 0x10) == 0x10 ? 1 : 0;	//等于1
		n[2] = (str[3] & 0x20) == 0x20 ? 1 : 0;	//等于0
		n[1] = (str[3] & 0x40) == 0x40 ? 1 : 0;	//等于1
		n[0] = (str[3] & 0x80) == 0x80 ? 1 : 0;	//等于0
		n[15] = (str[4] & 0x01) == 0x01 ? 1 : 0;	//等于0
		n[14] = (str[4] & 0x02) == 0x02 ? 1 : 0;	//等于1
		n[13] = (str[4] & 0x04) == 0x04 ? 1 : 0;	//等于0
		n[12] = (str[4] & 0x08) == 0x08 ? 1 : 0;	//等于0
		n[11] = (str[4] & 0x10) == 0x10 ? 1 : 0;	//等于1
		n[10] = (str[4] & 0x20) == 0x20 ? 1 : 0;	//等于0
		n[9] = (str[4] & 0x40) == 0x40 ? 1 : 0;	//等于1
		n[8] = (str[4] & 0x80) == 0x80 ? 1 : 0;	//等于0
		bit = n[bitfield] + '0';

	}
}
CString getCurrentTime(void) {
	CTime timeNow;
	CString strTime;

	timeNow = CTime::GetCurrentTime();
	strTime = timeNow.Format("%Y/%m/%d %H:%M:%S");
	return strTime;
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
	ON_BN_CLICKED(IDC_BUTTON_readdata, &c1::OnBnClickedButtonreaddata)
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
	m_send.EnableWindow(0);
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
	if (hCom == NULL) {
		AfxMessageBox(_T("串口没有打开"));
		return;
	}
	CEdit* pRecvStrEdit = (CEdit*)GetDlgItem(IDC_EDIT_SEND);
	pRecvStrEdit->GetWindowText(m_edit_send);
	int sendSize = m_edit_send.GetLength();

	BYTE* lpOutBuffer = new  BYTE[1024]{ 0 };
	BYTE* SendBuf = new  BYTE[1024]{ 0 };
	BYTE regaddr[2] = { 0 };
	BYTE regnum[2] = { 0 };
	int SendLen = 0;
	for (int i = 0; i <= sendSize; i++) {
		lpOutBuffer[i] = (BYTE)m_edit_send.GetBuffer()[i];
	}

	package03H(lpOutBuffer, sendSize, regaddr, regnum);
	int retxflag = send03H(0x01, regaddr, regnum, SendBuf);

	//	send10H(0x01, lpOutBuffer, sendSize, SendBuf, &SendLen);
	int lenl = retxflag;
	DWORD dwbyteswrite = 0;
	COMSTAT Comstat;
	DWORD dwErrorFlags = 0;
	BOOL bwritestat;
	TerminateThread(hthread, 0);
	ClearCommError(hCom, &dwErrorFlags, &Comstat);
	bwritestat = WriteFile(hCom, SendBuf, retxflag, &dwbyteswrite, NULL);
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

//	hthread = CreateThread(NULL, 0, ThreadProc1, (LPVOID)(this), 0, NULL);
	m_send.EnableWindow(1);
	m_close.EnableWindow(1);
	m_open.EnableWindow(0);
	close_flag = 0;
//	if (hthread == NULL) {
//		MessageBox(_T("读取失败！"));
//	}
	//readxml("DI");
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
		BYTE str[512];
		char str1[1024];
		DWORD wcount = 512;
		BOOL breadstat;
		c1* cr = (c1*)lpParam;
		memset(str, 0, sizeof(str));
		memset(str1, 0, sizeof(str1));
		breadstat = ReadFile(hCom, str, wcount, &wcount, NULL);
		if (!breadstat) {
			AfxMessageBox(_T("读取串口失败"));
			exit(0);
		}
		int len = wcount;
		PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		if (len != 0) {
			bool reptcrc = Retpisvalid(len, str);//对于接受的包进行crc校验
			if (reptcrc == false) {
				cr->m_edit_recv = "crc error";
				CEdit* precvStrEdit = (CEdit*)cr->GetDlgItem(IDC_EDIT_RECEIVE);
				precvStrEdit->SetSel(-1, -1);
				precvStrEdit->ReplaceSel(cr->m_edit_recv);
				continue;
			}
			char addr[2] = { 0 };
			char Funcode[2] = { 0 };
			char datastr[1024] = { 0 };
	//		unpackage03H(str, addr, Funcode, datastr, len);
			cr->m_edit_recv = datastr;
			CEdit* precvStrEdit = (CEdit*)cr->GetDlgItem(IDC_EDIT_RECEIVE);
			precvStrEdit->SetSel(-1, -1);
			precvStrEdit->ReplaceSel(cr->m_edit_recv);
		}
		Sleep(10);

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

void StringtoHex(BYTE* GB, int glen, BYTE* SB, int* slen)
{
	int i; //遍历输入的字符串
	int a = 0;
	char temp; //接收字符，用来判断是否为空格，若是则跳过
	char temp1, temp2; //接收一个字节的两个字符 例如EB，则temp1='E',temp2 = 'B'
	//*slen = 0; //输出的16进制字符串长度
	for (i = 0; i < glen; i++)
	{
		temp = GB[i];
		if (temp == ' ')
			continue;
		if (a == 0)
			temp1 = GB[i];
		if (a == 1)
			temp2 = GB[i];
		a++;
		if (a == 2)
		{
			a = 0;
			temp1 = temp1 - '0';
			if (temp1 > 10)
				temp1 = temp1 - 7;
			temp2 = temp2 - '0';
			if (temp2 > 10)
				temp2 = temp2 - 7;
			SB[*slen] = temp1 * 16 + temp2;
			(*slen)++;
		}
	}
}

int hex_to_str(BYTE* ptr, int& len, char* dtr)
{
	int count = 0;
	for (int i = 0; i < len; i++)
	{
		if (ptr[i] < 10) {
			dtr[count] = '0';
			count++;
			dtr[count] = ptr[i] + '0';
			count++;
		}
		else {
			if (ptr[i] < 10) {
				dtr[count] = ptr[i] / 10 + '0';
				count++;
				dtr[count] = ptr[i] - (ptr[i] / 10) * 10 + '0';
				count++;
			}
			else {
				if (ptr[i] / 16 < 10) {
					dtr[count] = ptr[i] / 16 + '0';
					count++;
				}
				else {
					dtr[count] = ptr[i] / 16 + '7';
					count++;
				}
				if (ptr[i] - (ptr[i] / 16) * 16 < 10) {
					dtr[count] = ptr[i] - (ptr[i] / 16) * 16 + '0';
					count++;
				}
				else {
					dtr[count] = ptr[i] - (ptr[i] / 16) * 16 + '7';
					count++;
				}
			}
		}
	}
	return count;
}

void c1::readxml(char* ainame,const char* filename) {
	BYTE regad[2] = { 0 };
	BYTE regnum[2] = { 0 };
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile(filename))
	{
		doc.PrintError();
		return;
	}
	tinyxml2::XMLElement* Root = doc.RootElement();
	tinyxml2::XMLElement* Node = Root->FirstChildElement("NodeProperty");
	if (Node == nullptr) {
		return;
	}
	tinyxml2::XMLElement* readnode = Node->FirstChildElement(ainame);
	if (readnode->FirstChildElement("Node") == nullptr) {
		return;
	}
	std::vector<int> regaddr;
	readnode = readnode->FirstChildElement("Node");
	readnodedata(readnode, ainame);
	while (readnode->NextSibling() != nullptr) {
		readnode = readnode->NextSiblingElement("Node");
		readnodedata(readnode, ainame);
	}

}
void c1::readnodedata(tinyxml2::XMLElement* readnode, char* ainame) {
	std::vector<int> regaddr;
	BYTE regad[2] = { 0 };
	BYTE regnum[2] = { 0 };
	CString RegNo = CString(readnode->Attribute("RegNo"));
	int regaddrno = _ttoi(RegNo);
	regaddr.push_back(regaddrno);
	regaddrno = _ttoi(RegNo);
	regaddr.push_back(regaddrno);
	unsigned short startaddr = *regaddr.begin();
	regad[1] = (BYTE)(startaddr & 0xff);//低八位
	regad[0] = (BYTE)(startaddr >> 8);
	regnum[1] = 0x01;//低八位
	regnum[0] = 0x00;
	BYTE sendbuf[1024] = { 0 };
	int retxflag = send03H(0x01, regad, regnum, sendbuf);
	DWORD dwbyteswrite = 0;
	COMSTAT Comstat;
	DWORD dwErrorFlags = 0;
	BOOL bwritestat;
	ClearCommError(hCom, &dwErrorFlags, &Comstat);
	bwritestat = WriteFile(hCom, sendbuf, retxflag, &dwbyteswrite, NULL);
	int bitfield = _ttoi(CString(readnode->Attribute("BitField")));
	readregbuff(this, bitfield, CString(readnode->Attribute("Id")), CString(U2G1(readnode->Attribute("Name"))), CString(readnode->Attribute("BitField")), getCurrentTime(), CString(ainame));
	regaddr.clear();
	regaddr.push_back(regaddrno);
	
}
void writeFileFromString(const std::string& filename, const std::string& body)
{
	std::ofstream ofile(filename, std::ios::app);
	ofile << body;
	ofile << std::endl;
	ofile.close();
}
void readregbuff(LPVOID lpParam,int bitfield,CString nodeid,CString nodename,CString nodeflag,CString nodetime,CString nodeype) {
	BYTE str[512];
	char str1[1024];
	DWORD wcount = 512;
	BOOL breadstat;
	c1* cr = (c1*)lpParam;
	memset(str, 0, sizeof(str));
	memset(str1, 0, sizeof(str1));
	breadstat = ReadFile(hCom, str, wcount, &wcount, NULL);
	if (!breadstat) {
		AfxMessageBox(_T("读取串口失败"));
		exit(0);
	}
	char addr[2] = { 0 };
	char Funcode[2] = { 0 };
	char datastr[1024] = { 0 };
	char bit = 0;
	int len = wcount;
	PurgeComm(hCom, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	if (len != 0) {
		bool reptcrc = Retpisvalid(len, str);//对于接受的包进行crc校验
		if (reptcrc == false) {
			cr->m_edit_recv = "crc error";
			CEdit* precvStrEdit = (CEdit*)cr->GetDlgItem(IDC_EDIT_RECEIVE);
			precvStrEdit->SetSel(-1, -1);
			precvStrEdit->ReplaceSel(cr->m_edit_recv);
			return;
		}
		unpackage03H(str, addr, Funcode, datastr, len,bitfield,bit);
		std::string writejson = "";
		std::string temp = " ";
		writejson += "ID:";
		writejson += CT2A(nodeid.GetString());
		writejson.push_back(', ');
		writejson += "Name:";
		writejson += CT2A(nodename.GetString());
		writejson.push_back(', ');
		writejson += "FLAG:";
		writejson += CT2A(nodeflag.GetString());
		writejson.push_back(', ');
		writejson += "Time:";
		writejson += CT2A(nodetime.GetString());
		writejson.push_back(', ');
		writejson += "Type:";
		writejson += CT2A(nodeype.GetString());
		writejson.push_back(', ');
		writejson+="ADDR:";
		writejson.push_back(addr[0]);
		writejson.push_back(addr[1]);
		writejson.push_back(', ');
		writejson += "Funcode:";
		writejson.push_back(Funcode[0]);
		writejson.push_back(Funcode[1]);
		writejson.push_back(', ');
		writejson += "DATA:";
		writejson += datastr;
		writejson.push_back(', ');
		writejson += "BITFIELD:";
		writejson += std::to_string(bitfield);
		writejson.push_back(', ');
		writejson += "BIT:";
		writejson += bit;
		writeFileFromString("DataCollected.json", writejson);
	}
}

void c1::OnBnClickedButtonreaddata()
{
	HTREEITEM m_CurTree = m_aitree->GetSelectedItem();
	CString cs = m_aitree->GetItemText(m_CurTree);
	if (m_aitree->ItemHasChildren(m_CurTree)) {
		m_CurTree = m_aitree->GetChildItem(m_CurTree);
	}
	cs = m_aitree->GetItemText(m_CurTree);
	cs = cs + _T(".xml");
	char ch[1024] = { 0 };
	cstrtch(ch, cs);
	// TODO: 在此添加控件通知处理程序代码
	readxml("AI", ch);
	readxml("DI", ch);
	readxml("AO", ch);
	readxml("DO", ch);
	readxml("ACC", ch);
	while (m_aitree->GetNextSiblingItem(m_CurTree) != NULL){
		m_CurTree = m_aitree->GetNextSiblingItem(m_CurTree);
		cs = m_aitree->GetItemText(m_CurTree);
		cs = cs + _T(".xml");
		char ch1[1024] = { 0 };
		cstrtch(ch1, cs);
		readxml("AI", ch1);
		readxml("DI", ch1);
		readxml("AO", ch1);
		readxml("DO", ch1);
		readxml("ACC", ch1);
	}
	MessageBox(_T("刷新完成"));
}
void cstrtch(char* ch, CString source) {
	for (int i = 0; i <= source.GetLength(); i++) {
		ch[i] = source[i];
	}
	return;
}