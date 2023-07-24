// c1.cpp: 实现文件
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
CUIntArray ports;	//所有存在的串口
CUIntArray portse;	//可用的串口
CUIntArray portsu;	//已占用的串口
#define MAX_SCAN_CNT 16



HANDLE hthread = NULL;
HANDLE hCom = INVALID_HANDLE_VALUE;

DWORD WINAPI ThreadProc1(LPVOID lpParam);
// c1 对话框

int close_flag = 0;




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
}


BEGIN_MESSAGE_MAP(c1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEND, &c1::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &c1::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &c1::OnBnClickedButtonClose)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &c1::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR1, &c1::OnBnClickedButtonClear1)
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



	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void c1::initSport()
{
	// TODO: 在此处添加实现代码.
	CString strtemp = NULL;
	char a[1024];
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
		dcb.Parity = ONESTOPBIT;
	}
	if (0 == strcmp(a, "2")) {
		dcb.Parity = TWOSTOPBITS;
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

	char* lpOutBuffer = new char[1024];
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
		char str[100];
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
	//因为至多有255个串口，所以依次检查各串口是否存在                                                   
	//如果能打开某一串口，或打开串口不成功，但返回的是 ERROR_ACCESS_DENIED错误信息，                    
	//都认为串口存在，只不过后者表明串口已经被占用                                                      
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
