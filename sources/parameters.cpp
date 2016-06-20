//Class Parameters.cpp


#include <parameters.h>


Parameters::Parameters(QWidget *parent)
:QWidget(parent)
{

	//Parameters for the simulation
	
	n_trial = 150000; // numero di Trial per Run
	n_step = 50;    // numero step per trial
	
	n_el_ret = 5;
	w_ret = 432;
	h_ret = 320;
	x_tr=100;		//Translate of axe x
	y_tr=800; 		//Translate of axe y
	expand = 2; 		//expand coefficient of graphics output
	
	
	
	//Self Organizing Map Parameters
	lr = 0.1;
	timer = 0;
	phase_of_sorting = 1500;
	numbers_of_input = 3; 
	numbers_of_output = 40;
	numbers_of_patterns = 3;
	sigma = numbers_of_output/2;
	tau_SOM = phase_of_sorting/log(sigma);
	w_inf = 1;
	w_sup = 0;
	amount_of_inhibition = 0.9;
}

Parameters::~Parameters()
{

}

void Parameters::init()
{
	
	
}	
