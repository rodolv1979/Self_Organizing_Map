//Implementation of class Text 
#include <QPushButton>
#include <QApplication>
#include <QFont>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QString>
#include <stdlib.h>
#include <ctime>
#include <QFileDialog>
#include <text.h>



Text::Text(Simulator* sim_W, QWidget *parent)
: QWidget(parent)
{
    setPalette(QPalette(QColor(100,100 ,120 )));
    setAutoFillBackground(true);
    
    simulator_text = sim_W;

    trials = new QLabel(this);
    trials->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    trials->setGeometry(0,30,200,30);

    steps = new QLabel(this);
    steps->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    steps->setGeometry(0,90,200,30);

    timing = new QLabel(this);
    timing->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    timing->setGeometry(0,510,250,30);

}

Text::~Text()
{
   
   
}

void Text::paintEvent(QPaintEvent *)
{
    //Texture
    trials->setText("Trial n. "+QString::number(simulator_text->getTrialNumber()));
    steps->setText("Step n. "+QString::number(simulator_text->getStepNumber()));
    timing->setText("Timing = "+QString::number(simulator_text->getTiming())+ " ms");
}                                                                                           
