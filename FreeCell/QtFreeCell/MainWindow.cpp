#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include "MainWindow.h"
#include "FreeCellWidget.h"
#include "../FreeCell/utils.h"

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
	//auto label = new QLabel("#");
	ui.mainToolBar->insertWidget(ui.action_New, new QLabel("# "));
	(m_leFCNum = new QLineEdit())->setMaximumWidth(100);
	ui.mainToolBar->insertWidget(ui.action_New, m_leFCNum);
	//
	QObject::connect(ui.action_New, SIGNAL(triggered()), this, SLOT(onNewGame()));
	QObject::connect(ui.action_NextHint, SIGNAL(triggered()), m_fcw, SLOT(nextHint()));
#if	1
	QObject::connect(ui.action_Undo, SIGNAL(triggered()), m_fcw, SLOT(doUndo()));
	//QObject::connect(ui.action_Undo, SIGNAL(aboutToShow()), m_fcw, SLOT(canUndo()));
	QObject::connect(ui.action_Redo, SIGNAL(triggered()), m_fcw, SLOT(doRedo()));
#else
	QObject::connect(ui.action_Undo, SIGNAL(triggered()), this, SLOT(onUndo()));
	QObject::connect(ui.action_Redo, SIGNAL(triggered()), this, SLOT(onRedo()));
#endif
	QObject::connect(ui.action_eXit, SIGNAL(triggered()), this, SLOT(onExit()));
	//
	onNewGame();
}
void MainWindow::onNewGame()
{
	int msnum = 0;
#if	1
	if( !m_leFCNum->text().isEmpty() ) {
		msnum = m_leFCNum->text().toInt();
	} else
		msnum = g_mt() % 32000 + 1;
	setWindowTitle(QString("QtFreeCell #") + QString::number(msnum));
#else
	if( !m_leFCNum->text().isEmpty() ) {
		msnum = m_leFCNum->text().toInt();
	}
#endif
	m_fcw->newGame(msnum);
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
