#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDiaA �Ի���

class CDiaA : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaA)

public:
	CDiaA(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaA();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	INT GetCpuUserate();
	DWORD GetMemUserate();
	BOOL ClearUpMem();
	CProgressCtrl m_ctrlProgress;
	CStatic m_ctrlStatic;
	CProgressCtrl m_ctrlProgress2;
	CStatic m_ctrlStatic2;
	afx_msg void OnBnClickedButton1();
	void CreateMonitorThread();
	static DWORD WINAPI MonitorThread(LPVOID lpParam);
};