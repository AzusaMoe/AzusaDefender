#pragma once
#include "afxcmn.h"


// CExportTable �Ի���

class CExportTable : public CDialogEx
{
	DECLARE_DYNAMIC(CExportTable)

public:
	CExportTable(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CExportTable();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG7 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_strEdit;
	CString m_strEdit2;
	CString m_strEdit3;
	CString m_strEdit4;
	CString m_strEdit5;
	CString m_strEdit6;
	CString m_strEdit7;
	CListCtrl m_ctrlList;
	virtual BOOL OnInitDialog();
};
