#include <QPainter>
#include "FreeCellWidget.h"

FreeCellWidget::FreeCellWidget(QWidget *parent)
	: QWidget(parent)
{
}
void FreeCellWidget::paintEvent(QPaintEvent*event)
{
	auto rct = rect();
	QPainter pt(this);
	pt.setPen(Qt::black);
	pt.drawLine(0, 0, rct.width(), rct.height());
	pt.drawLine(rct.width(), 0, 0, rct.height());
}
