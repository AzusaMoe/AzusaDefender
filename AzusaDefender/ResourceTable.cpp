// ResourceTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "ResourceTable.h"
#include "afxdialogex.h"
#include "LordPe.h"
#include "DiaPe.h"


// CResourceTable �Ի���

IMPLEMENT_DYNAMIC(CResourceTable, CDialogEx)

CResourceTable::CResourceTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG9, pParent)
{

}

CResourceTable::~CResourceTable()
{
}

void CResourceTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
	DDX_Control(pDX, IDC_LIST2, m_ctrlList2);
}


BEGIN_MESSAGE_MAP(CResourceTable, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CResourceTable::OnNMDblclkList1)
END_MESSAGE_MAP()


// CResourceTable ��Ϣ�������


BOOL CResourceTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��listCtrl
	m_ctrlList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ctrlList.InsertColumn(0, L"��Դ����", 0, 100);
	//��ʼ��listCtrl2
	m_ctrlList2.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol2[3] = { L"��ԴID������", L"��ԴRVA", L"��Դ��С" };
	for (int i = 0; i < 3; ++i)
	{
		m_ctrlList2.InsertColumn(i, szCol2[i], 0, 100);
	}

	CLordPe lordPe(CDiaPe::m_strFilePath);
	lordPe.ResourceTable();
	m_ctrlList.DeleteAllItems();
	DWORD i = 0;
	for (auto& each : lordPe.m_vecResourceTpye)
	{
		m_ctrlList.InsertItem(i, each);
		++i;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CResourceTable::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ctrlList2.DeleteAllItems();
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;  // �����˫����item�����
	//int nSubItem = pNMListView->iSubItem; // ���Ƕ�Ӧ�������

	CLordPe lordPe(CDiaPe::m_strFilePath);
	lordPe.ResourceTable();
	m_ctrlList2.DeleteAllItems();
	DWORD i = 0;
	CString temp;
	for (auto& each : lordPe.m_vvResourceInfo[nItem])
	{
		m_ctrlList2.InsertItem(i, each.NameOrID);
		temp.Format(L"%08X", each.ResourceRVA);
		m_ctrlList2.SetItemText(i, 1, temp);
		temp.Format(L"%08X", each.ResourceSize);
		m_ctrlList2.SetItemText(i, 2, temp);
		++i;
	}
	*pResult = 0;
}
