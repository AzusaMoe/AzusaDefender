// ExportTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "ExportTable.h"
#include "afxdialogex.h"
#include "LordPe.h"
#include "DiaPe.h"


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
	// ��ʾ������Ϣ
	CLordPe lordPe(CDiaPe::m_strFilePath);
	lordPe.ExportTable();
	m_strEdit = lordPe.m_my_im_ex_di.name;
	m_strEdit2.Format(L"%08X", lordPe.m_my_im_ex_di.Base);
	m_strEdit3.Format(L"%08X", lordPe.m_my_im_ex_di.NumberOfFunctions);
	m_strEdit4.Format(L"%08X", lordPe.m_my_im_ex_di.NumberOfNames);
	m_strEdit5.Format(L"%08X", lordPe.m_my_im_ex_di.AddressOfFunctions);
	m_strEdit6.Format(L"%08X", lordPe.m_my_im_ex_di.AddressOfNames);
	m_strEdit7.Format(L"%08X", lordPe.m_my_im_ex_di.AddressOfNameOrdinals);
	UpdateData(FALSE);
	
	//��ʼ��listCtrl
	m_ctrlList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[4] = { L"�������", L"RVA", L"ƫ��", L"������" };
	for (int i = 0; i < 4; ++i)
	{
		m_ctrlList.InsertColumn(i, szCol[i], 0, 100);
	}
	//����item
	m_ctrlList.DeleteAllItems();
	DWORD i = 0;
	for (EXPORTFUNINFO &exportFunInfo : lordPe.m_vecExportFunInfo)
	{
		CString temp;
		temp.Format(L"%08X", exportFunInfo.ExportOrdinals);
		m_ctrlList.InsertItem(i, temp);
		temp.Format(L"%08X", exportFunInfo.FunctionRVA);
		m_ctrlList.SetItemText(i, 1, temp);
		temp.Format(L"%08X", exportFunInfo.FunctionOffset);
		m_ctrlList.SetItemText(i, 2, temp);
		m_ctrlList.SetItemText(i, 3, exportFunInfo.FunctionName);
		++i;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
