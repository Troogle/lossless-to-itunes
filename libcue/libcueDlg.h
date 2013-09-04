
// libcueDlg.h : 头文件
//

#pragma once

// ClibcueDlg 对话框
class ClibcueDlg : public CDialogEx
{
// 构造
public:
	ClibcueDlg(CWnd* pParent = NULL);	// 标准构造函数
		~ClibcueDlg();
// 对话框数据
	enum { IDD = IDD_LIBCUE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	BOOL     m_bNeedConvert;        //需要转换
	char*    m_RawString;           //原始字符串（从文本读取，含BOM）
	UINT     m_RawStringLength;     //原始字符串的长度（从文本获取，含BOM长度)
	char*    m_String;              //字符串（不含BOM）
	UINT     m_StringLength;        //字符串的长度（不含BOM）
	CString  m_StringCodeType;      //字符串编码类型
	wchar_t* m_UnicodeString;       //Unicode字符串
	UINT     m_UnicodeLength;       //Unicode字符串的长度
	CString  m_FilePathName;        //文本文件路径
	CC4Context* m_context;          // converting context


	void FixCue();
	BOOL DealFile();

	// 生成的消息映射函数
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
