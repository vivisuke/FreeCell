#pragma once

#include <string>
#include <vector>
#include <QtWidgets/QMainWindow>
#include "../FreeCell/Board.h"

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

protected:
	void	paintEvent(QPaintEvent*);
	void	mousePressEvent(QMouseEvent*);
	void	mouseMoveEvent(QMouseEvent*);
	void	mouseReleaseEvent(QMouseEvent*);
	
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
	qreal			m_dy;				//	カードｙ座標差分
	int			m_pressedClmn;
	int			m_pressedRow;
	int			m_undoIX;
	card_t		m_mvCard;		//	移動中カード
	QImage		m_imgCard;		//	カード全体画像
	QRect		m_rctCard;			//	カード全体画像サイズ
	QRect		m_rctCard1;		//	カード1枚画像サイズ
	QImage		m_imgSpade;
	QImage		m_imgClub;
	QImage		m_imgHeart;
	QImage		m_imgDiamond;
	Moves		m_mvHist;
};
