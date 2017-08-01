// DiaC.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "DiaC.h"
#include "afxdialogex.h"
#include <winsvc.h>


// CDiaC �Ի���

IMPLEMENT_DYNAMIC(CDiaC, CDialogEx)

CDiaC::CDiaC(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

CDiaC::~CDiaC()
{
}

void CDiaC::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
}



BEGIN_MESSAGE_MAP(CDiaC, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaC::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CDiaC::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CDiaC::OnBnClickedButton2)
END_MESSAGE_MAP()



BOOL CDiaC::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ʼ��list��Ϣ
	m_ctrlList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[5] = { L"����", L"״̬", L"��������", L"��������", L"·����Ϣ" };
	for (int i = 0; i < 5; ++i)
	{
		m_ctrlList.InsertColumn(i, szCol[i], 0, 100);
	}
	//ö�ٷ���
	enumService();
	InsertServiceList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

// CDiaC ��Ϣ�������
//��������
void CDiaC::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD dwItem;
	for (int i = 0; i < m_ctrlList.GetItemCount(); i++)
	{
		if (m_ctrlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			dwItem = i;
			break;
		}
	}
	CString strState;
	CString strName;
	strState = m_ctrlList.GetItemText(dwItem, 1);
	strName = m_ctrlList.GetItemText(dwItem, 0);
	if (strState==L"��������")
	{
		AfxMessageBox(L"�����������У���������");
	}
	else
	{
		//�򿪼����������ƹ�����
		SC_HANDLE hSvc = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		SC_HANDLE hSC = OpenService(hSvc, strName, SERVICE_QUERY_CONFIG| SERVICE_START);
		// ��������  
		if (::StartService(hSC, NULL, NULL) == FALSE)
		{
			AfxMessageBox(L"��������ʧ��");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		AfxMessageBox(L"��������ɹ�");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		//ˢ�·����б�
		m_vecServiceInfo.clear();//��ձ���ķ�����Ϣ
		enumService();
		InsertServiceList();
	}
}

//ֹͣ����
void CDiaC::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DWORD dwItem;
	for (int i = 0; i < m_ctrlList.GetItemCount(); i++)
	{
		if (m_ctrlList.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED)
		{
			dwItem = i;
			break;
		}
	}
	CString strState;
	CString strName;
	strState = m_ctrlList.GetItemText(dwItem, 1);
	strName = m_ctrlList.GetItemText(dwItem, 0);
	if (strState == L"��ֹͣ")
	{
		AfxMessageBox(L"����Ϊֹͣ״̬������ֹͣ");
	}
	else
	{
		//�򿪼����������ƹ�����
		SC_HANDLE hSvc = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		SC_HANDLE hSC = OpenService(hSvc, strName, SERVICE_QUERY_CONFIG | SERVICE_STOP);
		// ֹͣ���� 
		SERVICE_STATUS status;
		if (::ControlService(hSC,
			SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			AfxMessageBox(L"ֹͣ����ʧ��");
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return;
		}
		AfxMessageBox(L"ֹͣ����ɹ�");
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		//ˢ�·����б�
		m_vecServiceInfo.clear();//��ձ���ķ�����Ϣ
		enumService();
		InsertServiceList();
	}
}


// �߼�ʵ�ֲ���

