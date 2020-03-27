#include <QPainter>
#include "../FreeCell/Board.h"
#include "FreeCellWidget.h"

FreeCellWidget::FreeCellWidget(QWidget *parent)
	: QWidget(parent)
{
	m_imgCard.load("Resources/card.png");
	m_rctCard = QRect(0, 0, m_imgCard.width(), m_imgCard.height());
	m_rctCard1 = QRect(0, 0, m_imgCard.width()/8, m_imgCard.height()/7);
}
void FreeCellWidget::paintEvent(QPaintEvent*event)
{
	QPainter pt(this);
	//
	auto rct = rect();
	auto cdWidth = rct.width() / 8;
	auto cdHeight = cdWidth * 90 / 60;
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
	for (int i = 0; i != N_FREECELL+N_HOME; ++i, px += cdWidth) {
		auto r = QRect(px, py, cdWidth, cdHeight);
		pt.drawRoundedRect(r, cdWidth/10, cdWidth/10);
	}
	//	ホームセル内に「A」表示
	pt.setFont(QFont("Arial", 40));
	px = cdWidth * N_FREECELL;
	for (int i = 0; i != N_HOME; ++i, px += cdWidth) {
		auto r = QRect(px, py, cdWidth, cdHeight);
		pt.drawText(r, Qt::AlignCenter|Qt::AlignVCenter, "A");
	}
	//
	px = 0;
	py = cdHeight + cdWidth/2;
	auto rct2 = QRect(px, py, cdWidth, cdHeight);
	//rct.setHeight(rct.width() * m_rctCard.height() / m_rctCard.width());
	pt.drawImage(rct2, m_imgCard, m_rctCard1);
}
