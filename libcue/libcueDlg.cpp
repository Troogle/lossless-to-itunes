
// libcueDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "libcue.h"
#include "libcueDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ClibcueDlg �Ի���

ClibcueDlg::ClibcueDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ClibcueDlg::IDD, pParent),m_bNeedConvert(TRUE),m_RawStringLength(0),m_StringLength(0),m_UnicodeLength(0),
	m_StringCodeType("Local Codepage")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_FilePathName="";
	m_RawString=NULL;
	m_String=NULL;
	m_UnicodeString=NULL;

	TCHAR szFull[_MAX_PATH];
	GetModuleFileName(NULL,szFull,_MAX_PATH);
	CString processPath(szFull);
	int pos;
	pos=processPath.ReverseFind('\\');
	processPath=processPath.Left(pos+1);
	CString m_map=_T("charmap-anisong.xml");
	m_context = new CC4Context((LPCTSTR)m_map, (LPCTSTR)processPath);
	if (!m_context->init())
	AfxMessageBox(_T("Failed to load charmaps!"));

}

ClibcueDlg::~ClibcueDlg()
{
	if (m_RawString)
	{
		delete []m_RawString;
		m_RawString=NULL;
	}
	if (m_UnicodeString)
	{
		delete []m_UnicodeString;
		m_UnicodeString=NULL;
	}
	if (m_context)
	{
		m_context->finalize();
		delete m_context;
		m_context = NULL;
	}
}
void ClibcueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(ClibcueDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_BN_CLICKED(IDOK, &ClibcueDlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST1, &ClibcueDlg::OnLbnSelchangeList1)

END_MESSAGE_MAP()


// ClibcueDlg ��Ϣ�������

BOOL ClibcueDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	CListBox *theList;
	theList=(CListBox*)GetDlgItem(IDC_LIST1);
	std::list<std::wstring> &encodeList = m_context->getEncodesNameList();
	std::list<std::wstring>::iterator iter;
	theList->InsertString(-1,_T("Local Codepage"));
	for (iter = encodeList.begin(); iter != encodeList.end(); iter++)
		theList->InsertString(-1, iter->c_str());
	theList->SetCurSel(0);
	if (AfxGetApp()->m_lpCmdLine[0]!=_T('\0'))
        {
                m_FilePathName=AfxGetApp()->m_lpCmdLine;
                if (m_FilePathName.GetAt(0)==_T('\"'))
                        m_FilePathName.Delete(0,1);
                if (m_FilePathName.GetAt(m_FilePathName.GetLength()-1)==_T('\"'))
                        m_FilePathName=m_FilePathName.Left(m_FilePathName.GetLength()-1);
                CString ExtensionName,FileName;
                ExtensionName=m_FilePathName.Right(m_FilePathName.GetLength()-m_FilePathName.ReverseFind('.')-1).MakeLower();
                FileName=m_FilePathName.Right(m_FilePathName.GetLength()-m_FilePathName.ReverseFind('\\')-1);
                if (TRUE==DealFile())
                        {
                                 FixCue();
                        }
                }

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void ClibcueDlg::OnPaint()
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
HCURSOR ClibcueDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL ClibcueDlg::DealFile()
{
	if (m_FilePathName==_T(""))
		return FALSE;
	CFile OpenFile;
	if (!OpenFile.Open(m_FilePathName,CFile::modeRead|CFile::shareDenyWrite|CFile::typeBinary))
	{
		OpenFile.Close();
		::AfxMessageBox(_T("��ʧ�ܣ�"),MB_OK);
		return FALSE;
	}
	m_bNeedConvert=TRUE;
	m_RawStringLength=OpenFile.GetLength();
	if (m_RawString)
	{
		delete []m_RawString;
		m_RawString=NULL;
		m_String=NULL;
	}
	if (m_UnicodeString)
	{
		delete []m_UnicodeString;
		m_UnicodeString=NULL;
	}
	m_RawString=new char[m_RawStringLength+1];
	OpenFile.Read(m_RawString,m_RawStringLength);
	OpenFile.Close();
	m_RawString[m_RawStringLength]='\0';
	m_String=m_RawString;
	m_StringLength=m_RawStringLength;

	CComboBox *theList  =(CComboBox*)GetDlgItem(IDC_LIST1);
	CEdit *RightEdit=(CEdit *)GetDlgItem(IDC_EDIT1);

	// Unicode(little-endian)
	if (((unsigned char)m_RawString[0]==0xFF)&&((unsigned char)m_RawString[1]==0xFE))
	{
		m_bNeedConvert=FALSE;
		m_StringCodeType=CC4EncodeUTF16::_getName().c_str();
		int nIndex = theList->FindStringExact(0, m_StringCodeType);
		theList->SetCurSel(nIndex);
		m_String=m_RawString+2; //��������ʼ��ַ
		m_StringLength=m_RawStringLength-2; //�����ĳ���
		if ((m_RawStringLength%2)!=0)
		{
			MessageBox(_T("�ı�����"));
			return FALSE;
		}
		m_UnicodeLength=m_StringLength>>1;
		m_UnicodeString=new wchar_t[m_UnicodeLength+1];
		memcpy((void*)m_UnicodeString,m_String,m_StringLength);
		m_UnicodeString[m_UnicodeLength]='\0';
	}
	// Unicode(big-endian)
	if (((unsigned char)m_RawString[0]==0xFE)&&((unsigned char)m_RawString[1]==0xFF))
	{
		m_bNeedConvert=FALSE;
		m_StringCodeType=CC4EncodeUTF16::_getName().c_str();
		int nIndex = theList->FindStringExact(0, m_StringCodeType);
		theList->SetCurSel(nIndex);
		m_String=m_RawString+2; //��������ʼ��ַ
		m_StringLength=m_RawStringLength-2; //�����ĳ���
		if ((m_RawStringLength%2)!=0)
		{
			MessageBox(_T("�ı�����"));
			return FALSE;
		}
		m_UnicodeLength=m_StringLength>>1;
		m_UnicodeString=new wchar_t[m_UnicodeLength+1];
		memcpy((void*)m_UnicodeString,m_String,m_StringLength);
		m_UnicodeString[m_UnicodeLength]='\0';
		//�����ߵ�λ˳��
		for (UINT i=0;i<m_UnicodeLength;i++)
		{
			unsigned char chars[2];
			memcpy(chars,(void*)(m_UnicodeString+i),2);
			wchar_t theChr=chars[0]*256+chars[1];
			m_UnicodeString[i]=theChr;
		}
	}
	// UTF-8(with BOM)
	if (((unsigned char)m_RawString[0]==0xEF)&&((unsigned char)m_RawString[1]==0xBB)&&((unsigned char)m_RawString[2]==0xBF))
	{
		m_bNeedConvert=FALSE;
		m_StringCodeType=CC4EncodeUTF8::_getName().c_str();
		int nIndex = theList->FindStringExact(0, m_StringCodeType);
		theList->SetCurSel(nIndex);
		m_String=m_RawString+3; //��������ʼ��ַ
		m_StringLength=m_RawStringLength-3; //�����ĳ���
	}

	if (m_bNeedConvert==FALSE)
	{
		if (m_StringCodeType==CC4EncodeUTF16::_getName().c_str())
		{
			CString RightEditText(m_UnicodeString);
			RightEdit->SetWindowText(RightEditText);
		}
		if (m_StringCodeType==CC4EncodeUTF8::_getName().c_str())
		{
			RightEdit->SetWindowText(CC4EncodeUTF8::convert2unicode(m_String,m_StringLength).c_str());
		}
	}
	else
	{
		//������

			const CC4Encode *encode = m_context->getMostPossibleEncode(m_String);
			if (encode)
			{
				m_StringCodeType = encode->getName().c_str();
				int nIndex = theList->FindStringExact(0, m_StringCodeType);
				theList->SetCurSel(nIndex);
			} else {
				theList->GetLBText(0,m_StringCodeType);
				theList->SetCurSel(0);
			}
		//��
		const CC4Encode *encode1 = m_context->getEncode((LPCTSTR)m_StringCodeType);
		if (encode1)
			RightEdit->SetWindowText(encode1->wconvertText(m_String, m_StringLength).c_str());
		else
			RightEdit->SetWindowText(CString(m_String));
	}
	return TRUE;
}

void ClibcueDlg::OnDropFiles(HDROP hDropInfo)
{
		int nFileCount;  
		nFileCount=::DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, MAX_PATH);
		if(nFileCount == 1)
		{
			TCHAR szFileName[MAX_PATH+1];
			::DragQueryFile(hDropInfo, 0, szFileName, MAX_PATH);
			m_FilePathName=CString(szFileName);
			CString ExtensionName,FileName;
			ExtensionName=m_FilePathName.Right(m_FilePathName.GetLength()-m_FilePathName.ReverseFind('.')-1).MakeLower();
			FileName=m_FilePathName.Right(m_FilePathName.GetLength()-m_FilePathName.ReverseFind('\\')-1);
			DealFile();
			FixCue();
			}
		else
		{
			::AfxMessageBox(_T(" ֻ��ͬʱ��һ���ļ�"),MB_OK);
		}
		::DragFinish(hDropInfo);
	CDialog::OnDropFiles(hDropInfo);
}

