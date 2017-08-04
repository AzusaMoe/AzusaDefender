// TlsTable.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "TlsTable.h"
#include "afxdialogex.h"
#include "LordPe.h"
#include "DiaPe.h"


// CTlsTable �Ի���

IMPLEMENT_DYNAMIC(CTlsTable, CDialogEx)

CTlsTable::CTlsTable(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG12, pParent)
	, m_strEdit(_T(""))
	, m_strEdit2(_T(""))
	, m_strEdit3(_T(""))
	, m_strEdit4(_T(""))
	, m_strEdit5(_T(""))
	, m_strEdit6(_T(""))
{

}

CTlsTable::~CTlsTable()
{
}

void CTlsTable::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strEdit);
	DDX_Text(pDX, IDC_EDIT2, m_strEdit2);
	DDX_Text(pDX, IDC_EDIT3, m_strEdit3);
	DDX_Text(pDX, IDC_EDIT4, m_strEdit4);
	DDX_Text(pDX, IDC_EDIT5, m_strEdit5);
	DDX_Text(pDX, IDC_EDIT6, m_strEdit6);
}


BEGIN_MESSAGE_MAP(CTlsTable, CDialogEx)
END_MESSAGE_MAP()


// CTlsTable ��Ϣ�������


BOOL CTlsTable::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CLordPe lordPe(CDiaPe::m_strFilePath);
	lordPe.TLSTable();
	m_strEdit.Format(L"%08X", lordPe.m_tlsInfo.StartAddressOfRawData);
	m_strEdit2.Format(L"%08X", lordPe.m_tlsInfo.EndAddressOfRawData);
	m_strEdit3.Format(L"%08X", lordPe.m_tlsInfo.AddressOfIndex);
	m_strEdit4.Format(L"%08X", lordPe.m_tlsInfo.AddressOfCallBacks);
	m_strEdit5.Format(L"%08X", lordPe.m_tlsInfo.SizeOfZeroFill);
	m_strEdit6.Format(L"%08X", lordPe.m_tlsInfo.Characteristics);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}