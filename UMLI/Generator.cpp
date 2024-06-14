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
void GeneratorTest(int dim, int num, double range_from, double range_to, double min_selectivity, double max_selectivity)
{
	// Name all datasets
	string fileName = "./Datasets/C_";
	fileName += to_string(dim);
	fileName += "d_";
	fileName += to_string(num);
	fileName += ".csv";

	// Name range query dataset
	string range_fileName = "./Datasets/C_";
	range_fileName += to_string(dim);
	range_fileName += "d_";
	range_fileName += to_string(num/1000);
	range_fileName += "_range.csv";
	


	// Generate random numbers following different distributions
	random_device rand_dev;
	mt19937 generator(rand_dev());
	uniform_real_distribution<double> uni_distr(range_from, range_to); //uniform distribution, [0, 1)
	// Generate random query selectivity
	uniform_real_distribution<double> query_selectivity(min_selectivity, max_selectivity);



	// Write random numbers to .csv files
	ofstream dofs;
	dofs.open(fileName, ios::out);
	for (int numCounter = 0; numCounter < num; ++numCounter) {
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			dofs << uni_distr(generator) << ",";
		}
		dofs << numCounter+1 << "\n";
	}
	dofs.close();
	// Write random queries to .csv files
	ofstream qofs;
	qofs.open(range_fileName, ios::out);
	vector<vector<double>> min;
	vector<vector<double>> max;
	for (int numCounter = 0; numCounter < num/1000; ++numCounter) {
		vector<double> v1;
		vector<double> v2;
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			double upper_bound = uni_distr(generator);
			double lower_bound = upper_bound - (range_to - range_from) * query_selectivity(generator);
			if (lower_bound < range_from) {
				lower_bound = range_from;
			}
			v1.push_back(lower_bound);
			v2.push_back(upper_bound);
		}
		min.push_back(v1);
		max.push_back(v2);
	}
	for (int numCounter = 0; numCounter < num/1000; ++numCounter) {
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


/**
* Generate several datasets containing data that are drawn from several particular distributions.
*
* @param dim Number of dimensions
* @param num NUmber of generated data record
* @param range_from Lower bound of generated data
* @param range_to Upperbound of generated data
*/
void Generator(int dim, int num, double range_from, double range_to, double min_selectivity, double max_selectivity)
{
	// Name all datasets
	string uniform_fileName = "./Datasets/uniform_";
	uniform_fileName += to_string(dim);
	uniform_fileName += "d_";
	uniform_fileName += to_string(num);
	uniform_fileName += ".csv";

	string exponential_fileName = "./Datasets/exponential_";
	exponential_fileName += to_string(dim);
	exponential_fileName += "d_";
	exponential_fileName += to_string(num);
	exponential_fileName += ".csv";

	string gamma_fileName = "./Datasets/gamma_";
	gamma_fileName += to_string(dim);
	gamma_fileName += "d_";
	gamma_fileName += to_string(num);
	gamma_fileName += ".csv";

	string normal_fileName = "./Datasets/normal_";
	normal_fileName += to_string(dim);
	normal_fileName += "d_";
	normal_fileName += to_string(num);
	normal_fileName += ".csv";

	string chisquared_fileName = "./Datasets/chisquared_";
	chisquared_fileName += to_string(dim);
	chisquared_fileName += "d_";
	chisquared_fileName += to_string(num);
	chisquared_fileName += ".csv";

	string f_fileName = "./Datasets/f_";
	f_fileName += to_string(dim);
	f_fileName += "d_";
	f_fileName += to_string(num);
	f_fileName += ".csv";

	//string piecewiselinear_fileName = "./Datasets/piecewiselinear_";
	//piecewiselinear_fileName += to_string(dim);
	//piecewiselinear_fileName += "d_";
	//piecewiselinear_fileName += to_string(num);
	//piecewiselinear_fileName += ".csv";

	//// Name range query dataset
	//string range_fileName = "./Datasets/";
	//range_fileName += to_string(dim);
	//range_fileName += "d_";
	//range_fileName += to_string(num/1000);
	//range_fileName += "_range.csv";



	//Pseudo-random number generation
	random_device rand_dev; // will be used to obtain a seed for the random number engine
	mt19937 generator(rand_dev()); // standard mersenne_twister_engine seeded with rand_dev()

	// Generate random numbers following different distributions
	uniform_real_distribution<double> uniform_distr(range_from, range_to);										// uniform distribution			[0, 10)
	exponential_distribution<double> exponential_distr(0.5);													// exponential distribution		¦Ë = 0.5 //
	gamma_distribution<double> gamma_distr(9, 2);																// gamma distribution			¦Á =9, ¦Â = 2
	normal_distribution<double> normal_distr{(range_to - range_from) / 2.0, 1};		// normal distribution			¦Ì = 0.5 * (range_to-range_from), ¦Ò = 1
	chi_squared_distribution<float> chisquared_distr{9};														// chi-squared distribution		n = 9
	fisher_f_distribution<float> f_distr {100, 100};																// f-distribution				d1 = 100, d2 = 100
	vector<double> i {0, 7, 10}; 
	vector<double> w {0, 0.3, 1}; // increase the probability from 0 to 3, remain flat from 3 to 7, decrease from 7 to 10 at the same rate
	//piecewise_linear_distribution<> piecewiselinear_distr {i.begin(), i.end(), w.begin()};						// piecewise linear distribution 

	// Generate random query selectivity
	uniform_real_distribution<double> query_selectivity(min_selectivity, max_selectivity);



	 //Write random numbers to .csv files
	// uniform distribution
	ofstream uniform_ofs;
	uniform_ofs.open(uniform_fileName, ios::out);
	for (int numCounter = 0; numCounter < num; ++numCounter) {
		cout << numCounter << endl;
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			uniform_ofs << uniform_distr(generator) << ",";
		}
		uniform_ofs << numCounter + 1 << "\n";
	}
	uniform_ofs.close();
	// exponential distribution
	ofstream exponential_ofs;
	exponential_ofs.open(exponential_fileName, ios::out);
	for (int numCounter = 0; numCounter < num; ++numCounter) {
		cout << numCounter << endl;
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			double field = exponential_distr(generator);
			while (field <= range_from || field >= range_to) {
				field = exponential_distr(generator);
			}
			exponential_ofs << field << ",";
		}
		exponential_ofs << numCounter + 1 << "\n";
	}
	exponential_ofs.close();
	// gamma distribution
	ofstream gamma_ofs;
	gamma_ofs.open(gamma_fileName, ios::out);
	for (int numCounter = 0; numCounter < num; ++numCounter) {
		cout << numCounter << endl;
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			double field = gamma_distr(generator);
			while (field <= range_from || field >= range_to) {
				field = gamma_distr(generator);
			}
			gamma_ofs << field << ",";
		}
		gamma_ofs << numCounter + 1 << "\n";
	}
	gamma_ofs.close();
	// normal distribution	
	ofstream normal_ofs;
	normal_ofs.open(normal_fileName, ios::out);
	for (int numCounter = 0; numCounter < num; ++numCounter) {
		cout << numCounter << endl;
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			double field = normal_distr(generator);
			while (field <= range_from || field >= range_to) {
				field = normal_distr(generator);
			}
			normal_ofs << field << ",";
		}
		normal_ofs << numCounter + 1 << "\n";
	}
	normal_ofs.close();
	// chi-squared distribution
	ofstream chisquared_ofs;
	chisquared_ofs.open(chisquared_fileName, ios::out);
	for (int numCounter = 0; numCounter < num; ++numCounter) {
		cout << numCounter << endl;
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			double field = chisquared_distr(generator);
			while (field <= range_from || field >= range_to) {
				field = chisquared_distr(generator);
			}
			chisquared_ofs << field << ",";
		}
		chisquared_ofs << numCounter + 1 << "\n";
	}
	chisquared_ofs.close();
	// f-distribution
	ofstream f_ofs;
	f_ofs.open(f_fileName, ios::out);
	for (int numCounter = 0; numCounter < num; ++numCounter) {
		cout << numCounter << endl;
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			double field = f_distr(generator);
			while (field <= range_from || field >= range_to) {
				field = f_distr(generator);
			}
			f_ofs << field << ",";
		}
		f_ofs << numCounter + 1 << "\n";
	}
	f_ofs.close();
	//// piecewise linear distribution 
	//ofstream piecewiselinear_ofs;
	//piecewiselinear_ofs.open(piecewiselinear_fileName, ios::out);
	//for (int numCounter = 0; numCounter < num; ++numCounter) {
	//	cout << numCounter << endl;
	//	for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
	//	{
	//		double field = piecewiselinear_distr(generator);
	//		while (field <= range_from || field >= range_to) {
	//			field = piecewiselinear_distr(generator);
	//		}
	//		piecewiselinear_ofs << field << ",";
	//	}
	//	piecewiselinear_ofs << numCounter + 1 << "\n";
	//}
	//piecewiselinear_ofs.close();

	//// Write random queries to .csv files
	//ofstream range_ofs;
	//range_ofs.open(range_fileName, ios::out);
	//vector<vector<double>> min;
	//vector<vector<double>> max;
	//for (int numCounter = 0; numCounter < num; ++numCounter) {
	//	vector<double> v1;
	//	vector<double> v2;
	//	for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
	//	{
	//		double upper_bound = uniform_distr(generator);
	//		double lower_bound = upper_bound - (range_to - range_from) * query_selectivity(generator);
	//		if (lower_bound < range_from) {
	//			lower_bound = range_from;
	//		}
	//		v1.push_back(lower_bound);
	//		v2.push_back(upper_bound);
	//	}
	//	min.push_back(v1);
	//	max.push_back(v2);
	//}
	//for (int numCounter = 0; numCounter < num; ++numCounter) {
	//	for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
	//	{
	//		range_ofs << min[numCounter][dimCounter] << ",";
	//	}
	//	for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
	//	{
	//		range_ofs << max[numCounter][dimCounter] << ",";
	//	}
	//	range_ofs << numCounter + 1 << "\n";
	//}
	//range_ofs.close();
}


