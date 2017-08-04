#pragma once
#include <vector>
#include "afxcmn.h"
using std::vector;

// CUninstall �Ի���

typedef struct _SOFTINFO
{
	WCHAR Name[50];//����
	WCHAR UniPath[MAX_PATH];//ж��·��
}SOFTINFO,*PSOFTINFO;




class CUninstall : public CDialogEx
{
	DECLARE_DYNAMIC(CUninstall)

public:
	CUninstall(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUninstall();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG14 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	vector<SOFTINFO> m_vecSoftInfo;
	BOOL GetSoftInfo(HKEY& rootKey,LPCTSTR lpSubKey,HKEY& hkResult);
	BOOL GetRunInfo(HKEY& rootKey, LPCTSTR lpSubKey, HKEY& hkResult);
	virtual BOOL OnInitDialog();
	CListCtrl m_ctrlList;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);

};
