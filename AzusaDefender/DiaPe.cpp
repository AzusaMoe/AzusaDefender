// DiaPe.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "DiaPe.h"
#include "afxdialogex.h"


// CDiaPe �Ի���

IMPLEMENT_DYNAMIC(CDiaPe, CDialogEx)

CDiaPe::CDiaPe(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG6, pParent)
	, m_strEdit(_T(""))
{

}

CDiaPe::~CDiaPe()
{
}

void CDiaPe::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strEdit);
}


BEGIN_MESSAGE_MAP(CDiaPe, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaPe::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDiaPe ��Ϣ�������


void CDiaPe::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OPENFILENAME ofn;      // �����Ի���ṹ��     
	TCHAR szFile[MAX_PATH]; // �����ȡ�ļ����ƵĻ�������               
	// ��ʼ��ѡ���ļ��Ի���     
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"All(*.*)\0*.*\0EXE(*.exe)\0*.exe\0DLL(*.dll)\0*.dll\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	//ofn.lpTemplateName =  MAKEINTRESOURCE(ID_TEMP_DIALOG);    
	// ��ʾ��ѡ���ļ��Ի���     

	if (GetOpenFileName(&ofn))
	{
		//��ʾѡ����ļ���     
		m_strEdit = szFile;
		strFilePath = szFile;
		UpdateData(FALSE);
	}
}

