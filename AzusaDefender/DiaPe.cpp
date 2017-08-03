// DiaPe.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "DiaPe.h"
#include "LordPe.h"
#include "afxdialogex.h"
#include "ExportTable.h"

// CDiaPe �Ի���

IMPLEMENT_DYNAMIC(CDiaPe, CDialogEx)
CString CDiaPe::m_strFilePath = L"";
CDiaPe::CDiaPe(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG6, pParent)
	, m_strEdit(_T(""))
	, m_strEdit2(_T(""))
	, m_strEdit3(_T(""))
	, m_strEdit4(_T(""))
	, m_strEdit5(_T(""))
	, m_strEdit6(_T(""))
	, m_strEdit7(_T(""))
	, m_strEdit8(_T(""))
	, m_strEdit9(_T(""))
	, m_strEdit10(_T(""))
	, m_strEdit11(_T(""))
	, m_strEdit12(_T(""))
	, m_strEdit13(_T(""))
	, m_strEdit14(_T(""))
	, m_strEdit15(_T(""))
	, m_strEdit16(_T(""))
{

}

CDiaPe::~CDiaPe()
{
}

void CDiaPe::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strEdit);
	DDX_Text(pDX, IDC_EDIT2, m_strEdit2);
	DDX_Text(pDX, IDC_EDIT3, m_strEdit3);
	DDX_Text(pDX, IDC_EDIT4, m_strEdit4);
	DDX_Text(pDX, IDC_EDIT5, m_strEdit5);
	DDX_Text(pDX, IDC_EDIT6, m_strEdit6);
	DDX_Text(pDX, IDC_EDIT7, m_strEdit7);
	DDX_Text(pDX, IDC_EDIT8, m_strEdit8);
	DDX_Text(pDX, IDC_EDIT9, m_strEdit9);
	DDX_Text(pDX, IDC_EDIT10, m_strEdit10);
	DDX_Text(pDX, IDC_EDIT11, m_strEdit11);
	DDX_Text(pDX, IDC_EDIT12, m_strEdit12);
	DDX_Text(pDX, IDC_EDIT13, m_strEdit13);
	DDX_Text(pDX, IDC_EDIT14, m_strEdit14);
	DDX_Text(pDX, IDC_EDIT15, m_strEdit15);
	DDX_Text(pDX, IDC_EDIT16, m_strEdit16);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
	DDX_Control(pDX, IDC_LIST2, m_ctrlList2);
}


BEGIN_MESSAGE_MAP(CDiaPe, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaPe::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDiaPe::OnBnClickedButton2)
END_MESSAGE_MAP()


// CDiaPe ��Ϣ�������
BOOL CDiaPe::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ʼ��list��Ϣ
	m_ctrlList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[2] = { L"��������ַ", L"��С"};
	for (int i = 0; i < 2; ++i)
	{
		m_ctrlList.InsertColumn(i, szCol[i], 0, 100);
	}
	// ��ʼ��list2��Ϣ
	m_ctrlList2.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol2[6] = { L"������", L"ʵ�ʴ�С",L"�ļ��д�С",L"�ļ���λ��",L"�ڴ���λ��",L"��������" };
	for (int i = 0; i < 6; ++i)
	{
		m_ctrlList2.InsertColumn(i, szCol2[i], 0, 100);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDiaPe::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OPENFILENAME ofn;      // �����Ի���ṹ��     
	TCHAR szFile[MAX_PATH]; // �����ȡ�ļ����ƵĻ�������               
	// ��ʼ��ѡ���ļ��Ի���     
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All(*.*)\0*.*\0EXE(*.exe)\0*.exe\0DLL(*.dll)\0*.dll\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	//ofn.lpTemplateName =  MAKEINTRESOURCE(ID_TEMP_DIALOG);    
	// ��ʾ��ѡ���ļ��Ի���     

	if (GetOpenFileName(&ofn))
	{
		//��ʾѡ����ļ���     
		m_strEdit = szFile;
		m_strFilePath = szFile;
		UpdateData(FALSE);
	}
	UpdateDataToEdits();
}

void CDiaPe::UpdateDataToEdits()
{
	CLordPe lordPe(m_strFilePath);
	lordPe.GetBasicInfo();
	m_strEdit2.Format(L"%08X", lordPe.m_basicInfo.NumberOfSections);
	m_strEdit3.Format(L"%08X", lordPe.m_basicInfo.SizeOfOptionalHeader);
	m_strEdit4.Format(L"%08X", lordPe.m_basicInfo.Characteristics);
	m_strEdit5.Format(L"%08X", lordPe.m_basicInfo.Magic);
	m_strEdit6.Format(L"%08X", lordPe.m_basicInfo.SizeOfCode);
	m_strEdit7.Format(L"%08X", lordPe.m_basicInfo.AddressOfEntryPoint);
	m_strEdit8.Format(L"%08X", lordPe.m_basicInfo.BaseOfCode);
	m_strEdit9.Format(L"%08X", lordPe.m_basicInfo.BaseOfData);
	m_strEdit10.Format(L"%08X", lordPe.m_basicInfo.ImageBase);
	m_strEdit11.Format(L"%08X", lordPe.m_basicInfo.SectionAlignment);
	m_strEdit12.Format(L"%08X", lordPe.m_basicInfo.FileAlignment);
	m_strEdit13.Format(L"%08X", lordPe.m_basicInfo.SizeOfImage);
	m_strEdit14.Format(L"%08X", lordPe.m_basicInfo.SizeOfHeaders);
	m_strEdit15.Format(L"%08X", lordPe.m_basicInfo.DllCharacteristics);
	m_strEdit16.Format(L"%08X", lordPe.m_basicInfo.NumberOfRvaAndSizes);
	UpdateData(FALSE);
	//��������Ŀ¼����Ϣ
	m_ctrlList.DeleteAllItems();
	DWORD i = 0;//����
	CString temp;//ת����
	for (DataTableOfContents &vecDataTable : lordPe.m_vecDataTable)
	{
		temp.Format(L"%08X", vecDataTable.VirtualAddress);
		m_ctrlList.InsertItem(i, temp);
		temp.Format(L"%08X", vecDataTable.Size);
		m_ctrlList.SetItemText(i, 1, temp);
		++i;
	}
	//�������α���Ϣ
	m_ctrlList2.DeleteAllItems();
	DWORD j = 0;
	CString temp2;
	for (SectionTable &vecSectionTable : lordPe.m_vecSectionTable)
	{
		m_ctrlList2.InsertItem(j, vecSectionTable.Name);
		temp2.Format(L"%08X", vecSectionTable.VirtualSize);
		m_ctrlList2.SetItemText(j, 1, temp2);
		temp2.Format(L"%08X", vecSectionTable.SizeOfRawData);
		m_ctrlList2.SetItemText(j, 2, temp2);
		temp2.Format(L"%08X", vecSectionTable.PointerToRawData);
		m_ctrlList2.SetItemText(j, 3, temp2);
		temp2.Format(L"%08X", vecSectionTable.PointerToVirData);
		m_ctrlList2.SetItemText(j, 4, temp2);
		temp2.Format(L"%08X", vecSectionTable.Characteristics);
		m_ctrlList2.SetItemText(j, 5, temp2);
		++j;
	}
}




void CDiaPe::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CExportTable* exportTable = new CExportTable;
	exportTable->Create(IDD_DIALOG7, this);
	exportTable->ShowWindow(SW_SHOW);
}
