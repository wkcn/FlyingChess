
// FlightServerView.cpp : CFlightServerView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CFlightServerView ����/����

CFlightServerView::CFlightServerView()
{
	// TODO: �ڴ˴���ӹ������
	InitConsole();
}

CFlightServerView::~CFlightServerView()
{
}

BOOL CFlightServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CFlightServerView ����

void CFlightServerView::OnDraw(CDC* /*pDC*/)
{
	CFlightServerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CFlightServerView ��ӡ

BOOL CFlightServerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CFlightServerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CFlightServerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CFlightServerView ���

#ifdef _DEBUG
void CFlightServerView::AssertValid() const
{
	CView::AssertValid();
}

void CFlightServerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CFlightServerDoc* CFlightServerView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CFlightServerDoc)));
	return (CFlightServerDoc*)m_pDocument;
}
#endif //_DEBUG


// CFlightServerView ��Ϣ�������


void CFlightServerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	AfxSocketInit();

	server.Create(4000);
	server.Listen();

	SetTimer(1, (UINT)(1000.0), NULL);
	SetTimer(2, (UINT)(100.0), NULL);
	// SetTimer(3, (UINT)(1000.0), NULL);
	// TODO: �ڴ����ר�ô����/����û���
}


void CFlightServerView::OnTimer(UINT_PTR nIDEvent)
{
	const int siz = 160;
	static char buf_last[siz];
	static CAsyncSocket * solast = 0;
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (nIDEvent == 1) {
		CAsyncSocket *so = new CAsyncSocket();
		if (server.Accept(*so)) {
			sockets.push_back(so);
			cout << "Join: " << int(so) << endl;
			//::MessageBox(NULL, L"��Ҽ���", L"��Ϸ��ʾ", MB_OK);
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
