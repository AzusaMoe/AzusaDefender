
// AzusaDefenderDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "AzusaDefenderDlg.h"
#include "afxdialogex.h"
#include "DiaA.h"
#include "DiaB.h"
#include "DiaC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CAzusaDefenderDlg �Ի���



CAzusaDefenderDlg::CAzusaDefenderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_AZUSADEFENDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAzusaDefenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_ctrlTab);
}

BEGIN_MESSAGE_MAP(CAzusaDefenderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CAzusaDefenderDlg ��Ϣ�������

BOOL CAzusaDefenderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ctrlTab.InsertItem(0, L"�ҵĹܼ�");
	m_ctrlTab.InsertItem(1, L"���������");
	m_ctrlTab.InsertItem(2, L"���������");
	//���Ӵ���ָ�븳ֵ
	m_ctrlTab.m_Dia[0] = new CDiaA();
	m_ctrlTab.m_Dia[1] = new CDiaB();
	m_ctrlTab.m_Dia[2] = new CDiaC();
	//�����Ӵ���
	m_ctrlTab.m_Dia[0]->Create(IDD_DIALOG1, &m_ctrlTab);
	m_ctrlTab.m_Dia[1]->Create(IDD_DIALOG2, &m_ctrlTab);
	m_ctrlTab.m_Dia[2]->Create(IDD_DIALOG4, &m_ctrlTab);
	//�����Ӵ��ڵĴ�С
	CRect rc;
	m_ctrlTab.GetClientRect(rc);
	rc.DeflateRect(2, 23, 2, 2);
	m_ctrlTab.m_Dia[0]->MoveWindow(rc);
	m_ctrlTab.m_Dia[1]->MoveWindow(rc);
	m_ctrlTab.m_Dia[2]->MoveWindow(rc);
	//��ʾ��һ������
	m_ctrlTab.m_Dia[0]->ShowWindow(SW_SHOW);
	m_ctrlTab.m_Dia[1]->ShowWindow(SW_HIDE);
	m_ctrlTab.m_Dia[2]->ShowWindow(SW_HIDE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CAzusaDefenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CAzusaDefenderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAzusaDefenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
