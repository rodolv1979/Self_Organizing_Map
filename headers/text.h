#ifndef TEXT_H
#define TEXT_H

#include <QWidget>
#include <QPainter>
#include <QLabel>
#include <QTimer>
#include <simulator.h>
#include <QObject>


class Text : public QWidget
{
	Q_OBJECT
		public:
			Text(Simulator* , QWidget *parent = 0);
			~Text();

		private:
    
			Simulator* simulator_text;
			QLabel* trials;
			QLabel* steps;
			QLabel* timing;
			QTimer *timer;
			QPointF eye;
			QPointF obj;
			void paintEvent(QPaintEvent *);

};
#endif // TEXT_H
