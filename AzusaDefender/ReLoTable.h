#pragma once
#include "afxcmn.h"


// CReLoTable �Ի���

class CReLoTable : public CDialogEx
{
	DECLARE_DYNAMIC(CReLoTable)

public:
	CReLoTable(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReLoTable();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG10 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctrlList;
	CListCtrl m_ctrlList2;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
