// EDncryptDlg.cpp : 实现文件
// 发送方处理字符串、文件的最大长度为2^64-1qa

#include "stdafx.h"
#include "EDncrypt.h"
#include "EDncryptDlg.h"
#include "afxdialogex.h"
#include "atlstr.h"

//自定义头文件
#include "toolFunc.h"		//通用工具函数
#include "MD5.h"
#include "SHA-1.h"
#include "DES.h"
#include "AES.h"
#include "RSA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEDncryptDlg 对话框



CEDncryptDlg::CEDncryptDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEDncryptDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEDncryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, op_str_file);
	DDX_Control(pDX, IDC_COMBO2, op_md5_sha);
	DDX_Control(pDX, IDC_COMBO4, op_des_aed);
	DDX_Control(pDX, IDC_EDIT1, in_msg);
	DDX_Control(pDX, IDC_EDIT2, out_hash);
	DDX_Control(pDX, IDC_EDIT7, in_key);
	DDX_Control(pDX, IDC_EDIT5, out_msg);
	DDX_Control(pDX, IDC_EDIT4, out_key);
	DDX_Control(pDX, IDC_EDIT6, in_prpu);
	DDX_Control(pDX, IDC_EDIT8, emsg_box);
	DDX_Control(pDX, IDC_EDIT10, in_prk);
	DDX_Control(pDX, IDC_EDIT9, out_dnc_hash);
}

BEGIN_MESSAGE_MAP(CEDncryptDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDEncrypt, &CEDncryptDlg::OnBnClickedEncrypt)
	ON_BN_CLICKED(IDGenePrPu, &CEDncryptDlg::OnBnClickedGenePrPu)
END_MESSAGE_MAP()


// CEDncryptDlg 消息处理程序

