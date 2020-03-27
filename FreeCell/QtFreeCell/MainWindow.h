#pragma once

#include <QtWidgets/QMainWindow>
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

private:
	Ui::MainWindowClass ui;
	class FreeCellWidget	*m_fcw;
};