void ClibcueDlg::OnLbnSelchangeList1()
{
	CComboBox *theList;
	theList=(CComboBox*)GetDlgItem(IDC_LIST1);
	if (m_bNeedConvert)
	{
		theList->GetWindowText(m_StringCodeType);
		const CC4Encode *encode = m_context->getEncode((LPCTSTR)m_StringCodeType);
		if (encode)
			GetDlgItem(IDC_EDIT1)->SetWindowText(encode->wconvertText(m_String, m_StringLength).c_str());
		else
			GetDlgItem(IDC_EDIT1)->SetWindowText(CString(m_String));
		FixCue();
	}
}

void ClibcueDlg::OnBnClickedOk()
{
	CFile SaveFile;
	if (!SaveFile.Open(m_FilePathName,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive|CFile::typeBinary))
	{
		::AfxMessageBox(_T("�޷�д���ļ���"),MB_OK);
		return;
	}
	CString UnicodeStr;
	GetDlgItem(IDC_EDIT1)->GetWindowText(UnicodeStr);
	std::string &utf8str = CC4EncodeUTF16::convert2utf8((LPCTSTR)UnicodeStr, UnicodeStr.GetLength());
	SaveFile.Write(CC4Encode::UTF_8_BOM, 3);
	SaveFile.Write(utf8str.c_str(), utf8str.length());
	SaveFile.Close();
	CDialogEx::OnOK();
}

void ClibcueDlg::FixCue()
{
	CString CueString;
	GetDlgItem(IDC_EDIT1)->GetWindowText(CueString);
	int BeginPos=CueString.Find(_T("FILE \""));
	int EndPos=CueString.Find(_T("\" WAVE"));
	BeginPos+=6;

	CString MusicFileName,FindFileName; //��Ƶ�ļ�����·��

	MusicFileName=CueString.Mid(BeginPos,EndPos-BeginPos);

	int pos=MusicFileName.ReverseFind('.');
	//�滻��չ��
	FindFileName=MusicFileName.Left(pos)+_T(".ape"); //ape
	CueString.Replace(MusicFileName,FindFileName);
	GetDlgItem(IDC_EDIT1)->SetWindowText(CueString);
	return;
	}