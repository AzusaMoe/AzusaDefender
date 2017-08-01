// DiaVs.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AzusaDefender.h"
#include "DiaVs.h"
#include "afxdialogex.h"


// CDiaVs �Ի���

IMPLEMENT_DYNAMIC(CDiaVs, CDialogEx)

CDiaVs::CDiaVs(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_strFolder(_T(""))
	, m_strDelete(_T(".obj.tlog.lastbuildstate.idb.pdb.pch.res.ilk.exe.sdf.ipch.log"))
{

}

CDiaVs::~CDiaVs()
{
}

void CDiaVs::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strFolder);
	DDX_Text(pDX, IDC_EDIT2, m_strDelete);
	DDX_Control(pDX, IDC_LIST4, m_ctlListBoxFolder);
	DDX_Control(pDX, IDC_LIST5, m_ctrListBoxFile);
}


BEGIN_MESSAGE_MAP(CDiaVs, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDiaVs::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDiaVs::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDiaVs::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CDiaVs::OnBnClickedButton4)
END_MESSAGE_MAP()


BOOL CDiaVs::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
// CDiaVs ��Ϣ�������


void CDiaVs::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR szPath1[MAX_PATH] = {};
	BROWSEINFO bi = {};
	// �ļ���ѡ��Ի����������ھ��
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szPath1;
	// ����˵����Ϣ
	bi.lpszTitle = L"��Ҫ�������ļ���";
	// �Ի�����༭��
	bi.ulFlags = BIF_EDITBOX;
	LPITEMIDLIST pid;
	// ���ر�ʶ�ļ���·���ġ�PIDL���������ҵ��ļ���·��
	pid = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(pid, szPath1);
	// ��·�����µ�·����ʾ�༭����
	m_strFolder = szPath1;
	// ��·�����µ�·����ʾListBox��
	if (LB_ERR == m_ctlListBoxFolder.FindString(0, m_strFolder))
	{// ���m_ListBox����û������µ��ļ���·��������ӽ�m_ListBox
		m_ctlListBoxFolder.AddString(m_strFolder);
	}
	// �ѱ������ݸ��µ��Ի���
	UpdateData(FALSE);
	// �����ͷŵ�֮ǰ�ġ�PID��
	CoTaskMemFree(pid);
}


void CDiaVs::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}


void CDiaVs::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// ����֮ǰ�����֮ǰ�������ļ���Ϣ���б������
	m_vecFileDelete.clear();
	m_ctrListBoxFile.ResetContent();
	// ��ȡҪ�������ļ�����Ŀ
	int nCount = m_ctlListBoxFolder.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CString test;
		// ��ȡListBox�ؼ��ڵ��ļ���·��
		m_ctlListBoxFolder.GetText(i, test);
		// �����ļ����ڵ������ļ������ļ���
		TraverseFile(test.GetBuffer(), m_strDelete, m_vecFileDelete);
	}
	// ����������Ҫɾ���ļ���ӵ�ListBox����ʾ����
	for (DWORD i = 0; i < m_vecFileDelete.size(); i++)
	{
		m_ctrListBoxFile.AddString(m_vecFileDelete[i]);
		m_ctrListBoxFile.SelectString(i, m_vecFileDelete[i]);

	}
}

void CDiaVs::TraverseFile(
	TCHAR  szPath[MAX_PATH], // Ҫ�������ļ�·��
	const CString &strFilter,// ���˵��ļ���׺
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
			TraverseFile(strDIr.GetBuffer(), strFilter, vecPath);
			continue;
		}
		// ��ȡ�ļ���չ���������ж��Ƿ������ļ���׺
		TCHAR *p = PathFindExtension(fi.cFileName);
		// Find���ز��ҵ��Ӵ�p��m_Delete�е����������Ҳ�������-1
		if (-1 == strFilter.Find(p))
		{
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

void CDiaVs::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for (DWORD i = 0; i < m_vecFileDelete.size(); i++)
	{
		DeleteFile(m_vecFileDelete[i].GetBuffer());
		m_ctrListBoxFile.DeleteString(0);
	}
	m_vecFileDelete.swap(vector<CString>());
}



