#pragma once

#include <QtWidgets/QMainWindow>
#include "../FreeCell/Board.h"

class FreeCellWidget : public QWidget
{
	Q_OBJECT

public:
	FreeCellWidget(QWidget *parent = Q_NULLPTR);

protected:
	void	paintEvent(QPaintEvent*);
private:
	Board	m_bd;
	QImage		m_imgCard;		//	カード全体画像
	QRect		m_rctCard;			//	カード全体画像サイズ
	QRect		m_rctCard1;		//	カード1枚画像サイズ
	QImage		m_imgSpade;
	QImage		m_imgClub;
	QImage		m_imgHeart;
	QImage		m_imgDiamond;
};
