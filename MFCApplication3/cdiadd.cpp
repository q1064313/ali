// cdiadd.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication3.h"
#include "afxdialogex.h"
#include "cdiadd.h"


extern CString m_stredit;

// cdiadd 对话框

IMPLEMENT_DYNAMIC(cdiadd, CDialogEx)

cdiadd::cdiadd(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_add()
{

}

cdiadd::~cdiadd()
{
}

void cdiadd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT_add, m_add);
	DDX_Control(pDX, IDC_EDIT_add, m_add);
}


BEGIN_MESSAGE_MAP(cdiadd, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &cdiadd::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT_add, &cdiadd::OnEnChangeEditadd)
END_MESSAGE_MAP()


// cdiadd 消息处理程序


/*void cdiadd::OnBnClickedButton1()
{
	
}*/


void cdiadd::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_add.GetWindowText(m_stredit);
	this->EndDialog(0);
}


void cdiadd::OnEnChangeEditadd()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void cdiadd::OnBnClickedButtoncancel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->EndDialog(0);
}
