#pragma once
#include "afxcmn.h"


// CImportTable �Ի���

class CImportTable : public CDialogEx
{
	DECLARE_DYNAMIC(CImportTable)

public:
	CImportTable(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImportTable();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG8 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrl m_ctrlList;
	CListCtrl m_ctrlList2;
};
