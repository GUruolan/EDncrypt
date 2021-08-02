// EDncryptDlg.cpp : ʵ���ļ�
// ���ͷ������ַ������ļ�����󳤶�Ϊ2^64-1qa

#include "stdafx.h"
#include "EDncrypt.h"
#include "EDncryptDlg.h"
#include "afxdialogex.h"
#include "atlstr.h"

//�Զ���ͷ�ļ�
#include "toolFunc.h"		//ͨ�ù��ߺ���
#include "MD5.h"
#include "SHA-1.h"
#include "DES.h"
#include "AES.h"
#include "RSA.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CEDncryptDlg �Ի���



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


// CEDncryptDlg ��Ϣ�������

BOOL CEDncryptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	ShowWindow(SW_MINIMIZE);


	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//����ѡ������ļ����ַ���
	op_str_file.AddString(_T("�ַ���"));
	op_str_file.AddString(_T("�ļ�"));
	//����ѡ�ɢ�к���MD5/SHA-1
	op_md5_sha.AddString(_T("MD5"));
	op_md5_sha.AddString(_T("SHA-1"));
	//����ѡ��ӽ����㷨ѡ��
	op_des_aed.AddString(_T("DES"));
	op_des_aed.AddString(_T("AES"));

	in_msg.SetLimitText(200);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEDncryptDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);
		// ʹͼ���ڹ����������о���

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		font.CreatePointFont(125,_T("����"));
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

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CEDncryptDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**************************�����ȫͨ��ģ�ⰴť�¼�����************************************/
void CEDncryptDlg::OnBnClickedEncrypt()	
{
	USES_CONVERSION;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ophash,oped,opin;	//ѡ�����
	ophash = op_md5_sha.GetCurSel();
	oped = op_des_aed.GetCurSel();
	opin = op_str_file.GetCurSel();

	unsigned long long msg_L = in_msg.GetWindowTextLength(); //�ַ����ֽڳ���
	if(msg_L<=0){
		MessageBox(_T("ERROR���������������Ϣ"));
		return;
	}

	CString msg;
	string shash;	//16���Ƶ�ɢ��ֵ,�����ַ���
	string Emsg,Dmsg;
	Emsg.reserve(300);
	Dmsg.reserve(248);
	in_msg.GetWindowText(msg); //��ȡ������Ϣ
	

/************************���ַ������ܺ�hush����********************************************/
	if(opin == 0){ 
		char* msgp = msg.GetBuffer();	//msgpΪָ��msg��charָ�룬 ��ΪCString����ָ���޷���Ϊ��������
		
		if(msg_L>200){ //�������ɢ�к��������2^64-1bit����
			MessageBox(_T("ERROR���ַ���������"));
			return;
		}

		///1��ɢ�к���
		if(ophash == 0){ //MD5
			shash = hashMD5(msgp);
			out_hash.SetWindowText(shash.c_str());
		}
		else if(ophash == 1){ //SHA-1
			shash = hashSha_1(msgp);
			out_hash.SetWindowText(shash.c_str());
		}
		////1��ɢ�к�������
		/////�Գ���Կ��ȡ
		CString inkey;
		in_key.GetWindowText(inkey);
		int keyL = in_key.GetWindowTextLength();
		char* p_inkey = inkey.GetBuffer();

		///2�������㷨
		if(oped == 0){ //DES
			if(keyL != 8){
				MessageBox(_T("ERROR����Կ������Ч��\n����������8�ַ���Կ"));
				return;
			}
			bitset<64> inkey_b;
			inkey_b = charToBit(p_inkey);
			bitset<48>* sub_key = new bitset<48>[16];// ���16������Կ
			Ndes::generKeys(inkey_b,sub_key); //��������Կ
			//unsigned long long Emsg_L;
			//��64bit��������
			int i=0;
			bitset<64> x_b,y_b;//���������������64λ������
			for(i=0; msg_L-i*8>8; ++i){ //�������
				string x(msgp,i*8,8);
				x_b = charToBit(x.c_str());
				y_b = desEncrypt(x_b,sub_key);
				string y = bitToChar(y_b);
				Emsg += y;
				x_b = desDecrypt(y_b,sub_key);
				x = bitToChar(x_b);

				Dmsg += x;
				x_b.reset();		//��0
				y_b.reset();		
			}
			//�������һ������
			string x(msgp,i*8,msg_L-i*8);
			for(unsigned int x_L = x.length();x_L<8;x_L++)
				x +='\0';		//0���
			x_b = charToBit(x.c_str());
			y_b = desEncrypt(x_b,sub_key);
			string y = bitToChar(y_b);	//������Ϣ
			Emsg += y;
			x_b = desDecrypt(y_b,sub_key);	//������Ϣ
			x = bitToChar(x_b);
			Dmsg += x;
			x_b.reset();		
			y_b.reset();	
			//out_msg.SetWindowText(Emsg.c_str());
			delete [] sub_key;
		}//DES����
		else if(oped == 1){ //AES
			if(keyL != 16){
				MessageBox(_T("ERROR����Կ������Ч��\n����������16�ַ���Կ"));
				return;
			}
			Byte inkey_b[16];
			for(int k=0; k<16; ++k) //��Կ�ַ���ת��Ϊ��������Կ�ֽ�����
				inkey_b[k] |= p_inkey[k];
			word* exp_key = new word[44];
			Naes::keyExpan(inkey_b,exp_key);
			int i;
			for(i=0; i*16<msg_L; ++i){ //�������
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
		}//AES����
		//��4�ֽ�д����ܺ��ֽڳ���
		BigInt out_L = BigInt(Emsg.length()).values;
		int fn = 4 - out_L.values.length();
		while(fn>0){
			out_L.values.insert(0,1,'0');
			fn--;
		}
		////RSA���ܽ���
		BigInt big_key(p_inkey);	//��ȡ��˽Կ
		CString pr,pu;
		in_prpu.GetWindowText(pu);
		in_prk.GetWindowText(pr);
		char* ppu = pu.GetBuffer();
		BigInt big_pu(ppu);
		char* ppr = pr.GetBuffer();
		BigInt big_pr(ppr);

		BigInt big_hash(shash);	//hashֵRSA����
		BigInt enc_hash = rsaEncrypt(big_hash,big_pr);
		Emsg = out_L.values.c_str() + Emsg;
		Emsg = Emsg + enc_hash.values.c_str();
		out_msg.SetWindowText(Emsg.c_str());
		BigInt dnc_hash = rsaEncrypt(enc_hash,big_pu);
		out_dnc_hash.SetWindowText(dnc_hash.values.c_str());

		emsg_box.SetWindowText(Dmsg.c_str()); 
		BigInt enc_key = rsaEncrypt(big_key,big_pr); //�Գ���ԿRSA����
		BigInt dnc_key = rsaDncrypt(enc_key,big_pu);
		out_key.SetWindowText(enc_key.values.c_str());
		/////3)RSA���ܽ���

		/*/������ܺ���Ϣɢ��ֵ
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
	}//�ַ�����������

/************************���ļ����ܺ�hush����****************************************/
	else if(opin == 1) 
	{
		CString ifname(msg + _T(".txt"));
		CString ofname_enc(msg +_T("_enc.txt"));
		CString ofname_dnc(msg + _T("_dnc.txt"));
		CFile infile;
		CFile outfile_enc;
		CFile outfile_dnc;
		///�ļ��쳣����
		if(!(infile.Open(ifname,CFile::modeRead))){
			MessageBox(_T("ERROR���Ҳ��������ļ�·����\n����������"));
			return;
		}

		if(!(outfile_enc.Open(ofname_enc,CFile::modeCreate|CFile::modeWrite))){
			MessageBox(_T("ERROR������ļ����ɴ���"));
			return;
		}
		if(!(outfile_dnc.Open(ofname_dnc,CFile::modeCreate|CFile::modeReadWrite))){
			MessageBox(_T("ERROR������ļ����ɴ���"));
			return;
		}

		unsigned long fL = infile.GetLength();//�ļ��ֽ���

		if(fL<=0){
			MessageBox(_T("ERROR�����ļ���"));
			return;
		}
		if(fL>=UINT_MAX-1){
			MessageBox(_T("ERROR���ļ�����"));
			return;
		}

		//���ļ������ַ�����
		char* msgp = new char[fL]; //��̬������Ӧ����char�ڴ棬��������ַ���
		infile.Read(msgp,fL);
		///2�������㷨
		CString inkey;
		in_key.GetWindowText(inkey);
		int keyL = in_key.GetWindowTextLength();
		char* p_inkey = inkey.GetBuffer();	
		if(oped == 0){ //DES
			if(keyL != 8){
				MessageBox(_T("ERROR����Կ������Ч��\n����������8�ַ���Կ"));
				return;
			}
		//д����ܺ��ļ�����
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
			bitset<48>* sub_key = new bitset<48>[16];// ���16������Կ
			Ndes::generKeys(inkey_b,sub_key); //��������Կ
			//��64bit��������
			int i;
			bitset<64> x_b,y_b;//���������������64λ������
			for(i=0; fL-i*8>8; ++i){
				string x(msgp,i*8,8);
				x_b = charToBit(x.c_str());
				y_b = desEncrypt(x_b,sub_key);
				string y = bitToChar(y_b);
				outfile_enc.Write(y.c_str(),8);
				x_b = desDecrypt(y_b,sub_key);
				x = bitToChar(x_b);
				outfile_dnc.Write(x.c_str(),8);
				x_b.reset();		//��0,�൱��0���
				y_b.reset();		
			}
			//�������һ������
			string x(msgp,i*8,fL-i*8);
			for(unsigned int x_L = x.length();x_L<8;x_L++)
				x +='\0';		//0���
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
		}//DES����
		else if(oped == 1){ //AES
			if(keyL != 16){
				MessageBox(_T("ERROR����Կ������Ч��\n����������16�ַ���Կ"));
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
			for(i=0; i*16 < fL; ++i){ //�������
				string x(msgp,i*16,16);
				Byte x_b[16];
				string y;
				string xx;
				for(int j=0; j<strlen(x.c_str()); ++j)
					x_b[j] |= x[j];
				aesEncrypt(x_b,exp_key);	//�������x_b
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
		}//AES����
		///2�����ܲ��ֽ���

		////3)�Գ���ԿRSA�ӽ���
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
		/////3)RSA���ܽ���


		inkey.ReleaseBuffer();
		
		///1��ɢ�в��ֿ�ʼ
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
		//AES�ӽ��ܹ�ϣֵ
		BigInt big_hash(shash);
		BigInt enc_hash = rsaEncrypt(big_hash,big_pr);
		outfile_enc.Write(enc_hash.values.c_str(),sizeof(enc_hash.values));
		BigInt dnc_hash = rsaEncrypt(enc_hash,big_pu);
		out_dnc_hash.SetWindowText(dnc_hash.values.c_str());
		////1��ɢ�в��ֽ���

		//������Ϣ��ϣֵ
		/*unsigned long dfL = outfile_dnc.GetLength();//�ļ��ֽ���
		char* dmsgp = new char[fL]; //��̬������Ӧ����char�ڴ棬��������ַ���
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
	}//��������ļ�����ɹ�

	else return;
}


void CEDncryptDlg::OnBnClickedGenePrPu()
{
	USES_CONVERSION;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	BigInt pr,pu;
	if(genePrPu(pr,pu)){
		string s("PrivateKey:"+pr.values+"\nPublicKey:"+pu.values);
		out_msg.SetWindowText(s.c_str());
	}
	else MessageBox(_T("����ʧ�ܣ�������"));
}


