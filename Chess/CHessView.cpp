
// CHessView.cpp : CCHessView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CHess.h"
#endif

#include "CHessDoc.h"
#include "CHessView.h"
#include "Resource.h"
#include "netdata.h"

#include <iostream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int SIZEOF_NETDATA = sizeof(NetData);

// CCHessView

IMPLEMENT_DYNCREATE(CCHessView, CView)

BEGIN_MESSAGE_MAP(CCHessView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_COMMAND(ID_RESTART, &CCHessView::OnRestart)
	ON_COMMAND(ID_AIGREEN, &CCHessView::OnAigreen)
	ON_COMMAND(ID_AIRED, &CCHessView::OnAired)
	ON_COMMAND(ID_AIYELLOW, &CCHessView::OnAiyellow)
	ON_COMMAND(ID_AIBLUE, &CCHessView::OnAiblue)
	ON_UPDATE_COMMAND_UI(ID_AIGREEN, &CCHessView::OnUpdateAigreen)
	ON_UPDATE_COMMAND_UI(ID_AIRED, &CCHessView::OnUpdateAired)
	ON_UPDATE_COMMAND_UI(ID_AIYELLOW, &CCHessView::OnUpdateAiyellow)
	ON_UPDATE_COMMAND_UI(ID_AIBLUE, &CCHessView::OnUpdateAiblue)
END_MESSAGE_MAP()

// CCHessView 构造/析构

CCHessView::CCHessView()
{
	cout << SIZEOF_NETDATA << endl;
	// TODO: 在此处添加构造代码

	//初始化四个玩家对象信息
	int i;
	for (i = 0; i < 52; i++) {
		map_color[i] = (i + 3) % 4;
	}
	for (; i < 58; i++) {
		map_color[i] = 4;
	}
	for (; i < 64; i++) {
		map_color[i] = 4;
	}
	for (; i < 70; i++) {
		map_color[i] = 4;
	}
	for (; i < 76; i++) {
		map_color[i] = 4;
	}

	for (int i = 0; i < 16; i++) {
		port_pos[i][0] += 16;
		port_pos[i][1] += 16;
	}

	dice_p = game.turn;
	random = 0;

	for (int i = 0;i < 4;++i) {
		Player &p = game.GetPlayer(i);
		for (int k = 0;k < 4;++k) {
			CPoint q = GetPlanePos(p.planes[k]);
			p.planes[k].x = q.x - 16;
			p.planes[k].y = q.y - 16;
		}
	}
	use_ai[0] = false;
	for (int i = 1;i < 4;++i) {
		use_ai[i] = false;
	}
	for (int i = 0;i < 4;++i) {
		over_vis[i] = false;
	}

}

CCHessView::~CCHessView()
{
}

BOOL CCHessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	m_map.LoadBitmap(IDB_MAP);
	b_plane.LoadBitmap(IDB_BPLANE);
	g_plane.LoadBitmap(IDB_GPLANE);
	y_plane.LoadBitmap(IDB_YPLANE);
	planes[0].LoadBitmap(IDB_GPLANE);
	planes[1].LoadBitmap(IDB_RPLANE);
	planes[2].LoadBitmap(IDB_YPLANE);
	planes[3].LoadBitmap(IDB_BPLANE);
	dice.LoadBitmap(IDB_DICE);
	ds[0].LoadBitmap(IDB_D1);
	ds[1].LoadBitmap(IDB_D2);
	ds[2].LoadBitmap(IDB_D3);
	ds[3].LoadBitmap(IDB_D4);
	ds[4].LoadBitmap(IDB_D5);
	ds[5].LoadBitmap(IDB_D6);
	return CView::PreCreateWindow(cs);
}

// CCHessView 绘制

void CCHessView::OnDraw(CDC* pDC)
{
	CCHessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	DrawMemDC(pDC);
}

