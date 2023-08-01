// MFCApplication3Dlg.cpp: 实现文件
//
// c1: 切换到x通道显示的界面
//MQTT_ZF：mqtt转发界面
// cdiadd：采集添加通道jiemian
// caiji：采集界面
// menu1：主菜单栏
// menu2：右键菜单栏 
// 
// 
//头文件
#include "c1.h"
#include "pch.h"
#include"devtab.h"
#include<vector>
#include "caiji.h"
#include<iostream>
#include<stddef.h>
#include "cdiadd.h"
#include "MQTT_zf.h"
#include "tinyxml2.h"
#include "framework.h"
#include "afxdialogex.h"
#include "MFCApplication3.h"
#include "MFCApplication3Dlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#define xmltitle "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
#endif
//////////////////////////


////////一些必须的全局变量
CString m_stredit = NULL;//获取的添加通道对话框ciadd的editctrl内容
CString m_csbaud = NULL;//全局波特率
CString m_csds= NULL;//全局数据位
CString m_cssp = NULL;//全局停止位
CString m_csparity = NULL;//全局校验位
CTreeCtrl* m_aitree = nullptr;
extern CString baud;

std::vector<CString> treeitem;//用于对tree切换窗口的数组
///////


//////用于应用程序“关于”菜单项的 CAboutDlg 对话框
//子窗体的id
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


////////////// CMFCApplication3Dlg 对话框


//////////////////////////////////////////////////////////
CMFCApplication3Dlg::CMFCApplication3Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION3_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_STATIC_CHILD, m_statuchild);
}

BEGIN_MESSAGE_MAP(CMFCApplication3Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CMFCApplication3Dlg::OnTvnSelchangedTree1)
	ON_NOTIFY(NM_RCLICK, IDC_TREE, &CMFCApplication3Dlg::OnNMRClickTree)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE, &CMFCApplication3Dlg::OnTvnBeginlabeleditTree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE, &CMFCApplication3Dlg::OnTvnEndlabeleditTree)
	ON_COMMAND(ID_32776, &CMFCApplication3Dlg::add_com)
	ON_COMMAND(ID_32777, &CMFCApplication3Dlg::delete_com)
	ON_COMMAND(ID_32783, &CMFCApplication3Dlg::aboutinfo)

ON_BN_CLICKED(IDCANCEL, &CMFCApplication3Dlg::OnBnClickedCancel)
ON_COMMAND(ID_32774, &CMFCApplication3Dlg::OnSaveXml)
ON_COMMAND(ID_32785, &CMFCApplication3Dlg::Onadddev)
END_MESSAGE_MAP()
/////////////////////////// CMFCApplication3Dlg 消息处理程序


////////////////////////////////////////////////////
BOOL CMFCApplication3Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	HTREEITEM hroot = m_tree.InsertItem(_T("默认工程"), 1, 0, TVI_ROOT);//添加一级结点
	HTREEITEM h1 = m_tree.InsertItem(_T("系统设置"), 1, 0, hroot);//添加二级结点
	treeitem.emplace_back(_T("系统设置"));
	HTREEITEM h2 = m_tree.InsertItem(_T("采集服务"), 1, 0, hroot);//添加二级结点
	treeitem.emplace_back(_T("采集服务"));
	HTREEITEM h3 = m_tree.InsertItem(_T("云服务"), 1, 0, hroot);//添加二级结点
	treeitem.emplace_back(_T("云服务"));
	treeitem.emplace_back(_T("通道"));
	HTREEITEM h11 = m_tree.InsertItem(_T("网关资料"), 1, 0, h1);//添加三级结点
	HTREEITEM h31 = m_tree.InsertItem(_T("MQTT云平台"), 1, 0, h3);//添加三级结点
	treeitem.emplace_back(_T("MQTT云平台"));
	treeitem.emplace_back(_T("Device"));
	m_tree.SetBkColor(RGB(200, 200, 200)); //设置树形控件的背景色
	m_tree.SetTextColor(RGB(127, 0, 0));   //设置文本颜色
	m_tree.Select(h11, TVGN_CARET);

	m_menu.LoadMenuW(IDR_MENU1); //IDR_MENU1是菜单资源ID

	pChildDialog = NULL;
	SetMenu(&m_menu); //将菜单加入到对话框。


	int nDstWidth = 1280;
	int nDstHeight = 900;

	//得到显示器大小
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	//再用MoveWindow
	CRect rcTemp;
	rcTemp.TopLeft() = CPoint(cx / 2 - nDstWidth / 2, cy / 2 - nDstHeight / 2);
	rcTemp.BottomRight() = CPoint(cx / 2 + nDstWidth / 2, cy / 2 + nDstHeight / 2);
	MoveWindow(&rcTemp);

	LoadXml();
	m_aitree = &m_tree;
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}
//////////////////////////////初始化函数


