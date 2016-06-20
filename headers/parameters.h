//Classe Simulator
#ifndef _PARAMETERS_H
#define _PARAMETERS_H
#include <QMainWindow>
#include <itpp/itbase.h>

using namespace itpp;
using namespace std;



class Parameters : public QWidget
{
	Q_OBJECT
		public:
			Parameters( QWidget *parent = 0);
			~Parameters();
      
			void init();
      
			int n_trial;
			int n_step;
			
			int n_el_ret;
			
			//Graph Parameters
			double w_ret;
			double h_ret;
			double expand;
			double y_tr;
			double x_tr;
			
			//SOM parameters
			double tau_SOM;
			double sigma;
			double lr;
			double w_inf;
			double w_sup;
			double amount_of_inhibition;
			int timer;
			int numbers_of_input;
			int numbers_of_output;
			double phase_of_sorting;
			int numbers_of_patterns;
      
};
#endif //PARAMETERS