#pragma once


// CDiaC �Ի���

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
};