/////////////////////////////////////////////////////////////
void CMFCApplication3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。


//////////////////////////////////////////////////////////////////
void CMFCApplication3Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication3Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
void CMFCApplication3Dlg::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	HTREEITEM selItem;
	

	selItem = m_tree.GetSelectedItem();
	

	CString cs = m_tree.GetItemText(selItem);

	int flag = -1;
	for (int i = 0; i < treeitem.size(); i++) {
		if (cs.Find(treeitem[i])>-1) {
			flag = i;
			break;
		}
	}
	CRect ClassInfoRect;
	GetDlgItem(IDC_STATIC_CHILD)->GetWindowRect(&ClassInfoRect);
	ScreenToClient(ClassInfoRect);
	if (pChildDialog != NULL)
	{
		pChildDialog->DestroyWindow();
		delete pChildDialog;
		pChildDialog = nullptr;
	}
	switch (flag)
	{
	case 0:
		break;
	case 1:
		caijidlg = new caiji();
		caijidlg->Create(IDD_caiji, this);//子窗体的id
		caijidlg->MoveWindow(ClassInfoRect);
		caijidlg->ShowWindow(SW_SHOW);
		pChildDialog = caijidlg;
		break;
	case 2:
		break;
	case 3:
		LoadXmlchange();
		pDlg = new c1();
		pDlg->Create(IDD_mfcc1, this);//子窗体的id
		pDlg->MoveWindow(ClassInfoRect);
		pDlg->ShowWindow(SW_SHOW);
		pChildDialog = pDlg;
		break;
	case 4:
		mqttdlg = new MQTT_ZF();
		mqttdlg->Create(IDD_MQTT_zf, this);//子窗体的id
		mqttdlg->MoveWindow(ClassInfoRect);
		mqttdlg->ShowWindow(SW_SHOW);
		pChildDialog = mqttdlg;
		break;
	case 5:
		devtabdlg = new Devtab();
		devtabdlg->Create(IDD_Kong, this);//子窗体的id
		devtabdlg->MoveWindow(ClassInfoRect);
		devtabdlg->ShowWindow(SW_SHOW);
		pChildDialog = devtabdlg;
		break;
	default:
		break;

	}
	
}
//////////////////切换树控件触发函数



void CMFCApplication3Dlg::OnTvnEndlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	CString strName;     //修改后的数据  
	CString rootstr;
	m_tree.GetEditControl()->GetWindowText(strName);

	if (strName.IsEmpty())
	{
		MessageBox(_T("数据项名称不能为空，请重新输入!"));
		return;
	}
	if (strName == g_sSelectStr)
	{
		return;
	}
	HTREEITEM hRoot = m_tree.GetRootItem();
	//rootstr = m_webTree.GetItemText(hRoot);  

	HTREEITEM hFind = FindItem(hRoot, strName);
	if (hFind == NULL)  //判断输入的数据是否和其他的相同 
	{
		CString strText;
		m_tree.GetEditControl()->GetWindowText(strText.GetBuffer(200), 200);
		m_tree.SetItemText(m_tree.GetSelectedItem(), strText);//设置编辑后的文本为结点  
		*pResult = TRUE;
	}
	else //
	{
		MessageBox(_T("该数据已存在"));
		return;
   
	}

	*pResult = 0;
}


