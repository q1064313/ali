// dev_basicinfo.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication3.h"
#include "afxdialogex.h"
#include "dev_basicinfo.h"


// dev_basicinfo 对话框

IMPLEMENT_DYNAMIC(dev_basicinfo, CDialogEx)

dev_basicinfo::dev_basicinfo(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_basicinfo, pParent)
{

}

dev_basicinfo::~dev_basicinfo()
{
}

void dev_basicinfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(dev_basicinfo, CDialogEx)
END_MESSAGE_MAP()


// dev_basicinfo 消息处理程序
