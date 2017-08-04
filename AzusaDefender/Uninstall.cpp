// Uninstall.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "Uninstall.h"
#include "afxdialogex.h"


// CUninstall �Ի���

IMPLEMENT_DYNAMIC(CUninstall, CDialogEx)

CUninstall::CUninstall(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG14, pParent)
{

}

CUninstall::~CUninstall()
{
}

void CUninstall::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
}



BEGIN_MESSAGE_MAP(CUninstall, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CUninstall::OnNMDblclkList1)
END_MESSAGE_MAP()


BOOL CUninstall::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��listCtrl
	m_ctrlList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[2] = { L"�������", L"ж��·��"};
	for (int i = 0; i < 2; ++i)
	{
		m_ctrlList.InsertColumn(i, szCol[i], 0, 100);
	}
	m_vecSoftInfo.clear();
	//����32λ���
	HKEY rootKey = HKEY_LOCAL_MACHINE;
	LPCTSTR lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	HKEY hkResult = 0;
	GetSoftInfo(rootKey, lpSubKey, hkResult);

	//����64λ���
	rootKey = HKEY_LOCAL_MACHINE;
	lpSubKey = L"SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	hkResult = 0;
	GetSoftInfo(rootKey, lpSubKey, hkResult);

	DWORD i = 0;
	CString temp;
	m_ctrlList.DeleteAllItems();
	for (auto softInfo : m_vecSoftInfo)
	{
		temp = softInfo.Name;
		m_ctrlList.InsertItem(i, temp);
		temp = softInfo.UniPath;
		m_ctrlList.SetItemText(i, 1, temp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

// CUninstall ��Ϣ�������


BOOL CUninstall::GetSoftInfo(HKEY& rootKey, LPCTSTR lpSubKey, HKEY& hkResult)
{
	//��һ���Ѿ����ڵ�ע����
	LONG lReturn = RegOpenKeyEx(rootKey, lpSubKey, 0, KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE, &hkResult);
	//ѭ������UninstallĿ¼�µ��Ӽ�
	SOFTINFO softInfo = {0};
	LONG lEnumReturn = 0;
	DWORD dwIndex = 0;
	while (lEnumReturn != ERROR_NO_MORE_ITEMS)
	{
		DWORD dwKeyLen = 255;
		WCHAR szNewKeyName[MAX_PATH] = { 0 };
		lEnumReturn = RegEnumKeyEx(hkResult, dwIndex, szNewKeyName, &dwKeyLen, 0, NULL, NULL, NULL);
		if (lEnumReturn!= ERROR_SUCCESS)
		{
			return FALSE;
		}
		//ͨ���õ����Ӽ�����������ϳ��µ��Ӽ�·��
		WCHAR strMidReg[MAX_PATH] = {0};
		swprintf_s(strMidReg, L"%s%s%s", lpSubKey, L"\\", szNewKeyName);
		//���µ��ӽ�����ȡ����
		HKEY hkValueKey = 0;//�ӽ����
		LONG openKeyReturn = 0;
		openKeyReturn = RegOpenKeyEx(rootKey, strMidReg, 0, KEY_QUERY_VALUE, &hkValueKey);
		if (openKeyReturn != ERROR_SUCCESS)
		{
			return FALSE;
		}
		//��ȡ��ֵ
		DWORD dwNameLen = 255;//ָ��szBuffer�Ĵ�С
		//��ȡ�������
		DWORD dwType = 0;
		LONG queryValueReturn = 0;
		queryValueReturn = RegQueryValueEx(hkValueKey, L"DisplayName", 0, &dwType, (LPBYTE)softInfo.Name, &dwNameLen);
		if (queryValueReturn != ERROR_SUCCESS)
		{
			++dwIndex;
			continue;
		}
		dwNameLen = 255;//���¸�ֵ�����û�����¸�ֵ����һ�ν���ȡ������Ϣ
		//��ж·��
		LONG queryValueReturn2 = 0;
		queryValueReturn2 = RegQueryValueEx(hkValueKey, L"UninstallString", 0, &dwType, (LPBYTE)softInfo.UniPath, &dwNameLen);
		if (queryValueReturn != ERROR_SUCCESS)
		{
			++dwIndex;
			continue;
		}
		dwNameLen = 255;
		m_vecSoftInfo.push_back(softInfo);
		++dwIndex;
	}
	return TRUE;
}




void CUninstall::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;  // �����˫����item�����
	//int nSubItem = pNMListView->iSubItem; // ���Ƕ�Ӧ�������
	CString sText = m_ctrlList.GetItemText(nItem, 1);
	LPCTSTR lpParameters = NULL;
	//ִ��ж�س���
	ShellExecute(NULL, NULL, sText, lpParameters, NULL, SW_SHOW);
	*pResult = 0;
}
