#include <QPainter>
//#include "../FreeCell/Board.h"
#include "FreeCellWidget.h"

#define		SUIT_WIDTH			540
#define		SUIT_HEIGHT			540

FreeCellWidget::FreeCellWidget(QWidget *parent)
	: QWidget(parent)
{
	m_imgCard.load("Resources/card.png");
	m_rctCard = QRect(0, 0, m_imgCard.width(), m_imgCard.height());
	m_rctCard1 = QRect(0, 0, m_imgCard.width()/8, m_imgCard.height()/7);
	m_imgSpade.load("Resources/spade.png");
	m_imgClub.load("Resources/club.png");
	m_imgHeart.load("Resources/heart.png");
	m_imgDiamond.load("Resources/diamond.png");
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
	//
#if	0
	pt.setPen(Qt::black);
	pt.drawLine(0, 0, rct.width(), rct.height());
	pt.drawLine(rct.width(), 0, 0, rct.height());
#endif
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
	//	カラムのカード表示
	px = 0;
	auto dy = m_cdWidth / 2.5;
	for (int cix = 0; cix < N_COLUMN; ++cix, px+=m_cdWidth) {
		auto py = m_cdHeight + m_cdWidth/2;		//	カラムカード表示位置
		auto& lst = m_bd.getColumn(cix);
		for (int i = 0; i != lst.size(); ++i, py+=dy) {
			drawCard(pt, px, py, lst[i]);
		}
	}
#if	0
	auto py0 = cdHeight + cdWidth/2;		//	カラムカード表示位置
	auto dy = cdWidth / 2.5;
	pt.setFont(QFont("Arial", 20));
	pt.setPen(Qt::black);
	pt.setBrush(Qt::white);
	for (int i = 0; i < N_CARD; ++i) {
		px = (i % 8) * cdWidth;
		py = py0 + (i / 8)*dy;
		auto rct2 = QRect(px, py, cdWidth, cdHeight);	//	表示位置
		pt.setPen(Qt::black);
		pt.drawRoundedRect(rct2, cdWidth/10, cdWidth/10);
		int n = (i/4) + 1;
		pt.setPen( (i % 4) < 2 ? Qt::black : Qt::red);
		auto r = QRect(px+4, py, cdWidth, cdHeight);
		QString txt;
		switch( n ) {
		case 1:	txt = "A";	break;
		case 11:	txt = "J";	break;
		case 12:	txt = "Q";	break;
		case 13:	txt = "K";	break;
		default:		txt = QString::number(n);	break;
		}
#if	0
		int st = i % 4;
		switch( st ) {
		case 0:	txt += QChar(0x2092);	break;
		//case 0:	txt += QString::fromUtf8("♣");	break;
		case 1:	txt += QChar(0x25c6);	break;
		//case 1:	txt += "◆";	break;
		case 2:	txt += "♥";	break;
		case 3:	txt += "♠";	break;
		}
#endif
		pt.drawText(r, Qt::AlignLeft|Qt::AlignTop, txt);
		auto rst = QRect(px+cdWidth-dy, py, dy, dy);
		QImage *ptr = nullptr;
		switch( i % 4 ) {
		case 0:	ptr = &m_imgSpade;	break;
		case 1:	ptr = &m_imgClub;	break;
		case 2:	ptr = &m_imgHeart;	break;
		case 3:	ptr = &m_imgDiamond;	break;
		}
		pt.drawImage(rst, *ptr, QRect(0, 0, SUIT_WIDTH, SUIT_HEIGHT));
		//	中央にもスート表示
		auto r2 = QRect(px, py+(cdHeight-cdWidth)/2+dy/2, cdWidth, cdWidth);
		pt.drawImage(r2, *ptr, QRect(0, 0, SUIT_WIDTH, SUIT_HEIGHT));
	}
#endif
#if	0
	for (int i = 0; i < N_CARD; ++i) {
		px = (i % 8) * cdWidth;
		py = py0 + (i / 8)*dy;
		auto rct2 = QRect(px, py, cdWidth, cdHeight);	//	表示位置
		auto cdx = (i % 8) * m_rctCard1.width();
		auto cdy = (i/8) * m_rctCard1.height();
		auto srcr = QRect(cdx, cdy, m_rctCard1.width(), m_rctCard1.height());
		pt.drawImage(rct2, m_imgCard, srcr);
	}
#endif
#if	0
	px = 0;
	py = cdHeight + cdWidth/2;
	auto rct2 = QRect(px, py, cdWidth, cdHeight);
	//rct.setHeight(rct.width() * m_rctCard.height() / m_rctCard.width());
	pt.drawImage(rct2, m_imgCard, m_rctCard1);
#endif
}
void FreeCellWidget::newGame()
{
	m_bd.init();
	update();
}
