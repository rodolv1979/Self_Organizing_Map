#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui/QDialog>
#include <QTimer>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QMainWindow>
#include <QLineEdit>
#include <QImage>
#include <QTime>
#include <QSlider>
#include <QPointF>
#include <simulator.h>
#include <graph.h>
#include "openCvWindow.h"
#include <text.h>
#include <QLCDNumber>
#include <parameters.h>

#include <vector>
#include <itpp/itbase.h>
#include <itpp/base/operators.h>


// using namespace cv;

class mainWidget : public QMainWindow
{
	Q_OBJECT
	public:

		mainWidget();
		~mainWidget();
		void setup_widget();
		void updateAll();
		bool checkT();
		bool checkG();
		
	
	public slots: 
		void reset();
		void run();
		void stop();
		void step();
		void disableGraph();
		void disableText();
		void setTimer();
		void init();
		int getTiming();
		int getTimer();
		
		
		
	public:
		Parameters* par;
		Simulator* sim;
 		Graph* 	graph;
 		Text*	text; 
		QCheckBox* check_T;
		QCheckBox* check_G;
		QLabel* trials;
		QLabel* steps;
		QPushButton* run_b;
		QPushButton* step_b;
		QPushButton* stop_b;
		QPushButton* quit_b;
		QPushButton* reset_b;
		openCvWindow* prova;
		QSlider* slider_timer;
		QLCDNumber* lcd_timer;
		QVBoxLayout* leftBox;
		QVBoxLayout* topBox;
		QLayout* box;
		QTimer* timer;
		QGridLayout *grid;
		int timerCount;
		int timing;
		int dx;
		int dy;
	
};
#endif // WIDGET_H