BOOL CMFCApplication3Dlg::PreTranslateMessage(MSG* pMsg)  //响应编辑框处于焦点时回车结束编辑
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			CEdit* pLableEdit = m_tree.GetEditControl();
			if (pLableEdit && pLableEdit->IsWindowVisible())
			{
				this->SetFocus(); //编辑框失去焦点，结束编辑，从而进入TVN_ENDLABELEDIT 处理  
				return TRUE;
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


void CMFCApplication3Dlg::OnTvnBeginlabeleditTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	m_tree.GetEditControl()->LimitText(1024);
	g_sSelectStr = m_tree.GetItemText(m_tree.GetSelectedItem());//得到修改前的数据

	*pResult = 0;
}


HTREEITEM CMFCApplication3Dlg::FindItem(HTREEITEM item, CString strText)
{
	HTREEITEM hFind;
	if (item == NULL)  //根为空，修改数据与根数据不同
	{
		return NULL;
	}
	while (item != NULL)  //根不为空，
	{
		if (m_tree.GetItemText(item) == strText)
		{
			return item;  //
		}
		if (m_tree.ItemHasChildren(item))
		{
			item = m_tree.GetChildItem(item);
			hFind = FindItem(item, strText);
			if (hFind)
			{
				return hFind;
			}
			else
			{
				item = m_tree.GetNextSiblingItem(m_tree.GetParentItem(item));  //得到兄弟姐妹节点
			}

		}
		else
		{
			item = m_tree.GetNextSiblingItem(item);
			return FindItem(item, strText);

		}
	}
	return item;
}


void CMFCApplication3Dlg::OnNMRClickTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	//临时鼠标的屏幕坐标，用来弹出menu
	CPoint ScreenPt;
	GetCursorPos(&ScreenPt);
	//获取到当前鼠标选择的树节点
	HTREEITEM m_CurTree = m_tree.GetSelectedItem();
	CString cs = m_tree.GetItemText(m_CurTree);
	if (cs ==treeitem[1]||cs.Find(treeitem[3])>-1|| cs.Find(_T("Device"))>-1)
	{
		//m_webTree.SelectItem(m_CurTree); //使右键单击的树节点被选中
		CMenu menu;
		menu.LoadMenu(IDR_MENU2);
		CMenu* pPopup = menu.GetSubMenu(0);//装载第一个子菜单，即我们菜单的第一列
		pPopup->TrackPopupMenu(TPM_LEFTALIGN, ScreenPt.x, ScreenPt.y, this);//弹出菜单
	}

	*pResult = 0;
}


void CMFCApplication3Dlg::add_com()//添加节点
{
	HTREEITEM selItem;
	//获得选择项
	selItem = m_tree.GetSelectedItem();
	//获取选中的内容
	CString cs = m_tree.GetItemText(selItem);
	CString che = _T("采集服务");
	if (cs != che) {
		MessageBox(_T("只能添加采集服务！"));
		return;
	}
	UpdateData(FALSE);
	CString m_stredit1 = _T("通道");
	cdiadd dlgadd;
	dlgadd.DoModal();
	if(!m_stredit.IsEmpty()) {
		m_stredit1 = m_stredit + m_stredit1;
	}
	else {
		MessageBox(_T("数据项名称不能为空，请重新输入!"));
		return;
	}
	HTREEITEM hRoot = m_tree.GetRootItem();
	//rootstr = m_webTree.GetItemText(hRoot);  

	HTREEITEM hFind = FindItem(hRoot, m_stredit1);
	if (hFind != NULL)  //判断输入的数据是否和其他的相同 
	{
		MessageBox(_T("该数据已存在"));
		return;
	}
	if (m_stredit.CompareNoCase(_T("1"))!=0&& m_stredit.CompareNoCase(_T("2")) != 0&&m_stredit.CompareNoCase(_T("3")) != 0&& m_stredit.CompareNoCase(_T("4")) != 0)
	{
		MessageBox(_T("通道名称有误"));
		return;
	}
	HTREEITEM hNew = m_tree.InsertItem(m_stredit1, 0, 0, m_tree.GetSelectedItem());
	
	UpdateData(TRUE);
}


