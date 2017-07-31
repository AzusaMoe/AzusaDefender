// DiaA.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "DiaA.h"
#include "afxdialogex.h"
#include <Psapi.h>


// CDiaA �Ի���

IMPLEMENT_DYNAMIC(CDiaA, CDialogEx)

CDiaA::CDiaA(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CDiaA::~CDiaA()
{
}

void CDiaA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
	DDX_Control(pDX, IDC_STATIC1, m_ctrlStatic);
	DDX_Control(pDX, IDC_PROGRESS2, m_ctrlProgress2);
	DDX_Control(pDX, IDC_STATIC2, m_ctrlStatic2);
}


BEGIN_MESSAGE_MAP(CDiaA, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaA::OnBnClickedButton1)
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

void CDiaA::CreateMonitorThread()
{
	CreateThread(NULL, NULL, MonitorThread, (LPVOID)this, NULL, NULL);
}

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
