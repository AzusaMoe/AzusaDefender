// DiaA.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "DiaA.h"
#include "afxdialogex.h"
#include <Psapi.h>
#include "DiaVs.h"
#include "DiaPe.h"


// CDiaA �Ի���

IMPLEMENT_DYNAMIC(CDiaA, CDialogEx)

CDiaA::CDiaA(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_boolCheck(FALSE)
	, m_boolCheck2(FALSE)
	, m_boolCheck3(FALSE)
	, m_isCheck(FALSE)
	, m_isCheck2(FALSE)
	, m_isCheck3(FALSE)
{

}

CDiaA::~CDiaA()
{
}

void CDiaA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
	DDX_Control(pDX, IDC_PROGRESS2, m_ctrlProgress2);
	DDX_Control(pDX, IDC_STATIC1, m_ctrlStatic);
	DDX_Control(pDX, IDC_STATIC2, m_ctrlStatic2);
	DDX_Check(pDX, IDC_CHECK1, m_boolCheck);
	DDX_Check(pDX, IDC_CHECK2, m_boolCheck2);
	DDX_Check(pDX, IDC_CHECK3, m_boolCheck3);
	DDX_Control(pDX, IDC_LIST1, m_ctrlListBox);
	DDX_Control(pDX, IDC_STATIC3, m_ctrlStatic3);
	DDX_Control(pDX, IDC_STATIC4, m_ctrlStatic4);
}


BEGIN_MESSAGE_MAP(CDiaA, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaA::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDiaA::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDiaA::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDiaA::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDiaA::OnBnClickedButton5)
END_MESSAGE_MAP()


// CDiaA ��Ϣ�������


BOOL CDiaA::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//��ʼ��CPUʹ��״̬����������
	INT cpuUserate = GetCpuUserate();
	m_ctrlProgress.SetRange(0, 100);
	m_ctrlProgress.SetPos(cpuUserate);
	CString strStatic;
	strStatic.Format(_T("%d"), cpuUserate);
	m_ctrlStatic.SetWindowTextW(strStatic);

	//��ʼ���ڴ�ʹ��״̬����������
	DWORD memUserate = GetMemUserate();
	m_ctrlProgress2.SetRange(0, 100);
	m_ctrlProgress2.SetPos(memUserate);
	CString strStatic2;
	strStatic2.Format(_T("%d"), memUserate);
	m_ctrlStatic2.SetWindowTextW(strStatic2);
	//�����߳���ʵϰ���CPU���ڴ��ʹ��״̬
	CreateMonitorThread();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

//************************************
// Method:    GetCpuUserate
// FullName:  CDiaA::GetCpuUserate
// Access:    public 
// Returns:   BOOL
// Qualifier: ��ȡCPUռ����
//************************************
INT CDiaA::GetCpuUserate()
{
	//��һ�λ�ȡ������ʱ��
	FILETIME idleTime, kernelTime, userTime;
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	//�����ں˶��󣬲��ҵȴ�1000����
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);
	//�ٴλ�ȡ������ʱ��
	FILETIME preidleTime, prekernelTime, preuserTime;
	GetSystemTimes(&preidleTime, &prekernelTime, &preuserTime);
	//ת������ʱ�����
	double dbIdleTime, dbKernelTime, dbUserTime, dbPreidleTime, dbPrekernelTime, dbPreuserTime;

	dbIdleTime = (double)(idleTime.dwHighDateTime*4.294967296E9) + (double)idleTime.dwLowDateTime;
	dbKernelTime = (double)(kernelTime.dwHighDateTime*4.294967296E9) + (double)kernelTime.dwLowDateTime;
	dbUserTime = (double)(userTime.dwHighDateTime*4.294967296E9) + (double)userTime.dwLowDateTime;

	dbPreidleTime = (double)(preidleTime.dwHighDateTime*4.294967296E9) + (double)preidleTime.dwLowDateTime;
	dbPrekernelTime = (double)(prekernelTime.dwHighDateTime*4.294967296E9) + (double)prekernelTime.dwLowDateTime;
	dbPreuserTime = (double)(preuserTime.dwHighDateTime*4.294967296E9) + (double)preuserTime.dwLowDateTime;
	
	INT cpuUserate = (INT)(100.0 - (dbPreidleTime - dbIdleTime) / (dbPrekernelTime - dbKernelTime + dbPreuserTime - dbUserTime)*100.0);
	return cpuUserate;
}

