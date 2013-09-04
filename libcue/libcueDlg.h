
// libcueDlg.h : ͷ�ļ�
//

#pragma once

// ClibcueDlg �Ի���
class ClibcueDlg : public CDialogEx
{
// ����
public:
	ClibcueDlg(CWnd* pParent = NULL);	// ��׼���캯��
		~ClibcueDlg();
// �Ի�������
	enum { IDD = IDD_LIBCUE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	BOOL     m_bNeedConvert;        //��Ҫת��
	char*    m_RawString;           //ԭʼ�ַ��������ı���ȡ����BOM��
	UINT     m_RawStringLength;     //ԭʼ�ַ����ĳ��ȣ����ı���ȡ����BOM����)
	char*    m_String;              //�ַ���������BOM��
	UINT     m_StringLength;        //�ַ����ĳ��ȣ�����BOM��
	CString  m_StringCodeType;      //�ַ�����������
	wchar_t* m_UnicodeString;       //Unicode�ַ���
	UINT     m_UnicodeLength;       //Unicode�ַ����ĳ���
	CString  m_FilePathName;        //�ı��ļ�·��
	CC4Context* m_context;          // converting context


	void FixCue();
	BOOL DealFile();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeList1();
};
