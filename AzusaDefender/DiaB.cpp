// DiaB.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "DiaB.h"
#include "afxdialogex.h"
#include <Psapi.h>
#include<iostream>  
#include<fstream>
#include <string>
using namespace std;

// ���ַ�ת��Ϊ���ַ�(Unicode --> ASCII)
#define  WCHAR_TO_CHAR(lpW_Char, lpChar) \
    WideCharToMultiByte(CP_ACP, NULL, lpW_Char, -1, lpChar, _countof(lpChar), NULL, FALSE);


// ���ַ�ת��Ϊ���ַ�(ASCII --> Unicode)
#define  CHAR_TO_WCHAR(lpChar, lpW_Char) \
    MultiByteToWideChar(CP_ACP, NULL, lpChar, -1, lpW_Char, _countof(lpW_Char));

// CDiaB �Ի���

IMPLEMENT_DYNAMIC(CDiaB, CDialogEx)

CDiaB::CDiaB(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CDiaB::~CDiaB()
{
}

void CDiaB::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
	DDX_Control(pDX, IDC_LIST2, m_ctrlList2);
	DDX_Control(pDX, IDC_LIST3, m_ctrlList3);
	DDX_Control(pDX, IDC_LIST4, m_ctrlList4);
}


BEGIN_MESSAGE_MAP(CDiaB, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CDiaB::OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaB::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CDiaB::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ʼ��list��Ϣ
	m_ctrlList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[6] = { L"��������", L"PID", L"������PID", L"�߳�����", L"���ȼ�", L"��������·��" };
	for (int i = 0; i < 6; ++i)
	{
		m_ctrlList.InsertColumn(i, szCol[i], 0, 100);
	}
	// ��ʼ��list2��Ϣ
	m_ctrlList2.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	TCHAR* szCol2[3] = { L"�߳�ID",L"����ID",L"���ȼ�" };
	for (int i = 0; i < 3; ++i)
	{
		m_ctrlList2.InsertColumn(i, szCol2[i], 0, 80);
	}
	// ��ʼ��list3��Ϣ
	m_ctrlList3.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol3[4] = { L"ģ����",L"��ַ",L"ģ���С",L"·��" };
	for (int i = 0; i < 4; ++i)
	{
		m_ctrlList3.InsertColumn(i, szCol3[i], 0, 80);
	}
	// ��ʼ��list4��Ϣ
	m_ctrlList4.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ctrlList4.InsertColumn(0, L"������", 0, 80);
	// �����߳���Ϣ
	EnumProcess();
	InsertProcessList(m_vecProcess);
	//����������Ϣ
	EnumWindow();
	InsertWindowInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}



// CDiaB ��Ϣ�������

//************************************
// Method:    EnumProcess
// FullName:  CDiaB::EnumProcess
// Access:    protected 
// Returns:   BOOL
// Qualifier: �������̼���Ϣ�����浽vector��
//************************************
BOOL CDiaB::EnumProcess()
{
	// ���
	m_vecProcess.clear();
	// ����
	HANDLE hProcessSnap;//���̿��վ��
	HANDLE hProcess;//���̾��
	PROCESSENTRY32 stcPe32 = { 0 };//���̿�����Ϣ
	stcPe32.dwSize = sizeof(PROCESSENTRY32);
	//1.����һ��������صĿ��վ��
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE) return FALSE;
	//2ͨ�����̿��վ����ȡ��һ��������Ϣ
	if (!Process32First(hProcessSnap, &stcPe32)) {
		CloseHandle(hProcessSnap);
		return FALSE;
	}
	//3ѭ������������Ϣ
	do {
		MY_PROCESSINFO stcInfo = { 0 };
		//3.1��ȡ������
		wcscpy_s(stcInfo.szProcess, MAX_PATH, stcPe32.szExeFile);
		//3.2��ȡ����·�������ȼ���Ϣ
		//hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, NULL, stcPe32.th32ProcessID);
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, NULL, stcPe32.th32ProcessID);
		if (hProcess) {
			// ��ȡ��������·��
			DWORD dwSize = MAX_PATH;
			QueryFullProcessImageName(hProcess, 0, stcInfo.szFullProcess, &dwSize);
			//GetProcessImageFileName(hProcess, stcInfo.szFullProcess, MAX_PATH);
			stcInfo.dwPriorityClass = GetPriorityClass(hProcess);//��ȡ�������ȼ�
			CloseHandle(hProcess);
		}
		//3.3��ȡ���̵����������Ϣ
		stcInfo.dwPid = stcPe32.th32ProcessID;
		stcInfo.dwThreadCount = stcPe32.cntThreads;
		stcInfo.dwParentProcess = stcPe32.th32ParentProcessID;
		//3.4����ȡ���������Ϣ���浽������
		m_vecProcess.push_back(stcInfo);
	} while (Process32Next(hProcessSnap, &stcPe32));
	CloseHandle(hProcessSnap);
	return TRUE;
}

