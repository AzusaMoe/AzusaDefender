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
/*
typedef struct _IMAGE_IMPORT_DESCRIPTOR {
union {
DWORD   Characteristics;            // 0 for terminating null import descriptor
DWORD   OriginalFirstThunk;         // RVA to original unbound IAT (PIMAGE_THUNK_DATA)
} DUMMYUNIONNAME;
DWORD   TimeDateStamp;                  // 0 if not bound,
// -1 if bound, and real date\time stamp
//     in IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT (new BIND)
// O.W. date/time stamp of DLL bound to (Old BIND)

DWORD   ForwarderChain;                 // -1 if no forwarders
DWORD   Name;
DWORD   FirstThunk;                     // RVA to IAT (if bound this IAT has actual addresses)
} IMAGE_IMPORT_DESCRIPTOR;
*/
typedef struct _MY_IMPORT_DESCRIPTOR
{
	CString Name;//DLL����
	DWORD OriginalFirstThunk;//INT(�������Ʊ�RVA)
	DWORD FirstThunk;//IAT(�����ַ��RVA)
	DWORD OffsetOriginalFirstThunk;//INT(�������Ʊ�ƫ��)
	DWORD OffsetFirstThunk;//IAT(�����ַ��ƫ��)

}MY_IMPORT_DESCRIPTOR,*PMY_IMPORT_DESCRIPTOR;

typedef struct _IMPORTFUNINFO
{
	DWORD Ordinal;
	CString Name;

}IMPORTFUNINFO ,*PIMPORTFUNINFO;

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
	void ImportTable();
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
	vector<MY_IMPORT_DESCRIPTOR> m_vecImportDescriptor;
	vector<IMPORTFUNINFO> m_vecImportFunInfo;
};

