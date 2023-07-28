// AI.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication3.h"
#include "afxdialogex.h"
#include "AI.h"
#include"tinyxml2.h"

// AI 对话框

IMPLEMENT_DYNAMIC(AI, CDialogEx)

AI::AI(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_AI, pParent)
{

}

AI::~AI()
{
}

void AI::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_AI, m_AI);
}


BEGIN_MESSAGE_MAP(AI, CDialogEx)
END_MESSAGE_MAP()

// AI 消息处理程序


BOOL AI::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	DWORD dwStyle = m_AI.GetExtendedStyle();                    //添加列表框的网格线！！！

	dwStyle |= LVS_EX_FULLROWSELECT;
	dwStyle |= LVS_EX_GRIDLINES;
	m_AI.SetExtendedStyle(dwStyle);
	m_AI.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 40);              //添加列标题！！！！  这里的80,40,90用以设置列的宽度。！！！LVCFMT_LEFT用来设置对齐方式！！！
	m_AI.InsertColumn(1, _T("点名称"), LVCFMT_LEFT, 50);
	m_AI.InsertColumn(2, _T("数据类型"), LVCFMT_LEFT, 60);
	m_AI.InsertColumn(3, _T("功能码"), LVCFMT_LEFT, 50);
	m_AI.InsertColumn(4, _T("寄存器地址"), LVCFMT_LEFT, 80);
	m_AI.InsertColumn(5, _T("位偏移"), LVCFMT_LEFT, 50);
	m_AI.InsertColumn(6, _T("系数值"), LVCFMT_LEFT, 50);
	m_AI.InsertColumn(7, _T("基数"), LVCFMT_LEFT, 40);
	m_AI.InsertColumn(8, _T("超时时间"), LVCFMT_LEFT, 60);
	m_AI.InsertColumn(9, _T("单位"), LVCFMT_LEFT, 40);
	//Loadxml();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
char* U2G(const char* utf8)
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

void AI::Loadxml(char* ainame,CString cs) {
	char ch[128] = { 0 };
	using namespace tinyxml2;
	tinyxml2::XMLDocument doc;
	getval(ch,cs);
	if (doc.LoadFile(ch))
		{
			doc.PrintError();
			return;
		}
		// 根元素
		XMLElement* devconfig = doc.RootElement();
		XMLElement* AI = devconfig->FirstChildElement("NodeProperty");

		AI = AI->FirstChildElement(ainame);
		if (AI == nullptr) {
			return;
		}
		if (AI->FirstChildElement("Node") == nullptr) {
			return;
		}
		XMLElement* Node = AI->FirstChildElement("Node");
		int count = 0;
		while (Node->NextSibling() != nullptr) {
			m_AI.InsertItem(count, _T(""));
			CString ID = CString(Node->Attribute("Id"));
			CString Name = CString(U2G(Node->Attribute("Name")));
			CString DataType = CString(Node->Attribute("DataType"));
			CString Cmd = CString(Node->Attribute("Cmd"));
			CString RegNo = CString(Node->Attribute("RegNo"));
			CString BitField = CString(Node->Attribute("BitField"));
			CString Factor = CString(Node->Attribute("Factor"));
			CString bFactor = CString(Node->Attribute("bFactor"));
			CString RecvTimeout = CString(Node->Attribute("RecvTimeout"));
			CString EUUnits = CString(U2G(Node->Attribute("EUUnits")));
			m_AI.SetItemText(count, 0, ID);
			m_AI.SetItemText(count, 1, Name);
			m_AI.SetItemText(count, 2, DataType);
			m_AI.SetItemText(count, 3, Cmd);
			m_AI.SetItemText(count, 4, RegNo);
			m_AI.SetItemText(count, 5, BitField);
			m_AI.SetItemText(count, 6, Factor);
			m_AI.SetItemText(count, 7, bFactor);
			m_AI.SetItemText(count, 8, RecvTimeout);
			m_AI.SetItemText(count, 9, EUUnits);
			Node = Node->NextSiblingElement();
			count++;
		}


}
void AI::getval(char* ch, CString source) {
	for (int i = 0; i <= source.GetLength(); i++) {
		ch[i] = source[i];
	}
	return;
}