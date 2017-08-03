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

//�����������Ϣ
typedef struct _MY_IM_EX_DI
{
	CString name;//dll��
	DWORD Base;//��Ż���
	DWORD NumberOfFunctions;//��������
	DWORD NumberOfNames;//������������
	DWORD AddressOfFunctions;//��ַ��RVA
	DWORD AddressOfNames;//���Ʊ�RVA
	DWORD AddressOfNameOrdinals;//��ű�RVA
}MY_IM_EX_DI,*PMY_IM_EX_DI;

//����������Ϣ
typedef struct _EXPORTFUNINFO
{
	DWORD ExportOrdinals;//�������
	DWORD FunctionRVA;//����RVA
	DWORD FunctionOffset;//�����ļ�ƫ��
	CString FunctionName;//������
}EXPORTFUNINFO,*PEXPORTFUNINFO;

class CLordPe
{
public:
	CLordPe();
	~CLordPe();
	CLordPe(CString& filePath);
public:
	void GetBasicInfo();
	void ExportTable();
	BOOL GetDosHead(CString& filePath);
	DWORD RVAToOffset(IMAGE_DOS_HEADER* pDos, DWORD dwRva);
public:
	BYTE* m_pBuf;//�����ͷ�����Ŀռ�
	PIMAGE_DOS_HEADER m_pDosHdr;//DOSͷ��ַ
	BASICINFO m_basicInfo;//ͷ�ļ��еĻ�����Ϣ
	vector<DataTableOfContents> m_vecDataTable;//����Ŀ¼��
	vector<SectionTable> m_vecSectionTable;//���α�
	vector<EXPORTFUNINFO> m_vecExportFunInfo;
	MY_IM_EX_DI m_my_im_ex_di;
};

