#include "mainWidget.h"
#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication application(argc, argv);
	mainWidget window;
	window.show();
	return application.exec();
}