void CCHessView::DrawMemDC(CDC *pDC) {
	// TODO: 在此处为本机数据添加绘制代码

	//BITMAP bs; // 定义位图结构变量
	//m_map.GetBitmap(&bs); // 获取位图结构
	CDC map;     // 棋盘DC

	CDC pDice;   // 筛子

	map.CreateCompatibleDC(pDC); // 创建兼容DC
	map.SelectObject(&m_map);    // 选入位图对象搜索

	pDice.CreateCompatibleDC(pDC); //装入筛子图案


								   // 绘制棋盘：
	pDC->BitBlt(0, 0, 600, 600, &map, 0, 0, SRCCOPY);

	//pDC->BitBlt(0, 0,32 , 32, &yPlane, 0, 0, SRCCOPY);
	for (int i = 0; i < 4; ++i) {
		Player &p = game.GetPlayer(i);
		for (int k = 0; k < 4; ++k) {
			DrawPlane(p.planes[k], pDC);
		}
	}


	// 绘制筛子
	if (select_mode != SELECT_DICE) {
		pDice.SelectObject(&ds[random - 1]);
	}
	else {
		pDice.SelectObject(&dice);
	}
	pDC->BitBlt(600, 0, 176, 144, &pDice, 0, 0, SRCCOPY);
	pDC->TextOut(650, 10, L"点击旋转筛子");

	CDC cPlanes[4];
	for (int i = 0; i < 4; i++) {
		cPlanes[i].CreateCompatibleDC(pDC);
		cPlanes[i].SelectObject(&planes[i]);
	}

	pDC->BitBlt(640, 60, 32, 32, &cPlanes[dice_p], 0, 0, SRCCOPY);
	
}

CPoint CCHessView::GetPlanePos(Plane &p) {

	if (p.state == Plane::NOFLY) {
		return CPoint(port_pos[p.color * 4 + p.id][0], port_pos[p.color * 4 + p.id][1]);
	}
	else {
		if (p.steps == 0) {
			return CPoint(init_pos[p.color][0], init_pos[p.color][1]);
		}
	}
	return GetPlaneTPos(p, p.GetWorldPos());
}

CPoint CCHessView::GetPlaneTPos(Plane &p, int wpos) {
	if (p.state == Plane::NOFLY) {
		return CPoint(port_pos[p.color * 4 + p.id][0], port_pos[p.color * 4 + p.id][1]);
	}
	else {
		if (p.steps == 0) {
			return CPoint(init_pos[p.color][0], init_pos[p.color][1]);
		}
	}
	return CPoint(map_pos[wpos - 1][0], map_pos[wpos - 1][1]);
}

void CCHessView::DrawPlane(Plane& p, CDC *pDC) {

	CDC cPlanes[4];
	for (int i = 0; i < 4; i++) {
		cPlanes[i].CreateCompatibleDC(pDC);
		cPlanes[i].SelectObject(&planes[i]);
	}

	CPoint q = GetPlanePos(p);
	int hx = q.x - 16;
	int hy = q.y - 16;
	if (p.history.empty()) {
		if (p.first) {
			p.x = hx;
			p.y = hy;
			p.first = false;
		}
	}
	else {
		CPoint h = GetPlaneTPos(p, p.history.front());
		hx = h.x - 16;
		hy = h.y - 16;
	}
	double ax = hx - p.x;
	double ay = hy - p.y;
	double le = sqrt(ax*ax + ay*ay);
	const double ml = 5;
	if (le > ml) {
		ax = ax / le * ml;
		ay = ay / le * ml;
	}
	p.x += ax;
	p.y += ay;
	if (abs(p.x - hx) + abs(p.y - hy) <= 5) {
		if (!p.history.empty())p.history.pop();
		else p.animation = false;
	}
	pDC->BitBlt(p.x, p.y, 32, 32, &cPlanes[p.color], 0, 0, SRCCOPY);


}

// CCHessView 打印

BOOL CCHessView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCHessView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCHessView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CCHessView 诊断

#ifdef _DEBUG
void CCHessView::AssertValid() const
{
	CView::AssertValid();
}

void CCHessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCHessDoc* CCHessView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCHessDoc)));
	return (CCHessDoc*)m_pDocument;
}
#endif //_DEBUG


// CCHessView 消息处理程序

int CCHessView::ThrowNnm()
{
	random = rand() % 6 + 1;
	return random;
}


void CCHessView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值


	CView::OnMouseMove(nFlags, point);
}

void CCHessView::judge_winning(Player p) {
	int k;
	k = 0;
	Plane a = p.planes[0];
	for (int i = 0; i < 4; ++i) {
		a = p.planes[i];
		if (a.steps == 56) k++;
	}
	if (k == 4) {
		if (a.color == 0)
			::MessageBox(NULL, L"绿方胜利！", L"游戏提示", MB_OK);
		if (a.color == 1)
			::MessageBox(NULL, L"红方胜利！", L"游戏提示", MB_OK);
		if (a.color == 2)
			::MessageBox(NULL, L"黄方胜利！", L"游戏提示", MB_OK);
		if (a.color == 3)
			::MessageBox(NULL, L"蓝方胜利！", L"游戏提示", MB_OK);
		over_vis[a.color] = true;
	}
		
}

