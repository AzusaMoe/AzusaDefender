
// AzusaDefender.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CAzusaDefenderApp: 
// �йش����ʵ�֣������ AzusaDefender.cpp
//

class CAzusaDefenderApp : public CWinApp
{
public:
	CAzusaDefenderApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAzusaDefenderApp theApp;
