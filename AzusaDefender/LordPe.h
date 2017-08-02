#pragma once
#include <vector>
using std::vector;


//������Ϣ
typedef struct _BASICINFO
{
	WORD NumberOfSections;//��������
	WORD SizeOfOptionalHeader;//��չͷ��С
	WORD Characteristics;//�ļ�����

	WORD Magic;//�ļ����ͱ�־
	DWORD SizeOfCode;//������ܴ�С
	DWORD AddressOfEntryPoint;//��ڵ�
	DWORD BaseOfCode;//�����ַ
	DWORD BaseOfData;//���ݻ�ַ
	DWORD ImageBase;//Ĭ�ϼ��ػ�ַ
	DWORD SectionAlignment;//�����
	DWORD FileAlignment;//�ļ������
	DWORD SizeOfImage;//�����С
	DWORD SizeOfHeaders;//ͷ���ܴ�С
	WORD DllCharacteristics;//DLL����
	DWORD NumberOfRvaAndSizes;//����Ŀ¼����
}BASICINFO,*PBASICINFO;

//����Ŀ¼��
typedef struct _DataTableOfContents
{
	DWORD VirtualAddress;//��������ַ
	DWORD Size;//��С
}DataTableOfContents,*PDataTableOfContents;

//���α�
typedef struct _SectionTable
{
	CString Name;//������
	DWORD VirtualSize;//ʵ�ʴ�С
	DWORD SizeOfRawData;//�ļ��д�С
	DWORD PointerToRawData;//�ļ���λ��
	DWORD PointerToVirData;//�ڴ���λ��
	DWORD Characteristics;//��������
}SectionTable, *PSectionTable;

class CLordPe
{
public:
	CLordPe();
	~CLordPe();
public:
	void GetBasicInfo(CString& filePath);
	BASICINFO m_basicInfo;
	vector<DataTableOfContents> m_vecDataTable;
	vector<SectionTable> m_vecSectionTable;
};