//************************************
// Method:    InsertProcessList
// FullName:  CDiaB::InsertProcessList
// Access:    protected 
// Returns:   BOOL
// Qualifier: �����ݲ��뵽�б�ؼ���
//************************************
BOOL CDiaB::InsertProcessList(vector<MY_PROCESSINFO>& vecProcess)
{
	m_ctrlList.DeleteAllItems();
	int i = 0;//������
	CString temp;
	// L"��������",L"PID",L"������PID",L"�߳�����",L"���ȼ�",L"��������·��"
	for (MY_PROCESSINFO &stc : vecProcess) {
		// ��1��,��������
		m_ctrlList.InsertItem(i, stc.szProcess);
		// ��2��,PID
		temp.Format(L"%d", stc.dwPid);
		m_ctrlList.SetItemText(i, 1, temp);
		// ��3��,������PID
		temp.Format(L"%d", stc.dwParentProcess);
		m_ctrlList.SetItemText(i, 2, temp);
		// ��4��,�߳�����
		temp.Format(L"%d", stc.dwThreadCount);
		m_ctrlList.SetItemText(i, 3, temp);
		// ��5��,���ȼ�
		temp.Format(L"%d", stc.dwPriorityClass);
		m_ctrlList.SetItemText(i, 4, temp);
		// ��6��,��������·��
		m_ctrlList.SetItemText(i, 5, stc.szFullProcess);
		++i;
	}
	return TRUE;
}

// CDlgThread ��Ϣ�������
// �����߳�
BOOL CDiaB::EnumThread(DWORD dwPid)
{
	m_vecThread.clear();
	//1.�����߳̿���
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnap == INVALID_HANDLE_VALUE) return 0;
	//2ͨ���߳̿��վ����ȡ��һ���߳���Ϣ
	THREADENTRY32 stcThread = { sizeof(THREADENTRY32) };
	if (!Thread32First(hSnap, &stcThread)) {
		CloseHandle(hSnap);
		return 0;
	}
	//3ѭ�������߳���Ϣ
	do {
		if (stcThread.th32OwnerProcessID != dwPid) {
			continue;
		}
		m_vecThread.push_back(stcThread);
	} while (Thread32Next(hSnap, &stcThread));
	CloseHandle(hSnap);
	return 0;
}

// �����߳���Ϣ
BOOL CDiaB::InsertThreadInfo()
{
	//List�ؼ�����
	CString str;
	int i = 0;
	for (THREADENTRY32 &thread : m_vecThread) {
		//�߳�ID
		str.Format(L"%d", thread.th32ThreadID);
		m_ctrlList2.InsertItem(i, str);
		//����ID
		str.Format(L"%d", thread.th32OwnerProcessID);
		m_ctrlList2.SetItemText(i, 1, str);
		//���ȼ�
		str.Format(L"%d", thread.tpBasePri);
		m_ctrlList2.SetItemText(i, 2, str);
	}
	return TRUE;
}


void CDiaB::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_ctrlList2.DeleteAllItems();
	m_ctrlList3.DeleteAllItems();
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = pNMListView->iItem;  // �����˫����item�����
	//int nSubItem = pNMListView->iSubItem; // ���Ƕ�Ӧ�������
	CString sText = m_ctrlList.GetItemText(nItem,1);
	DWORD dwPId = _ttoi(sText);
	EnumThread(dwPId);
	InsertThreadInfo();
	EnumModule(dwPId);
	InsertModuleInfo();
	*pResult = 0;
}

// ����ģ����Ϣ
BOOL CDiaB::EnumModule(DWORD dwPid)
{
	m_vecModule.clear();
	//1.1�򿪽���
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPid);
	if (NULL == hProcess) {
		return FALSE;
	}
	//1.2ȷ������ģ�����������С
	HMODULE hModules[0x1] = { 0 };//ģ������
	DWORD cbNeed = 0;//ʵ�ʻ�ȡ�Ĵ�С
	EnumProcessModulesEx(/*ö�ٽ���ģ��*/
		hProcess,//���̾��
		hModules,//ģ��������
		sizeof(hModules),//ģ��������Ĵ�С
		&cbNeed,//ʵ����Ҫ�������С
		LIST_MODULES_ALL);//ö��ģ�������
						  //1.3��ȡģ������
	DWORD dwModCount = cbNeed / sizeof(HMODULE);//ģ������
	HMODULE* pModBuf = new HMODULE[dwModCount];//����ģ�����Ļ���
	EnumProcessModulesEx(/*ö�ٽ���ģ��*/
		hProcess,//���̾��
		pModBuf,//ģ��������
		dwModCount * sizeof(HMODULE),//ģ��������Ĵ�С
		&cbNeed,//ʵ����Ҫ�������С
		LIST_MODULES_ALL);//ö��ģ�������
						  //2.ö�ٽ���ģ����Ϣ
						  //char szModuleName[0x200] = {0};
	MODULEENTRY32 stcMod32 = { sizeof(MODULEENTRY32) };
	MODULEINFO stcModInfo;
	//3.����vec�ռ�
	for (UINT i = 0; i < dwModCount; i++) {
		GetModuleBaseName(hProcess, pModBuf[i], stcMod32.szModule, MAX_PATH);
		GetModuleFileNameEx(hProcess, pModBuf[i], stcMod32.szExePath, MAX_PATH);
		GetModuleInformation(hProcess, pModBuf[i], &stcModInfo, sizeof(MODULEINFO));
		stcMod32.hModule = pModBuf[i];
		stcMod32.modBaseAddr = (PBYTE)stcModInfo.lpBaseOfDll;
		stcMod32.modBaseSize = stcModInfo.SizeOfImage;
		stcMod32.th32ProcessID = dwPid;
		m_vecModule.push_back(stcMod32);
	}
	CloseHandle(hProcess);
	delete[] pModBuf;
	return TRUE;
}

