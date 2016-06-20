#include "openCvWindow.h"

#include <Qt/qapplication.h>
#include <Qt/qscrollarea.h>
#include <Qt/qgridlayout.h>
#include <Qt/qfiledialog.h>
#include <Qt/qmessagebox.h>
#include <Qt/qicon.h>
#include <Qt/qmenubar.h>
#include <Qt/qaction.h>
#include <Qt/qtoolbar.h>
#include <Qt/qstatusbar.h>
#include <Qt/qsettings.h>
#include <QtGui/qscrollbar.h>
#include <QtGui/qlineedit.h>


#include <QtGui>
#include <QGridLayout>  
#include <QKeyEvent>    
#include <QFileDialog> 
#include <QMessageBox>  

//opencv stuff
// #include <opencv/cv.h>
// #include <opencv/highgui.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
# include <vector>

//my costants
//#include "constants.h"

using namespace std;


//constructor!
openCvWindow::openCvWindow(QWidget *parent)
:QWidget(parent)
{

	//my default values for widget dimensions
	widgetWidth = width();
	widgetHeight = height();
	
	grid_status = false;
	
	//default is 1, otherwise we have a floating point exception 
	// (division by 0, see paint event code!)
	n_grid_w = 1;
	n_grid_h = 1;
	gunTarget_status=false;
	grayscale = false;
	//default value, overwritten by mainWidget
	fovea_dim = 100;
	setFoveaDim(100);
	setPalette(QPalette(QColor(255,255,255,254)));
	setAutoFillBackground(true);
}

openCvWindow::~openCvWindow()
{
	
	
}


void openCvWindow::setGunTarget(bool status)
{
	if(status==true)
		gunTarget_status = true;
	else
		gunTarget_status = false;
	
}


void openCvWindow::setGrid(bool status)
{
	if(status==true)
		grid_status = true;
	else
		grid_status = false;
	
}


//this is called every time we resize the window
void openCvWindow::resizeEvent(QResizeEvent * event)
{
	widgetWidth = width();
	widgetHeight = height();
}


void openCvWindow::setImage( Mat _mImage)
{
	//The original image has 3 channels 
	_mImage.copyTo(mImage);
	//the imake is empty!!!!
	//There will be no visualisation!
	if(mImage.empty() == true)
	{
		cout<<"\n[WARNING] in openCvWindow::setImage()\n";
		cout<<"The Mat is empty!!!";
		cout<<" message from window: "<<windowTitle().toStdString()<<endl;
		return;
	}
	
	
	//the image is grayscale! Qt cannot visualise it directly!
	// Here I build a three channel image, where rgb channels are equal (--> a graysacle image)!
	if(mImage.channels()==1)
	{
		//cout<<"\n[WARNING] in openCvWindow::setImage()\n";
		//cout<<"The Mat is 1 channel!!! It will be transformed in 3 channel in order to be visualised by QT";
		
		//build a standard vector containing 3 equal image with 1 channel
		std::vector<cv::Mat> planes(3, mImage); 					
		
		//merge these thre iamges in a single image!
		cv::merge( planes, mImage); 
			
		grayscale = true;
		
	}
	else
	{
		grayscale = false;
	}
	
}






