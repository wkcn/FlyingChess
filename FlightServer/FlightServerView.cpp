
// FlightServerView.cpp : CFlightServerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "FlightServer.h"
#endif

#include "FlightServerDoc.h"
#include "FlightServerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
	
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <set>
using namespace std;

void InitConsole() {
	AllocConsole();
	freopen("CON", "r", stdin);
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
}


// CFlightServerView

IMPLEMENT_DYNCREATE(CFlightServerView, CView)

BEGIN_MESSAGE_MAP(CFlightServerView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CFlightServerView 构造/析构

CFlightServerView::CFlightServerView()
{
	// TODO: 在此处添加构造代码
	InitConsole();
}

CFlightServerView::~CFlightServerView()
{
}

BOOL CFlightServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CFlightServerView 绘制

void CFlightServerView::OnDraw(CDC* /*pDC*/)
{
	CFlightServerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CFlightServerView 打印

BOOL CFlightServerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CFlightServerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CFlightServerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CFlightServerView 诊断

#ifdef _DEBUG
void CFlightServerView::AssertValid() const
{
	CView::AssertValid();
}

void CFlightServerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFlightServerDoc* CFlightServerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFlightServerDoc)));
	return (CFlightServerDoc*)m_pDocument;
}
#endif //_DEBUG


// CFlightServerView 消息处理程序


void CFlightServerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	AfxSocketInit();

	server.Create(4000);
	server.Listen();

	SetTimer(1, (UINT)(1000.0), NULL);
	SetTimer(2, (UINT)(100.0), NULL);
	// SetTimer(3, (UINT)(1000.0), NULL);
	// TODO: 在此添加专用代码和/或调用基类
}


void CFlightServerView::OnTimer(UINT_PTR nIDEvent)
{
	const int siz = 160;
	static char buf_last[siz];
	static CAsyncSocket * solast = 0;
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == 1) {
		CAsyncSocket *so = new CAsyncSocket();
		if (server.Accept(*so)) {
			sockets.push_back(so);
			cout << "Join: " << int(so) << endl;
			//::MessageBox(NULL, L"玩家加入", L"游戏提示", MB_OK);
		}
		else {
			delete so;
		}
	}else if(nIDEvent == 2){
		set<int> del;
		for (int w = 0;w < sockets.size();++w) {
			CAsyncSocket *so = sockets[w];
			char buf[siz];
			if (so->Receive(buf, siz) > 0) {
				cout << "Recv" << int(so) << endl;
				const char CHECK_CODE[16] = "abcdflightabcde";
				bool failed = false;
				for (int i = 0;i < 16;++i) {
					if (buf[i] != CHECK_CODE[i]) {
						failed = true;
						break;
					}
				}
				if (failed) {
					del.insert(w);
					cout << "Fail" << endl;
				}
				else {
					for (CAsyncSocket *to : sockets) {
						if (so != to) {
							cout << time(0) << "Send To" << int(to) << endl;
							to->Send(buf, siz);
						}
					}
					//solast = so;
					//memcpy(buf_last, buf, siz);
				}
			}
		}

		if (!del.empty()) {
			vector<CAsyncSocket *> s = sockets;
			sockets.clear();
			for (int w = 0;w < s.size();++w) {
				if (!del.count(w))sockets.push_back(s[w]);
				else {
					cout << "Del: " << s[w] << endl;
				}
			}
		}

	}
	else if (nIDEvent == 3) {
		if (solast) {
			for (CAsyncSocket *to : sockets) {
				if (solast != to) {
					cout << time(0) << "Send To" << int(to) << endl;
					to->Send(buf_last, siz);
				}
			}
			solast = 0;
		}
	}
	CView::OnTimer(nIDEvent);
}