BOOL CEDncryptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_MINIMIZE);


	// TODO: 在此添加额外的初始化代码
	//下拉选项：输入文件或字符串
	op_str_file.AddString(_T("字符串"));
	op_str_file.AddString(_T("文件"));
	//下拉选项：散列函数MD5/SHA-1
	op_md5_sha.AddString(_T("MD5"));
	op_md5_sha.AddString(_T("SHA-1"));
	//下拉选项：加解密算法选择
	op_des_aed.AddString(_T("DES"));
	op_des_aed.AddString(_T("AES"));

	in_msg.SetLimitText(200);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEDncryptDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEDncryptDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// 使图标在工作区矩形中居中

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		font.CreatePointFont(125,_T("宋体"));
		out_hash.SetFont(&font);
		out_msg.SetFont(&font);
		in_key.SetFont(&font);
		in_msg.SetFont(&font);
		out_dnc_hash.SetFont(&font);
		in_prk.SetFont(&font);
		in_prpu.SetFont(&font);
		out_hashcpr.SetFont(&font);
		emsg_box.SetFont(&font);
		out_key.SetFont(&font);

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEDncryptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**************************点击安全通信模拟按钮事件函数************************************/
void CEDncryptDlg::OnBnClickedEncrypt()	
{
	USES_CONVERSION;
	// TODO: 在此添加控件通知处理程序代码
	int ophash,oped,opin;	//选项变量
	ophash = op_md5_sha.GetCurSel();
	oped = op_des_aed.GetCurSel();
	opin = op_str_file.GetCurSel();

	unsigned long long msg_L = in_msg.GetWindowTextLength(); //字符串字节长度
	if(msg_L<=0){
		MessageBox(_T("ERROR：请输入待处理信息"));
		return;
	}

	CString msg;
	string shash;	//16进制的散列值,加密字符串
	string Emsg,Dmsg;
	Emsg.reserve(300);
	Dmsg.reserve(248);
	in_msg.GetWindowText(msg); //获取输入信息
	

/************************对字符串加密和hush运算********************************************/
	if(opin == 0){ 
		char* msgp = msg.GetBuffer();	//msgp为指向msg的char指针， 因为CString类型指针无法作为函数参数
		
		if(msg_L>200){ //根据填充散列函数最长处理2^64-1bit数据
			MessageBox(_T("ERROR：字符串过长！"));
			return;
		}

		///1）散列函数
		if(ophash == 0){ //MD5
			shash = hashMD5(msgp);
			out_hash.SetWindowText(shash.c_str());
		}
		else if(ophash == 1){ //SHA-1
			shash = hashSha_1(msgp);
			out_hash.SetWindowText(shash.c_str());
		}
		////1）散列函数结束
		/////对称密钥获取
		CString inkey;
		in_key.GetWindowText(inkey);
		int keyL = in_key.GetWindowTextLength();
		char* p_inkey = inkey.GetBuffer();

		///2）加密算法
		if(oped == 0){ //DES
			if(keyL != 8){
				MessageBox(_T("ERROR：密钥长度无效，\n请重新输入8字符密钥"));
				return;
			}
			bitset<64> inkey_b;
			inkey_b = charToBit(p_inkey);
			bitset<48>* sub_key = new bitset<48>[16];// 存放16轮子密钥
			Ndes::generKeys(inkey_b,sub_key); //生成子密钥
			//unsigned long long Emsg_L;
			//按64bit逐分组加密
			int i=0;
			bitset<64> x_b,y_b;//输入分组和输出分组64位二进制
			for(i=0; msg_L-i*8>8; ++i){ //分组加密
				string x(msgp,i*8,8);
				x_b = charToBit(x.c_str());
				y_b = desEncrypt(x_b,sub_key);
				string y = bitToChar(y_b);
				Emsg += y;
				x_b = desDecrypt(y_b,sub_key);
				x = bitToChar(x_b);

				Dmsg += x;
				x_b.reset();		//置0
				y_b.reset();		
			}
			//处理最后一个分组
			string x(msgp,i*8,msg_L-i*8);
			for(unsigned int x_L = x.length();x_L<8;x_L++)
				x +='\0';		//0填充
			x_b = charToBit(x.c_str());
			y_b = desEncrypt(x_b,sub_key);
			string y = bitToChar(y_b);	//加密信息
			Emsg += y;
			x_b = desDecrypt(y_b,sub_key);	//解密信息
			x = bitToChar(x_b);
			Dmsg += x;
			x_b.reset();		
			y_b.reset();	
			//out_msg.SetWindowText(Emsg.c_str());
			delete [] sub_key;
		}//DES结束
		else if(oped == 1){ //AES
			if(keyL != 16){
				MessageBox(_T("ERROR：密钥长度无效，\n请重新输入16字符密钥"));
				return;
			}
			Byte inkey_b[16];
			for(int k=0; k<16; ++k) //密钥字符串转化为二进制密钥字节数组
				inkey_b[k] |= p_inkey[k];
			word* exp_key = new word[44];
			Naes::keyExpan(inkey_b,exp_key);
			int i;
			for(i=0; i*16<msg_L; ++i){ //分组加密
				string x(msgp,i*16,16);
				Byte x_b[16];
				char y;
				for(int j=0; j<strlen(x.c_str()); ++j)
					x_b[j] |= x[j];
				aesEncrypt(x_b,exp_key);
				for(int k=0; k<16; ++k){
					y = x_b[k].to_ulong();
					Emsg += y;
				}
				aesDecrypt(x_b,exp_key);
				for(int k=0; k<16; ++k){
					y = x_b[k].to_ulong();
					Dmsg += y;
				}
			}
			delete [] exp_key;
		}//AES结束
		//用4字节写入加密后字节长度
		BigInt out_L = BigInt(Emsg.length()).values;
		int fn = 4 - out_L.values.length();
		while(fn>0){
			out_L.values.insert(0,1,'0');
			fn--;
		}
		////RSA加密解密
		BigInt big_key(p_inkey);	//获取公私钥
		CString pr,pu;
		in_prpu.GetWindowText(pu);
		in_prk.GetWindowText(pr);
		char* ppu = pu.GetBuffer();
		BigInt big_pu(ppu);
		char* ppr = pr.GetBuffer();
		BigInt big_pr(ppr);

		BigInt big_hash(shash);	//hash值RSA加密
		BigInt enc_hash = rsaEncrypt(big_hash,big_pr);
		Emsg = out_L.values.c_str() + Emsg;
		Emsg = Emsg + enc_hash.values.c_str();
		out_msg.SetWindowText(Emsg.c_str());
		BigInt dnc_hash = rsaEncrypt(enc_hash,big_pu);
		out_dnc_hash.SetWindowText(dnc_hash.values.c_str());

		emsg_box.SetWindowText(Dmsg.c_str()); 
		BigInt enc_key = rsaEncrypt(big_key,big_pr); //对称密钥RSA加密
		BigInt dnc_key = rsaDncrypt(enc_key,big_pu);
		out_key.SetWindowText(enc_key.values.c_str());
		/////3)RSA加密结束

		/*/计算解密后信息散列值
		CString dm;
		string dmphash;
		emsg_box.GetWindowText(dm);
		char* dmp = dm.GetBuffer();
		if(ophash == 0){ //MD5
			dmphash = hashMD5(dmp);
			out_hashcpr.SetWindowText(dmphash.c_str());
		}
		else if(ophash == 1){ //SHA-1
			dmphash = hashSha_1(dmp);
			out_hashcpr.SetWindowText(dmphash.c_str());
		}
		dm.ReleaseBuffer();*/
		inkey.ReleaseBuffer();
		msg.ReleaseBuffer();
	}//字符串操作结束

/************************对文件加密和hush运算****************************************/
	else if(opin == 1) 
	{
		CString ifname(msg + _T(".txt"));
		CString ofname_enc(msg +_T("_enc.txt"));
		CString ofname_dnc(msg + _T("_dnc.txt"));
		CFile infile;
		CFile outfile_enc;
		CFile outfile_dnc;
		///文件异常处理
		if(!(infile.Open(ifname,CFile::modeRead))){
			MessageBox(_T("ERROR：找不到输入文件路径！\n请重新输入"));
			return;
		}

		if(!(outfile_enc.Open(ofname_enc,CFile::modeCreate|CFile::modeWrite))){
			MessageBox(_T("ERROR：输出文件生成错误！"));
			return;
		}
		if(!(outfile_dnc.Open(ofname_dnc,CFile::modeCreate|CFile::modeReadWrite))){
			MessageBox(_T("ERROR：输出文件生成错误！"));
			return;
		}

		unsigned long fL = infile.GetLength();//文件字节数

		if(fL<=0){
			MessageBox(_T("ERROR：空文件！"));
			return;
		}
		if(fL>=UINT_MAX-1){
			MessageBox(_T("ERROR：文件过大！"));
			return;
		}

		//将文件存入字符数组
		char* msgp = new char[fL]; //动态分配相应长度char内存，存放输入字符串
		infile.Read(msgp,fL);
		///2）加密算法
		CString inkey;
		in_key.GetWindowText(inkey);
		int keyL = in_key.GetWindowTextLength();
		char* p_inkey = inkey.GetBuffer();	
		if(oped == 0){ //DES
			if(keyL != 8){
				MessageBox(_T("ERROR：密钥长度无效，\n请重新输入8字符密钥"));
				return;
			}
		//写入加密后文件长度
			BigInt out_L = BigInt(fL - fL%8 +8).values;
			int fn = 16 - out_L.values.length();
			while(fn>0){
				out_L.values.insert(0,1,'0');
				fn--;
			}
			outfile_enc.Write(out_L.values.c_str(),32);
		/////////////////
			bitset<64> inkey_b;
			inkey_b = charToBit(p_inkey);
			bitset<48>* sub_key = new bitset<48>[16];// 存放16轮子密钥
			Ndes::generKeys(inkey_b,sub_key); //生成子密钥
			//按64bit逐分组加密
			int i;
			bitset<64> x_b,y_b;//输入分组和输出分组64位二进制
			for(i=0; fL-i*8>8; ++i){
				string x(msgp,i*8,8);
				x_b = charToBit(x.c_str());
				y_b = desEncrypt(x_b,sub_key);
				string y = bitToChar(y_b);
				outfile_enc.Write(y.c_str(),8);
				x_b = desDecrypt(y_b,sub_key);
				x = bitToChar(x_b);
				outfile_dnc.Write(x.c_str(),8);
				x_b.reset();		//置0,相当于0填充
				y_b.reset();		
			}
			//处理最后一个分组
			string x(msgp,i*8,fL-i*8);
			for(unsigned int x_L = x.length();x_L<8;x_L++)
				x +='\0';		//0填充
			x_b = charToBit(x.c_str());
			y_b = desEncrypt(x_b,sub_key);
			string y = bitToChar(y_b);
			outfile_enc.Write(y.c_str(),8);
			x_b = desDecrypt(y_b,sub_key);
			x = bitToChar(x_b);
			outfile_dnc.Write(x.c_str(),8);
			x_b.reset();		
			y_b.reset();
			delete []sub_key;
		}//DES结束
		else if(oped == 1){ //AES
			if(keyL != 16){
				MessageBox(_T("ERROR：密钥长度无效！\n请重新输入16字符密钥"));
				return;
			}
			BigInt out_L = BigInt(fL - fL%16 + 16).values;
			int fn = 16 - out_L.values.length();
			while(fn>0){
				out_L.values.insert(0,1,'0');
				fn--;
			}
			outfile_enc.Write(out_L.values.c_str(),16);
			Byte* inkey_b = new Byte[16];
			for(int k=0; k<16; ++k){
				inkey_b[k].reset();
				inkey_b[k] |= p_inkey[k];
			}
			word* exp_key = new word[44];
			Naes::keyExpan(inkey_b,exp_key);
			int i;
			for(i=0; i*16 < fL; ++i){ //分组加密
				string x(msgp,i*16,16);
				Byte x_b[16];
				string y;
				string xx;
				for(int j=0; j<strlen(x.c_str()); ++j)
					x_b[j] |= x[j];
				aesEncrypt(x_b,exp_key);	//结果传入x_b
				for(int k=0; k<16; ++k){
					char a = x_b[k].to_ulong();
					y += a;
				}
				outfile_enc.Write(y.c_str(),16);
				aesDecrypt(x_b,exp_key);
				for(int k=0; k<16; ++k){
					char a = x_b[k].to_ulong();
					xx += a;
				}
				outfile_dnc.Write(xx.c_str(),16);
			}
			delete [] exp_key;
		}//AES结束
		///2）加密部分结束

		////3)对称密钥RSA加解密
		BigInt big_key(p_inkey);
		CString pr,pu;
		in_prpu.GetWindowText(pu);
		in_prk.GetWindowText(pr);
		char* ppu = pu.GetBuffer();
		BigInt big_pu(ppu);
		char* ppr = pr.GetBuffer();
		BigInt big_pr(ppr);
		BigInt enc_key = rsaEncrypt(big_key,big_pr);
		BigInt dnc_key = rsaDncrypt(enc_key,big_pu);
		out_key.SetWindowText(enc_key.values.c_str());
		/////3)RSA加密结束


		inkey.ReleaseBuffer();
		
		///1）散列部分开始
		if(ophash == 0){ //MD5
			shash = hashMD5(msgp);
			out_hash.SetWindowText(shash.c_str());
			//outfile_enc.Write(shash.c_str(),sizeof(shash));
		}
		else if(ophash == 1){//SHA-1
			shash = hashSha_1(msgp);
			out_hash.SetWindowText(shash.c_str());
			//outfile_enc.Write(shash.c_str(),sizeof(shash));
		}
		delete []msgp;
		//AES加解密哈希值
		BigInt big_hash(shash);
		BigInt enc_hash = rsaEncrypt(big_hash,big_pr);
		outfile_enc.Write(enc_hash.values.c_str(),sizeof(enc_hash.values));
		BigInt dnc_hash = rsaEncrypt(enc_hash,big_pu);
		out_dnc_hash.SetWindowText(dnc_hash.values.c_str());
		////1）散列部分结束

		//解密信息哈希值
		/*unsigned long dfL = outfile_dnc.GetLength();//文件字节数
		char* dmsgp = new char[fL]; //动态分配相应长度char内存，存放输入字符串
		outfile_dnc.Read(dmsgp,fL);
		string dnchash;
		if(ophash == 0){ //MD5
			dnchash = hashMD5(dmsgp);
			out_hashcpr.SetWindowText(dnchash.c_str());
			//outfile_enc.Write(shash.c_str(),sizeof(shash));
		}
		else if(ophash == 1){//SHA-1
			dnchash = hashSha_1(dmsgp);
			out_hashcpr.SetWindowText(dnchash.c_str());
			//outfile_enc.Write(shash.c_str(),sizeof(shash));
		}
		delete []dmsgp;*/

		infile.Close();
		outfile_enc.Close();
		outfile_dnc.Close();
		out_msg.SetWindowText(ofname_enc);
		emsg_box.SetWindowText(ofname_dnc);
	}//输入输出文件运算成功

	else return;
}


void CEDncryptDlg::OnBnClickedGenePrPu()
{
	USES_CONVERSION;
	// TODO: 在此添加控件通知处理程序代码
	BigInt pr,pu;
	if(genePrPu(pr,pu)){
		string s("PrivateKey:"+pr.values+"\nPublicKey:"+pu.values);
		out_msg.SetWindowText(s.c_str());
	}
	else MessageBox(_T("生成失败，请重试"));
}


