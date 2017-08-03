#include "stdafx.h"
#include "LordPe.h"

RESOURCEINFO resourceInfo = { 0 };

CLordPe::CLordPe(CString& filePath)
{
	GetDosHead(filePath);
}

CLordPe::CLordPe()
{

}

CLordPe::~CLordPe()
{
	delete[] m_pBuf;
}


BOOL CLordPe::GetDosHead(CString& filePath)
{
	// 1. ���ļ�,���ļ���ȡ���ڴ�.
	// CreateFile,ReadFile.
	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	DWORD dwFileSize = 0;
	dwFileSize = GetFileSize(hFile, NULL);

	// 2. �����ڴ�ռ�
	BYTE* pBuf = new BYTE[dwFileSize];
	m_pBuf = pBuf;//�������������������ͷſռ�

	// 3. ���ļ����ݶ�ȡ���ڴ���
	DWORD dwRead = 0;
	ReadFile(hFile, pBuf, dwFileSize, &dwRead, NULL);

	// ������������DOSͷ�ṹ��������
	m_pDosHdr = (IMAGE_DOS_HEADER*)pBuf;//��DOSͷָ�뱣������

	// ntͷ,�������ļ�ͷ����չͷ
	IMAGE_NT_HEADERS* pNtHdr;
	pNtHdr = (IMAGE_NT_HEADERS*)(m_pDosHdr->e_lfanew + (DWORD)m_pDosHdr);

	// �ж��Ƿ���һ����Ч��pe�ļ�
	if (m_pDosHdr->e_magic != IMAGE_DOS_SIGNATURE || pNtHdr->Signature != IMAGE_NT_SIGNATURE)
	{
		AfxMessageBox(_T("������Ч��PE�ļ�"));
		return FALSE;
	}
	return TRUE;
}

