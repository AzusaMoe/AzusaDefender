// Rocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "Rocket.h"
#include "afxdialogex.h"


// CRocket �Ի���

IMPLEMENT_DYNAMIC(CRocket, CDialogEx)

CRocket::CRocket(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG13, pParent)
{

}

CRocket::~CRocket()
{
}

void CRocket::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRocket, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CRocket ��Ϣ�������


BOOL CRocket::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ȡͼƬ��С��Ϣ
	m_bitmap.LoadBitmapW(IDB_BITMAP1);
	m_bitmap.GetBitmap(&m_bitinfo);
	// ���ô��ں�ͼƬһ����
	CRect rt = { 1832,962,1832+m_bitinfo.bmWidth,962+m_bitinfo.bmHeight };
	MoveWindow(&rt);


	// ���ô��ڷ��Ϊ��͸��
	LONG lStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, lStyle | WS_EX_LAYERED);
	// ����͸����
	SetLayeredWindowAttributes(RGB(255, 255, 255), 0, LWA_COLORKEY);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CRocket::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SendMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CRocket::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	dcMem.SelectObject(m_bitmap);
	dc.BitBlt(0, 0, m_bitinfo.bmWidth, m_bitinfo.bmHeight, &dcMem, 0, 0, SRCCOPY);
	CDialogEx::OnPaint();
}


void CRocket::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//::PostMessage(,WM_COMMAND, MAKEWPARAM(0x000003F4, BN_CLICKED), NULL);
	//HWND hWND = GetParent()->m_hWnd;
	::PostMessage(m_hParent, WM_COMMAND, IDC_BUTTON1, NULL);
	for (int i=1831;i>=-88;--i)
	{
		CRect rt = { i,962,i + m_bitinfo.bmWidth,962 + m_bitinfo.bmHeight };
		MoveWindow(&rt);
		Sleep(0.5);
	}
	for (int i = 1920; i >= 1832; --i)
	{
		CRect rt = { i,962,i + m_bitinfo.bmWidth,962 + m_bitinfo.bmHeight };
		MoveWindow(&rt);
		Sleep(0.5);
	}
	CDialogEx::OnLButtonDblClk(nFlags, point);
}
