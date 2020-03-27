#include "MainWindow.h"
#include "FreeCellWidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	QObject::connect(ui.action_New, SIGNAL(triggered()), this, SLOT(onNewGame()));
	//
	setFixedSize(600, 600*1.6);
	m_fcw = new FreeCellWidget();
	setCentralWidget(m_fcw);
	setWindowTitle("QtFreeCell");
}
void MainWindow::onNewGame()
{
	m_fcw->newGame();
}
void MainWindow::onExit()
{
}
