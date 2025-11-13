// MyAboutBox.cpp: 实现文件
//

#include "pch.h"
#include "VHDXSnap02.h"
#include "MyAboutBox.h"
#include "afxdialogex.h"


// MyAboutBox 对话框

IMPLEMENT_DYNAMIC(MyAboutBox, CDialogEx)

MyAboutBox::MyAboutBox(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ABOUTBOX, pParent)
{
	/*CDC* dc = GetDC();
	CFont font;
	font.CreatePointFont(60, _T("微软雅黑"), dc);//0 for default size
	CFont* oldFont = (CFont*)dc->SelectObject(&font);
	*/
}

MyAboutBox::~MyAboutBox()
{

}

void MyAboutBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(MyAboutBox, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// MyAboutBox 消息处理程序


BOOL MyAboutBox::OnEraseBkgnd(CDC* pDC)
{
	//return TRUE;
	return CDialogEx::OnEraseBkgnd(pDC);
}


void MyAboutBox::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CFont font;
	font.CreatePointFont(90, _T("微软雅黑"), &dc);//0 for default size
	CFont* oldFont = (CFont*)dc.SelectObject(font);
	RECT* rect = new RECT;
	GetClientRect(rect);
	dc.DrawText(__TEXT("该软件利用差分VHDX实现磁盘的备份和快照\n操作\nSnapshot：拍摄快照（不支持热操作）\n（创建当前VHDX的子VHDX并切换启动项）\nDelete：删除快照（内容合并到当前或先前）\n（该操作会一并删除后续子快照）\n（从当前VHDX开始一直合并(包括目标)之后在建子VHDX）\nRecover：恢复到快照\n（删除快照之后的VHDX链条并创建子VHDX）\n（切换默认启动项为新子VHDX）\nSwitchTo：切换分支\n（更改默认启动项为目标的\"当前\"VHDX）\nFork：产生分支 (热操作)\n（对当前VHDX生成两个子VHDX）\n（或对快照VHDX产生子VHDX)\n\n说明：\n1.差分VHDX的父磁盘最好不要修改 因此\n a)拍摄快照需要父磁盘位于冷态\n b)不支持直接并行切换到之前的状态而要Fork\n c)当前状态与快照有一层隔离VHDX\n（假如有n个快照，实际有n+1个VHDX）\n2.不支持分支合并,文件版本处理(保留哪个分支的)是困难的\n3.本软件是测试用,不对数据及硬件负任何责任\n"), rect, DT_LEFT);
	//DrawText(dc.GetSafeHdc(), __TEXT("该软件利用差分VHDX实现磁盘的备份和快照\n操作\nSnapshot：拍摄快照（不支持热操作）\n（创建当前VHDX的子VHDX并切换启动项）\nDelete：删除快照（内容合并到当前或先前）\n（该操作会一并删除后续子快照）\n（从当前VHDX开始一直合并(包括目标)之后在建子VHDX）\nRecover：恢复到快照\n（删除快照之后的VHDX链条并创建子VHDX）\n（切换默认启动项为新子VHDX）\nSwitchTo：切换分支\n（更改默认启动项为目标的\"当前\"VHDX）\nFork：产生分支 (热操作)\n（对当前VHDX生成两个子VHDX）\n（或对快照VHDX产生子VHDX)\n\n说明：\n1.差分VHDX的父磁盘最好不要修改 因此\n a)拍摄快照需要父磁盘位于冷态\n b)不支持直接并行切换到之前的状态而要Fork\n c)当前状态与快照有一层隔离VHDX\n（假如有n个快照，实际有n+1个VHDX）\n2.不支持分支合并,文件版本处理(保留哪个分支的)是困难的\n3.本软件是测试用,不对数据及硬件负任何责任\n"), 9, rect, DT_CENTER);
}
