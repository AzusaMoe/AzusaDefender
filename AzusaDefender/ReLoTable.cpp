// ReLoTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "ReLoTable.h"
#include "afxdialogex.h"
#include "LordPe.h"
#include "DiaPe.h"


// CReLoTable �Ի���

IMPLEMENT_DYNAMIC(CReLoTable, CDialogEx)

CReLoTable::CReLoTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG10, pParent)
{

}

CReLoTable::~CReLoTable()
{
}

void CReLoTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
	DDX_Control(pDX, IDC_LIST2, m_ctrlList2);
}


BEGIN_MESSAGE_MAP(CReLoTable, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CReLoTable::OnNMDblclkList1)
END_MESSAGE_MAP()


// CReLoTable ��Ϣ�������


BOOL CReLoTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��listCtrl
	m_ctrlList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[3] = { L"������", L"�ض�λ��RVA", L"�ض�λ����" };
	for (int i = 0; i < 3; ++i)
	{
		m_ctrlList.InsertColumn(i, szCol[i], 0, 100);
	}
	//��ʼ��listCtrl2
	m_ctrlList2.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol2[4] = { L"���ض�λRVA", L"���ض�λƫ��", L"�ض�λ����", L"�����ַ" };
	for (int i = 0; i < 3; ++i)
	{
		m_ctrlList2.InsertColumn(i, szCol2[i], 0, 100);
	}

	CLordPe lordPe(CDiaPe::m_strFilePath);
	lordPe.ReLoTable();
	m_ctrlList.DeleteAllItems();
	DWORD i = 0;
	CString temp;
	for (auto& each : lordPe.m_vecReloInfo)
	{
		m_ctrlList.InsertItem(i, each.szSectionName);
		temp.Format(L"%08X", each.dwAreaRVA);
		m_ctrlList.SetItemText(i, 1, temp);
		temp.Format(L"%08X", each.dwNumberofReloc);
		m_ctrlList.SetItemText(i, 2, temp);
		++i;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CReLoTable::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ctrlList2.DeleteAllItems();
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;  // �����˫����item�����
									 //int nSubItem = pNMListView->iSubItem; // ���Ƕ�Ӧ�������

	CLordPe lordPe(CDiaPe::m_strFilePath);
	lordPe.ReLoTable();
	m_ctrlList2.DeleteAllItems();
	DWORD i = 0;
	CString temp;
	for (auto& each : lordPe.m_vecReloInfo[nItem].vecRelocInfo)
	{
		temp.Format(L"%08X", each.dwRelocRVA);
		m_ctrlList2.InsertItem(i, temp);
		temp.Format(L"%08X", each.dwOffect);
		m_ctrlList2.SetItemText(i, 1, temp);
		temp.Format(L"%08X", each.dwType);
		m_ctrlList2.SetItemText(i, 2, temp);
		temp.Format(L"%08X", each.dwRelicValue);
		m_ctrlList2.SetItemText(i, 2, temp);
		++i;
	}
	*pResult = 0;

}