void CLordPe::GetBasicInfo()
{
	// ntͷ,�������ļ�ͷ����չͷ
	IMAGE_NT_HEADERS* pNtHdr;
	pNtHdr = (IMAGE_NT_HEADERS*)(m_pDosHdr->e_lfanew + (DWORD)m_pDosHdr);

	IMAGE_FILE_HEADER* pFileHdr; // �ļ�ͷ
	pFileHdr = &(pNtHdr->FileHeader);

	IMAGE_OPTIONAL_HEADER* pOptHdr;// ��չͷ
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
	// ntͷ,�������ļ�ͷ����չͷ
	IMAGE_NT_HEADERS* pNtHdr;
	pNtHdr = (IMAGE_NT_HEADERS*)(m_pDosHdr->e_lfanew + (DWORD)m_pDosHdr);

	IMAGE_OPTIONAL_HEADER* pOptHdr;// ��չͷ
	pOptHdr = &(pNtHdr->OptionalHeader);

	//����Ŀ¼��
	PIMAGE_DATA_DIRECTORY pDataDirectory = pOptHdr->DataDirectory;

	// 5. �ҵ�������
	DWORD dwExpRva = pDataDirectory[0].VirtualAddress;

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
	//ZeroMemory(&m_my_im_ex_di, sizeof(m_my_im_ex_di));//ȫ�ֶ���Ϊ��
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

	// ��ַ��,���Ʊ���һ��DWORD��������
	DWORD* pExpAddr =(DWORD*)(dwExpAddrTabOfs + (DWORD)m_pDosHdr);

	DWORD* pExpName =(DWORD*)(dwExpNameTabOfs + (DWORD)m_pDosHdr);

	// ��ű���WORD���͵�����
	WORD* pExpOrd =(WORD*)(dwExpOrdTabOfs + (DWORD)m_pDosHdr);

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



void CLordPe::ImportTable()
{
	// ntͷ,�������ļ�ͷ����չͷ
	IMAGE_NT_HEADERS* pNtHdr;
	pNtHdr = (IMAGE_NT_HEADERS*)(m_pDosHdr->e_lfanew + (DWORD)m_pDosHdr);

	IMAGE_OPTIONAL_HEADER* pOptHdr;// ��չͷ
	pOptHdr = &(pNtHdr->OptionalHeader);

	//����Ŀ¼��
	PIMAGE_DATA_DIRECTORY pDataDirectory = pOptHdr->DataDirectory;

	// 5. �ҵ������
	// 5. �õ�������RVA
	DWORD dwImpRva = pDataDirectory[1].VirtualAddress;

	IMAGE_IMPORT_DESCRIPTOR* pImpArray;

	pImpArray = (IMAGE_IMPORT_DESCRIPTOR*)(RVAToOffset(m_pDosHdr, dwImpRva) + (DWORD)m_pDosHdr);

	// ���������ĸ�����û�������ֶμ�¼.
	// �����ı�־����һ��ȫ0��Ԫ����Ϊ��β
	m_vecImportDescriptor.clear();
	m_vvImportFunInfo.clear();
	while (pImpArray->Name != 0)
	{
		MY_IMPORT_DESCRIPTOR myImportDescriptor = { 0 };
		// �����Dll������(Rva)
		DWORD dwNameOfs = RVAToOffset(m_pDosHdr, pImpArray->Name);
		char* pDllName = (char*)(dwNameOfs + (DWORD)m_pDosHdr);
		myImportDescriptor.Name = pDllName;

		// ����,�����dll��,һ��������Щ����
		myImportDescriptor.OriginalFirstThunk = pImpArray->OriginalFirstThunk;
		myImportDescriptor.FirstThunk = pImpArray->FirstThunk;

		// IAT(��������)��¼��һ����һ��dll�е�������Щ����,��Щ����Ҫô�������Ƶ���,Ҫô������ŵ����
		// ��¼��һ��������. ���������IMAGE_THUNK_DATA���͵Ľṹ������.
		DWORD INTOfs = RVAToOffset(m_pDosHdr, pImpArray->OriginalFirstThunk);
		myImportDescriptor.OffsetOriginalFirstThunk = INTOfs;

		DWORD IATOfs = RVAToOffset(m_pDosHdr, pImpArray->FirstThunk);
		myImportDescriptor.OffsetFirstThunk = IATOfs;

		m_vecImportDescriptor.push_back(myImportDescriptor);
		/*
		����һ��ֻ��4���ֽڵĽṹ��.�����������е�ÿһ���ֶα����ֵ����һ��.��Щֵ, ���ǵ��뺯������Ϣ.
		���뺯������Ϣ�����²���:
		1. ���뺯�������
		2. ���뺯��������(�����п���û��)
		���Ը��ݽṹ���е��ֶε����λ�ж�, ������Ϣ�������Ƶ��뻹������ŵ���
		typedef struct _IMAGE_THUNK_DATA32 {
		union {
		DWORD ForwarderString;      // PBYTE
		DWORD Function;             // PDWORD
		DWORD Ordinal;
		DWORD AddressOfData;        // PIMAGE_IMPORT_BY_NAME
		} u1;
		} IMAGE_THUNK_DATA32;
		*/
		IMAGE_THUNK_DATA* pInt = NULL;
		IMAGE_THUNK_DATA* pIat = NULL;
		pInt = (IMAGE_THUNK_DATA*)(INTOfs + (DWORD)m_pDosHdr);
		pIat = (IMAGE_THUNK_DATA*)(IATOfs + (DWORD)m_pDosHdr);

		//INT���Կ�����IAT�ı��ݣ�����û�б��ݵ��������˽���IAT
		m_vecImportFunInfo.clear();
		while (pIat->u1.Function != 0)
		{
			IMPORTFUNINFO importFunInfo = {0};
			// �ж��Ƿ�������ŵ���
			if (IMAGE_SNAP_BY_ORDINAL32(pIat->u1.Function))
			{
				// ����ŷ�ʽ����ṹ�屣���ֵ��16λ����һ����������
				importFunInfo.Ordinal = pIat->u1.Ordinal & 0xFFFF;
				importFunInfo.Name = L"";
			}
			else
			{
				// �������������Ƶ����ʱ��, pInt->u1.Function �������һ��rva , 
				//���RVAָ��һ�����溯��������Ϣ�Ľṹ��
				DWORD dwImpNameOfs = RVAToOffset(m_pDosHdr, pIat->u1.Function);
				IMAGE_IMPORT_BY_NAME* pImpName;
				pImpName = (IMAGE_IMPORT_BY_NAME*)(dwImpNameOfs + (DWORD)m_pDosHdr);
				importFunInfo.Ordinal = pImpName->Hint;
				importFunInfo.Name = pImpName->Name;
				m_vecImportFunInfo.push_back(importFunInfo);
			}
			++pIat;
		}
		m_vvImportFunInfo.push_back(m_vecImportFunInfo);
		++pImpArray;
	}
}

//�õ���Դ����ʼ��ַ�������ý�������
void CLordPe::ResourceTable()
{
	// ntͷ,�������ļ�ͷ����չͷ
	IMAGE_NT_HEADERS* pNtHdr;
	pNtHdr = (IMAGE_NT_HEADERS*)(m_pDosHdr->e_lfanew + (DWORD)m_pDosHdr);

	IMAGE_OPTIONAL_HEADER* pOptHdr;// ��չͷ
	pOptHdr = &(pNtHdr->OptionalHeader);

	//����Ŀ¼��
	PIMAGE_DATA_DIRECTORY pDataDirectory = pOptHdr->DataDirectory;

	// 5. �ҵ���Դ��
	// 5. �õ���Դ���RVA
	DWORD dwResRootRva = pDataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
	PIMAGE_RESOURCE_DIRECTORY pResRoot;
	pResRoot = (IMAGE_RESOURCE_DIRECTORY*)(RVAToOffset(m_pDosHdr, dwResRootRva) + (DWORD)m_pDosHdr);
	parseResourcesTable((DWORD)pResRoot, pResRoot, 1);

}

//������Դ��ĺ������ݹ����
void CLordPe::parseResourcesTable(DWORD dwResRootDirAddr,//��Ŀ¼���׵�ַ
	IMAGE_RESOURCE_DIRECTORY* pResDir,//��Ҫ��������ԴĿ¼
	int nDeep)//��¼���ǵڼ���Ŀ¼
{
	// ��ȡĿ¼��ڵ��ܸ���
	DWORD dwEntryCount = pResDir->NumberOfIdEntries + pResDir->NumberOfNamedEntries;
	// ��ȡĿ¼���������׵�ַ
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pResDirEntry;
	pResDirEntry = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pResDir + 1);
	//pResDir + 1��˼���Ǹ���pResDir����������͵ĳ��ȣ���pResDirԭ���ĵ�ַ���ټ���������ȣ�ʹpResDirָ����һ����ַ��

	WCHAR buff[512];

	if (nDeep == 1) { //��һ��Ŀ¼���,ID��Ϣ����Դ���͵�id,ָ����һ���ƫ��ָ����ǵڶ������ԴĿ¼.
		m_vecResourceTpye.clear();
		m_vvResourceInfo.clear();
		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			CString resourceType;
			// ������Դ������,�ж���Դ���������εĻ����ַ������͵�
			if (pResDirEntry[i].NameIsString == 1) {
				// ��Դ�������ַ���
				IMAGE_RESOURCE_DIR_STRING_U* pTypeName;
				// NameOffset �����ƫ��,�ǰ���Դ��Ŀ¼�ĵ�ַ��Ϊ����ַ��ƫ��
				pTypeName =(IMAGE_RESOURCE_DIR_STRING_U*)(pResDirEntry[i].NameOffset + dwResRootDirAddr);

				wcsncpy_s(buff, pTypeName->NameString, pTypeName->Length);//�ṹ����������_U˵���˽ṹ���ڴ�ŵ��ǿ��ֽ��ַ�
				resourceType = buff;
			}
			else {
				// ˵����Դ����ID��һ�����ε�ֵ
				char *szType[] =
				{
					("") ,				// 0
					("���") ,				// 1 
					("λͼ") ,				// 2 RT_BITMAP
					("ͼ��") ,			    // 3 RT_ICON
					("�˵�") ,				// 4 RT_MENU
					("�Ի���") ,			// 5 RT_DIALOG      
					("�ַ�����") ,			// 6 RT_STRING      
					("����Ŀ¼") ,			// 7 RT_FONTDIR     
					("����") ,				// 8 RT_FONT        
					("���ټ�") ,			// 9 RT_ACCELERATOR 
					("RC����") ,		// 10 RT_RCDATA      
					("��Ϣ��") ,			// 11 RT_MESSAGETABLE
					("�����") ,			// 12 
					("") ,				   // 13 
					("ͼ����") ,			// 14 
					"" ,					// 15
					("�汾��Ϣ") ,			// 16
					("�Ի������Ŀ¼") ,	// 17 #define RT_DLGINCLUDE   MAKEINTRESOURCE(17)
					"" ,								// 18 #define RT_PLUGPLAY     MAKEINTRESOURCE(19)
					"" ,								// 19 #define RT_VXD          MAKEINTRESOURCE(20)
					"" ,								// 20 #define RT_ANICURSOR    MAKEINTRESOURCE(21)
					"" ,								// 21 #define RT_ANIICON      MAKEINTRESOURCE(22)
					"" ,									// 22 
					("HTML") ,						    // 23 #define RT_HTML         MAKEINTRESOURCE(23)
					("�嵥�ļ�")			// 24 RT_MANIFEST
				};

				if (pResDirEntry[i].Id >= 1 && pResDirEntry[i].Id <= 24) {
					resourceType = szType[pResDirEntry[i].Id];
				}
				else {
					resourceType.Format(L"%08X", pResDirEntry[i].Id);
				}
			}
			m_vecResourceTpye.push_back(resourceType);

			// ������һ��Ŀ¼
			IMAGE_RESOURCE_DIRECTORY* pNextDir =
				(IMAGE_RESOURCE_DIRECTORY*)(pResDirEntry[i].OffsetToDirectory + dwResRootDirAddr);

			parseResourcesTable(dwResRootDirAddr, pNextDir, nDeep + 1);
			m_vvResourceInfo.push_back(m_vecResourceInfo);
		}
	}
	else if (nDeep == 2) { /*�ڶ���*/

		m_vecResourceInfo.clear();
		for (DWORD i = 0; i < dwEntryCount; ++i)
		{
			//ZeroMemory(&resourceInfo, sizeof(RESOURCEINFO));
			// ��ԴĿ¼�ĵڶ���, �������ÿ����Դ����Դid
			// ����ID
			// 1. ����ID
			// 2. �ַ���ID
			if (pResDirEntry[i].NameIsString) {
				// ��Դ�������ַ���
				IMAGE_RESOURCE_DIR_STRING_U* pTypeName;
				// NameOffset �����ƫ��,�ǰ���Դ��Ŀ¼�ĵ�ַ��Ϊ����ַ��ƫ��
				pTypeName =
					(IMAGE_RESOURCE_DIR_STRING_U*)
					(pResDirEntry[i].NameOffset + dwResRootDirAddr);

				wcsncpy_s(buff, pTypeName->NameString, pTypeName->Length);
				resourceInfo.NameOrID = buff;
			}
			else {
				CString temp;
				temp.Format(L"%08X", pResDirEntry[i].Id);
				resourceInfo.NameOrID = temp;
			}

			// ����ƫ��
			// ������һ��Ŀ¼
			IMAGE_RESOURCE_DIRECTORY* pNextDir =(IMAGE_RESOURCE_DIRECTORY*)(pResDirEntry[i].OffsetToDirectory + dwResRootDirAddr);

			parseResourcesTable(dwResRootDirAddr, pNextDir, nDeep + 1);
			m_vecResourceInfo.push_back(resourceInfo);
		}
	}
	else if (nDeep == 3) {/*������*/

		// ��������ԴĿ¼���, ���������Դ���������
		// ����ƫ��(ƫ����һ��ָ��������ڵ�ƫ��)
		IMAGE_RESOURCE_DATA_ENTRY* pResDataEntry = 0;
		pResDataEntry =(IMAGE_RESOURCE_DATA_ENTRY*)(pResDirEntry->OffsetToData + dwResRootDirAddr);
		if (pResDirEntry->DataIsDirectory == 0) {
			resourceInfo.ResourceRVA = pResDataEntry->OffsetToData;
			resourceInfo.ResourceSize = pResDataEntry->Size;
		}
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