
// CHessView.h : CCHessView ��Ľӿ�
//

#pragma once

#include "Plane.h"
#include "Player.h"
#include "Game.h"

class CCHessView : public CView
{
protected: // �������л�����
	CCHessView();
	DECLARE_DYNCREATE(CCHessView)

// ����
public:
	CCHessDoc* GetDocument() const;

// ����
public:
	enum SELECT_MODE {
		SELECT_DICE, SELECT_PLANE, SELECT_TMP
	};
	SELECT_MODE select_mode = SELECT_DICE;
	CBitmap planes[4];
	CBitmap m_map;     // ����λͼ
	CBitmap g_plane;   // ��ɫ�ɻ�λͼ
	CBitmap b_plane;   // ��ɫ�ɻ�λͼ
	CBitmap y_plane;   // ��ɫ�ɻ�λͼ
	CBitmap r_plane;   // ��ɫ�ɻ�λͼ
	CBitmap dice;      // ����ɸ��λͼ
	CBitmap ds[6];
	Player players[4]; //�����ĸ���Ҷ���
	                   // 0��1��2��3������ɫ �̡��졢�ơ���
	bool use_ai[4];
	BOOL start_dice;
	// ������ÿ������ ��������
	const int map_pos[76][2] = {
		/*��1-13������*/
		{47,191},{88,176},{122,177},{160,191},{190,166},{177,124},
		{177,88},{189,49},{230,35},{265,35},{300,35},{335,35},{368,35},
		/*��14-26������*/
		{408,53},{423,88},{423,124},{410,166},{434,194},{478,176},
		{512,176},{553,191},{565,232},{ 565,265 },{ 565,303 },{ 565, 336},{ 565,371 },
		/*��27-39������*/
		{551,412},{515,424},{476,424},{437,411},{409,438},{423,476},
		{423,512},{411,552},{370,565},{335,565},{299,565},{265,565},{230,565},
		/*��40-52������*/
		{190,554},{177,511},{177,477},{190,438},{161,411},{124,423},
		{89,424},{50,412},{35,370},{35,336},{35,300},{35,266},{35,231},
		/*��53-58������*/
		{ 90,300 },{ 125,300 },{ 160,300 },{ 195,300 },{ 230,300 },{ 264,300 },
		/*��59-64������*/
		{300,95},{300,130},{300,165},{300,200},{300,234},{300,269},
		/*��65-70������*/
		{ 509,303 },{ 474,303 },{ 439,303 },{ 405,303 },{ 370,303 },{ 335,303 },
		/*��71-76������*/
		{300,514},{300,479},{300,445},{300,410},{300,375},{300,340}
	};

	int map_color[76]{

	};

	//�ɻ��������ʱ��λ��
	int init_pos[4][2] = {
		{20,162},{435,19},{580,437},{163,579}
	};
	// �ɻ�û�����ʱ��λ��
	int port_pos[16][2] = {
		{28,28},{70,28},{28,70},{70,70},
		{498,28},{540,28},{498,70},{540,70},
		{498,498},{498,540},{540,498},{540,540},
		{ 28,498 },{ 70,498 },{ 28,540 },{ 70,540 }
	};

	int random;
	int dice_p = 0;
	
	Game game = Game();

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	void DrawMemDC(CDC *);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void ThrowDice(CDC *, int value = 0);
	void SelectPlane(CDC*, int plane_id);
	bool isGameOver();
	bool over_vis[4];
	// ��������
	//void PrintPos(CDC *pdc);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CCHessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	int ThrowNnm();
	void DrawPlane(Plane &p, CDC*);
	CPoint GetPlanePos(Plane &p);
	CPoint GetPlaneTPos(Plane &p, int);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void OnInitialUpdate();
	void judge_winning(Player p);
	afx_msg void OnRestart();
	afx_msg void OnAigreen();
	afx_msg void OnAired();
	afx_msg void OnAiyellow();
	afx_msg void OnAiblue();
	afx_msg void OnUpdateAigreen(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAired(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAiyellow(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAiblue(CCmdUI *pCmdUI);
public:
	CAsyncSocket socket;
};

#ifndef _DEBUG  // CHessView.cpp �еĵ��԰汾
inline CCHessDoc* CCHessView::GetDocument() const
   { return reinterpret_cast<CCHessDoc*>(m_pDocument); }
#endif