void CMFCApplication3Dlg::Onadddev()
{
	// TODO: 在此添加命令处理程序代码
	HTREEITEM selItem;
	//获得选择项
	selItem = m_tree.GetSelectedItem();
	//获取选中的内容
	CString cs = m_tree.GetItemText(selItem);
	CString che = _T("通道");
	if (cs.Find(che)<=-1) {
		MessageBox(_T("只能在通道中添加设备！"));
		return;
	}
	UpdateData(FALSE);
	CString m_stredit1 = _T("Device");
	cdiadd dlgadd;
	dlgadd.DoModal();
	if (!m_stredit.IsEmpty()) {
		m_stredit1 = m_stredit + m_stredit1;
	}
	else {
		MessageBox(_T("数据项名称不能为空，请重新输入!"));
		return;
	}
	HTREEITEM hRoot = m_tree.GetRootItem();
	//rootstr = m_webTree.GetItemText(hRoot);  

	HTREEITEM hFind = FindItem(hRoot, m_stredit1);
	if (hFind != NULL)  //判断输入的数据是否和其他的相同 
	{
		MessageBox(_T("该数据已存在"));
		return;
	}
	HTREEITEM hNew = m_tree.InsertItem(m_stredit1, 0, 0, m_tree.GetSelectedItem());

	UpdateData(TRUE);
}


void CMFCApplication3Dlg::delete_com()//删除节点
{
	using namespace tinyxml2;

	// TODO: 在此添加命令处理程序代码
	HTREEITEM selItem;
	//获得选择项
	selItem = m_tree.GetSelectedItem();
	//获取选中的内容
	CString cs = m_tree.GetItemText(selItem);
	if (cs.Find(_T("通道")) > -1) {
		m_tree.DeleteItem(m_tree.GetSelectedItem());
		tinyxml2::XMLDocument doc;
		if (doc.LoadFile("ConfigFile.xml"))
		{
			doc.PrintError();
			return;
		}

		XMLElement* chaneelconfig = doc.RootElement();
		XMLElement* channel = chaneelconfig->FirstChildElement("Channel");
		CString no = CString(channel->Attribute("No"));
		while (channel->NextSibling() != nullptr) {
			if (cs.Find(no) > -1) {
				break;
			}
			else {
				channel = channel->NextSiblingElement();
				no = CString(channel->Attribute("No"));
			}
		}
		if (cs.Find(no) > -1) {
			chaneelconfig->DeleteChild(channel);
			doc.SaveFile("ConfigFile.xml");
		}
		else {
			return;
		}
	}
	else {
		if (cs.Find(_T("Device")) > -1) {
			HTREEITEM parentcom;
			parentcom = m_tree.GetParentItem(selItem);
			m_tree.DeleteItem(m_tree.GetSelectedItem());
			tinyxml2::XMLDocument doc;
			if (doc.LoadFile("ConfigFile.xml"))
			{
				doc.PrintError();
				return;
			}
			XMLElement* chaneelconfig = doc.RootElement();
			XMLElement* channel = chaneelconfig->FirstChildElement("Channel");
			CString no = CString(channel->Attribute("No"));
			while (channel->NextSibling() != nullptr) {
				if (m_tree.GetItemText(parentcom).Find(no) > -1) {
					break;
				}
				else {
					channel = channel->NextSiblingElement();
					no = CString(channel->Attribute("No"));
				}
			}
			if (m_tree.GetItemText(parentcom).Find(no) > -1) {
				if (channel->FirstChildElement()->NextSibling() != nullptr) {
					XMLElement* dev = channel->FirstChildElement("Device");
					CString devname = CString(dev->Attribute("Name"));
					while (dev->NextSibling() != nullptr) {
						if (cs.Find(devname) > -1) {
							break;
						}
						else {
							dev = dev->NextSiblingElement();
							devname = CString(dev->Attribute("Name"));
						}

					}
					if (cs.Find(devname) > -1){
						channel->DeleteChild(dev);
						doc.SaveFile("ConfigFile.xml");
					}

				}
				else {
					return;
				}
			}
			else{
				return;
			}
	}else
		MessageBox(_T("非法删除"));
	}


}


void CMFCApplication3Dlg::aboutinfo()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CMFCApplication3Dlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
	exit(0);
}


