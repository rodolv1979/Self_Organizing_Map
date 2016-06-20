//Classe Simulator
#ifndef _SIMULATOR_H
#define _SIMULATOR_H

#include <QMainWindow>
#include <itpp/itbase.h>
#include <parameters.h>



using namespace itpp;
using namespace std;



class Simulator: public  QWidget
{
	Q_OBJECT
		public :
			Simulator(Parameters* , QWidget *parent = 0);
			~Simulator();
			
			void init();
			void reset();
			void step();
			void updateGraph();
			void SaveOnFile();
			void init_weights(const mat&,string);
			void init_pattern_of_input();
			void norm_weights();
			int getTiming();
			int getTrialNumber();
			int getStepNumber();
			vec& getInput();
			mat& getOutput_SOM();
			vec& getInput_SOM();
			mat& getWeights_SOM();

			double getDistance(const vec&,const vec& );
			double decay_learning_rate(double ,double ,double );
			double decay_sigma(double ,double ,double );
			vec get_coordinates_of_matrix(int ,int );
			void function_of_neighborhood(double ,int ,const vec&, mat&); 
			void learning(const vec&, const mat& ,const vec&, double,string );
			vec extract_pattern(string );
			int find_BMU(const vec&,string );
			void E_perCent_max(double ); 

			vec indexes_of_max_value_mat(const mat&);



			
		private: 
			Parameters *par;
			mat in;
			vec input,v1,v2,v3,v4,v5,v6,v7,v8;
			mat w,output_SOM_mat,DeltaW;
			vec input_SOM_vec;
			bool saveOnFile;
			int n_step;
			double timing;
			int n_trial;
			int size;
			string type;
};
#endif