// DiaD.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "DiaD.h"
#include "MD5.h"
#include "afxdialogex.h"
#include<iostream>  
#include<fstream>
#include <string>
using namespace std;


// ���ַ�ת��Ϊ���ַ�(Unicode --> ASCII)
#define  WCHAR_TO_CHAR(lpW_Char, lpChar) \
    WideCharToMultiByte(CP_ACP, NULL, lpW_Char, -1, lpChar, _countof(lpChar), NULL, FALSE);


// ���ַ�ת��Ϊ���ַ�(ASCII --> Unicode)
#define  CHAR_TO_WCHAR(lpChar, lpW_Char) \
    MultiByteToWideChar(CP_ACP, NULL, lpChar, -1, lpW_Char, _countof(lpW_Char));
// CDiaD �Ի���

IMPLEMENT_DYNAMIC(CDiaD, CDialogEx)

CDiaD::CDiaD(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

CDiaD::~CDiaD()
{
}

void CDiaD::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
}


BEGIN_MESSAGE_MAP(CDiaD, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaD::OnBnClickedButton1)
END_MESSAGE_MAP()


BOOL CDiaD::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ʼ��list��Ϣ
	m_ctrlList.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	TCHAR *szCol[5] = { L"�ļ�·��", L"����ʱ��", L"�޸�ʱ��", L"�ļ���С", L"MD5ֵ"};
	for (int i = 0; i < 5; ++i)
	{
		m_ctrlList.InsertColumn(i, szCol[i], 0, 100);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

// CDiaD ��Ϣ�������
void CDiaD::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TraverseFile(L"D://Test", m_vecFile);
	CalculateMD5();
}


// �߼����벿��
//���������ļ���
void CDiaD::TraverseFile(
	TCHAR  szPath[MAX_PATH], // Ҫ�������ļ�·��
	vector<CString> &vecPath)// ������������ļ�
{
	// ���ļ���ĩβ���\\*�����������ļ�
	CString strPath = szPath;
	strPath += L"\\*";
	WIN32_FIND_DATA fi = {};
	// �ҵ����ļ���Ϣ�ŵ�fi����
	HANDLE hFile = FindFirstFile(strPath, &fi);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		// �ж��Ƿ�ǰĿ¼���ϼ�Ŀ¼
		if (!lstrcmp(fi.cFileName, L".") || !lstrcmp(fi.cFileName, L".."))
		{
			continue;
		}
		// �ж��Ƿ����ļ��У����ǣ��ݹ����
		if (fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString strDIr = szPath;
			strDIr += L"\\";
			strDIr += fi.cFileName;
			TraverseFile(strDIr.GetBuffer(), vecPath);
			continue;
		}
		// ��������ɸѡ�������ҵ�Ҫɾ�����ļ����ˣ���֮ǰ��·�����������·��
		CString strDIr = szPath;
		strDIr += L"\\";
		strDIr += fi.cFileName;
		// �����·��������
		vecPath.push_back(strDIr);
	} while (FindNextFile(hFile, &fi));
}

//�����������ļ�����MD5�ȶ�
void CDiaD::CalculateMD5()
{
	CMD5 md5;
	CHAR strFilePath[MAX_PATH] = { 0 };
	CStringA cstrMD5;
	//�򿪱�����MD5���ļ�
	//HANDLE hFile = CreateFile(L"MD5.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	//CHAR ch[100] = {};
	//DWORD dwReads;
	//ReadFile(hFile, ch, 100, &dwReads, NULL);
	string buf;
	CStringA strObj;
	ifstream infile;
	infile.open(_T("MD5.txt"));
	if (!infile)
	{
		return;
	}
	//���ж�ȡֱ����ȡ���
	while (getline(infile, buf))
	{
		strObj.Format("%s", buf.c_str());
		m_vecMD5Save.push_back(strObj);
	}
	infile.close();
	for (auto vecFile : m_vecFile)
	{
		WCHAR_TO_CHAR(vecFile.GetBuffer(), strFilePath);
		//�����ļ�MD5ֵ
		cstrMD5 = md5.md5FileValue(strFilePath);
		//�ͱ����MD5ֵ���бȶ�
		for (auto vecMD5Save : m_vecMD5Save)
		{
			if (vecMD5Save == cstrMD5)//MD5ֵ��ȣ��ǲ���
			{
				m_vecVirus.push_back(vecFile);//�������ļ���������
				m_vecVirusMD5.push_back(cstrMD5);//����Ӧ�Ĳ���MD5ֵ��������
			}
		}
	}
}

void CDiaD::GetVirusInfo()
{
	WIN32_FIND_DATA stcData = { 0 };
	for (auto vecVirusFile : m_vecVirus)
	{
		HANDLE hFind = FindFirstFile(vecVirusFile, &stcData);
		if (hFind == INVALID_HANDLE_VALUE)
		{
			return;
		}

	}
}


