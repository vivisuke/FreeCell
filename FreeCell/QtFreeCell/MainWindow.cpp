#include "MainWindow.h"
#include "FreeCellWidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setFixedSize(600, 600*1.6);
	auto fc = new FreeCellWidget();
	setCentralWidget(fc);
	setWindowTitle("QtFreeCell");
}
