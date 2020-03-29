#pragma once

#include <QtWidgets/QMainWindow>
//#include <QLineEdit>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);

public slots:
	void	onNewGame();
	void	onUndo();
	void	onRedo();
	void	onExit();
	void	showMessageOnSB(const QString&);

private:
	Ui::MainWindowClass ui;
	class QLineEdit	*m_leFCNum;
	class FreeCellWidget	*m_fcw;
};
