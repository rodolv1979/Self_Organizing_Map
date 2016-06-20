#include <QCoreApplication>
#include "mainWidget.h"
#include "parameters.h"
#include "simulator.h"
#include "graph.h"
#include "text.h"

#include <QApplication>
#include <QFont>
#include <QPushButton>
#include <QPainter>
#include <QHBoxLayout>
#include <QGridLayout>
#include <stdlib.h>
#include <ctime>
#include <QCheckBox>
#include <QTimer>
#include <itpp/itbase.h>
#include <QCoreApplication>
#include <QDesktopWidget>


using namespace std;
using namespace itpp;

mainWidget::mainWidget()
{
	setWindowState(Qt::WindowMaximized);	//apre la finestra per intero al 
						//massimo della risoluzione corrente
	setup_widget();	
	init();
	timing=0;	
}

mainWidget::~mainWidget()
{
	if (par != NULL)	{delete par;}
	if (sim != NULL)	{delete sim;}
	if (text != NULL)	{delete text;}
	if (graph != NULL)	{delete graph;}
	
	delete quit_b;
 	delete run_b;
 	delete step_b;
 	delete stop_b;
 	delete reset_b;
 	delete check_G;
 	delete check_T;
 	delete slider_timer;
 	delete lcd_timer;
	delete timer;
}


void mainWidget::setup_widget()
{
	par = new Parameters(this); 
	par->init();
	sim = new Simulator(par,this);
	graph = new Graph(par,sim,this);
	text = new Text(sim,this);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()),this, SLOT(step()));
	
	
	/*webcamWin->resize(320,240);
	webcamWin->setCursor(QCursor(Qt::CrossCursor));
	webcamWin->move(10,10);
	webcamWin-> setFoveaDimension(itti_area_range_w, itti_area_range_h);
	webcamWin->setFoveaPosition(itti_area_x,itti_area_y);
	webcamWin->setEyeStatus(true);*/
	
	QDesktopWidget desktop;
	//resize(desktop.geometry().width(),desktop.geometry().height());
	//resize(5000,5000);
	dx = 10;
	dy = 10;
	
	run_b = new QPushButton(tr("Run"),this);
	run_b->setFont(QFont("Times", 12, QFont::Bold));
	run_b->setGeometry(dx, dy, 80, 40);
	connect(run_b, SIGNAL(clicked()), this, SLOT(run()));
	dx =dx+80;
	
	step_b = new QPushButton(tr("Step"),this);
	step_b->setFont(QFont("Times", 12, QFont::Bold));
	step_b->setGeometry(dx,dy,80,40);
	connect(step_b, SIGNAL(clicked()), this, SLOT(step()));
	dx =dx+80;

	stop_b = new QPushButton(tr("Stop"),this);
	stop_b->setFont(QFont("Times", 12, QFont::Bold));
	stop_b->setGeometry(dx,dy,80,40);
	connect(stop_b, SIGNAL(clicked()), this, SLOT(stop()));
	dx =dx+80;

	reset_b = new QPushButton(tr("Reset"),this);
	reset_b->setFont(QFont("Times", 12, QFont::Bold));
	reset_b->setGeometry(dx,dy,80,40);
	connect(reset_b, SIGNAL(clicked()), this, SLOT(reset()));
	dx =dx+80;

	quit_b= new QPushButton(tr("Quit"),this);
	quit_b->setFont(QFont("Times", 12, QFont::Bold));
	quit_b->setGeometry(dx,dy,80,40);
	connect(quit_b, SIGNAL(clicked()), qApp, SLOT(quit()));
	dx =dx+80;

	check_T = new QCheckBox(tr("Disable All"),this);
	check_T->setFont(QFont("Times", 10, QFont::Bold));
	check_T->setGeometry(dx, dy, 150, 40);
	check_T->setEnabled(true);
	connect(check_T,SIGNAL(stateChanged(int)),this,SLOT(disableText())); 
	dx = dx+150;
	
	check_G = new QCheckBox(tr("Disable Graphics"),this);
	check_G->setFont(QFont("Times", 10, QFont::Bold));
	check_G->setGeometry(dx, dy, 150, 40);
	check_G->setEnabled(true);
	connect(check_G,SIGNAL(stateChanged(int)),this,SLOT(disableGraph())); 
	
	dx = dx + 200;
	
	slider_timer = new QSlider(Qt::Horizontal,this);
	slider_timer->setRange(par->timer, 3000);
	slider_timer->setValue(par->timer);
	slider_timer->setGeometry(dx,dy,250,40);
	
	dx = dx + 250;
	
	lcd_timer = new QLCDNumber(4,this);
	lcd_timer->setSegmentStyle(QLCDNumber::Flat);
	lcd_timer->setGeometry(dx,dy,100,40);
	
	
	connect(slider_timer, SIGNAL(valueChanged(int)),lcd_timer, SLOT(display(int)));
	connect(slider_timer, SIGNAL(valueChanged(int)),this, SLOT(setTimer()));
			
	text->resize((desktop.geometry().height()/10)*3, (desktop.geometry().width()/10)*8);
	text->setCursor(QCursor(Qt::CrossCursor));
	text->move(10,100);
	
	graph->resize((desktop.geometry().height()/10)*12, (desktop.geometry().width()/10)*8);
	graph->setCursor(QCursor(Qt::CrossCursor));
	graph->move(((desktop.geometry().height()/10)*3)+20,100);
	
	
	prova = new openCvWindow(0);
	prova->setEnabled(true);
	prova->setWindowTitle("Questa è una prova!");
	prova->resize(150, 150);
	prova->setGrid(true);
	prova->setVisible(true);
	prova->move(400,400);
	
}





void mainWidget::disableGraph()
{
      if (check_G->checkState()==Qt::Checked) {timer->setInterval(0);}
      else {timer->setInterval(slider_timer->value());}   
}

void mainWidget::disableText()
{
      if (check_T->checkState()==Qt::Checked) {timer->setInterval(0);}
      else {timer->setInterval(slider_timer->value());}
}

void mainWidget::updateAll()
{
	if ((check_T->isChecked()==false) && (check_G->isChecked()==false))
    	{
 		graph->update();
 		text->update();
		QCoreApplication::processEvents();
    	}
	else
	if ((check_T->isChecked()==false) && check_G->isChecked())
   	{
 		text->update();
		QCoreApplication::processEvents();
    	}
	else
	if (check_T->isChecked() && (check_G->isChecked()==false))
    	{
         	graph->update();
		QCoreApplication::processEvents();
    	}
}

void mainWidget::setTimer()
{ 
	timer->setInterval(slider_timer->value());
}

void mainWidget::init()
{
	sim->init();
	timing=0;
	updateAll();
}

void mainWidget::reset()
{
	timer->stop();
	sim->reset();
	timing=0;
	updateAll();
}

void mainWidget::run()
{
	timer->start(slider_timer->value());
	updateAll();
}

void mainWidget::stop()
{	
	timer->stop();
	updateAll();
}

void mainWidget::step()
{
	sim->step();
	timing = timing + slider_timer->value();
	updateAll();
}

int mainWidget::getTiming()
{
	return timing;
}

int mainWidget::getTimer()
{
	return slider_timer->value();
}
