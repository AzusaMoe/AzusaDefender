// ExportTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "ExportTable.h"
#include "afxdialogex.h"
#include "LordPe.h"


// CExportTable �Ի���

IMPLEMENT_DYNAMIC(CExportTable, CDialogEx)

CExportTable::CExportTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG7, pParent)
	, m_strEdit(_T(""))
	, m_strEdit2(_T(""))
	, m_strEdit3(_T(""))
	, m_strEdit4(_T(""))
	, m_strEdit5(_T(""))
	, m_strEdit6(_T(""))
	, m_strEdit7(_T(""))
{

}

CExportTable::~CExportTable()
{
}

void CExportTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strEdit);
	DDX_Text(pDX, IDC_EDIT2, m_strEdit2);
	DDX_Text(pDX, IDC_EDIT3, m_strEdit3);
	DDX_Text(pDX, IDC_EDIT4, m_strEdit4);
	DDX_Text(pDX, IDC_EDIT5, m_strEdit5);
	DDX_Text(pDX, IDC_EDIT6, m_strEdit6);
	DDX_Text(pDX, IDC_EDIT7, m_strEdit7);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
}


BEGIN_MESSAGE_MAP(CExportTable, CDialogEx)
END_MESSAGE_MAP()


// CExportTable ��Ϣ�������


BOOL CExportTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ʼ��list��Ϣ
	m_ctrlList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[4] = { L"�������", L"RVA", L"ƫ��", L"������"};
	for (int i = 0; i < 4; ++i)
	{
		m_ctrlList.InsertColumn(i, szCol[i], 0, 100);
	}
	CLordPe lordPe;
	/////////////////����һ�½����ĺ���/////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
