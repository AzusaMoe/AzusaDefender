#pragma once
#include <vector>
#include "afxcmn.h"
#include <TlHelp32.h>
using std::vector;

typedef struct _MY_PROCESSINFO
{
	DWORD     dwPid;//PID
	TCHAR     szProcess[MAX_PATH];//����
	TCHAR     szFullProcess[MAX_PATH];//ȫ·��
	DWORD     dwPriorityClass;//���ȼ�
	DWORD     dwParentProcess;//��PID
	DWORD     dwThreadCount;//�߳���
}MY_PROCESSINFO;

// CDiaB �Ի���

class CDiaB : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaB)

public:
	CDiaB(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaB();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	BOOL EnumProcess();//ö�ٽ���
	BOOL InsertProcessList(vector<MY_PROCESSINFO>& vecProcess);
	BOOL EnumThread(DWORD dwPid);
	BOOL InsertThreadInfo();
	vector<MY_PROCESSINFO> m_vecProcess;
	vector<THREADENTRY32> m_vecThread;
	vector<MODULEENTRY32> m_vecModule;
	vector<CString> m_vecStrName;
	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	CListCtrl m_ctrlList;
	CListCtrl m_ctrlList2;
	CListCtrl m_ctrlList3;
	CListCtrl m_ctrlList4;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	BOOL EnumModule(DWORD dwPid);
	BOOL InsertModuleInfo();
	BOOL EnumWindow();
	BOOL InsertWindowInfo();
	afx_msg void OnBnClickedButton1();
	vector<CString> m_vecWhitelist;
	vector<CString> m_vecBlacklist;
	vector<MY_PROCESSINFO> m_vecEvilProcess;
	void ReadWBList();
	void GetEvilProcessInfo();
};
