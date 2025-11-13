#pragma once


// MyAboutBox 对话框

class MyAboutBox : public CDialogEx
{
	DECLARE_DYNAMIC(MyAboutBox)

public:
	MyAboutBox(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~MyAboutBox();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
