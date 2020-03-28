#pragma once

#include <string>
#include <vector>
#include <QtWidgets/QMainWindow>
#include <QTimer>
#include "../FreeCell/Board.h"

struct Vec2 {
public:
	Vec2(qreal x = 0, qreal y = 0)
		: m_x(x)
		, m_y(y)
	{
	}
	Vec2(const Vec2& x)
		: m_x(x.m_x)
		, m_y(x.m_y)
	{
	}
public:
	qreal	x() const { return m_x; }
	qreal	y() const { return m_y; }
public:
	Vec2	operator-(Vec2 rhs) const
	{
		return Vec2(m_x - rhs.m_x, m_y - rhs.m_y);
	}
public:
	qreal	m_x;
	qreal	m_y;
};

struct MovingCard {
public:
	MovingCard(card_t card, Vec2 org, Vec2 diff)
		: m_card(card)
		, m_count(0)
		, m_org(org)
		, m_diff(diff)
	{
	}
public:
	card_t	m_card;
	int		m_count;	//	[0, 10]
	Vec2		m_org;		//	移動元
	Vec2		m_diff;		//	移動先 - 移動元
#if	0
	qreal		m_x0;		//	移動開始位置
	qreal		m_y0;
	qreal		m_dx;		//	移動先までの差分
	qreal		m_dy;
#endif
};
//----------------------------------------------------------------------
class FreeCellWidget : public QWidget
{
	Q_OBJECT

public:
	FreeCellWidget(QWidget *parent = Q_NULLPTR);
	
	void	newGame(int msnum = 0);
public slots:
	void	doReStart();
	void	doUndo();
	void	doRedo();
	bool	canUndo();
	bool	canRedo();
	void	nextHint();
	void	onTimer();

protected:
	void	paintEvent(QPaintEvent*);
	void	mousePressEvent(QMouseEvent*);
	void	mouseMoveEvent(QMouseEvent*);
	void	mouseReleaseEvent(QMouseEvent*);
	
protected:
	Vec2	posToVec2(char pos, int row) const;
	bool	isMoving(card_t cd) const;
protected:
	void	drawCard(QPainter&, qreal px, qreal py, card_t cd);		//	左上点 (px, py) にカードを描画
	bool	xyToColumnRow(qreal x, qreal y, int& column, int& row);		//	false for 非カード位置、row:-1 for フリーセル・ホームセル
	void	onTapped(int clmn, int row);		//	row: -1 for フリーセル・ホームセル
private:
	Board	m_bd;
	std::string	m_initHKey;
	qreal			m_cdWidth;		//	画面上でのカード表示幅
	qreal			m_cdHeight;		//	画面上でのカード表示高;
	qreal			m_columnY0;		//	カラム表示位置
	qreal			m_dy;				//	カラムカードｙ座標差分
	int			m_pressedClmn;
	int			m_pressedRow;
	int			m_undoIX;
	//card_t		m_mvCard;		//	移動中カード
	QImage		m_imgCard;		//	カード全体画像
	QRect		m_rctCard;			//	カード全体画像サイズ
	QRect		m_rctCard1;		//	カード1枚画像サイズ
	QImage		m_imgSpade;
	QImage		m_imgClub;
	QImage		m_imgHeart;
	QImage		m_imgDiamond;
	QTimer		m_timer;
	Moves		m_mvHist;
	std::vector<MovingCard>		m_mvCard;		//	移動中カード
};