void CCHessView::ThrowDice(CDC* pDC, int value) {
	random = value == 0 ? ThrowNnm() : value;
	if (value == 0) {
		dice_p = game.turn;
	}

	CDC pDice;
	pDice.CreateCompatibleDC(pDC); //装入筛子图案
	for (int i = 0;i < 30; ++i) {
		pDice.SelectObject(&ds[rand() % 6]);
		pDC->BitBlt(600, 0, 176, 144, &pDice, 0, 0, SRCCOPY);
		Sleep(20);
	}

	pDice.SelectObject(&ds[random - 1]);
	pDC->BitBlt(600, 0, 176, 144, &pDice, 0, 0, SRCCOPY);

	if (value == 0) {

		Player &p = game.GetPlayer(dice_p);
		if (random == 6 || !p.NoFlyPlane()) {
			select_mode = SELECT_PLANE;
		}
		else {
			game.NextTurn();
			select_mode = SELECT_TMP;
		}


		NetData data;
		GetNetData(data, game);
		data.dice = random;
		data.dice_p = dice_p;
		data.select_mode = select_mode;
		socket.Send((char*)&data, SIZEOF_NETDATA);
	}
	else {
		select_mode = SELECT_TMP;
	}
	OnDraw(pDC);
}

void CCHessView::SelectPlane(CDC *pDC, int plane_id) {
	Player &p = game.GetPlayer(game.turn);
	Plane &a = p.planes[plane_id];
	if (random == 6 && a.state == Plane::NOFLY) {
		a.state = Plane::FLY;
		a.animation = true;
		a.steps = 0;
		select_mode = SELECT_DICE;
		dice_p = game.turn;
	}
	else {
		if (a.state != Plane::LAND) {
			game.Go(game.turn, plane_id, random);


			// 判断是否胜利
			if (!over_vis[game.turn]) {
				judge_winning(p);
			}

			if (random != 6)game.NextTurn();
			select_mode = SELECT_DICE;
			dice_p = game.turn;
		}
	}

	NetData data;
	GetNetData(data, game);
	data.dice = 0;
	data.dice_p = dice_p;
	data.select_mode = select_mode;
	socket.Send((char*)&data, SIZEOF_NETDATA);

	OnDraw(pDC);
}

bool CCHessView::isGameOver() {
	int k = 0;
	for (int i = 0;i < 4;++i) {
		if (game.GetPlayer(i).GetLandNum() == 4)++k;
	}
	return k == 4;
}

void CCHessView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (isGameOver())return;
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC* pDC = GetDC();
	if (select_mode == SELECT_DICE && (point.x >= 670) && (point.x <= 750) && (point.y >= 41) && (point.y < 143)){
		ThrowDice(pDC);
	}
	else if (select_mode == SELECT_PLANE) {
		Player &p = game.GetPlayer(game.turn);
		int plane_id = -1;
		for (int i = 0; i < 4; ++i) {
			Plane &a = p.planes[i];
			if (a.state == Plane::LAND)continue;
			CPoint w = GetPlanePos(a) - point;
			if (w.x * w.x + w.y * w.y < 20 * 20) {
				plane_id = i;
				break;
			}
		}
		if (plane_id != -1) {
			SelectPlane(pDC, plane_id);
		}
	}
	else if (select_mode == SELECT_TMP) {
		select_mode = SELECT_DICE;
		dice_p = game.turn;
		OnDraw(pDC);
	}
	ReleaseDC(pDC);
	CView::OnLButtonDown(nFlags, point);
}

#include <iostream>
using namespace std;

