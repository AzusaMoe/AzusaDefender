#include "stdafx.h"
#include "LordPe.h"


CLordPe::CLordPe()
{
}


CLordPe::~CLordPe()
{
}

void CLordPe::GetBasicInfo(CString& filePath)
{
	// 1. ���ļ�,���ļ���ȡ���ڴ�.
	// CreateFile,ReadFile.
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile(filePath,GENERIC_READ,FILE_SHARE_READ,NULL,
						OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	DWORD dwFileSize = 0;
	dwFileSize = GetFileSize(hFile, NULL);

	// 2. �����ڴ�ռ�
	BYTE* pBuf = new BYTE[dwFileSize];

	// 3. ���ļ����ݶ�ȡ���ڴ���
	DWORD dwRead = 0;
	ReadFile(hFile,pBuf,dwFileSize,&dwRead,NULL);

	// ������������DOSͷ�ṹ��������
	IMAGE_DOS_HEADER* pDosHdr;// DOSͷ
	pDosHdr = (IMAGE_DOS_HEADER*)pBuf;
	m_pDosHdr = (IMAGE_DOS_HEADER*)pBuf;//��DOSͷָ�뱣������

	// ntͷ,�������ļ�ͷ����չͷ
	IMAGE_NT_HEADERS* pNtHdr;
	pNtHdr = (IMAGE_NT_HEADERS*)(pDosHdr->e_lfanew + (DWORD)pBuf);

	// �ж��Ƿ���һ����Ч��pe�ļ�
	if (pDosHdr->e_magic != IMAGE_DOS_SIGNATURE|| pNtHdr->Signature != IMAGE_NT_SIGNATURE)
	{
		AfxMessageBox(_T("������Ч��PE�ļ�"));
		return;
	}

	IMAGE_FILE_HEADER* pFileHdr; // �ļ�ͷ
	IMAGE_OPTIONAL_HEADER* pOptHdr;// ��չͷ
	pFileHdr = &(pNtHdr->FileHeader);
	pOptHdr = &(pNtHdr->OptionalHeader);
	
	ZeroMemory(&m_basicInfo, sizeof(m_basicInfo));
	//�ļ�ͷ�ڵ���Ϣ
	m_basicInfo.NumberOfSections = pFileHdr->NumberOfSections;
	m_basicInfo.SizeOfOptionalHeader = pFileHdr->SizeOfOptionalHeader;
	m_basicInfo.Characteristics = pFileHdr->Characteristics;
	//��չͷ����Ϣ
	m_basicInfo.Magic = pOptHdr->Magic;
	m_basicInfo.SizeOfCode = pOptHdr->SizeOfCode;
	m_basicInfo.AddressOfEntryPoint = pOptHdr->AddressOfEntryPoint;
	m_basicInfo.BaseOfCode = pOptHdr->BaseOfCode;
	m_basicInfo.BaseOfData = pOptHdr->BaseOfData;
	m_basicInfo.ImageBase = pOptHdr->ImageBase;
	m_basicInfo.SectionAlignment = pOptHdr->SectionAlignment;
	m_basicInfo.FileAlignment = pOptHdr->FileAlignment;
	m_basicInfo.SizeOfImage = pOptHdr->SizeOfImage;
	m_basicInfo.SizeOfHeaders = pOptHdr->SizeOfHeaders;
	m_basicInfo.DllCharacteristics = pOptHdr->DllCharacteristics;
	m_basicInfo.NumberOfRvaAndSizes = pOptHdr->NumberOfRvaAndSizes;

	PIMAGE_DATA_DIRECTORY pDataDirectory = pOptHdr->DataDirectory;
	DWORD i = 0;
	m_vecDataTable.clear();
	while (i < pOptHdr->NumberOfRvaAndSizes)
	{
		DataTableOfContents dataTableObj = { 0 };
		dataTableObj.VirtualAddress = pDataDirectory[i].VirtualAddress;
		dataTableObj.Size = pDataDirectory[i].Size;
		m_vecDataTable.push_back(dataTableObj);
		++i;
	}

	IMAGE_SECTION_HEADER* pScnHdr = NULL;
	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);
	m_vecSectionTable.clear();
	for (int i = 0; i < pFileHdr->NumberOfSections; ++i)
	{
		SectionTable sectionTable = { 0 };
		sectionTable.Name = pScnHdr[i].Name;
		sectionTable.VirtualSize = pScnHdr[i].Misc.VirtualSize;
		sectionTable.SizeOfRawData = pScnHdr[i].SizeOfRawData;
		sectionTable.PointerToRawData = pScnHdr[i].PointerToRawData;
		sectionTable.PointerToVirData = pScnHdr[i].VirtualAddress + pOptHdr->ImageBase;
		sectionTable.Characteristics = pScnHdr[i].Characteristics;
		m_vecSectionTable.push_back(sectionTable);
	}
}