void CMFCApplication3Dlg::OnSaveXml()
{
	HTREEITEM selItem;
	selItem = m_tree.GetSelectedItem();
	CString cs = m_tree.GetItemText(selItem);

	if (cs.Find(_T("通道")) > 0) {
		savexml(cs,selItem);
	}
else if(cs.Find(_T("Device")) > 0)
{
		selItem = m_tree.GetParentItem(selItem);
		cs = m_tree.GetItemText(selItem);
		savexml(cs, selItem);
	}
else {
		MessageBox(_T("非法保存"));
	}
   return;
	
}


void CMFCApplication3Dlg::LoadXml() {
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	if (doc.LoadFile("ConfigFile.xml"))
	{
		doc.PrintError();
		return;
	}
	// 根元素
	XMLElement* chaneelconfig = doc.RootElement();
	XMLElement* channel= chaneelconfig->FirstChildElement("Channel");
	if (channel == nullptr) {
		return;
	}
	XMLElement* CommMode = channel->FirstChildElement("CommMode");
	CString no = CString(channel->Attribute("No"));
	CString baud = CString(CommMode->Attribute("Baud"));
	CString datas = CString(CommMode->Attribute("DataSize"));
	CString sp = CString(CommMode->Attribute("Stop"));
	CString pari = CString(CommMode->Attribute("Parity"));
	m_csbaud = baud;
	m_csds = datas;
	m_csparity = pari;
	m_cssp = sp;

	HTREEITEM hRoot = m_tree.GetRootItem();
	HTREEITEM selItem;

	//获得选择项
	CString che = _T("采集服务");
	selItem = FindItem(hRoot, che);
	if (selItem == NULL)  //判断输入的数据是否和其他的相同 
	{
		MessageBox(_T("加载失败!"));
		return;
	}
	UpdateData(FALSE);
	CString m_stredit1 = _T("通道");
	if (!no.IsEmpty()) {
		m_stredit1 = no + m_stredit1;
	}
	else {
		MessageBox(_T("数据项名称为空，加载失败!"));
		return;
	}
	//rootstr = m_webTree.GetItemText(hRoot);  

	HTREEITEM hNew = m_tree.InsertItem(m_stredit1, 0, 0, selItem);
	while(CommMode->NextSibling() != nullptr) {
		CommMode = CommMode->NextSiblingElement();
		selItem = FindItem(hRoot, m_stredit1);
		CString temp = CString(CommMode->Attribute("Name"));
		HTREEITEM hNew = m_tree.InsertItem(temp, 0, 0, selItem);
	}
	UpdateData(TRUE);

	while (channel->NextSibling()!= nullptr) {
		channel = channel->NextSiblingElement();
		CommMode = channel->FirstChildElement("CommMode");

		no = CString(channel->Attribute("No"));
		baud = CString(CommMode->Attribute("Baud"));
		datas = CString(CommMode->Attribute("DataSize"));
		sp = CString(CommMode->Attribute("Stop"));
		pari = CString(CommMode->Attribute("Parity"));
		m_csbaud = baud;
		m_csds = datas;
		m_csparity = pari;
		m_cssp = sp;
		hRoot = m_tree.GetRootItem();
		selItem;
		//获得选择项
		CString che = _T("采集服务");
		selItem = FindItem(hRoot, che);
		if (selItem == NULL)  //判断输入的数据是否和其他的相同 
		{
			MessageBox(_T("加载失败!"));
			return;
		}
		UpdateData(FALSE);
		m_stredit1 = _T("通道");
		if (!no.IsEmpty()) {
			m_stredit1 = no + m_stredit1;
		}
		else {
			MessageBox(_T("数据项名称为空，加载失败!"));
			return;
		}

		HTREEITEM hNew = m_tree.InsertItem(m_stredit1, 0, 0, selItem);
		while (CommMode->NextSibling() != nullptr) {
			CommMode = CommMode->NextSiblingElement();
			selItem = FindItem(hRoot, m_stredit1);
			CString temp = CString(CommMode->Attribute("Name"));
			HTREEITEM hNew = m_tree.InsertItem(temp, 0, 0, selItem);
		}
		UpdateData(TRUE);
	}


}//启动时的初始化加载


