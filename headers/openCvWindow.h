

/**
 * @file 
 *      openCvWindow.h
 * @brief 
 *        
 *        
 */





#ifndef _OPENCV_WINDOW_H_
#define _OPENCV_WINDOW_H_


#include <QMainWindow>
#include <QtGui/qpen.h>
#include <QtGui/qwidget.h>
#include <QDialog>
#include <QToolBar>
#include <QBoxLayout>
#include <QImage>
#include <QPixmap>
#include <QLabel>

// #include <opencv/cv.h>
// #include <opencv/highgui.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;


class openCvWindow : public  QWidget
{
	Q_OBJECT
	
	public:
		
		openCvWindow(QWidget *parent = 0);
		virtual ~openCvWindow();
		void setImage( cv::Mat _mImage);
		void setGrid(bool);
		void setGunTarget(bool);
		int getFoveaDim(){return fovea_dim;}
		void setFoveaDim(int w);
		
		
	protected:
		
		void paintEvent(QPaintEvent *event);
		
		
	public:
		
		void closeEvent(QCloseEvent *event);
		
	private:
		
	
		cv::Mat mImage;
		bool grid_status;
		int n_grid_w;
		int n_grid_h;
		bool gunTarget_status;
		int widgetWidth;
		int widgetHeight;
		int fovea_dim;
		QToolBar *mToolBar;
		void createToolBar();
		// I use this flag, just to signal that the image we want to visualise is originally bw (1 channel).
		// QT cannot visualise directly bw image, so I have to transform it in an rgb image! 
		bool grayscale;
		
		
		
	protected:
		void resizeEvent(QResizeEvent * event);
};


#endif