//����������
void CLordPe::ExportTable()
{
	// 5. �ҵ�������
	DWORD dwExpRva = m_vecDataTable[0].VirtualAddress;

	// 5.1 �õ�RVA���ļ�ƫ��
	DWORD dwExpOfs = RVAToOffset(m_pDosHdr, dwExpRva);
	IMAGE_EXPORT_DIRECTORY* pExpTab = NULL;
	pExpTab = (IMAGE_EXPORT_DIRECTORY*)(dwExpOfs + (DWORD)m_pDosHdr);

	// ����DLL�ĵ�����
	/*
	typedef struct _IMAGE_EXPORT_DIRECTORY {
	DWORD   Characteristics;
	DWORD   TimeDateStamp;
	WORD    MajorVersion;
	WORD    MinorVersion;
	DWORD   Name; // dll��[�ַ�����RVA]
	DWORD   Base;
	DWORD   NumberOfFunctions;
	DWORD   NumberOfNames;
	DWORD   AddressOfFunctions;     // ��ַ��(DWORD����)��rva
	DWORD   AddressOfNames;         // ���Ʊ�(DWORD����)��rva
	DWORD   AddressOfNameOrdinals;  // ��ű�(WORD����)��rva
	} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;
	*/
	// 1. ����DLL����
	DWORD dwNameOfs = RVAToOffset(m_pDosHdr, pExpTab->Name);
	char* pDllName =(char*)(dwNameOfs + (DWORD)m_pDosHdr);
	//�����������Ϣ
	ZeroMemory(&m_my_im_ex_di, sizeof(m_my_im_ex_di));//ȫ�ֶ���Ϊ��
	m_my_im_ex_di.name = pDllName;
	m_my_im_ex_di.Base = pExpTab->Base;
	m_my_im_ex_di.NumberOfFunctions = pExpTab->NumberOfFunctions;
	m_my_im_ex_di.NumberOfNames = pExpTab->NumberOfNames;
	m_my_im_ex_di.AddressOfFunctions = pExpTab->AddressOfFunctions;
	m_my_im_ex_di.AddressOfNames = pExpTab->AddressOfNames;
	m_my_im_ex_di.AddressOfNameOrdinals = pExpTab->AddressOfNameOrdinals;

	// �������ű�
	DWORD dwExpAddrTabOfs = RVAToOffset(m_pDosHdr, pExpTab->AddressOfFunctions);
	DWORD dwExpNameTabOfs = RVAToOffset(m_pDosHdr, pExpTab->AddressOfNames);
	DWORD dwExpOrdTabOfs = RVAToOffset(m_pDosHdr, pExpTab->AddressOfNameOrdinals);

	// �����еĵ�ַ��,���ƶ���һ��DWORD��������
	DWORD* pExpAddr =
		(DWORD*)(dwExpAddrTabOfs + (DWORD)m_pDosHdr);

	DWORD* pExpName =
		(DWORD*)(dwExpNameTabOfs + (DWORD)m_pDosHdr);

	// ��ű���WORD���͵�����
	DWORD* pExpOrd =
		(DWORD*)(dwExpOrdTabOfs + (DWORD)m_pDosHdr);

	// �������еĺ�����ַ
	m_vecExportFunInfo.clear();
	for (int i = 0; i< pExpTab->NumberOfFunctions; ++i)
	{
		EXPORTFUNINFO exportFunInfo = {0};
		exportFunInfo.FunctionRVA = pExpAddr[i];//����RVA
		exportFunInfo.FunctionOffset = RVAToOffset(m_pDosHdr, pExpAddr[i]);//��������ļ�ƫ��
		// ���ҵ�ǰ�������ĵ�ַ��û������
		int j = 0;
		for (; j < pExpTab->NumberOfNames; ++j)
		{
			// �жϵ�ַ����±��Ƿ񱻱������ں���������ű���.
			// �����������, ��˵������±�������ĵ�ַ,��һ���к������Ƶĵ�ַ
			if (i == pExpOrd[j])
				break;
		}
		// �ж�ѭ������Ȼ����,����ͨ��break����
		if (j < pExpTab->NumberOfNames)
		{
			// ��ַ������,j���Ƕ�Ӧ�ĺ���������±�.ͨ��j�����ҵ�һ���������Ƶ�Rva
			DWORD dwNameRva = pExpName[j];
			DWORD dwNameOfs = RVAToOffset(m_pDosHdr, dwNameRva);
			char* pFunctionName = nullptr;
			pFunctionName = (char*)(dwNameOfs + (DWORD)m_pDosHdr);
			exportFunInfo.FunctionName = pFunctionName;//������
			exportFunInfo.ExportOrdinals = pExpTab->Base + i;//�����������
		}
		else
		{
			// ˵����ַû�������Ƶķ�ʽ����.
			if (pExpAddr[i] != 0)
			{
				// �����ĵ������:
				// ��Ż��� + �����(��ַ����±�)
				exportFunInfo.FunctionName = L"";//������Ϊ��
				exportFunInfo.ExportOrdinals = pExpTab->Base + i;//�����������
			}
		}
		m_vecExportFunInfo.push_back(exportFunInfo);
	}
}

//RVAת�ļ�ƫ��
DWORD CLordPe::RVAToOffset(IMAGE_DOS_HEADER* pDos,
	DWORD dwRva)
{
	IMAGE_SECTION_HEADER* pScnHdr;

	IMAGE_NT_HEADERS* pNtHdr =
		(IMAGE_NT_HEADERS*)(pDos->e_lfanew + (DWORD)pDos);

	pScnHdr = IMAGE_FIRST_SECTION(pNtHdr);
	DWORD dwNumberOfScn = pNtHdr->FileHeader.NumberOfSections;

	// 1. �������������ҵ���������
	for (int i = 0; i < dwNumberOfScn; ++i)
	{
		DWORD dwEndOfSection = pScnHdr[i].VirtualAddress + pScnHdr[i].SizeOfRawData;
		// �ж����RVA�Ƿ���һ�����εķ�Χ��
		if (dwRva >= pScnHdr[i].VirtualAddress
			&& dwRva < dwEndOfSection)
		{
			// 2. �����RVA�������ڵ�ƫ��:rva ��ȥ�׵�ַ
			DWORD dwOffset = dwRva - pScnHdr[i].VirtualAddress;
			// 3. ��������ƫ�Ƽ������ε��ļ���ʼƫ��
			return dwOffset + pScnHdr[i].PointerToRawData;
		}
	}
	return -1;
}