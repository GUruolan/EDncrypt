#pragma once


// EDncryptView �Ի���

class EDncryptView : public CDialogEx
{
	DECLARE_DYNAMIC(EDncryptView)

public:
	EDncryptView(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~EDncryptView();

// �Ի�������
	enum { IDD = IDD_EDNCRYPT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