//************************************
// Method:    GetMemUserate
// FullName:  CDiaA::GetMemUserate
// Access:    public 
// Returns:   BOOL
// Qualifier: ��ȡ�ڴ�ʹ����
//************************************
DWORD CDiaA::GetMemUserate()
{
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	GlobalMemoryStatusEx(&stcMemStatusEx);
	//DWORDLONG preUsedMem = stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	DWORD memUserate = stcMemStatusEx.dwMemoryLoad;
	return memUserate;
}

//************************************
// Method:    ClearUpMem
// FullName:  CDiaA::ClearUpMem
// Access:    public 
// Returns:   BOOL
// Qualifier: �����ڴ�
//************************************
BOOL CDiaA::ClearUpMem()
{
	DWORD dwPIDList[1000];
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA, false, dwPIDList[i]);
		SetProcessWorkingSetSize(hProcess, -1, -1);
	}
	return TRUE;
}


//�����ڴ�
void CDiaA::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ClearUpMem();
	//�����ڴ�ʹ��״̬����������
	DWORD memUserate = GetMemUserate();
	m_ctrlProgress2.SetPos(memUserate);
	CString strStatic2;
	strStatic2.Format(_T("%d"), memUserate);
	m_ctrlStatic2.SetWindowTextW(strStatic2);
}

//�����߳�
void CDiaA::CreateMonitorThread()
{
	CreateThread(NULL, NULL, MonitorThread, (LPVOID)this, NULL, NULL);
}

//�̻߳ص�����������ʵʱ���CPU���ڴ�״̬
DWORD WINAPI CDiaA::MonitorThread(LPVOID lpParam)
{
	CDiaA *pThis = (CDiaA*)lpParam;
	CString strStatic,strStatic2;
	INT cpuUserate;
	DWORD memUserate;
	while (true)
	{
		//ˢ��CPUʹ��״̬������
		cpuUserate = pThis->GetCpuUserate();
		pThis->m_ctrlProgress.SetPos(cpuUserate);
		strStatic.Format(_T("%d"), cpuUserate);
		pThis->m_ctrlStatic.SetWindowTextW(strStatic);

		//ˢ���ڴ�ʹ��״̬������
		memUserate = pThis->GetMemUserate();
		pThis->m_ctrlProgress2.SetPos(memUserate);
		strStatic2.Format(_T("%d"), memUserate);
		pThis->m_ctrlStatic2.SetWindowTextW(strStatic2);
		Sleep(1000);
	}
}

//����VS���������߶Ի���
void CDiaA::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDiaVs* diaVs = new CDiaVs;
	diaVs->Create(IDD_DIALOG3, this);
	diaVs->ShowWindow(SW_SHOW);
}

//��Ӧ��ʼ����������ť
void CDiaA::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);

	if (m_boolCheck && !m_isCheck)
	{
		m_isCheck = TRUE;
		SHQUERYRBINFO RecycleBininfo = {};
		RecycleBininfo.cbSize = sizeof(RecycleBininfo);
		SHQueryRecycleBin(NULL, &RecycleBininfo);
		CString cstrSize =ByteConversionGBMBKB(RecycleBininfo.i64Size);
		CString cstrNumItems;
		cstrNumItems.Format(_T("%d"), RecycleBininfo.i64NumItems);
		m_ctrlStatic3.SetWindowTextW(cstrSize);
		m_ctrlStatic4.SetWindowTextW(cstrNumItems);
		m_ctrlStatic3.ShowWindow(TRUE);
		m_ctrlStatic4.ShowWindow(TRUE);
	}
	if (m_boolCheck2 && !m_isCheck2)//ϵͳ����·��
	{
		m_isCheck2 = TRUE;
		m_vecPath.push_back(_T("C:\\Windows\\Temp"));
		m_vecPath.push_back(_T("C:\\Users\\aimomo\\AppData\\Local\\Temp"));
		m_vecPath.push_back(_T("C:\\Users\\aimomo\\AppData\\Local\\Microsoft\\Windows\\WER\\ReportQueue"));
	}
	if (m_boolCheck3 && !m_isCheck3)//���������·��
	{
		m_isCheck3 = TRUE;
		m_vecPath.push_back(_T("C:\\Users\\aimomo\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\Cache"));
		m_vecPath.push_back(_T("C:\\Users\\aimomo\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\GPUCache"));
		m_vecPath.push_back(_T("C:\\Users\\aimomo\\AppData\\Local\\Google\\Chrome\\User Data\\ShaderCache\\GPUCache"));
		m_vecPath.push_back(_T("C:\\Users\\aimomo\\AppData\\Local\\Google\\Chrome\\User Data\\Default\\JumpListIcons"));
	}
	//����֮ǰ�����֮ǰ�������ļ���Ϣ���б������
	m_vecFile.clear();
	m_ctrlListBox.ResetContent();
	for (auto vecPathObj : m_vecPath)//����
	{
		TraverseFile(vecPathObj.GetBuffer(), m_vecFile);
	}
	int i = 0;
	for (auto vecFileObj : m_vecFile)//��ӵ�listbox�ؼ�
	{
		m_ctrlListBox.AddString(vecFileObj);
		m_ctrlListBox.SelectString(i, vecFileObj);
		i++;
	}
}


