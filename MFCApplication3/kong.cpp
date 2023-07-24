// kong.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication3.h"
#include "afxdialogex.h"
#include "kong.h"


// kong 对话框

IMPLEMENT_DYNAMIC(kong, CDialogEx)

kong::kong(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Kong, pParent)
{

}

kong::~kong()
{
}

void kong::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(kong, CDialogEx)
END_MESSAGE_MAP()


// kong 消息处理程序
