// Class Simulator
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <simulator.h>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>


Simulator::Simulator(Parameters* p, QWidget *parent):QWidget(parent)
{
	par = p;
	n_trial=0;
	n_step=0;
	saveOnFile=false;
	timing=par->timer;
	
	size = 25;
	input = zeros(size);
	
	type="eu";
	//type="not_eu";
	
	v1 = "255,255,255";//Rosso
	v2 = "0,50,50";//Green
 	v3 = "0,0,255";//Blu
// 	
	v1=v1/255;
	v2=v2/255;
	v3=v3/255;
	
	
// 	v1 = "210,31,27";//Rosso pompeiano
// 	v2 = "255,153,0";//Arancione
// 	v3 = "0,0,128";//Blu marino
// 	v4 = "28,57,187";//Blu di Persia
// 	v5 = "102,255,0";//Verde chiaro
// 	v6 = "46,139,87";//Verde Marino
// 	v7 = "255,255,0";//Giallo
// 	v8 = "153,17,153";//Viola Melanzana
// 	
// 	v1=v1/255;
// 	v2=v2/255;
// 	v3=v3/255;
// 	v4=v4/255;
// 	v5=v5/255;
// 	v6=v6/255;
// 	v7=v7/255;
// 	v8=v8/255;
// 	
	in = zeros(p->numbers_of_patterns,p->numbers_of_input);
	input_SOM_vec = zeros(p->numbers_of_input);
	w = zeros(p->numbers_of_output * p->numbers_of_output,p->numbers_of_input);
	DeltaW = zeros(p->numbers_of_output * p->numbers_of_output,p->numbers_of_input);
	output_SOM_mat=zeros(p->numbers_of_output,p->numbers_of_output);
}

Simulator::~Simulator()
{

	
}

void Simulator::init()
{
	init_pattern_of_input();
	init_weights(in,type);
	n_step=0;
	n_trial=0;
	timing=par->timer;
}

void Simulator::reset()//after the reset()
{
	n_trial=0;
	n_step=0;
	timing=0;
}

void Simulator::step()
{  	
	RNG_randomize();
	for (int i=0;i<size;i++)
	{
		input[i]=randi(0,1);
	}
	
	//SOM ALGORITHM
	
	//Extract a random pattern of input
	input_SOM_vec = extract_pattern(type);
	
	//Calculate the BMU
	int index_BMU=0;
	index_BMU=find_BMU(input_SOM_vec,type);
	
	//get the coordinates of BMU
	vec coordinates_BMU_vec=zeros(2);
	coordinates_BMU_vec=get_coordinates_of_matrix(index_BMU,par->numbers_of_output);

	//calculate the learning rate decay
	double lr_t=0;
	if (timing<par->phase_of_sorting)
	{
		lr_t=decay_learning_rate(par->lr,par->tau_SOM,timing);
	}
	else
	{
		lr_t=0.001;
	}
		
	//calculate the sigma rate decay
	double sigma_t=0;
	if (timing<par->phase_of_sorting)
	{
		sigma_t = decay_sigma(par->sigma,par->tau_SOM,timing);
	}
	else
	{
		sigma_t=0.001;
	}
	
	//Compute Activations of Neurons
	function_of_neighborhood(sigma_t,par->numbers_of_output,coordinates_BMU_vec,output_SOM_mat);
	//E_perCent_max(par->amount_of_inhibition); 
	
	//Learning Phase
	learning(input_SOM_vec,output_SOM_mat,coordinates_BMU_vec,lr_t,type);
	
	timing++;
	n_step++;
}

void Simulator::SaveOnFile()
{
    
}

int Simulator::getTrialNumber()
{
	return n_trial;
}

int Simulator::getStepNumber()
{
	return n_step;
}

int Simulator::getTiming()
{
	return timing;
}

vec& Simulator::getInput()
{
	return input;
}

vec& Simulator::getInput_SOM()
{
	return input_SOM_vec;
}

mat& Simulator::getOutput_SOM()
{
	return output_SOM_mat;
}

mat& Simulator::getWeights_SOM()
{
	return w;
}

void Simulator::init_weights(const mat& input,string mode)
{
	RNG_randomize();
	w=randu(par->numbers_of_output*par->numbers_of_output,par->numbers_of_input)*(par->w_sup - par->w_inf) + par->w_inf;
// 	for (int rows=0;rows<w.rows();rows++)
// 	{
// 		RNG_randomize();
// 		int num=randi(0,input.rows()-1);
// 		for (int cols=0;cols<w.cols();cols++)
// 		{
// 			w(rows,cols)=input(num,cols);
// 		}
// 	}
	
	if (mode=="not_eu")
	{
		norm_weights();
	}
}

void Simulator::init_pattern_of_input()
{
	in.set_row(0,v1);
	in.set_row(1,v2);
	in.set_row(2,v3);
	in.set_row(3,v4);
	in.set_row(4,v5);
	in.set_row(5,v6);
	in.set_row(6,v7);
	in.set_row(7,v8);
}

double Simulator::getDistance(const vec& input_vec1, const vec& input_vec2)
{
	double value=0;
	for (int i=0;i<input_vec1.size();i++)
	{
		value = value + (sqr(input_vec1[i]-input_vec2[i]));
	}
	return sqrt(value);
}