//����Ҫ����������ļ���
void CDiaA::TraverseFile(
	TCHAR  szPath[MAX_PATH], // Ҫ�������ļ�·��
	vector<CString> &vecPath)// ������������ļ�
{
	// ���ļ���ĩβ���\\*�����������ļ�
	CString strPath = szPath;
	strPath += L"\\*";
	WIN32_FIND_DATA fi = {};
	// �ҵ����ļ���Ϣ�ŵ�fi����
	HANDLE hFile = FindFirstFile(strPath, &fi);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		// �ж��Ƿ�ǰĿ¼���ϼ�Ŀ¼
		if (!lstrcmp(fi.cFileName, L".") || !lstrcmp(fi.cFileName, L".."))
		{
			continue;
		}
		// �ж��Ƿ����ļ��У����ǣ��ݹ����
		if (fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString strDIr = szPath;
			strDIr += L"\\";
			strDIr += fi.cFileName;
			TraverseFile(strDIr.GetBuffer(), vecPath);
			continue;
		}
		// ��������ɸѡ�������ҵ�Ҫɾ�����ļ����ˣ���֮ǰ��·�����������·��
		CString strDIr = szPath;
		strDIr += L"\\";
		strDIr += fi.cFileName;
		// �����·��������
		vecPath.push_back(strDIr);
	} while (FindNextFile(hFile, &fi));

}


//�ֽ�תKB��MB��GB
CString CDiaA::ByteConversionGBMBKB(__int64 KSize)
{
	const int GB = 1024 * 1024 * 1024;//����GB�ļ��㳣��
	const int MB = 1024 * 1024;//����MB�ļ��㳣��
	const int KB = 1024;//����KB�ļ��㳣��
	CString strObj;
	if (KSize / GB >= 1)//�����ǰByte��ֵ���ڵ���1GB
	{
		strObj.Format(_T("%0.1f"), round(KSize / (float)GB));
		return strObj + _T("GB");//����ת����GB
	}
	else if (KSize / MB >= 1)//�����ǰByte��ֵ���ڵ���1MB
	{
		strObj.Format(_T("%0.1f"), round(KSize / (float)MB));
		return strObj + _T("MB");//����ת����MB
	}
	else if (KSize / KB >= 1)//�����ǰByte��ֵ���ڵ���1KB
	{
		strObj.Format(_T("%0.1f"), round(KSize / (float)KB));
		return strObj + _T("KB");//����ת����KB
	}
	else
	{
		strObj.Format(_T("%0.1d"), KSize);
		return strObj + _T("Byte");//��ʾByteֵ
	}
}

//ɾ�����������ļ�
void CDiaA::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (DWORD i = 0; i < m_vecFile.size(); i++)
	{
		DeleteFile(m_vecFile[i].GetBuffer());
		m_ctrlListBox.DeleteString(0);
	}
	m_vecFile.swap(vector<CString>());
}

//����lordpe����
void CDiaA::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDiaPe* diaPe = new CDiaPe;
	diaPe->Create(IDD_DIALOG6, this);
	diaPe->ShowWindow(SW_SHOW);
}
