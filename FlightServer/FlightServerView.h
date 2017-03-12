
// FlightServerView.h : CFlightServerView ��Ľӿ�
//

#pragma once

#include <vector>
using namespace std;

class CFlightServerView : public CView
{
protected: // �������л�����
	CFlightServerView();
	DECLARE_DYNCREATE(CFlightServerView)

// ����
public:
	CFlightServerDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CFlightServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	CAsyncSocket server;
	vector<CAsyncSocket *> sockets;
};

#ifndef _DEBUG  // FlightServerView.cpp �еĵ��԰汾
inline CFlightServerDoc* CFlightServerView::GetDocument() const
   { return reinterpret_cast<CFlightServerDoc*>(m_pDocument); }
#endif

