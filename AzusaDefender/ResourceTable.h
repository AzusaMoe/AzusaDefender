#pragma once
#include "afxcmn.h"


// CResourceTable �Ի���

class CResourceTable : public CDialogEx
{
	DECLARE_DYNAMIC(CResourceTable)

public:
	CResourceTable(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CResourceTable();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG9 };
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
