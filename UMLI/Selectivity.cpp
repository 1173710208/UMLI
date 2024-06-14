#include<iostream>
#include<fstream>
#include<string>
#include<random>
#include<vector>

using namespace std;


/**
* Generate a test dataset containing data that are drawn from a uniform distribution.
*
* @param dim Number of dimensions
* @param num NUmber of generated data record
* @param range_from Lower bound of generated data
* @param range_to Upperbound of generated data
* @param min_selectivity Min selectivity
* @param max_selectivity Max selectivity
*/
void GeneratorUniform(int num, int dim, double range_from, double range_to)
{
	// Name all datasets
	string fileName = "./Datasets/uniform_";
	fileName += to_string(dim);
	fileName += "d_";
	fileName += to_string(num);
	fileName += ".csv";

	// Generate random numbers following uniform distributions
	random_device rand_dev;
	mt19937 generator(rand_dev());
	uniform_real_distribution<double> uni_distr(range_from, range_to); //uniform distribution

	// Write random numbers to .csv files
	ofstream dofs;
	dofs.open(fileName, ios::out);
	for (int numCounter = 0; numCounter < num; ++numCounter) {
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			dofs << uni_distr(generator) << ",";
		}
		dofs << numCounter + 1 << "\n";
	}
	dofs.close();
}

/**
* Generate a test 1d range query dataset that are drawn from a uniform distribution.
*
* @param dim Number of dimensions
* @param num NUmber of generated data record
* @param range_from Lower bound of generated data
* @param range_to Upperbound of generated data
* @param selectivity Selectivity
*/
void GeneratorSelectivity(int num, int dim,  double range_from, double range_to, float selectivity)
{
	// Name range query dataset
	string range_fileName = "./Datasets/";
	range_fileName += to_string(dim);
	range_fileName += "d_selectivity";
	range_fileName += to_string(selectivity);
	range_fileName += "_";
	range_fileName += to_string(num);
	range_fileName += "_range.csv";
	// Generate random numbers following different distributions
	random_device rand_dev;
	mt19937 generator(rand_dev());
	uniform_real_distribution<double> uni_distr(range_from, range_to); //uniform distribution

	// Write random queries to .csv files
	ofstream qofs;
	qofs.open(range_fileName, ios::out);
	vector<vector<double>> min;
	vector<vector<double>> max;
	for (int numCounter = 0; numCounter < num; ++numCounter) {
		vector<double> v1;
		vector<double> v2;
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			double upper_bound = uni_distr(generator);
			double lower_bound = upper_bound - (range_to - range_from) * selectivity;
			while (lower_bound < range_from) {
				upper_bound = uni_distr(generator);
				lower_bound = upper_bound - (range_to - range_from) * selectivity;
			}
			v1.push_back(lower_bound);
			v2.push_back(upper_bound);
		}
		min.push_back(v1);
		max.push_back(v2);
	}
	for (int numCounter = 0; numCounter < num; ++numCounter) {
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			qofs << min[numCounter][dimCounter] << ",";
		}
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			qofs << max[numCounter][dimCounter] << ",";
		}
		qofs << numCounter + 1 << "\n";
	}
	qofs.close();
}




int mainSELE() {

	//GeneratorUniform(1000, 1, 0, 10);
	//GeneratorUniform(10000, 1, 0, 10);
	//GeneratorUniform(100000, 1, 0, 10);
	//GeneratorSelectivity(1000, 1, 0, 10, 0.01);
	//GeneratorSelectivity(1000, 1, 0, 10, 0.05);
	//GeneratorSelectivity(1000, 1, 0, 10, 0.1);
	//GeneratorSelectivity(1000, 1, 0, 10, 0.15);
	//GeneratorSelectivity(1000, 1, 0, 10, 0.2);

	//GeneratorUniform(1000, 2, 0, 10);
	//GeneratorUniform(10000, 2, 0, 10);
	//GeneratorSelectivity(1000, 2, 0, 10, 0.2);
	
	//GeneratorUniform(5000, 1, 0, 10);
	//GeneratorUniform(4000, 1, 0, 10);
	//GeneratorUniform(3000, 1, 0, 10);
	//GeneratorUniform(2000, 1, 0, 10);
	//GeneratorUniform(1000, 1, 0, 10);
	//GeneratorUniform(100, 1, 0, 10);
	//GeneratorUniform(200, 1, 0, 10);
	//GeneratorUniform(300, 1, 0, 10);
	//GeneratorUniform(400, 1, 0, 10);
	//GeneratorUniform(500, 1, 0, 10);
	//GeneratorUniform(600, 1, 0, 10);
	//GeneratorUniform(700, 1, 0, 10);
	//GeneratorUniform(800, 1, 0, 10);
	//GeneratorUniform(900, 1, 0, 10);

	system("pause");
	return 0;
}
