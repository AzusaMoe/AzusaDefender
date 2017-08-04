#pragma once
#include "afxcmn.h"
#include <vector>
using std::vector;

//�������Ӽ���ֵ
typedef struct _RUNINFO
{
	WCHAR Name[50];//����
	WCHAR Value[100];//ֵ
}RUNINFO, *PRUNINFO;

//�������ӽ���Ϣ
typedef struct _RUNCHILDKEYINFO
{
	HKEY RootKey;
	CString strMidReg;
	vector<RUNINFO> vecRunInfo;
}RUNCHILDKEYINFO, *PRUNCHILDKEYINFO;

// CRunInfo �Ի���

class CRunInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CRunInfo)

public:
	CRunInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRunInfo();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG15 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL GetRunInfo(HKEY& rootKey, LPCTSTR lpSubKey, HKEY& hkResult);
	CListCtrl m_ctrlList;
	vector<RUNCHILDKEYINFO> m_vecRunChildInfo;
};