void CMFCApplication3Dlg::LoadXmlchange(){
	using namespace tinyxml2;
	HTREEITEM selItem;
	//获得选择项
	selItem = m_tree.GetSelectedItem();
	//获取选中的内容
	CString cs = m_tree.GetItemText(selItem);
	if (cs.Find(_T("通道")) > -1) {
		
		tinyxml2::XMLDocument doc;
		if (doc.LoadFile("ConfigFile.xml"))
		{
			doc.PrintError();
			return;
		}
		// 根元素
		XMLElement* chaneelconfig = doc.RootElement();
		XMLElement* channel = chaneelconfig->FirstChildElement("Channel");
		if (channel == nullptr) {
			return;
		}
		CString no = CString(channel->Attribute("No"));
		while (channel->NextSibling() != nullptr) {
			if (cs.Find(no) > -1) {
				break;
			}
			else {
				channel = channel->NextSiblingElement();
				no = CString(channel->Attribute("No"));
			}
		}
		if (!(cs.Find(no) > -1)) {
			return;
		}
		XMLElement* CommMode = channel->FirstChildElement("CommMode");
		CString baud = CString(CommMode->Attribute("Baud"));
		CString datas = CString(CommMode->Attribute("DataSize"));
		CString sp = CString(CommMode->Attribute("Stop"));
		CString pari = CString(CommMode->Attribute("Parity"));
		m_csbaud = baud;
		m_csds = datas;
		m_csparity = pari;
		m_cssp = sp;
	}

}
//当页面切换时，对其进行加载



void CMFCApplication3Dlg::savexml(CString cs,HTREEITEM selItem) {
	if (m_csbaud.IsEmpty() || m_csds.IsEmpty() || m_csparity.IsEmpty() || m_cssp.IsEmpty()) {
		MessageBox(_T("参数不全，保存通道配置失败！"));
		return;
	}
	tinyxml2::XMLDocument doc;
	CString cno = cs;
	cno.TrimRight(_T("通道"));
	char* noBuff = (LPSTR)(LPCTSTR)cno;
	if (doc.LoadFile("ConfigFile.xml"))
		{
			const char* xmlContent = xmltitle;
			doc.Parse(xmlContent);//添加ChannelConfig节点
			tinyxml2::XMLElement* ChannelConfig = doc.NewElement("ChannelConfig");
			doc.InsertEndChild(ChannelConfig);
			//添加ChannelConfig节点

			tinyxml2::XMLElement* Channel = doc.NewElement("Channel");
			Channel->SetAttribute("No", noBuff);
			tinyxml2::XMLElement* CommMode = doc.NewElement("CommMode");
			setchannel(CommMode);
			ChannelConfig->InsertEndChild(Channel);
			Channel->InsertEndChild(CommMode);
			
			setdevice(selItem, &doc, Channel);
			//保存成XML文件
			doc.SaveFile("ConfigFile.xml");
			MessageBox(_T("保存配置文件成功"));
			return;
		}
	else {
			tinyxml2::XMLElement* chaneelconfig = doc.RootElement();
			tinyxml2::XMLElement* channel = chaneelconfig->FirstChildElement("Channel");
			if (channel == nullptr) {
				tinyxml2::XMLElement* Channel = doc.NewElement("Channel");
				Channel->SetAttribute("No", noBuff);
				tinyxml2::XMLElement* CommMode = doc.NewElement("CommMode");

				setchannel(CommMode);

				chaneelconfig->InsertEndChild(Channel);
				Channel->InsertEndChild(CommMode);
				
				setdevice(selItem, &doc, Channel);

				doc.SaveFile("ConfigFile.xml");
				MessageBox(_T("保存配置文件成功"));
				return;
			}
			CString no = CString(channel->Attribute("No"));
			while (channel->NextSibling() != nullptr) {
				if (cs.Find(no) > -1) {
					break;
				}
				else {
					channel = channel->NextSiblingElement();
					no = CString(channel->Attribute("No"));
				}
			}
			if (!(cs.Find(no) > -1)) {
				tinyxml2::XMLElement* Channel = doc.NewElement("Channel");
				Channel->SetAttribute("No", noBuff);
				tinyxml2::XMLElement* CommMode = doc.NewElement("CommMode");

				setchannel(CommMode);

				chaneelconfig->InsertEndChild(Channel);
				Channel->InsertEndChild(CommMode);
				setdevice(selItem, &doc, Channel);
				doc.SaveFile("ConfigFile.xml");
				MessageBox(_T("保存配置文件成功"));
				return;
			}
			else {
				tinyxml2::XMLElement* CommMode = channel->FirstChildElement("CommMode");
				setchannel(CommMode);
				setdevice(selItem, &doc,channel);
				doc.SaveFile("ConfigFile.xml");
				MessageBox(_T("保存配置文件成功"));
				return;
			}
		}
	return;
}


