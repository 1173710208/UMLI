/*
This is a analysis program for hyperparameter 'Number of Columns'.
*/

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>

#include "UMLI.h"

using namespace std;


void test_errb_uniform_1d(const char* fileName, const char* outfileName, int dim)
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

    ofstream dofs;
    dofs.open(outfileName, ios::app);

    for (double err_b = DATA_SIZE * 0.001; err_b <= DATA_SIZE*0.1; err_b = err_b+ DATA_SIZE * 0.001)
    {
        //cout << "=================================================" << endl;
        //cout << "error bound:" << err_b << endl;
        dofs << err_b << ",";

        //// Build all indexes
        //cout << "=================================================" << endl;
        //cout << "Build UMLI: " << endl;
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
            vector<double> scales = { 0,10 };
            linear_scales_UMLI.push_back(scales);
        }
        auto start_umli = high_resolution_clock::now();
        UMLI<int, double, 1> umli(id_arr, umli_build_arr, linear_scales_UMLI, err_b);
        auto stop_umli = high_resolution_clock::now();
        auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
        //cout << duration_umli.count() << " (us)" << endl;
        dofs << duration_umli.count() << ",";


        //// Point Query
        //cout << "=================================================" << endl;
        //cout << "Point Query:" << endl;
        int* pointquery_time_arr_umli = new int[POINT_QUERY_SIZE];//microseconds
        double avg_pointqueryTime_umli = 0.0;
        for (int i = 0; i < POINT_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            vector<int> Result;
            auto start = high_resolution_clock::now();
            umli.PointQuery(Result, point_query[i]);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            pointquery_time_arr_umli[i] = duration.count();
            avg_pointqueryTime_umli += duration.count();
            //sort(Result.begin(), Result.end());
            //for (int j = 0; j < Result.size(); j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
        }
        avg_pointqueryTime_umli = avg_pointqueryTime_umli / POINT_QUERY_SIZE;
        //cout << "average point query time(us):\t" << avg_pointqueryTime_umli << endl;
        dofs << avg_pointqueryTime_umli << "\n";


        delete[] pointquery_time_arr_umli;


    }
}


int mainERRB() {

    //for (int i = 0; i < 20; i++) {
    //    test_errb_uniform_1d("./Datasets/uniform_1d_100.csv", "./Results/uniform_1d_100_errb.csv", 1);
    //}

    //for (int i = 0; i < 20; i++) {
    //    test_errb_uniform_1d("./Datasets/uniform_1d_1000.csv", "./Results/uniform_1d_1000_errb_3.csv", 1); 
    //}

    //for (int i = 0; i < 1; i++) {
    //    test_errb_uniform_1d("./Datasets/uniform_1d_10000.csv", "./Results/uniform_1d_10000_errb.csv", 1);
    //}
    //for (int i = 0; i < 10; i++) {
        //test_errb_uniform_1d("./Datasets/uniform_1d_100.csv", "./Results/uniform_1d_100_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_200.csv", "./Results/uniform_1d_200_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_300.csv", "./Results/uniform_1d_300_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_400.csv", "./Results/uniform_1d_400_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_500.csv", "./Results/uniform_1d_500_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_600.csv", "./Results/uniform_1d_600_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_700.csv", "./Results/uniform_1d_700_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_800.csv", "./Results/uniform_1d_800_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_900.csv", "./Results/uniform_1d_900_errb_3.csv", 1);

    //}
    for (int i = 0; i < 10; i++) {
        //test_errb_uniform_1d("./Datasets/uniform_1d_2000.csv", "./Results/uniform_1d_2000_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_3000.csv", "./Results/uniform_1d_3000_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_4000.csv", "./Results/uniform_1d_4000_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_5000.csv", "./Results/uniform_1d_5000_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_5000.csv", "./Results/uniform_1d_6000_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_5000.csv", "./Results/uniform_1d_7000_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_5000.csv", "./Results/uniform_1d_8000_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_5000.csv", "./Results/uniform_1d_9000_errb_3.csv", 1);
        //test_errb_uniform_1d("./Datasets/uniform_1d_1000.csv", "./Results/uniform_1d_1000_errb_4.csv", 1);
    }


    system("pause");
    return 0;
}


