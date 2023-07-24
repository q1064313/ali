// caiji.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication3.h"
#include "afxdialogex.h"
#include "caiji.h"


// caiji 对话框

IMPLEMENT_DYNAMIC(caiji, CDialogEx)

caiji::caiji(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_caiji, pParent)
{

}

caiji::~caiji()
{
}

void caiji::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(caiji, CDialogEx)
END_MESSAGE_MAP()


// caiji 消息处理程序


BOOL caiji::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
