#pragma once

#include <string>
#include "Resource.h"
class CInputDialog : public CDialog
{
public:

	CInputDialog();
	CInputDialog(CWnd * pParent);
	~CInputDialog();

	const wchar_t* label  = L"Enter a categorie : ";
};

