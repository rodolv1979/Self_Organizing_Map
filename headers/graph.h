#ifndef GRAPH_H
#define GRAPH_H
#include <simulator.h>
#include <parameters.h>

class Graph : public QWidget
{
    Q_OBJECT

	public:
		Graph( Parameters*, Simulator* , QWidget* parent = 0);
		~Graph();


	public slots: 
	
	private:
  
		Parameters* parameters;
		Simulator* simulator_graph;
		void 	paintEvent(QPaintEvent *);
		void 	paintAxis(QPainter &);
		void 	paintWorld(QPainter &);
		void 	paintAll(QPainter& );
		void 	paintRetina(QPainter& , vec&);
		void 	paintInput_SOM(QPainter&, vec& );
		void 	paintOutput_SOM(QPainter &, mat&, vec&);
		void 	paintWeights_SOM(QPainter &p, mat &weights_SOM,vec &input_SOM);


		int dx;
		int dy;
		int pass_input;
};

#endif // GRAPH_H
