#pragma once


// CRocket �Ի���

class CRocket : public CDialogEx
{
	DECLARE_DYNAMIC(CRocket)

public:
	CRocket(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRocket();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG13 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CBitmap m_bitmap;
	BITMAP m_bitinfo;
	HWND m_hParent;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
