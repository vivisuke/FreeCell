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
};
