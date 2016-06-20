#include <graph.h>
#include <QPainter>
#include <iomanip>

using namespace std;
using namespace itpp;


Graph::Graph(Parameters* par, Simulator* gra, QWidget *parent) :QWidget(parent)
{	
	parameters = par;
	simulator_graph = gra;	
	setPalette(QPalette(QColor(250, 250, 200)));
    	setAutoFillBackground(true);
	dx=50;
	dy=200;
	pass_input=40;
}

Graph::~Graph()
{
	
}

void Graph::paintEvent(QPaintEvent * /* event */)
{
	QPainter p(this);
	//paintAxis(p);//Paint Axis
	//paintWorld(p);//Paint World
	paintAll(p);  
}

void Graph::paintAxis(QPainter &p)
{
	p.setPen(Qt::DotLine);//Choose the style pen
	p.save();
	//p.translate(parameters->x_tr,parameters->y_tr);//Translate the coordinate system
    	p.scale(15,-15);
    	p.drawLine(0,100,0,-150);//Vertical Axe
    	p.drawLine(-100,0,100,0);//Orizzontal Axe
	p.restore();
}

void Graph::paintWorld(QPainter &p)//OK
{
	p.save();
	p.translate(parameters->x_tr,parameters->y_tr);
	p.setPen(Qt::DashDotLine);
	p.drawLine(0,(-parameters->h_ret/2)*parameters->expand,(parameters->w_ret/2)*parameters->expand,(-parameters->h_ret/2)*parameters->expand);//up orizzontal
	p.drawLine((parameters->w_ret/2)*parameters->expand,(-parameters->h_ret/2)*parameters->expand,(parameters->w_ret/2)*parameters->expand,0);//right  vertical
	p.drawLine((parameters->w_ret/2)*parameters->expand,0,0,0);//down orizzontal
	p.drawLine(0,0,0,(-parameters->h_ret/2)*parameters->expand);//left  vertical
	p.restore();
}


void Graph::paintRetina(QPainter &p, vec &input)
{
	static QBrush myBrush(QColor(0,0,0,200));
	for (int i=0;i<input.size();i++)
	{
		if (input(i)==1)
		{
			myBrush.setColor(QColor(100,100,100,255));
		}
		else
		{
			myBrush.setColor(QColor(255,255,255,255));
		}
		p.setPen(Qt::SolidLine);
		p.setBrush(myBrush);
		
		if ( (mod(i,parameters->n_el_ret) != 0) || (i==0))
		{
			p.drawRect(dx,dy,pass_input,pass_input);
			dx=dx+pass_input;
		}
		else
		{ 
			if (mod(i,parameters->n_el_ret) == 0)
			{
				dx = 50;
				dy = dy + pass_input;
				p.drawRect(dx,dy,pass_input,pass_input);
				dx=dx+pass_input;
			}  
		}
	}
	dx=50;
	dy=200;
}

void Graph::paintInput_SOM(QPainter &p, vec &input_SOM)
{
	static QBrush myBrush(QColor(0,0,0,200));
	myBrush.setColor(QColor(input_SOM(0)*255,input_SOM(1)*255,input_SOM(2)*255,255));
	p.setPen(Qt::SolidLine);
	p.setBrush(myBrush);
	QPointF point;
	QPointF point1;
	qreal x=200;
	qreal y=300;
	point1.setX(x);
	point1.setY(y);
	int radius=40; 
	p.drawEllipse(point1,radius,radius);
}


void Graph::paintOutput_SOM(QPainter &p, mat &output_SOM,vec &input_SOM)
{
	static QBrush myBrush(QColor(0,0,0,200));
	myBrush.setColor(QColor(255,0,0,128));	
	p.setPen(Qt::SolidLine);
	p.setBrush(myBrush);
	
	int elements = 20;
	int radius=8;
	QPointF point1;
	qreal x=370;
	qreal y=50;
	point1.setX(x);
	point1.setY(y);
	for (int row=0;row<elements;row++)
	{
		for (int col=0;col<elements;col++)
		{
 			qreal r_x=output_SOM(row,col)*radius;
			qreal r_y=output_SOM(row,col)*radius;
			p.drawEllipse(point1,r_x,r_y);
			x=x+(radius*2);
			point1.setX(x);
		}
		x=370;
		point1.setX(370);
		y=y+(2*radius);
		point1.setY(y);
	}
	
}

void Graph::paintWeights_SOM(QPainter &p, mat &weights_SOM,vec &input_SOM)
{
	static QBrush myBrush(QColor(0,0,0,200));
	myBrush.setColor(QColor(255,0,0,128));	
	p.setPen(Qt::SolidLine);
	p.setBrush(myBrush);
	int elements = sqrt(weights_SOM.rows());
	int radius=8;
	QPointF point1;
	qreal x=370;
	qreal y=50;
	int cont=0;
	point1.setX(x);
	point1.setY(y);
	for (int row=0;row<elements;row++)
	{
		for (int col=0;col<elements;col++)
		{
			double w1=weights_SOM(cont,0);
			if (w1<0) {w1=0;}
			double w2=weights_SOM(cont,1);
			if (w2<0) {w2=0;}
			double w3=weights_SOM(cont,2);
			if (w3<0) {w3=0;}
			myBrush.setColor(QColor(round_i(w1*255),round_i(w2*255),round_i(w3*255),255));	
			p.setPen(Qt::SolidLine);
			p.setBrush(myBrush);
			p.drawEllipse(point1,radius,radius);
			x=x+(radius*2);
			point1.setX(x);
			cont++;
		}
		x=370;
		point1.setX(370);
		y=y+(2*radius);
		point1.setY(y);
	}
	   
}


void Graph::paintAll(QPainter &p)
{	
	paintInput_SOM(p,simulator_graph->getInput_SOM());
	//paintOutput_SOM(p,simulator_graph->getOutput_SOM(),simulator_graph->getInput_SOM());
	paintWeights_SOM(p,simulator_graph->getWeights_SOM(),simulator_graph->getInput_SOM());

}