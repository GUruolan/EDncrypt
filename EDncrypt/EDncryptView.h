#pragma once


// EDncryptView 对话框

class EDncryptView : public CDialogEx
{
	DECLARE_DYNAMIC(EDncryptView)

public:
	EDncryptView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EDncryptView();

// 对话框数据
	enum { IDD = IDD_EDNCRYPT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
