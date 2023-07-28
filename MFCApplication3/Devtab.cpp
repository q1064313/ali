// Devtab.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication3.h"
#include "afxdialogex.h"
#include "Devtab.h"

// Devtab 对话框
extern CTreeCtrl* m_aitree;
IMPLEMENT_DYNAMIC(Devtab, CDialogEx)

Devtab::Devtab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Kong, pParent)
{

}

Devtab::~Devtab()
{
}

void Devtab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_devtab, m_devtab);
}


BEGIN_MESSAGE_MAP(Devtab, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_devtab, &Devtab::OnTcnSelchangedevtab)
END_MESSAGE_MAP()


// Devtab 消息处理程序


BOOL Devtab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_devtab.InsertItem(0, _T("基本信息"));
	m_devtab.InsertItem(1, _T("AI"));
	m_devtab.InsertItem(2, _T("DI"));
	m_devtab.InsertItem(3, _T("ACC"));
	m_devtab.InsertItem(4, _T("DO"));
	m_devtab.InsertItem(5, _T("AO"));
	HTREEITEM m_CurTree = m_aitree->GetSelectedItem();
	CString cs = m_aitree->GetItemText(m_CurTree);
	cs = cs + _T(".xml");
	devdlg.Create(IDD_basicinfo, GetDlgItem(IDC_devtab));
	aidlg.Create(IDD_AI, GetDlgItem(IDC_devtab));
	aidlg.Loadxml("AI",cs);
	accdlg.Create(IDD_AI, GetDlgItem(IDC_devtab));
	accdlg.Loadxml("ACC",cs);
	didlg.Create(IDD_AI, GetDlgItem(IDC_devtab));
	didlg.Loadxml("DI",cs);
	aodlg.Create(IDD_AI, GetDlgItem(IDC_devtab));
	aodlg.Loadxml("AO",cs);
	dodlg.Create(IDD_AI, GetDlgItem(IDC_devtab));
	dodlg.Loadxml("DO",cs);
	//m_page2.Create(IDD_DIALOG2, GetDlgItem(IDC_TABCTRL));
	/*CRect rc;
	m_devtab.GetClientRect(rc);
	rc.top += 21;
	rc.bottom -= 1;
	rc.left += 1;
	rc.right -= 1;
	devdlg.MoveWindow(rc);
	devdlg.ShowWindow(SW_SHOW);*/

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void Devtab::OnTcnSelchangedevtab(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	CRect rc;
	m_devtab.GetClientRect(rc);
	rc.top += 21;
	rc.bottom -= 1;
	rc.left += 1;
	rc.right -= 1;
	switch (m_devtab.GetCurSel())
	{
	case 0:
	{
		
		devdlg.MoveWindow(rc);
		devdlg.ShowWindow(SW_SHOW);
		aidlg.ShowWindow(SW_HIDE);
		accdlg.ShowWindow(SW_HIDE);
		didlg.ShowWindow(SW_HIDE);
		aodlg.ShowWindow(SW_HIDE);
		dodlg.ShowWindow(SW_HIDE);
		break;
	}
	case 1:
	{
		
		aidlg.MoveWindow(rc);
		devdlg.ShowWindow(SW_HIDE);
		aidlg.ShowWindow(SW_SHOW);
		accdlg.ShowWindow(SW_HIDE);
		didlg.ShowWindow(SW_HIDE);
		aodlg.ShowWindow(SW_HIDE);
		dodlg.ShowWindow(SW_HIDE);
		break;
	}
	case 2:
	{
		didlg.MoveWindow(rc);
		devdlg.ShowWindow(SW_HIDE);
		aidlg.ShowWindow(SW_HIDE);
		accdlg.ShowWindow(SW_HIDE);
		didlg.ShowWindow(SW_SHOW);
		aodlg.ShowWindow(SW_HIDE);
		dodlg.ShowWindow(SW_HIDE);
		break;
	}
	case 3:
	{
		accdlg.MoveWindow(rc);
		devdlg.ShowWindow(SW_HIDE);
		aidlg.ShowWindow(SW_HIDE);
		accdlg.ShowWindow(SW_SHOW);
		didlg.ShowWindow(SW_HIDE);
		aodlg.ShowWindow(SW_HIDE);
		dodlg.ShowWindow(SW_HIDE);
		break;
	}
	case 4:
	{

		aodlg.MoveWindow(rc);
		devdlg.ShowWindow(SW_HIDE);
		aidlg.ShowWindow(SW_HIDE);
		accdlg.ShowWindow(SW_HIDE);
		didlg.ShowWindow(SW_HIDE);
		aodlg.ShowWindow(SW_SHOW);
		dodlg.ShowWindow(SW_HIDE);
		break;
	}
	case 5:
	{

		dodlg.MoveWindow(rc);
		devdlg.ShowWindow(SW_HIDE);
		aidlg.ShowWindow(SW_HIDE);
		accdlg.ShowWindow(SW_HIDE);
		didlg.ShowWindow(SW_HIDE);
		aodlg.ShowWindow(SW_HIDE);
		dodlg.ShowWindow(SW_SHOW);
		break;
	}
	default:
		devdlg.ShowWindow(SW_HIDE);
		aidlg.ShowWindow(SW_HIDE);
		accdlg.ShowWindow(SW_HIDE);
		didlg.ShowWindow(SW_HIDE);
		aodlg.ShowWindow(SW_HIDE);
		dodlg.ShowWindow(SW_HIDE);
		break;
		break;
	}




	*pResult = 0;
}