void RangeGenerator(int dim, int num, double range_from, double range_to, double* selectivity)
{
	// Name range query dataset
	string range_fileName = "./Datasets/";
	range_fileName += to_string(dim);
	range_fileName += "d_";
	range_fileName += to_string(num);
	range_fileName += "_range.csv";

	//Pseudo-random number generation
	random_device rand_dev; // will be used to obtain a seed for the random number engine
	mt19937 generator(rand_dev()); // standard mersenne_twister_engine seeded with rand_dev()
	uniform_real_distribution<double> uniform_distr(range_from, range_to);										// uniform distribution			[0, 10)

	// Generate random query selectivity
	uniform_real_distribution<double> query_selectivity0(selectivity[0], selectivity[0]);
	uniform_real_distribution<double> query_selectivity1(selectivity[1], selectivity[1]);
	uniform_real_distribution<double> query_selectivity2(selectivity[2], selectivity[2]);
	uniform_real_distribution<double> query_selectivity3(selectivity[3], selectivity[3]);

	// Write random queries to .csv files
	ofstream range_ofs;
	range_ofs.open(range_fileName, ios::out);
	vector<vector<double>> min;
	vector<vector<double>> max;
	for (int numCounter = 0; numCounter < num; ++numCounter) {
		vector<double> v1;
		vector<double> v2;
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			double upper_bound = uniform_distr(generator);
			if (dimCounter == 0) {
				double lower_bound = upper_bound - (range_to - range_from) * query_selectivity0(generator);
				if (lower_bound < range_from) {
					lower_bound = range_from+0.000001;
				}
				v1.push_back(lower_bound);
				v2.push_back(upper_bound);
			}
			else if (dimCounter == 1) {
				double lower_bound = upper_bound - (range_to - range_from) * query_selectivity1(generator);
				if (lower_bound < range_from) {
					lower_bound = range_from+0.000001;
				}
				v1.push_back(lower_bound);
				v2.push_back(upper_bound);

			}
			else if (dimCounter == 2) {
				double lower_bound = upper_bound - (range_to - range_from) * query_selectivity2(generator);
				if (lower_bound < range_from) {
					lower_bound = range_from+ 0.000001;
				}
				v1.push_back(lower_bound);
				v2.push_back(upper_bound);

			}
			else {
				double lower_bound = upper_bound - (range_to - range_from) * query_selectivity3(generator);
				if (lower_bound < range_from) {
					lower_bound = range_from+ 0.000001;
				}
				v1.push_back(lower_bound);
				v2.push_back(upper_bound);

			}
		}
		min.push_back(v1);
		max.push_back(v2);
	}
	for (int numCounter = 0; numCounter < num; ++numCounter) {
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			range_ofs << min[numCounter][dimCounter] << ",";
		}
		for (int dimCounter = 0; dimCounter < dim; ++dimCounter)
		{
			range_ofs << max[numCounter][dimCounter] << ",";
		}
		range_ofs << numCounter + 1 << "\n";
	}
	range_ofs.close();

	return;
}