void openCvWindow::paintEvent(QPaintEvent *event)
{
	
	QPainter painter(this);
	
	static QPen myRedPen;
	myRedPen.setColor(QColor(255,0,0,255));
	myRedPen.setWidth(1);
	
	static QPen myBlackPen;
	myBlackPen.setColor(QColor(0,255,0,255));
	myBlackPen.setWidth(1);
	
	static QPen myWhitePen;
	myWhitePen.setColor(QColor(255,255,255,100));
	myWhitePen.setWidth(1);

	//we draw the image only if the Mat contains something!
	if(!mImage.empty() )
	{
		//image main data
		int rows = mImage.rows;
		int cols = mImage.cols;
		int bytesPerLine = mImage.step;
		unsigned char *p = mImage.data;
		int channels = mImage.channels();
		
		
		//we draw the image
		QImage currentView( p, cols, rows, bytesPerLine, QImage::Format_RGB888  );
		//currentView = currentView.scaled(widgetWidth,widgetHeight);
		currentView = currentView.scaled(widgetWidth,widgetHeight);
		painter.drawImage(0,0, currentView);
		//info about the image
		painter.setPen(myWhitePen);
		
		QString q1,q2, q3, q4;

		//q1 = "dim: ";
		q2 = QString::number(mImage.rows);
		q3 = QString::number(mImage.cols);
		//q4 = q1 + q2 + " " + q3;
		q4 = q2 +  " x " +   q3;
		
		painter.drawText(10, 15, q4);
		
		/*
		*				q1 = "channels: ";
		*				
		*				if(grayscale)
		*				{
		*								q2 = "1";
		}
		else
		{
			q2 = QString::number(mImage.channels());
		}
		
		q4 = q1 + q2;
		painter.drawText(10, 30, q4);
		*/
		q1 = "depth: ";
		if(mImage.depth() == CV_8U)
		{
			q2 = "CV_8U";
		}
		else
		{
			q2 = "???";
		}
		q4 = q1 + q2;
		//painter.drawText(10, 50, q4);
		
		q1 = "type: ";
		
		//if(mImage.type() == CV_8UC3)
		if(grayscale)
		{
			q2 = "CV_8UC1";
			
		}
		else
		{
			q2 = "CV_8UC3";
		} 
		
		
		q4 = q1 + q2;
		//painter.drawText(10, 70, q4);

		//draw a grid on the image
		if(grid_status==true)
		{
			//------------------------------
			static QPen penna;
			penna.setColor(QColor(255,0,0,50));
			penna.setWidth(1);
			painter.setPen(penna);
			
			
			// n_grid is rows or cols (they have to coincide)
			n_grid_h = rows;
			n_grid_w = cols;
			
			float w = width();
			float h = height();
			
			//it determines the jump from a box to the other
			float s_w = w/(float)n_grid_w;
			float s_h =  h/(float)n_grid_h;
			
			
			//vertical lines
			for(int c=1 ; c<n_grid_w; c++)
			{
				painter.drawLine(c*s_w , 0, c*s_w, w );
			}
			
			//horizontal lines
			for(int r=1 ; r<n_grid_h; r++)
			{
				painter.drawLine(0 , r*s_h,  w , r*s_h );
			}
			
			//------------draw the value of pixel  just if the window is big enough--------
			if( w>300 && h>300)
			{
				
				static QPen penna_2;
				penna_2.setColor(QColor(255,0,0,255));
				penna_2.setWidth(1);
				painter.setPen(penna_2);
				static QFont littleFont("Courier New", 6);
				painter.setFont(littleFont);
				for(int r=0; r<rows; r++)
				{
					for(int c=0; c<cols; c++ )
					{
						//current gray value in the Mat image.
						// I take the red channel 0,  because channel 1 or 2 should be the same, being
						// the image in bw
						int value = p[r*bytesPerLine + c*channels+0]; 
						QString qstr = QString::number( value);
						//writing the value in [0,255] of the box, exactly in the centre of box itself
						painter.drawText(s_w/2.0 + s_w*(c),  s_h/2.0 + s_h*(r), qstr);
					}
				}
				
			}
			
			
			
			
			//---------------------------------------------------
		}
		
		
		if(gunTarget_status==true)
		{
			// drawing the taget cross!
			static QPen p;
			static QPen r;
			
			p.setColor(QColor(255,255,255,150));
			p.setWidth(1);
			painter.setPen(p);
			
			r.setColor(QColor(255,0,0,200));
			r.setWidth(1);
			
			painter.setPen(p);
			painter.drawLine(width()/2 , 0,width()/2, height() );
			painter.drawLine(0 , height()/2, width(), height()/2 );
			
			// 	painter.drawLine(width()/3,height()/3,(width()*2)/3,height()/3);
			//	painter.drawLine((width()*2)/3,height()/3,(width()*2)/3,(height()*2)/3);
			// 	painter.drawLine((width()*2)/3,(height()*2)/3,width()/3,(height()*2/3));
			// 	painter.drawLine(width()/3,(height()*2)/3,width()/3,height()/3);
			
			
			
			
			// drowing the fovea  rect
			painter.setPen(r);
			
			//computing the ratio between fovea and image cols and rows
			float fovea_h_ratio = rows / (float)fovea_dim;
			float fovea_w_ratio = cols / (float)fovea_dim;
			
			
			//computing the scaled value of the fovea, in respect of windows dimensions
			int fovea_scaled_h = height() / (float)fovea_h_ratio; 
			int fovea_scaled_w = width() / (float)fovea_w_ratio;
			
			//painter.drawRect(width()/2 -fovea_scaled_w/2  , height()/2 - fovea_scaled_h/2 , fovea_scaled_w ,fovea_scaled_h  );			
		}
	}	
}
	
void openCvWindow::setFoveaDim(int w)
{
	fovea_dim = w; 
}
	
void openCvWindow::closeEvent(QCloseEvent *)
{
		
}
	
void openCvWindow::createToolBar()
{
		//	mToolBar = addToolBar("prova toolbar");
}