// ����ģ����Ϣ
BOOL CDiaB::InsertModuleInfo()
{
	//List�ؼ�����
	CString str;
	int i = 0;
	for (MODULEENTRY32 &module : m_vecModule) {
		//ģ����
		m_ctrlList3.InsertItem(i, module.szModule);
		//��ַ
		str.Format(L"%08X", module.modBaseAddr);
		m_ctrlList3.SetItemText(i, 1, str);
		//ģ���С
		str.Format(L"%d", module.modBaseSize);
		m_ctrlList3.SetItemText(i, 2, str);
		//����·��
		m_ctrlList3.SetItemText(i, 3, module.szExePath);
		i++;
	}
	return TRUE;
}

BOOL CDiaB::EnumWindow()
{
	HWND hWnd = ::GetDesktopWindow();
	hWnd = ::GetWindow(hWnd, GW_CHILD);
	WCHAR szName[266] = { 0 };
	CString nameObj;
	while (hWnd!=NULL)
	{
		::GetWindowText(hWnd, szName, 266);
		nameObj = szName;
		m_vecStrName.push_back(nameObj);
		hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT);
	}
	return TRUE;
}

// ���봰����Ϣ
BOOL CDiaB::InsertWindowInfo()
{
	m_ctrlList4.DeleteAllItems();
	//List�ؼ�����
	int i = 0;
	for (CString & strName : m_vecStrName) {
		//������
		if (!strName.IsEmpty())
		{
			m_ctrlList4.InsertItem(i, strName);
		}
		i++;
	}
	return TRUE;
}

void CDiaB::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	GetEvilProcessInfo();
	InsertProcessList(m_vecEvilProcess);
}

//���ڰ���������Ϣ��ȡ����Ӧvector��
void CDiaB::ReadWBList()
{
	//��ȡ�����Ű��������ļ���Ϣ
	string buf;
	CStringA strObj;
	CString wstrObj;
	WCHAR wstrBuf[100] = { 0 };
	ifstream infile;
	infile.open(_T("whitelist.txt"));
	if (!infile)
	{
		return;
	}
	//���ж�ȡֱ����ȡ���
	while (getline(infile, buf))
	{
		strObj.Format("%s", buf.c_str());
		CHAR_TO_WCHAR(strObj.GetBuffer(), wstrBuf);
		wstrObj = wstrBuf;
		m_vecWhitelist.push_back(wstrObj);
	}
	infile.close();
	//��ȡ�����ź��������ļ���Ϣ
	string buf2;
	CStringA strObj2;
	CString wstrObj2;
	WCHAR wstrBuf2[100] = { 0 };
	ifstream infile2;
	infile2.open(_T("blacklist.txt"));
	if (!infile2)
	{
		return;
	}
	//���ж�ȡֱ����ȡ���
	while (getline(infile2, buf2))
	{
		strObj2.Format("%s", buf2.c_str());
		CHAR_TO_WCHAR(strObj2.GetBuffer(), wstrBuf2);
		wstrObj2 = wstrBuf2;
		m_vecBlacklist.push_back(wstrObj2);
	}
	infile2.close();
}

//Ѱ�Ҷ������
void CDiaB::GetEvilProcessInfo()
{
	//���ڰ�������Ϣ
	ReadWBList();
	//�ȶԽ�����
	CString strProName;
	for (auto vecProcess : m_vecProcess)//���н�����Ϣ
	{
		MY_PROCESSINFO stcInfo = { 0 };
		strProName = vecProcess.szProcess;
		for (auto vecBlacklist : m_vecBlacklist)//������������
		{
			if (vecBlacklist == strProName)
			{
				BOOL isNotInWL = TRUE;
				//�����ڲ��ڰ�������
				for (auto vecWhitelist : m_vecWhitelist)//������������
				{
					if (vecWhitelist == strProName)
					{
						isNotInWL = FALSE;
						break;
					}
				}
				//���ڰ�������
				if (isNotInWL)
				{
					//����Ϣת��
					stcInfo.dwParentProcess = vecProcess.dwParentProcess;
					stcInfo.dwPid = vecProcess.dwPid;
					stcInfo.dwPriorityClass = vecProcess.dwPriorityClass;
					stcInfo.dwThreadCount = vecProcess.dwThreadCount;
					wcscpy_s(stcInfo.szFullProcess, MAX_PATH, vecProcess.szFullProcess);
					wcscpy_s(stcInfo.szProcess, MAX_PATH, vecProcess.szProcess);
					m_vecEvilProcess.push_back(stcInfo);
					break;
				}
				else
				{
					break;
				}
			}
		}
	}
}

