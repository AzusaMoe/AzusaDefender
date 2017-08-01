#pragma once
#include <vector>
#include "afxcmn.h"
using std::vector;

// CDiaC �Ի���
typedef struct _SERVICEINFO
{
	CString cstrSerName;
	CString cstrSerType;
	CString cstrSerState;
	CString cstrStartType;
	CString cstrBinaryPathName;
}SERVICEINFO, *PSERVICEINFO;

class CDiaC : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaC)

public:
	CDiaC(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaC();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL enumService();
	BOOL InsertServiceList();
	vector<SERVICEINFO> m_vecServiceInfo;
	virtual BOOL OnInitDialog();
	CListCtrl m_ctrlList;
	afx_msg void OnBnClickedButton1();
};
