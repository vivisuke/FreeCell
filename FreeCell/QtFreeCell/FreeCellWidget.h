#pragma once

#include <QtWidgets/QMainWindow>
#include "../FreeCell/Board.h"

class FreeCellWidget : public QWidget
{
	Q_OBJECT

public:
	FreeCellWidget(QWidget *parent = Q_NULLPTR);
	
	void	newGame();

protected:
	void	paintEvent(QPaintEvent*);
	
protected:
	void	drawCard(QPainter&, qreal px, qreal py, card_t cd);		//	左上点 (px, py) にカードを描画
private:
	Board	m_bd;
	qreal			m_cdWidth;		//	画面上でのカード表示幅
	qreal			m_cdHeight;		//	画面上でのカード表示高;
	QImage		m_imgCard;		//	カード全体画像
	QRect		m_rctCard;			//	カード全体画像サイズ
	QRect		m_rctCard1;		//	カード1枚画像サイズ
	QImage		m_imgSpade;
	QImage		m_imgClub;
	QImage		m_imgHeart;
	QImage		m_imgDiamond;
};
