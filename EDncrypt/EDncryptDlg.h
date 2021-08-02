
// EDncryptDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"


// CEDncryptDlg �Ի���
class CEDncryptDlg : public CDialogEx
{
// ����
public:
	CEDncryptDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_EDNCRYPT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox op_str_file;
	CComboBox op_md5_sha;
	CComboBox op_des_aed;
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedEncrypt();
	CEdit in_msg;
	CEdit out_hash;
	CFont font;
	CEdit in_key;
	CEdit out_msg;
	afx_msg void OnEnErrspaceEdit1();
	afx_msg void OnBnClickedGenePrPu();
	CEdit out_key;
	CEdit in_prpu;
	afx_msg void OnBnClickedDncrypt();
	CEdit testbox;
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	CEdit emsg_box;
	CEdit in_prk;
	CEdit out_dnc_key;
	CEdit out_dnc_hash;
	afx_msg void OnEnChangeEdit9();
	CEdit out_hashcpr;
};
