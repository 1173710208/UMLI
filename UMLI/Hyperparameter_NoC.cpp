/*
This is a analysis program for hyperparameter 'Number of Columns'.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>

#include "Grid.h"
#include "UMLI.h"

using namespace std;


void test_NoC_uniform_1d(const char* fileName, const char* rfileName, int dim)
{
	cout << "==============================================================================================================" << endl;
	cout << "NoC TEST" << endl;



    //// Read data to data_arr
    cout << "Read data to data_arr" << endl;
    vector<vector<double>> data_arr;
    ifstream ifs;
    string buf;
    string val;
    ifs.open(fileName, ios::in);
    if (!ifs.is_open())
    {
        cout << "error: data file open failed " << endl;
        return;
    }
    while (getline(ifs, buf))
    {
        vector<double> v;
        stringstream s(buf); // stringstream line
        while (getline(s, val, ',')) // get each value (',' delimited)
        {
            //stof() is a standard library function that turns a string into an float
            //stoi() is a standard library function that turns a string into an integer
            v.push_back(stod(val));  // add to row vector
        }
        data_arr.push_back(v);
    }
    ifs.close();
    int DATA_SIZE = data_arr.size();



    //// Generate build dataset
    cout << "Generate build dataset" << endl;
    int BUILD_SIZE = DATA_SIZE;
    vector<vector<double>> build_arr;
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        build_arr.push_back(data_arr[i]);
    }



    //// Generate point query
    cout << "Generate point query" << endl;
    int POINT_QUERY_SIZE = DATA_SIZE;
    double** point_query = new double* [POINT_QUERY_SIZE];
    for (int i = 0; i < POINT_QUERY_SIZE; ++i)
    {
        point_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            point_query[i][j] = data_arr[i][j];
        }
    }



    //// Read query to range_query
    cout << "Read query to range_query" << endl;
    vector<vector<double>> range_query;
    ifstream rifs;
    string rbuf;
    string rval;
    rifs.open(rfileName, ios::in);
    if (!rifs.is_open())
    {
        cout << "error: range query file open failed " << endl;
        return;
    }
    while (getline(rifs, rbuf))
    {
        vector<double> v;
        stringstream s(rbuf); // stringstream line
        while (getline(s, rval, ',')) // get each value (',' delimited)
        {
            //stof() is a standard library function that turns a string into an float
            //stoi() is a standard library function that turns a string into an integer
            v.push_back(stod(rval));  // add to row vector
        }
        range_query.push_back(v);
    }
    rifs.close();
    int RANGE_QUERY_SIZE = range_query.size();


    vector<vector<double>> std_scales;
    vector<double> std_scale1 = { 0, 10 }; // NoC = 1
    vector<double> std_scale2 = { 0, 5, 10 }; // NoC = 2
    vector<double> std_scale3 = { 0, 3.33, 6.66, 10 }; // NoC = 3
    vector<double> std_scale4 = { 0, 2.5, 5, 7.5, 10 }; // NoC = 4
    vector<double> std_scale5 = { 0, 2, 4, 6, 8, 10 }; // NoC = 5
    vector<double> std_scale6 = { 0, 1.67, 3.33, 5, 6.67, 8.33, 10 }; // NoC = 6
    vector<double> std_scale7 = { 0, 1.43, 2.86, 4.29, 5.71, 7.14, 8.57, 10 }; // NoC = 7
    vector<double> std_scale8 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 }; // NoC = 8
    vector<double> std_scale9 = { 0, 1.11, 2.22, 3.33, 4.44, 5.55, 6.66, 7.77, 8.88, 10 }; // NoC = 9
    vector<double> std_scale10 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 }; // NoC = 10
    vector<double> std_scale11 = { 0, 0.91, 1.82, 2.73, 3.64, 4.55, 5.46, 6.37, 7.28, 8.19, 9.09, 10 }; // NoC = 11
    vector<double> std_scale12 = { 0, 0.83, 1.67, 2.5, 3.33, 4.17, 5, 5.83, 6.67, 7.5, 8.33, 9.17, 10 }; // NoC = 12
    vector<double> std_scale13 = { 0, 0.77, 1.54, 2.30769, 3.0769, 3.84615, 4.6153, 5.3846, 6.1538, 6.923, 7.6723, 8.4615, 9.23, 10 }; // NoC = 13
    vector<double> std_scale14 = { 0, 0.7143, 1.4286, 2.143, 2.857, 3.5712, 4.2857, 5, 5.7143, 6.4286, 7.143, 7.857, 8.5712, 9.2857, 10 }; // NoC = 14
    vector<double> std_scale15 = { 0, 0.67, 1.33, 2, 2.67, 3.33, 4, 4.67, 5.33, 6, 6.67, 7.33, 8, 8.67, 9.33, 10 }; // NoC = 15
    vector<double> std_scale16 = { 0, 0.625, 1.25, 1.875, 2.5, 3.125, 3.75, 4.375, 5, 5.625, 6.25, 6.875, 7.5, 8.125, 8.75, 9.375, 10 }; // NoC = 16
    vector<double> std_scale17 = { 0, 0.588, 1.17165, 1.7647, 2.353, 2.941, 3.5294, 4.118, 4.706, 5.2941, 5.882, 6.47, 7.059, 7.647, 8.235, 8.8235, 99.4118, 10 }; // NoC = 17
    vector<double> std_scale18 = { 0, 0.5556, 1.1111, 1.66667, 2.2222, 2.7778, 3.3333, 3.888889, 4.4444, 5, 5.5556, 6.1111, 6.6667, 7.2222, 7.7778, 8.3333, 8.88889, 9.4444, 10 }; // NoC = 18
    vector<double> std_scale19 = { 0, 0.5263, 1.0526, 1.5789, 2.1053, 2.6316, 3.1579, 3.6842, 4.2105, 4.7368, 5.2632, 5.7895, 6.3158, 6.8421, 7.3684, 7.8947, 8.421, 8.9474, 9.4737, 10 }; // NoC = 19
    vector<double> std_scale20 = { 0, 0.5, 1, 1.5, 2, 2.5, 3, 3.5, 4, 4.5, 5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 10 }; // NoC = 20
    vector<double> std_scale25 = { 0, 0.4, 0.8, 1.2, 1.6, 2, 2.4, 2.8, 3.2, 3.6, 4, 4.4, 4.8, 5.2, 5.6, 6, 6.4, 6.8, 7.2, 7.6, 8, 8.4, 8.8, 9.2, 9.6, 10 }; // NoC = 25
    vector<double> std_scale30 = { 0, 0.33, 0.67, 1, 1.33, 1.67, 2, 2.33, 2.67, 3, 3.33, 3.67, 4, 4.33, 4.67, 5, 5.33, 5.67, 6, 6.33, 6.67, 7, 7.33, 7.67, 8, 8.33, 8.67, 9, 9.33, 9.67, 10 }; // NoC = 30
    vector<double> std_scale35 = { 0, 0.286, 0.5715, 0.8571, 1.1438, 1.4286, 1.7143, 2, 2.268, 2.5715, 2.8571, 3.1438, 3.4286, 3.7143, 4, 4.268, 4.5715, 4.8571, 5.1438, 5.4286, 5.7143, 6, 6.268, 6.5715, 6.8571, 7.1438, 7.4286, 7.7143, 8, 8.268, 8.5715, 8.8571, 9.1438, 9.4286, 9.7143, 10 }; // NoC = 35
    vector<double> std_scale40 = { 0, 0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2, 2.25, 2.5, 2.75, 3, 3.25, 3.5, 3.75, 4, 4.25, 4.5, 4.75, 5, 5.25, 5.5, 5.75, 6, 6.25, 6.5, 6.75, 7, 7.25, 7.5, 7.75, 8, 8.25, 8.5, 8.75, 9, 9.25, 9.5, 9.75, 10 }; // NoC = 40
    vector<double> std_scale45 = { 0, 0.222, 0.444, 0.667, 0.889, 1.111, 1.333, 1.556, 1.778, 2, 2.222, 2.444, 2.667, 2.889, 3.111, 3.333, 3.556, 3.778, 4, 4.222, 4.444, 4.666, 4.888, 5.111, 5.333, 5.556, 5.779, 6, 6.222, 6.444, 6.667, 6.889, 7.111, 7.333, 7.556, 7.778, 8, 8.222, 8.444, 8.667, 8.889, 9.111, 9.333, 9.556, 9.778, 10 }; // NoC = 45
    vector<double> std_scale50 = { 0, 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, 1.4, 1.6, 1.8, 2, 2.2, 2.4, 2.6, 2.8, 3.0, 3.2, 3.4, 3.6, 3.8, 4, 4.2, 4.4, 4.6, 4.8, 5.0, 5.2, 5.4, 5.6, 5.8, 6, 6.2, 6.4, 6.6, 6.8, 7.0, 7.2, 7.4, 7.6, 7.8, 8, 8.2, 8.4, 8.6, 8.8, 9.0, 9.2, 9.4, 9.6, 9.8, 10 }; // NoC = 50
    vector<double> std_scale60 = { 0, 0.167, 0.333, 0.5, 0.667, 0.833, 1.0,
                                      1.167, 1.333, 1.5, 1.667, 1.833, 2,
                                      2.167, 2.333, 2.5, 2.667, 2.833, 3.0,
                                      3.167, 3.333, 3.5, 3.667, 3.833, 4,
                                      4.167, 4.333, 4.5, 4.667, 4.833, 5.0,
                                      5.167, 5.333, 5.5, 5.667, 5.833, 6,
                                      6.167, 6.333, 6.5, 6.667, 6.833, 7.0,
                                      7.167, 7.333, 7.5, 7.667, 7.833, 8,
                                      8.167, 8.333, 8.5, 8.667, 8.833, 9.0,
                                      9.167, 9.333, 9.5, 9.667, 9.833, 10 }; // NoC = 60
    vector<double> std_scale70 = { 0, 0.143, 0.286, 0.429, 0.571, 0.714, 0.857, 1.0,
                                      1.143, 1.286, 1.429, 1.571, 1.714, 1.857, 2,
                                      2.143, 2.286, 2.429, 2.571, 2.714, 2.857, 3.0,
                                      3.143, 3.286, 3.429, 3.571, 3.714, 3.857, 4,
                                      4.143, 4.286, 4.429, 4.571, 4.714, 4.857, 5.0,
                                      5.143, 5.286, 5.429, 5.571, 5.714, 5.857, 6,
                                      6.143, 6.286, 6.429, 6.571, 6.714, 6.857, 7.0,
                                      7.143, 7.286, 7.429, 7.571, 7.714, 7.857, 8,
                                      8.143, 8.286, 8.429, 8.571, 8.714, 8.857, 9.0,
                                      9.143, 9.286, 9.429, 9.571, 9.714, 9.857, 10 }; // NoC = 70
    vector<double> std_scale80 = { 0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1.0,
                                      1.125, 1.25, 1.375, 1.5, 1.625, 1.75, 1.875, 2,
                                      2.125, 2.25, 2.375, 2.5, 2.625, 2.75, 2.875, 3.0,
                                      3.125, 3.25, 3.375, 3.5, 3.625, 3.75, 3.875, 4,
                                      4.125, 4.25, 4.375, 4.5, 4.625, 4.75, 4.875, 5.0,
                                      5.125, 5.25, 5.375, 5.5, 5.625, 5.75, 5.875, 6,
                                      6.125, 6.25, 6.375, 6.5, 6.625, 6.75, 6.875, 7.0,
                                      7.125, 7.25, 7.375, 7.5, 7.625, 7.75, 7.875, 8,
                                      8.125, 8.25, 8.375, 8.5, 8.625, 8.75, 8.875, 9.0,
                                      9.125, 9.25, 9.375, 9.5, 9.625, 9.75, 9.875, 10 }; // NoC = 80
    vector<double> std_scale90 = { 0, 0.111, 0.222, 0.333, 0.4444, 0.555, 0.666, 0.777, 0.888, 1.0,
                                      1.111, 1.222, 1.333, 1.444, 1.555, 1.666, 1.777, 1.888, 2,
                                      2.111, 2.222, 2.333, 2.444, 2.555, 2.666, 2.777, 2.888, 3.0,
                                      3.111, 3.222, 3.333, 3.444, 3.555, 3.666, 3.777, 3.888, 4,
                                      4.111, 4.222, 4.333, 4.444, 4.555, 4.666, 4.777, 4.888, 5.0,
                                      5.111, 5.222, 5.333, 5.444, 5.555, 5.666, 5.777, 5.888, 6,
                                      6.111, 6.222, 6.333, 6.444, 6.555, 6.666, 6.777, 6.888, 7.0,
                                      7.111, 7.222, 7.333, 7.444, 7.555, 7.666, 7.777, 7.888, 8,
                                      8.111, 8.222, 8.333, 8.444, 8.555, 8.666, 8.777, 8.888, 9.0,
                                      9.111, 9.222, 9.333, 9.444, 9.555, 9.666, 9.777, 9.888, 10 }; // NoC = 90
    vector<double> std_scale100 = { 0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1,
                                       1.1, 1.2, 1.3, 1.4, 1.5, 1.6, 1.7, 1.8, 1.9, 2,
                                       2.1, 2.2, 2.3, 2.4, 2.5, 2.6, 2.7, 2.8, 2.9, 3,
                                       3.1, 3.2, 3.3, 3.4, 3.5, 3.6, 3.7, 3.8, 3.9, 4,
                                       4.1, 4.2, 4.3, 4.4, 4.5, 4.6, 4.7, 4.8, 4.9, 5,
                                       5.1, 5.2, 5.3, 5.4, 5.5, 5.6, 5.7, 5.8, 5.9, 6,
                                       6.1, 6.2, 6.3, 6.4, 6.5, 6.6, 6.7, 6.8, 6.9, 7,
                                       7.1, 7.2, 7.3, 7.4, 7.5, 7.6, 7.7, 7.8, 7.9, 8,
                                       8.1, 8.2, 8.3, 8.4, 8.5, 8.6, 8.7, 8.8, 8.9, 9,
                                       9.1, 9.2, 9.3, 9.4, 9.5, 9.6, 9.7, 9.8, 9.9, 10 }; // NoC = 100
    std_scales.push_back(std_scale1);
    std_scales.push_back(std_scale2);
    std_scales.push_back(std_scale3);
    std_scales.push_back(std_scale4);
    std_scales.push_back(std_scale5);
    std_scales.push_back(std_scale6);
    std_scales.push_back(std_scale7);
    std_scales.push_back(std_scale8);
    std_scales.push_back(std_scale9);
    std_scales.push_back(std_scale10);
    std_scales.push_back(std_scale11);
    std_scales.push_back(std_scale12);
    std_scales.push_back(std_scale13);
    std_scales.push_back(std_scale14);
    std_scales.push_back(std_scale15);
    std_scales.push_back(std_scale16);
    std_scales.push_back(std_scale17);
    std_scales.push_back(std_scale18);
    std_scales.push_back(std_scale19);
    std_scales.push_back(std_scale20);
    std_scales.push_back(std_scale25);
    std_scales.push_back(std_scale30);
    std_scales.push_back(std_scale35);
    std_scales.push_back(std_scale40);
    std_scales.push_back(std_scale45);
    std_scales.push_back(std_scale50);
    std_scales.push_back(std_scale60);
    std_scales.push_back(std_scale70);
    std_scales.push_back(std_scale80);
    std_scales.push_back(std_scale90);
    std_scales.push_back(std_scale100);

    for (int scalei = 0; scalei < std_scales.size(); scalei++)
    {
        cout << "=================================================" << endl;
        cout << "index of NoC:" << std_scales[scalei].size()-1 << endl;

        //// Build all indexes
        //cout << "=================================================" << endl;
        //cout << "Build all indexes: " << endl;
        // Grid
        //cout << "Grid:\t\t";
        vector<vector<double>> linear_scales;
        for (int i = 0; i < dim; ++i)
        {
            linear_scales.push_back(std_scales[scalei]);
        }
        auto start_grid = high_resolution_clock::now();
        Grid<int, double, 1> grid(linear_scales);
        for (int i = 0; i < BUILD_SIZE; i++)
        {
            double* point = new double[dim];
            for (int j = 0; j < dim; ++j)
            {
                point[j] = build_arr[i][j];
            }
            int id = build_arr[i][dim];
            grid.Insert(point, id);
            delete[] point;
        }
        auto stop_grid = high_resolution_clock::now();
        auto duration_grid = duration_cast<microseconds>(stop_grid - start_grid);
        //cout << duration_grid.count() << " (us)" << endl;
        // UMLI
        //cout << "UMLI:\t\t";
        vector<int> id_arr;
        vector<vector<double>> umli_build_arr;
        sort(build_arr.begin(), build_arr.end());
        for (int i = 0; i < BUILD_SIZE; ++i)
        {
            id_arr.push_back(build_arr[i][dim]);
            vector<double> point;
            for (int j = 0; j < dim; ++j)
            {
                point.push_back(build_arr[i][j]);
            }
            umli_build_arr.push_back(point);
        }
        vector<vector<double>> linear_scales_UMLI;
        for (int i = 0; i < dim; ++i)
        {
            linear_scales_UMLI.push_back(std_scales[scalei]);
        }
        auto start_umli = high_resolution_clock::now();
        UMLI<int, double, 1> umli(id_arr, umli_build_arr, linear_scales_UMLI, 15);
        auto stop_umli = high_resolution_clock::now();
        auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
        //cout << duration_umli.count() << " (us)" << endl;

        ////// Point Query
        //cout << "=================================================" << endl;
        //cout << "Point Query:" << endl;
        //cout << "-------------------------------------------------" << endl;
        ////cout << "Grid:" << endl;
        //int* pointquery_time_arr_grid = new int[POINT_QUERY_SIZE];//microseconds
        //double avg_pointqueryTime_grid = 0.0;
        //for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //    //cout << "Query" << i + 1 << ": \t";
        //    vector<int> Result;
        //    auto start = high_resolution_clock::now();
        //    grid.PointQuery(Result, point_query[i]);
        //    auto stop = high_resolution_clock::now();
        //    auto duration = duration_cast<microseconds>(stop - start);
        //    pointquery_time_arr_grid[i] = duration.count();
        //    avg_pointqueryTime_grid += duration.count();
        //    //sort(Result.begin(), Result.end());
        //    //for (int j = 0; j < Result.size(); j++) {
        //    //    cout << Result[j] << "\t";
        //    //}
        //    //cout << endl;
        //}
        //avg_pointqueryTime_grid = avg_pointqueryTime_grid / POINT_QUERY_SIZE;
        ////cout << "average point query time(us):\t" << avg_pointqueryTime_grid << endl;
        //delete[] pointquery_time_arr_grid;
        //cout << "-------------------------------------------------" << endl;
        //cout << "UMLI:" << endl;
        //int* pointquery_time_arr_umli = new int[POINT_QUERY_SIZE];//microseconds
        //double avg_pointqueryTime_umli = 0.0;
        //for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //    //cout << "Query" << i + 1 << ": \t";
        //    vector<int> Result;
        //    auto start = high_resolution_clock::now();
        //    umli.PointQuery(Result, point_query[i]);
        //    auto stop = high_resolution_clock::now();
        //    auto duration = duration_cast<microseconds>(stop - start);
        //    pointquery_time_arr_umli[i] = duration.count();
        //    avg_pointqueryTime_umli += duration.count();
        //    //sort(Result.begin(), Result.end());
        //    //for (int j = 0; j < Result.size(); j++) {
        //    //    cout << Result[j] << "\t";
        //    //}
        //    //cout << endl;
        //}
        //avg_pointqueryTime_umli = avg_pointqueryTime_umli / POINT_QUERY_SIZE;
        //cout << "average point query time(us):\t" << avg_pointqueryTime_umli << endl;
        //delete[] pointquery_time_arr_umli;

        //// Range Query
        cout << "=================================================" << endl;
        cout << "Range Query:" << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "Grid:" << endl;
        int* rangequery_time_arr_grid = new int[RANGE_QUERY_SIZE];//microseconds
        double avg_rangequeryTime_grid = 0.0;
        for (int i = 0; i < RANGE_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            vector<int> Result;
            double* min = new double[dim];
            double* max = new double[dim];
            for (int j = 0; j < dim; j++) {
                min[j] = range_query[i][j];
                max[j] = range_query[i][j + dim];
            }
            auto start = high_resolution_clock::now();
            grid.RangeQuery(Result, min, max);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            rangequery_time_arr_grid[i] = duration.count();
            avg_rangequeryTime_grid += duration.count();
            //sort(Result.begin(), Result.end());
            //for (int j = 0; j < Result.size(); j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] min;
            delete[] max;
        }
        avg_rangequeryTime_grid = avg_rangequeryTime_grid / RANGE_QUERY_SIZE;
        cout << "average range query time(us):\t" << avg_rangequeryTime_grid << endl;
        delete[] rangequery_time_arr_grid;
        cout << "-------------------------------------------------" << endl;
        //cout << "UMLI:" << endl;
        int* rangequery_time_arr_umli = new int[RANGE_QUERY_SIZE];//microseconds
        double avg_rangequeryTime_umli = 0.0;
        for (int i = 0; i < RANGE_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            vector<int> Result;
            double* min = new double[dim];
            double* max = new double[dim];
            for (int j = 0; j < dim; j++) {
                min[j] = range_query[i][j];
                max[j] = range_query[i][j + dim];
            }
            auto start = high_resolution_clock::now();
            umli.RangeQuery(Result, min, max);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            rangequery_time_arr_umli[i] = duration.count();
            avg_rangequeryTime_umli += duration.count();
            //sort(Result.begin(), Result.end());
            //for (int j = 0; j < Result.size(); j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] min;
            delete[] max;
        }
        avg_rangequeryTime_umli = avg_rangequeryTime_umli / RANGE_QUERY_SIZE;
        //cout << "average range query time(us):\t" << avg_rangequeryTime_umli << endl;
        delete[] rangequery_time_arr_umli;

        cout << "==============================================================================================================" << endl;
        cout << endl;
    }
}


int mainNOC() {

    //test_NoC_uniform_1d("./Datasets/uniform_1d_1000.csv", "./Datasets/1d_selectivity0.100000_1000_range.csv", 1);
    //test_NoC_uniform_1d("./Datasets/uniform_1d_10000.csv", "./Datasets/1d_selectivity0.100000_1000_range.csv", 1);
    //test_NoC_uniform_1d("./Datasets/uniform_1d_100000.csv", "./Datasets/1d_selectivity0.100000_1000_range.csv", 1);
    //test_NoC_uniform_1d("./Datasets/uniform_1d_1000.csv", "./Datasets/1d_selectivity0.200000_1000_range.csv", 1);
    //test_NoC_uniform_1d("./Datasets/uniform_1d_10000.csv", "./Datasets/1d_selectivity0.200000_1000_range.csv", 1);
    //test_NoC_uniform_1d("./Datasets/uniform_1d_100000.csv", "./Datasets/1d_selectivity0.200000_1000_range.csv", 1);
    //test_NoC_uniform_1d("./Datasets/uniform_1d_1000000.csv", "./Datasets/1d_selectivity0.100000_1000_range.csv", 1);

    //test_NoC_uniform_1d("./Datasets/uniform_1d_5000.csv", "./Datasets/1d_selectivity0.010000_1000_range.csv", 1);
    //test_NoC_uniform_1d("./Datasets/uniform_1d_2000.csv", "./Datasets/1d_selectivity0.010000_1000_range.csv", 1);
    //test_NoC_uniform_1d("./Datasets/uniform_1d_1000.csv", "./Datasets/1d_selectivity0.050000_1000_range.csv", 1);


    //// Relationship between selectivity * NoC and range query performance
    test_NoC_uniform_1d("./Datasets/uniform_1d_10000.csv", "./Datasets/1d_selectivity0.050000_1000_range.csv", 1);
    test_NoC_uniform_1d("./Datasets/uniform_1d_10000.csv", "./Datasets/1d_selectivity0.100000_1000_range.csv", 1);
    test_NoC_uniform_1d("./Datasets/uniform_1d_10000.csv", "./Datasets/1d_selectivity0.150000_1000_range.csv", 1);
    test_NoC_uniform_1d("./Datasets/uniform_1d_10000.csv", "./Datasets/1d_selectivity0.200000_1000_range.csv", 1);


	system("pause");
	return 0;
}
