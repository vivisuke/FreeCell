#include <QMessageBox>
#include "MainWindow.h"
#include "FreeCellWidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//
	setFixedSize(600, 600*1.6);
	m_fcw = new FreeCellWidget();
	setCentralWidget(m_fcw);
	setWindowTitle("QtFreeCell");
	//
	QObject::connect(ui.action_New, SIGNAL(triggered()), this, SLOT(onNewGame()));
#if	1
	QObject::connect(ui.action_Undo, SIGNAL(triggered()), m_fcw, SLOT(doUndo()));
	QObject::connect(ui.action_Redo, SIGNAL(triggered()), m_fcw, SLOT(doRedo()));
#else
	QObject::connect(ui.action_Undo, SIGNAL(triggered()), this, SLOT(onUndo()));
	QObject::connect(ui.action_Redo, SIGNAL(triggered()), this, SLOT(onRedo()));
#endif
	QObject::connect(ui.action_eXit, SIGNAL(triggered()), this, SLOT(onExit()));
}
void MainWindow::onNewGame()
{
	m_fcw->newGame();
}
void MainWindow::onUndo()
{
	m_fcw->doUndo();
}
void MainWindow::onRedo()
{
	m_fcw->doRedo();
}
void MainWindow::onExit()
{
	QMessageBox dlg;
	dlg.setWindowTitle("QtFreeCell");
	dlg.setText("Are you sure to Exit ?");
	dlg.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	dlg.setDefaultButton(QMessageBox::Yes);
	dlg.setIcon(QMessageBox::Warning);
	auto res = dlg.exec();
	if( res == QMessageBox::Yes )
		close();
}
