#pragma once
#include "afxcmn.h"


// CReLoTable 对话框

class CReLoTable : public CDialogEx
{
	DECLARE_DYNAMIC(CReLoTable)

public:
	CReLoTable(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReLoTable();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG10 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctrlList;
	CListCtrl m_ctrlList2;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
};