//ö�ٷ��񣬻�ȡ��Ϣ
BOOL CDiaC::enumService()
{
	//�򿪼����������ƹ�����
	SC_HANDLE hsCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//��һ�ε��ã���ȡ��Ҫ���ڴ��С
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx(hsCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32,SERVICE_STATE_ALL, NULL, 0, &dwSize, &dwServiceNum, NULL, NULL);
	//�����ڴ�
	LPENUM_SERVICE_STATUS_PROCESS pEnumService = (LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	//�ٴε���,ö�ٳ�����
	BOOL bStatus = FALSE;
	bStatus = EnumServicesStatusEx(hsCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
									(PBYTE)pEnumService, dwSize, &dwSize, &dwServiceNum, NULL, NULL);
	if (!bStatus)//ö��ʧ���򷵻�
	{
		::CloseServiceHandle(hsCM);
		return FALSE;
	}
	for (DWORD i = 0; i < dwServiceNum; i++)
	{
		//��ȡ������Ϣ,���浽�ṹ����
		SERVICEINFO serviceInfo;
		//������
		serviceInfo.cstrSerName = pEnumService[i].lpServiceName;
		//����״̬
		switch (pEnumService[i].ServiceStatusProcess.dwCurrentState)
		{
		case SERVICE_CONTINUE_PENDING:
			serviceInfo.cstrSerState = _T("��������");
			break;
		case SERVICE_PAUSE_PENDING:
			serviceInfo.cstrSerState = _T("������ͣ");
			break;
		case SERVICE_PAUSED:
			serviceInfo.cstrSerState = _T("����ͣ");
			break;
		case SERVICE_RUNNING:
			serviceInfo.cstrSerState = _T("��������");
			break;
		case SERVICE_START_PENDING:
			serviceInfo.cstrSerState = _T("��������");
			break;
		case SERVICE_STOP_PENDING:
			serviceInfo.cstrSerState = _T("����ֹͣ");
			break;
		case SERVICE_STOPPED:
			serviceInfo.cstrSerState = _T("��ֹͣ");
			break;
		default:
			break;
		}
		//��������
		switch (pEnumService[i].ServiceStatusProcess.dwServiceType)
		{
		case SERVICE_FILE_SYSTEM_DRIVER:
			serviceInfo.cstrSerType = _T("�ļ�ϵͳ��������");
			break;
		case SERVICE_KERNEL_DRIVER:
			serviceInfo.cstrSerType = _T("�豸��������");
			break;
		case SERVICE_WIN32_OWN_PROCESS:
			serviceInfo.cstrSerType = _T("�������Լ��Ľ�����");
			break;
		case SERVICE_WIN32_SHARE_PROCESS:
			serviceInfo.cstrSerType = _T("���������������");
			break;
		default:
			break;
		}
		//�򿪷����ȡ������Ϣ
		SC_HANDLE hService = OpenService(hsCM, pEnumService[i].lpServiceName, SERVICE_QUERY_CONFIG);
		//��һ�ε��û�ȡ��Ҫ�Ļ�������С
		QueryServiceConfig(hService, NULL, 0, &dwSize);
		//�����ڴ�
		LPQUERY_SERVICE_CONFIG pServiceConfig = (LPQUERY_SERVICE_CONFIG)new char[dwSize];
		//�ٴε��ã���ȡ��Ϣ
		QueryServiceConfig(hService, pServiceConfig, dwSize, &dwSize);
		//��ȡ��Ϣ����ṹ�����
		switch (pServiceConfig->dwStartType)
		{
		case SERVICE_AUTO_START:
			serviceInfo.cstrStartType = _T("�Զ�");
			break;
		case SERVICE_BOOT_START:
			serviceInfo.cstrStartType = _T("��ϵͳ���س�������");
			break;
		case SERVICE_DEMAND_START:
			serviceInfo.cstrStartType = _T("�ֶ�");
			break;
		case SERVICE_DISABLED:
			serviceInfo.cstrStartType = _T("����");
			break;
		case SERVICE_SYSTEM_START:
			serviceInfo.cstrStartType = _T("��IoInitSystem��������");
			break;
		default:
			break;
		}
		//·����Ϣ
		serviceInfo.cstrBinaryPathName = pServiceConfig->lpBinaryPathName;
		m_vecServiceInfo.push_back(serviceInfo);
	}
	return TRUE;
}

BOOL CDiaC::InsertServiceList()
{
	m_ctrlList.DeleteAllItems();
	int i = 0;//������
	CString temp;
	// L"����", L"״̬", L"��������", L"��������", L"·����Ϣ"
	for (SERVICEINFO &serviceInfo : m_vecServiceInfo) {
		// ��1��,����
		m_ctrlList.InsertItem(i, serviceInfo.cstrSerName);
		// ��2��,״̬
		m_ctrlList.SetItemText(i, 1, serviceInfo.cstrSerState);
		// ��3��,��������
		m_ctrlList.SetItemText(i, 2,serviceInfo.cstrSerType);
		// ��4��,��������
		m_ctrlList.SetItemText(i, 3, serviceInfo.cstrStartType);
		// ��5��,·����Ϣ
		m_ctrlList.SetItemText(i, 4, serviceInfo.cstrBinaryPathName);
		++i;
	}
	return TRUE;
}




void CDiaC::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ö�ٷ���
	m_vecServiceInfo.clear();//��ձ���ķ�����Ϣ
	enumService();
	InsertServiceList();
}



