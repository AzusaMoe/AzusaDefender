#pragma once


// CDiaPe �Ի���

class CDiaPe : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaPe)

public:
	CDiaPe(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaPe();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString m_strEdit;
	static CString strFilePath;
};