void CCHessView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1) {
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		bool animation = false;
		for (int i = 0;i < 4;++i) {
			Player &p = game.GetPlayer(i);
			for (int k = 0;k < 4;++k) {
				if (p.planes[k].animation) {
					animation = true;
					break;
				}
			}
		}
		if (true) {
			CDC memDC;
			CDC *pDC = GetDC();
			CRect rect; // 矩形对象，用于表示客户区
			GetClientRect(&rect); // 获取客户区矩形
			CBitmap bmp; // 位图对象，作为画布，用于创建可传送的兼容DC
						 // 创建大小与客户区一致并且与视图DC兼容的位图对象
			bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
			memDC.CreateCompatibleDC(pDC);
			memDC.SelectObject(&bmp);
			memDC.SetBkColor(RGB(255, 255, 255));
			memDC.Rectangle(rect);
			DrawMemDC(&memDC);
			pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);
			ReleaseDC(pDC);
		}

		bool over = true;
		for (int i = 0;i < 4;++i) {
			if (!over_vis[i]) {
				over = false;
				break;
			}
		}
		if (over)return;
		while (over_vis[game.turn]) {
			game.NextTurn();
		}
		//bool animation = false;
		if (!animation && use_ai[game.turn]) {
			Player &p = game.GetPlayer(game.turn);
			CDC *pDC = GetDC();
			// AI走子
			if (select_mode == SELECT_DICE) {
				ThrowDice(pDC);
			}
			else if (select_mode == SELECT_TMP) {
				Sleep(1000);
				select_mode = SELECT_DICE;
				dice_p = game.turn;
			}
			else if (select_mode == SELECT_PLANE) {
				int plane_id = -1;
				vector<int> ids;
				for (int i = 0;i < 4;++i) {
					if (p.planes[i].state == Plane::FLY) {
						ids.push_back(i);
					}
				}
				if (ids.size() == 0) {
					plane_id = rand() % 4;
				}
				else {
					int ra = rand() % ids.size();
					plane_id = ids[ra];
				}
				if (random == 6 && rand() % 30 > 8) {
					for (Plane &a : p.planes) {
						if (a.state == Plane::NOFLY) {
							plane_id = a.id;
							break;
						}
					}
				}
				for (int i = 0;i < 4;++i) {
					Plane &a = p.planes[i];
					if (a.state == Plane::FLY) {
						bool crash = false;
						for (int k = 0;k < 4;++k) {
							if (k == i)continue;
							Player &b = game.GetPlayer(k);
							for (Plane &b : b.planes) {
								if (a.GetWorldPosI(a.steps + random) == b.GetWorldPos()) {
									crash = true;
									break;
								}
							}
						}
						if (crash) {
							plane_id = i;
							break;
						}
					}
				}
				Sleep(1000);
				SelectPlane(pDC, plane_id);
			}
			ReleaseDC(pDC);
		}
	}
	else if (nIDEvent == 3) {
		NetData data;
		if (socket.Receive((char*)&data, sizeof(NetData)) > 0) {
			if (ApplyNetData(data, game)) {
				if (data.dice >= 1 && data.dice <= 6 && data.dice_p >= 0 && data.dice_p < 4) {
					CDC *pDC = GetDC();
					ThrowDice(pDC, data.dice);
					dice_p = data.dice_p;
					select_mode = SELECT_MODE(data.select_mode);
					ReleaseDC(pDC);
				}
				OnPaint();
			}
		}
	}
	

	CView::OnTimer(nIDEvent);
}


void CCHessView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	SetTimer(1, (UINT)(1000.0 / 60 + 0.5), NULL);
	SetTimer(3, (UINT)(100.0), NULL);
	AfxSocketInit();

	CString addr = L"loopback";
	socket.Create();
	socket.Connect(LPCTSTR(addr), 4000);
}


void CCHessView::OnRestart()
{
	// TODO: 在此添加命令处理程序代码
	for (int i = 0; i < 4; ++i) {
		Player &p = game.GetPlayer(i);
		p.score = 0;
		for (int j = 0; j < 4; ++j) {
			p.planes[j].steps = 0;
			p.planes[j].state = p.planes[j].NOFLY;
			p.planes[j].first = true;
		}
		over_vis[i] = false;
	}
	CDC* pDC = GetDC();
	OnDraw(pDC);
}


void CCHessView::OnAigreen()
{
	// TODO: 在此添加命令处理程序代码
	use_ai[0] = !use_ai[0];
}


void CCHessView::OnAired()
{
	// TODO: 在此添加命令处理程序代码
	use_ai[1] = !use_ai[1];
}


void CCHessView::OnAiyellow()
{
	// TODO: 在此添加命令处理程序代码
	use_ai[2] = !use_ai[2];
}


void CCHessView::OnAiblue()
{
	// TODO: 在此添加命令处理程序代码
	use_ai[3] = !use_ai[3];
}


void CCHessView::OnUpdateAigreen(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(use_ai[0]);
}


void CCHessView::OnUpdateAired(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(use_ai[1]);
}


void CCHessView::OnUpdateAiyellow(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(use_ai[2]);
}


void CCHessView::OnUpdateAiblue(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(use_ai[3]);
}
