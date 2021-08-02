// EDncryptView.cpp : 实现文件
//

#include "stdafx.h"
#include "EDncrypt.h"
#include "EDncryptView.h"
#include "afxdialogex.h"


// EDncryptView 对话框

IMPLEMENT_DYNAMIC(EDncryptView, CDialogEx)

EDncryptView::EDncryptView(CWnd* pParent /*=NULL*/)
	: CDialogEx(EDncryptView::IDD, pParent)
{

}

EDncryptView::~EDncryptView()
{
}

void EDncryptView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EDncryptView, CDialogEx)
END_MESSAGE_MAP()


// EDncryptView 消息处理程序
