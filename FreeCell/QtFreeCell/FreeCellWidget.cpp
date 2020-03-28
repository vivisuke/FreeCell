#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QApplication>
#include "../FreeCell/Board.h"
#include "FreeCellWidget.h"

using namespace std;

#define		SUIT_WIDTH			540
#define		SUIT_HEIGHT			540
#define		MOVE_COUNT		15

//----------------------------------------------------------------------
//	フリーセル（'F'～'I'）、ホームセル（'A'～'D'）、カラムベース位置（'0'～'7'）の左上点座標を返す
Vec2 FreeCellWidget::posToVec2(char pos, int row) const
{
	if( pos >= 'F' && pos <= 'I' )
		return Vec2(m_cdWidth*(pos-'F'), 0);
	if( pos >= 'A' && pos <= 'D' )
		return Vec2(m_cdWidth*(pos-'A'+4), 0);
	if( pos >= '0' && pos <= '7' )
		return Vec2(m_cdWidth*(pos-'0'), m_columnY0 + row * m_dy);
	return Vec2(0, 0);
}
bool FreeCellWidget::isMoving(card_t cd) const
{
	for(const auto& mvc: m_mvCard) {
		if( mvc.m_card == cd ) return true;
	}
	return false;
}
//----------------------------------------------------------------------
FreeCellWidget::FreeCellWidget(QWidget *parent)
	: QWidget(parent)
{
	//m_mvCard = 0;
	m_imgCard.load("Resources/card.png");
	m_rctCard = QRect(0, 0, m_imgCard.width(), m_imgCard.height());
	m_rctCard1 = QRect(0, 0, m_imgCard.width()/8, m_imgCard.height()/7);
	m_imgSpade.load("Resources/spade.png");
	m_imgClub.load("Resources/club.png");
	m_imgHeart.load("Resources/heart.png");
	m_imgDiamond.load("Resources/diamond.png");
	//
	connect(&m_timer, SIGNAL(timeout()), this, SLOT(onTimer()));
	m_timer.start(10);		//	100FPS
	//
	m_initHKey = m_bd.hkeyText();
	m_undoIX = 0;
}
void FreeCellWidget::drawCard(QPainter& pt, qreal px, qreal py, card_t cd)		//	左上点 (px, py) にカードを描画
{
	pt.setPen(Qt::black);
	pt.setBrush(Qt::white);
	pt.setFont(QFont("Arial", 20));
	//	枠表示
	auto rct2 = QRect(px, py, m_cdWidth, m_cdHeight);	//	表示位置
	pt.setPen(Qt::black);
	pt.drawRoundedRect(rct2, m_cdWidth/10, m_cdWidth/10);
	//	数字表示
	int n = cardNum(cd);
	int cix = cardColIX(cd);
	pt.setPen( cix < 2 ? Qt::black : Qt::red);
	auto r = QRect(px+4, py, m_cdWidth, m_cdHeight);
	QString txt;
	switch( n ) {
	case 1:	txt = "A";	break;
	case 11:	txt = "J";	break;
	case 12:	txt = "Q";	break;
	case 13:	txt = "K";	break;
	default:		txt = QString::number(n);	break;
	}
	pt.drawText(r, Qt::AlignLeft|Qt::AlignTop, txt);
	//	スート表示
	auto dy = m_cdWidth / 2.5;
	auto rst = QRect(px+m_cdWidth-dy, py, dy, dy);
	QImage *ptr = nullptr;
	switch( cix ) {
	case 0:	ptr = &m_imgSpade;	break;
	case 1:	ptr = &m_imgClub;	break;
	case 2:	ptr = &m_imgHeart;	break;
	case 3:	ptr = &m_imgDiamond;	break;
	}
	pt.drawImage(rst, *ptr, QRect(0, 0, SUIT_WIDTH, SUIT_HEIGHT));
	//	中央にもスート表示
	auto r2 = QRect(px, py+(m_cdHeight-m_cdWidth)/2+dy/2, m_cdWidth, m_cdWidth);
	pt.drawImage(r2, *ptr, QRect(0, 0, SUIT_WIDTH, SUIT_HEIGHT));
}
void FreeCellWidget::paintEvent(QPaintEvent*event)
{
	QPainter pt(this);
	//
	auto rct = rect();
	m_cdWidth = rct.width() / 8;			//	画面でのカード表示幅
	m_cdHeight = m_cdWidth * 90 / 60;		//	画面でのカード表示高
	m_columnY0 = m_cdHeight + m_cdWidth/2;		//	カラムカード表示位置
	m_dy = m_cdWidth / 2.5;
	//
	pt.setBrush(QColor("darkgreen"));		//	背景色
	pt.drawRect(rct);
	//	フリーセル・ホームセル枠
	qreal px = 0;
	qreal py = 0;
	pt.setPen(Qt::gray);
	pt.setBrush(Qt::transparent);
	for (int i = 0; i != N_FREECELL+N_HOME; ++i, px += m_cdWidth) {
		auto r = QRect(px, py, m_cdWidth, m_cdHeight);
		pt.drawRoundedRect(r, m_cdWidth/10, m_cdWidth/10);
	}
	//	ホームセル内に「A」表示
	pt.setFont(QFont("Arial", 40));
	px = m_cdWidth * N_FREECELL;
	for (int i = 0; i != N_HOME; ++i, px += m_cdWidth) {
		auto r = QRect(px, py, m_cdWidth, m_cdHeight);
		pt.drawText(r, Qt::AlignCenter|Qt::AlignVCenter, "A");
	}
	//	フリーセルカード表示
	px = 0;
	for (int i = 0; i < N_FREECELL; ++i, px+=m_cdWidth) {
		card_t cd = m_bd.getAt('F'+i);
		if( cd != 0 && !isMoving(cd) )
			drawCard(pt, px, 0, cd);
	}
	//	ホームセルカード表示
	px = m_cdWidth * N_FREECELL;
	for (int i = 0; i < N_HOME; ++i, px+=m_cdWidth) {
		card_t cd = m_bd.getAt('A'+i);
		if( cd != 0 && !isMoving(cd) )
			drawCard(pt, px, 0, cd);
	}
	//	カラムのカード表示
	px = 0;
	//auto dy = m_cdWidth / 2.5;
	for (int cix = 0; cix < N_COLUMN; ++cix, px+=m_cdWidth) {
		auto py = m_columnY0;						//	カラムカード表示位置
		auto& lst = m_bd.getColumn(cix);
		for (int i = 0; i != lst.size(); ++i, py+=m_dy) {
			if( !isMoving(lst[i]) )
				drawCard(pt, px, py, lst[i]);
		}
	}
	//	移動中カード表示
	for(const auto& mvc: m_mvCard) {
		qreal px = mvc.m_org.x() + mvc.m_diff.x() * mvc.m_count / MOVE_COUNT;
		qreal py = mvc.m_org.y() + mvc.m_diff.y() * mvc.m_count / MOVE_COUNT;
		drawCard(pt, px, py, mvc.m_card);
	}
}
//	false for 非カード位置、-1 for フリーセル・ホームセル
bool FreeCellWidget::xyToColumnRow(qreal x, qreal y, int& clmn, int& row)
{
	clmn = floor(x / m_cdWidth);
	if( y < m_cdHeight ) {
		row = -1;		//	フリーセル・ホームセル
		return true;
	}
	if( y < m_columnY0 ) {
		clmn = -1;
		return false;
	}
	row = floor((y - m_columnY0) / m_dy);
	return true;
}
void FreeCellWidget::mousePressEvent(QMouseEvent*event)
{
	auto x = event->x();
	auto y = event->y();
	qDebug() << "mousePressEvent(" << x << ", " << y << ")";
	int clmn, row;
	if( xyToColumnRow(event->x(), event->y(), clmn, row) ) {
		qDebug() << "clmn = " << clmn << ", " << row << "\n";
		m_pressedClmn = clmn;
		m_pressedRow = row;
	} else {
		m_pressedClmn = -1;
	}
}
void FreeCellWidget::mouseMoveEvent(QMouseEvent* event)
{
	qDebug() << "mouseMoveEvent()";
}
void FreeCellWidget::mouseReleaseEvent(QMouseEvent* event)
{
	auto x = event->x();
	auto y = event->y();
	qDebug() << "mouseReleaseEvent(" << x << ", " << y << ")";
	int clmn, row;
	if( !xyToColumnRow(event->x(), event->y(), clmn, row) ) {
		return;
	}
	if( clmn == m_pressedClmn && row == m_pressedRow ) {		//	タップ
		onTapped(clmn, row);
	}
}
void FreeCellWidget::onTapped(int clmn, int row)		//	row: -1 for フリーセル・ホームセル
{
	char src = 0, dst = 0;
	int srcRow = 0, dstRow = 0;
	card_t cd = 0;
	int n = 1;
	if( row < 0 ) {		//	フリーセル・ホームセルの場合
		if( clmn < N_FREECELL ) {		//	フリーセルの場合
			src = 'F'+clmn;
			cd = m_bd.getAt(src);
			if( m_bd.canMoveToHome(cd) ) {
				dst = 'A'+cardColIX(cd);
			} else {
				vector<int> v;
				m_bd.canPushBackList(v, cd);
				if( !v.empty() ) {
					dst = '0'+ v.back();
					dstRow = m_bd.getColumn(dst-'0').size() - 1;
				} else {
					//	undone:
				}
			}
		}
	} else {		//	カラムの場合
		auto& lst = m_bd.getColumn(clmn);
		if( lst.empty() ) return;
		row = std::min(row, (int)lst.size()-1);
		if( row < lst.size() - 1 ) {		//	列の途中がタップされた場合
			if( !m_bd.isDescSeq(clmn, row) )
				return;
			n = lst.size() - row;
			cd = lst[row];
			srcRow = row;
			if( n <= m_bd.nMobableDesc() ) {
				vector<int> v;
				m_bd.canPushBackList(v, cd, false);		//	空欄への移動不可
				if( v.empty() ) {
					m_bd.canPushBackList(v, cd, true);		//	空欄への移動可
					if( v.empty() )
						return;
					qDebug() << "toEmpty";
				}
				src = '0' + clmn;
				dst = '0'+v.front();
				dstRow = m_bd.getColumn(dst-'0').size() - 1;
			}
		} else {		//	列の末尾がタップされた場合
			cd = lst[row];
			src = '0'+clmn;
			srcRow = row;
			if( m_bd.canMoveToHome(cd) ) {
				dst = 'A'+cardColIX(cd);
			} else {
				vector<int> v;
				m_bd.canPushBackList(v, cd);
				if( !v.empty() ) {
					dst = '0'+v.front();
					dstRow = m_bd.getColumn(dst-'0').size() - 1;
				} else if( m_bd.canMoveTo('F', cd) ) {
					dst = 'F'+m_bd.nCardFreeCell();
				}
			}
		}
	}
	if( dst != 0 ) {
		auto sv = posToVec2(src, srcRow);
		auto dv = posToVec2(dst, dstRow);
		m_mvCard.push_back(MovingCard(cd, sv, dv - sv));
		Move mv(src, dst, n);
		m_mvHist.push_back(mv);
		m_undoIX = m_mvHist.size();
		//
		//m_mvCard = cd;
		m_bd.doMove(mv);
		update();
		return;
	}
}
void FreeCellWidget::newGame(int msnum)
{
	//m_mvCard = 0;
	if( msnum >= 1 && msnum <= 32000 )
		m_bd.initMS(msnum);
	else
		m_bd.init();
	m_initHKey = m_bd.hkeyText();
	m_mvHist.clear();
	m_undoIX = 0;
	update();
}
void FreeCellWidget::doReStart()
{
	m_bd.set(m_initHKey);
	m_undoIX = 0;
	update();
}
bool FreeCellWidget::canUndo()
{
	return m_undoIX != 0;
}
void FreeCellWidget::doUndo()
{
	if( m_undoIX == 0 ) return;
	auto mv = m_mvHist[--m_undoIX];
	m_bd.unMove(mv);
	update();
}
bool FreeCellWidget::canRedo()
{
	return m_undoIX < m_mvHist.size();
}
void FreeCellWidget::doRedo()
{
	if( m_undoIX == m_mvHist.size() ) return;
	auto mv = m_mvHist[m_undoIX++];
	m_bd.doMove(mv);
	update();
}
void FreeCellWidget::nextHint()
{
	Moves mvs;
	QApplication::setOverrideCursor(Qt::WaitCursor);
	m_bd.genOpenClmnMoves(mvs, 10);
	QApplication::setOverrideCursor(Qt::ArrowCursor);
	qDebug() << "mvs.size() = " << mvs.size();
	if( mvs.empty() ) return;
	m_mvHist.push_back(mvs.front());
	m_undoIX = m_mvHist.size();
	m_bd.doMove(mvs.front());
	update();
}
void FreeCellWidget::onTimer()
{
	for (int i = m_mvCard.size(); --i >= 0; ) {
		if( ++m_mvCard[i].m_count >= MOVE_COUNT )
			m_mvCard.erase(m_mvCard.begin() + i);
	}
	update();
}