vec Simulator::extract_pattern(string mode)
{
	vec result = zeros(in.cols());
	int number=0;
	RNG_randomize();
	number = randi(0,in.rows()-1);
	result = in.get_row(number);
	if (mode=="not_eu")
	{
		double value=0;
		for (int i=0;i<result.size();i++)
		{
			value=value+sqr(result(i));
		}
		double norm=sqrt(value);
		result=result/norm;
	}
	return result;
}

int Simulator::find_BMU(const vec& input,string mode)
{
	vec result = zeros(2);	//the first element contains the min distance, the secondo element 						contains the index of it
	vec pos_output_matrix = zeros(2);
	double value=0;
	for (int i=0;i<w.rows();i++)
	{	
		if (mode=="eu")
		{
			value = getDistance(input,w.get_row(i));
		}
		else
		{
			value = w.get_row(i) * input;
			pos_output_matrix=get_coordinates_of_matrix(i,output_SOM_mat.cols());
			output_SOM_mat(pos_output_matrix(0),pos_output_matrix(1))=value;
			//Insert the values to save in output!!!
		}
		
		if (i==0)
		{
			result[0]=value;
			result[1]=i;
		}
		else
		{
			if (mode=="eu")
			{
				if (value<result[0])
				{
					result[0]=value;
					result[1]=i;
				}
			}
			else
			{
				if (value>result[0])
				{
					result[0]=value;
					result[1]=i;
				}
			}
			
		}
	}
	return result[1];//The n unity with the lower distance/higher energy 
}


double Simulator::decay_learning_rate(double lr,double tau,double timing)
{	
	double lr_new=0;
	return lr_new = lr * (1/exp(timing/tau));
}

double Simulator::decay_sigma(double s,double tau,double timing)
{	
	double s_new=0;
	return s_new = s * (1/exp(timing/tau));
}

vec Simulator::get_coordinates_of_matrix(int index,int amp)
{
	int cont=0;
	vec result=zeros(2);
	for (int row=0;row<amp;row++)
	{
		for (int col=0;col<amp;col++)
		{
			if (cont==index)
			{
				result[0]=row;
				result[1]=col;
			}
			cont++;
		}
	}
	return result;
}

void Simulator::function_of_neighborhood(double s_t,int amp,const vec& coord, mat& output_mat) 
{
	for (int i=0;i<amp;i++)
	{
		for (int j=0;j<amp;j++)
		{
			double dist = sqrt(sqr(coord(0)-i) + sqr(coord(1)-j));
			//output_mat(i,j)=(1 / exp(sqr(dist)/2*sqr(s_t)));
			//cout<<"dist "<<dist<<endl;
			
			if (dist/2<s_t)
			{
				output_mat(i,j)=1;
			}
			else
			{
				output_mat(i,j)=0;
			}
			
			
		}
	}
}

void Simulator::E_perCent_max(double am_of_inh) 
{
	double max_value=output_SOM_mat(0,0);
	for (int rows=0;	rows<output_SOM_mat.rows();rows++)
	{
		for(int cols=0;cols<output_SOM_mat.cols();cols++)
		{
			if (output_SOM_mat(rows,cols)>max_value)
			{
				max_value=output_SOM_mat(rows,cols);
			}
		}
	}
	
	for (int rows=0;rows<output_SOM_mat.rows();rows++)
	{
		for(int cols=0;cols<output_SOM_mat.cols();cols++)
		{
			if (output_SOM_mat(rows,cols)<(max_value*am_of_inh))
			{
				output_SOM_mat(rows,cols)=0;
			}
			else
			{
				output_SOM_mat(rows,cols)=output_SOM_mat(rows,cols) - (max_value*am_of_inh);
			}
		}
	}
	
}


void Simulator::learning(const vec& input_vec, const mat& output_mat,const vec& coord,double lr,string mode)
{
	int cont=0;
	vec out_coord=zeros(2);
	for (int rows=0;rows<DeltaW.rows();rows++)
	{
		for (int cols=0;cols<DeltaW.cols();cols++)
		{
			out_coord=get_coordinates_of_matrix(cont,output_mat.cols());
			DeltaW(rows,cols) = lr * output_mat(out_coord(0),out_coord(1)) * (input_vec(cols) - w(rows,cols));
		}
		cont++;
	}
	
// 	vec res=zeros(2);
// 	res=indexes_of_max_value_mat(output_mat);
// 	DeltaW(res(0),res(1)) = lr * 1 * (input_vec(res(1)) - w(res(0),res(1)));
	
	w=w+DeltaW;
	if (mode=="not_eu")
	{
		norm_weights();
	}
}

void Simulator::norm_weights()
{
	double value=0;
	double norm=0;
	for (int rows=0;rows<w.rows();rows++)
	{
		for(int cols=0;cols<w.cols();cols++)
		{
			value=value+sqr(w(rows,cols)); 
		}
		norm=sqrt(value);
		for (int cols=0;cols<w.cols();cols++)
		{
			w(rows,cols)=w(rows,cols)/norm;
		}
		value=0;
		norm=0;
	}
}

vec Simulator::indexes_of_max_value_mat(const mat &input)
{
	vec result=zeros(2);
	double value=input(0,0);
	for (int rows=0;rows<input.rows();rows++)
	{
		for(int cols=0;cols<input.cols();cols++)
		{
			if (input(rows,cols)>value)
			{
				value=input(rows,cols);
				vec(0)=rows;
				vec(1)=cols;
			}
		}
	}
	return result;	
}