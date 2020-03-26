#include <QPainter>
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
	auto rct = rect();
	QPainter pt(this);
#if	0
	pt.setPen(Qt::black);
	pt.drawLine(0, 0, rct.width(), rct.height());
	pt.drawLine(rct.width(), 0, 0, rct.height());
#endif
	pt.setBrush(QColor("darkgreen"));
	pt.drawRect(rct);
	auto rct2 = QRect(100, 100, m_rctCard.width()/8, m_rctCard.height() / 7);
	//rct.setHeight(rct.width() * m_rctCard.height() / m_rctCard.width());
	pt.drawImage(rct2, m_imgCard, m_rctCard1);
}