int mainlkjh() {

	//GeneratorTest(2, 10000, 0, 10, 0.02, 0.04);
	//GeneratorTest(3, 10000, 0, 10, 0.02, 0.04);
	//Generator(2, 1000000, 0, 10, 0.05, 0.1);
	//Generator(3, 1000000, 0, 10, 0.05, 0.1);
	//Generator(4, 1000000, 0, 10, 0.05, 0.1);
	//Generator(5, 1000000, 0, 10, 0.05, 0.1);
	//Generator(6, 1000000, 0, 10, 0.05, 0.1);
	//Generator(7, 1000000, 0, 10, 0.05, 0.1);
	//Generator(8, 1000000, 0, 10, 0.05, 0.1);
	//Generator(9, 1000000, 0, 10, 0.05, 0.1);
	//Generator(10, 1000000, 0, 10, 0.05, 0.1);

	double selectivity[4] = { 0.1, 0.1, 0.1, 0.1 };
	//double selectivity[5] = { 0.15, 0.08, 0.08, 0.04, 0.08};
	//RangeGenerator(5, 1000, 0, 10, selectivity);
	//Generator(4, 10000000, 0, 10, 0.05, 0.1);
	//Generator(4, 1000000, 0, 10, 0.05, 0.1);
	//Generator(5, 1000000, 0, 10, 0.05, 0.1);

	//Generator(2, 1000000, 0, 10, 0.05, 0.1);
	//Generator(3, 1000000, 0, 10, 0.05, 0.1);
	//Generator(5, 1000000, 0, 10, 0.05, 0.1);
	//Generator(6, 1000000, 0, 10, 0.05, 0.1);
	//Generator(7, 1000000, 0, 10, 0.05, 0.1);
	//Generator(8, 1000000, 0, 10, 0.05, 0.1);
	//Generator(9, 1000000, 0, 10, 0.05, 0.1);
	//Generator(10, 1000000, 0, 10, 0.05, 0.1);

	//Generator(4, 100000, 0, 10, 0.05, 0.1);
	//Generator(4, 500000, 0, 10, 0.05, 0.1);
	//Generator(4, 5000000, 0, 10, 0.05, 0.1);

	Generator(4, 5000000, 0, 10, 0.05, 0.1);


	//Generator(1, 1100, 0, 10, 0.05, 0.1);

	system("pause");
	return 0;
}