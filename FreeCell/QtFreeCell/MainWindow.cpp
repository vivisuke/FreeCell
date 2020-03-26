#include "MainWindow.h"
#include "FreeCellWidget.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	auto fc = new FreeCellWidget();
	setCentralWidget(fc);
	setWindowTitle("QtFreeCell");
}
