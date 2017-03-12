
// FlightServerView.h : CFlightServerView 类的接口
//

#pragma once

#include <vector>
using namespace std;

class CFlightServerView : public CView
{
protected: // 仅从序列化创建
	CFlightServerView();
	DECLARE_DYNCREATE(CFlightServerView)

// 特性
public:
	CFlightServerDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CFlightServerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

public:
	CAsyncSocket server;
	vector<CAsyncSocket *> sockets;
};

#ifndef _DEBUG  // FlightServerView.cpp 中的调试版本
inline CFlightServerDoc* CFlightServerView::GetDocument() const
   { return reinterpret_cast<CFlightServerDoc*>(m_pDocument); }
#endif