void CMFCApplication3Dlg::saveDevicexml(CString cs) {
}


void CMFCApplication3Dlg::setdevice(HTREEITEM selItem, tinyxml2::XMLDocument* doc,tinyxml2::XMLElement* Channel) {
	HTREEITEM dev = NULL;
	char ch[128] = { 0 };
	CString temp = _T(".xml");
	tinyxml2::XMLElement* CommMode = Channel->FirstChildElement("CommMode");
	if (m_tree.ItemHasChildren(selItem)) {
		tinyxml2::XMLElement* devsource = nullptr;
		tinyxml2::XMLElement* devnode = doc->NewElement("Device");
		dev = m_tree.GetChildItem(selItem);
		getval(ch, m_tree.GetItemText(dev));
		int flagdev = -1;
		if (CommMode->NextSibling() != nullptr) {
			devsource = Channel->FirstChildElement("Device");
			if (strcmp(ch,devsource->Attribute("Name"))==0) {
				flagdev = 1;
			}
			while (devsource->NextSibling()!=nullptr&&flagdev==-1)
			{
				devsource = devsource->NextSiblingElement();
				if (strcmp(ch, devsource->Attribute("Name")) == 0) {
					flagdev = 1;
				}
			}
			if (flagdev == -1){
				devnode->SetAttribute("Name", ch);
				getval(ch, (m_tree.GetItemText(dev) + temp));
				devnode->SetAttribute("File", ch);
				Channel->InsertEndChild(devnode);
			}
		}
		else
		{
			devnode->SetAttribute("Name", ch);
			getval(ch, (m_tree.GetItemText(dev) + temp));
			devnode->SetAttribute("File", ch);
			Channel->InsertEndChild(devnode);
		}
	}
	else {
		return;
	}
	while (m_tree.GetNextSiblingItem(dev) != NULL)
	{
		tinyxml2::XMLElement* devsource = nullptr;
		tinyxml2::XMLElement* devnode = doc->NewElement("Device");
		dev = m_tree.GetNextSiblingItem(dev);
		getval(ch, m_tree.GetItemText(dev));
		int flagdev = -1;
		if (CommMode->NextSibling() != nullptr) {
			devsource = Channel->FirstChildElement("Device");
			if (strcmp(ch, devsource->Attribute("Name")) == 0) {
				flagdev = 1;
			}
			while (devsource->NextSibling() != nullptr && flagdev == -1)
			{
				devsource = devsource->NextSiblingElement();
				if (strcmp(ch, devsource->Attribute("Name")) == 0) {
					flagdev = 1;
				}
			}
			if (flagdev == -1) {
				devnode->SetAttribute("Name", ch);
				getval(ch, (m_tree.GetItemText(dev) + temp));
				devnode->SetAttribute("File", ch);
				Channel->InsertEndChild(devnode);
			}
		}
		else
		{
			devnode->SetAttribute("Name", ch);
			getval(ch, (m_tree.GetItemText(dev) + temp));
			devnode->SetAttribute("File", ch);
			Channel->InsertEndChild(devnode);
		}
	}
}

void CMFCApplication3Dlg::getval(char* ch, CString source) {
	for (int i = 0; i <= source.GetLength(); i++) {
		ch[i] = source[i];
	}
	return;
}

void CMFCApplication3Dlg::setchannel(tinyxml2::XMLElement* CommMode) {
	char ch[128] = { 0 };
	getval(ch, m_csbaud);
	CommMode->SetAttribute("Baud", ch);
	getval(ch, m_csds);
	CommMode->SetAttribute("DataSize", ch);
	getval(ch, m_cssp);
	CommMode->SetAttribute("Stop", ch);
	getval(ch, m_csparity);
	CommMode->SetAttribute("Parity", ch);
}

