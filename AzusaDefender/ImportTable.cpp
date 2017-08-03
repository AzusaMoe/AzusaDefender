// ImportTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "ImportTable.h"
#include "LordPe.h"
#include "afxdialogex.h"
#include "DiaPe.h"


// CImportTable �Ի���

IMPLEMENT_DYNAMIC(CImportTable, CDialogEx)

CImportTable::CImportTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG8, pParent)
{

}

CImportTable::~CImportTable()
{
}

void CImportTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
	DDX_Control(pDX, IDC_LIST2, m_ctrlList2);
}


BEGIN_MESSAGE_MAP(CImportTable, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CImportTable::OnNMDblclkList1)
END_MESSAGE_MAP()


// CImportTable ��Ϣ�������


BOOL CImportTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��listCtrl
	m_ctrlList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[5] = { L"DLL����", L"INT RVA",L"INTƫ��", L"IAT RVA", L"IATƫ��" };
	for (int i = 0; i < 5; ++i)
	{
		m_ctrlList.InsertColumn(i, szCol[i], 0, 100);
	}
	//��ʼ��listCtrl2
	m_ctrlList2.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol2[2] = { L"���", L"����"};
	for (int i = 0; i < 2; ++i)
	{
		m_ctrlList2.InsertColumn(i, szCol2[i], 0, 100);
	}

	CLordPe lordPe(CDiaPe::m_strFilePath);
	lordPe.ImportTable();
	m_ctrlList.DeleteAllItems();
	DWORD i = 0;
	CString temp;
	for (MY_IMPORT_DESCRIPTOR &importDescriptor : lordPe.m_vecImportDescriptor)
	{
		m_ctrlList.InsertItem(i, importDescriptor.Name);
		temp.Format(L"%08X", importDescriptor.OriginalFirstThunk);
		m_ctrlList.SetItemText(i, 1, temp);
		temp.Format(L"%08X", importDescriptor.OffsetOriginalFirstThunk);
		m_ctrlList.SetItemText(i, 2, temp);
		temp.Format(L"%08X", importDescriptor.FirstThunk);
		m_ctrlList.SetItemText(i, 3, temp);
		temp.Format(L"%08X", importDescriptor.OffsetFirstThunk);
		m_ctrlList.SetItemText(i, 4, temp);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CImportTable::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ctrlList2.DeleteAllItems();
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;  // �����˫����item�����
	 //int nSubItem = pNMListView->iSubItem; // ���Ƕ�Ӧ�������

	CLordPe lordPe(CDiaPe::m_strFilePath);
	lordPe.ImportTable();
	m_ctrlList2.DeleteAllItems();
	DWORD i = 0;
	CString temp;
	for (auto& each : lordPe.m_vvImportFunInfo[nItem])
	{
		temp.Format(L"%08X", each.Ordinal);
		m_ctrlList2.InsertItem(i, temp);
		m_ctrlList2.SetItemText(i, 1, each.Name);
	}
	*pResult = 0;
}
