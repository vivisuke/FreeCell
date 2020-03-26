#pragma once

#include <QtWidgets/QMainWindow>

class FreeCellWidget : public QWidget
{
	Q_OBJECT

public:
	FreeCellWidget(QWidget *parent = Q_NULLPTR);

protected:
	void	paintEvent(QPaintEvent*);
private:
	QImage		m_imgCard;		//	カード全体画像
	QRect		m_rctCard;			//	カード全体画像サイズ
	QRect		m_rctCard1;		//	カード1枚画像サイズ
};
