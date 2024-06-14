#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <chrono>
#include <algorithm>

#include "FullScan.h"
#include "KDTree.h"
#include "RTree.h"
#include "Grid.h"
#include "UMLI.h"


void a1test_uniform_4d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "4D TEST" << endl;

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
    int BUILD_SIZE = 999000;
    vector<vector<double>> build_arr;
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        build_arr.push_back(data_arr[i]);
    }


    //// Generate point & KNN query
    cout << "Generate point & KNN query" << endl;
    int POINT_QUERY_SIZE = 1000;
    int KNN_QUERY_SIZE = 10;
    double** point_query = new double* [POINT_QUERY_SIZE];
    double** knn_query = new double* [KNN_QUERY_SIZE];
    for (int i = 0; i < POINT_QUERY_SIZE; ++i)
    {
        point_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            point_query[i][j] = data_arr[i * 998][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i * 998][j];
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


    //// Build all indexes
    cout << "=================================================" << endl;
    cout << "Build all indexes: " << endl;
    
    vector<vector<double>> linear_scales;
    //vector<double> scale1 = { 0,2.5,5,7.5,10 };
    vector<double> scale1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    vector<double> scale2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    vector<double> scale3 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    vector<double> scale4 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    vector<double> lscale1 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale2 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale3 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale4 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    
    // UMLI1
    cout << "UMLI1 with optimized grid:\t\t";
    vector<int> id_arr1;
    vector<vector<double>> umli_build_arr1;
    sort(build_arr.begin(), build_arr.end());
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        id_arr1.push_back(build_arr[i][dim]);
        vector<double> point;
        for (int j = 0; j < dim; ++j)
        {
            point.push_back(build_arr[i][j]);
        }
        umli_build_arr1.push_back(point);
    }
    vector<vector<double>> linear_scales_UMLI;
    linear_scales_UMLI.push_back(scale1);
    linear_scales_UMLI.push_back(scale2);
    linear_scales_UMLI.push_back(scale3);
    linear_scales_UMLI.push_back(scale4);
    UMLI<int, double, 4> umli1(id_arr1, umli_build_arr1, linear_scales_UMLI, 6);
    // UMLI2
    cout << "UMLI2:\t\t";
    vector<int> id_arr2;
    vector<vector<double>> umli_build_arr2;
    sort(build_arr.begin(), build_arr.end());
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        id_arr2.push_back(build_arr[i][dim]);
        vector<double> point;
        for (int j = 0; j < dim; ++j)
        {
            point.push_back(build_arr[i][j]);
        }
        umli_build_arr2.push_back(point);
    }
    vector<vector<double>> linear_scales_UMLI2;
    linear_scales_UMLI2.push_back(lscale1);
    linear_scales_UMLI2.push_back(lscale2);
    linear_scales_UMLI2.push_back(lscale3);
    linear_scales_UMLI2.push_back(lscale4);
    UMLI<int, double, 4> umli2(id_arr2, umli_build_arr2, linear_scales_UMLI2, 6);





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    int* pointquery_time_arr_umli1 = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_umli1 = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        umli1.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_umli1[i] = duration.count();
        avg_pointqueryTime_umli1 += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_umli1 = avg_pointqueryTime_umli1 / POINT_QUERY_SIZE;
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli1 << endl;
    delete[] pointquery_time_arr_umli1;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    int* pointquery_time_arr_umli2 = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_umli2 = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        umli2.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_umli2[i] = duration.count();
        avg_pointqueryTime_umli2 += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_umli2 = avg_pointqueryTime_umli2 / POINT_QUERY_SIZE;
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli2 << endl;
    delete[] pointquery_time_arr_umli2;


    //// Range Query
    cout << "=================================================" << endl;
    cout << "Range Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    int* rangequery_time_arr_umli1 = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_umli1 = 0.0;
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
        umli1.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_umli1[i] = duration.count();
        avg_rangequeryTime_umli1 += duration.count();
        //sort(Result.begin(), Result.end());
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli1 = avg_rangequeryTime_umli1 / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli1 << endl;
    delete[] rangequery_time_arr_umli1;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    int* rangequery_time_arr_umli2 = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_umli2 = 0.0;
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
        umli2.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_umli2[i] = duration.count();
        avg_rangequeryTime_umli2 += duration.count();
        //sort(Result.begin(), Result.end());
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli2 = avg_rangequeryTime_umli2 / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli2 << endl;
    delete[] rangequery_time_arr_umli2;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli1 = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli1 = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli1.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli1[i] = duration.count();
            avg_knnqueryTime_umli1 += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli1 = avg_knnqueryTime_umli1 / KNN_QUERY_SIZE;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli1 << endl;
        delete[] knnquery_time_arr_umli1;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli2 = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli2 = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli2.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli2[i] = duration.count();
            avg_knnqueryTime_umli2 += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli2 = avg_knnqueryTime_umli2 / KNN_QUERY_SIZE;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli2 << endl;
        delete[] knnquery_time_arr_umli2;
    }








    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void a1test_exponential_4d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "4D TEST" << endl;

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
    int BUILD_SIZE = 999000;
    vector<vector<double>> build_arr;
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        build_arr.push_back(data_arr[i]);
    }


    //// Generate point & KNN query
    cout << "Generate point & KNN query" << endl;
    int POINT_QUERY_SIZE = 1000;
    int KNN_QUERY_SIZE = 10;
    double** point_query = new double* [POINT_QUERY_SIZE];
    double** knn_query = new double* [KNN_QUERY_SIZE];
    for (int i = 0; i < POINT_QUERY_SIZE; ++i)
    {
        point_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            point_query[i][j] = data_arr[i * 998][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i * 998][j];
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


    //// Build all indexes
    cout << "=================================================" << endl;
    cout << "Build all indexes: " << endl;

    vector<double> lscale1 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale2 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale3 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale4 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    //vector<double> lscale5 = { 0,2,4,6,8,10 };
    vector<double> scale1 = { 0,0.2,0.444,0.69,1.0,1.35,1.81,2.38,3.14,4.46,10 };
    //vector<double> scale1 = { 0, 4.15, 4.7755, 5.27, 5.842, 10 };
    vector<double> scale2 = { 0,0.2,0.444,0.69,1.0,1.35,1.81,2.38,3.14,4.46,10 };
    vector<double> scale3 = { 0,0.2,0.444,0.69,1.0,1.35,1.81,2.38,3.14,4.46,10 };
    vector<double> scale4 = { 0,0.2,0.444,0.69,1.0,1.35,1.81,2.38,3.14,4.46,10 };
    //vector<double> scale4 = { 0, 3.35, 3.7, 3.95, 4.15, 4.345, 4.489, 4.6496, 4.7755, 4.8979, 5.0, 5.12, 5.27, 5.39, 5.532, 5.6811, 5.842, 6.04, 6.278, 6.638, 10 };
    //vector<double> scale5 = { 0, 3.7, 4.15, 4.489, 4.7755, 5.0, 5.27, 5.532, 5.842, 6.278, 10 };

    // UMLI1
    cout << "UMLI1 with optimized grid:\t\t";
    vector<int> id_arr1;
    vector<vector<double>> umli_build_arr1;
    sort(build_arr.begin(), build_arr.end());
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        id_arr1.push_back(build_arr[i][dim]);
        vector<double> point;
        for (int j = 0; j < dim; ++j)
        {
            point.push_back(build_arr[i][j]);
        }
        umli_build_arr1.push_back(point);
    }
    vector<vector<double>> linear_scales_UMLI;
    linear_scales_UMLI.push_back(scale1);
    linear_scales_UMLI.push_back(scale2);
    linear_scales_UMLI.push_back(scale3);
    linear_scales_UMLI.push_back(scale4);
    UMLI<int, double, 4> umli1(id_arr1, umli_build_arr1, linear_scales_UMLI, 6);
    // UMLI2
    cout << "UMLI2:\t\t";
    vector<int> id_arr2;
    vector<vector<double>> umli_build_arr2;
    sort(build_arr.begin(), build_arr.end());
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        id_arr2.push_back(build_arr[i][dim]);
        vector<double> point;
        for (int j = 0; j < dim; ++j)
        {
            point.push_back(build_arr[i][j]);
        }
        umli_build_arr2.push_back(point);
    }
    vector<vector<double>> linear_scales_UMLI2;
    linear_scales_UMLI2.push_back(lscale1);
    linear_scales_UMLI2.push_back(lscale2);
    linear_scales_UMLI2.push_back(lscale3);
    linear_scales_UMLI2.push_back(lscale4);
    UMLI<int, double, 4> umli2(id_arr2, umli_build_arr2, linear_scales_UMLI2, 6);





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    int* pointquery_time_arr_umli1 = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_umli1 = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        umli1.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_umli1[i] = duration.count();
        avg_pointqueryTime_umli1 += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_umli1 = avg_pointqueryTime_umli1 / POINT_QUERY_SIZE;
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli1 << endl;
    delete[] pointquery_time_arr_umli1;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    int* pointquery_time_arr_umli2 = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_umli2 = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        umli2.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_umli2[i] = duration.count();
        avg_pointqueryTime_umli2 += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_umli2 = avg_pointqueryTime_umli2 / POINT_QUERY_SIZE;
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli2 << endl;
    delete[] pointquery_time_arr_umli2;


    //// Range Query
    cout << "=================================================" << endl;
    cout << "Range Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    int* rangequery_time_arr_umli1 = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_umli1 = 0.0;
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
        umli1.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_umli1[i] = duration.count();
        avg_rangequeryTime_umli1 += duration.count();
        //sort(Result.begin(), Result.end());
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli1 = avg_rangequeryTime_umli1 / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli1 << endl;
    delete[] rangequery_time_arr_umli1;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    int* rangequery_time_arr_umli2 = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_umli2 = 0.0;
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
        umli2.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_umli2[i] = duration.count();
        avg_rangequeryTime_umli2 += duration.count();
        //sort(Result.begin(), Result.end());
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli2 = avg_rangequeryTime_umli2 / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli2 << endl;
    delete[] rangequery_time_arr_umli2;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli1 = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli1 = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli1.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli1[i] = duration.count();
            avg_knnqueryTime_umli1 += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli1 = avg_knnqueryTime_umli1 / KNN_QUERY_SIZE;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli1 << endl;
        delete[] knnquery_time_arr_umli1;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli2 = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli2 = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli2.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli2[i] = duration.count();
            avg_knnqueryTime_umli2 += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli2 = avg_knnqueryTime_umli2 / KNN_QUERY_SIZE;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli2 << endl;
        delete[] knnquery_time_arr_umli2;
    }



    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void a1test_gamma_4d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "4D TEST" << endl;

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
    int BUILD_SIZE = 999000;
    vector<vector<double>> build_arr;
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        build_arr.push_back(data_arr[i]);
    }


    //// Generate point & KNN query
    cout << "Generate point & KNN query" << endl;
    int POINT_QUERY_SIZE = 1000;
    int KNN_QUERY_SIZE = 10;
    double** point_query = new double* [POINT_QUERY_SIZE];
    double** knn_query = new double* [KNN_QUERY_SIZE];
    for (int i = 0; i < POINT_QUERY_SIZE; ++i)
    {
        point_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            point_query[i][j] = data_arr[i * 998][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i * 998][j];
        }
    }

    //// Generate insert dataset
    cout << "Generate insert dataset" << endl;
    int INSERT_SIZE = 1000;
    vector<vector<double>> insert_arr;
    for (int i = 999000; i < 1000000; ++i)
    {
        insert_arr.push_back(data_arr[i]);
    }


    //// Generate delete dataset
    cout << "Generate delete dataset" << endl;
    int DELETE_SIZE = 100;
    vector<vector<double>> delete_arr;
    for (int i = 500000; i < 500100; ++i)
    {
        delete_arr.push_back(data_arr[i]);
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


    //// Build all indexes
    cout << "=================================================" << endl;
    cout << "Build all indexes: " << endl;
    
    vector<double> lscale1 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale2 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale3 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale4 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    //vector<double> lscale5 = { 0,2,4,6,8,10 };
    vector<double> scale1 = { 0, 6.64, 7.42, 7.935, 8.3545, 8.735, 9.0376, 9.3027, 9.562, 9.79, 10 };
    vector<double> scale2 = { 0, 6.64, 7.42, 7.935, 8.3545, 8.735, 9.0376, 9.3027, 9.562, 9.79, 10 };
    vector<double> scale3 = { 0, 6.64, 7.42, 7.935, 8.3545, 8.735, 9.0376, 9.3027, 9.562, 9.79, 10 };
    vector<double> scale4 = { 0, 6.64, 7.42, 7.935, 8.3545, 8.735, 9.0376, 9.3027, 9.562, 9.79, 10 };
    //vector<double> scale4 = { 0, 3.35, 3.7, 3.95, 4.15, 4.345, 4.489, 4.6496, 4.7755, 4.8979, 5.0, 5.12, 5.27, 5.39, 5.532, 5.6811, 5.842, 6.04, 6.278, 6.638, 10 };
    //vector<double> scale5 = { 0, 3.7, 4.15, 4.489, 4.7755, 5.0, 5.27, 5.532, 5.842, 6.278, 10 };

    // UMLI1
    cout << "UMLI1 with optimized grid:\t\t";
    vector<int> id_arr1;
    vector<vector<double>> umli_build_arr1;
    sort(build_arr.begin(), build_arr.end());
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        id_arr1.push_back(build_arr[i][dim]);
        vector<double> point;
        for (int j = 0; j < dim; ++j)
        {
            point.push_back(build_arr[i][j]);
        }
        umli_build_arr1.push_back(point);
    }
    vector<vector<double>> linear_scales_UMLI;
    linear_scales_UMLI.push_back(scale1);
    linear_scales_UMLI.push_back(scale2);
    linear_scales_UMLI.push_back(scale3);
    linear_scales_UMLI.push_back(scale4);
    UMLI<int, double, 4> umli1(id_arr1, umli_build_arr1, linear_scales_UMLI, 6);
    // UMLI2
    cout << "UMLI2:\t\t";
    vector<int> id_arr2;
    vector<vector<double>> umli_build_arr2;
    sort(build_arr.begin(), build_arr.end());
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        id_arr2.push_back(build_arr[i][dim]);
        vector<double> point;
        for (int j = 0; j < dim; ++j)
        {
            point.push_back(build_arr[i][j]);
        }
        umli_build_arr2.push_back(point);
    }
    vector<vector<double>> linear_scales_UMLI2;
    linear_scales_UMLI2.push_back(lscale1);
    linear_scales_UMLI2.push_back(lscale2);
    linear_scales_UMLI2.push_back(lscale3);
    linear_scales_UMLI2.push_back(lscale4);
    UMLI<int, double, 4> umli2(id_arr2, umli_build_arr2, linear_scales_UMLI2, 6);





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    int* pointquery_time_arr_umli1 = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_umli1 = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        umli1.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_umli1[i] = duration.count();
        avg_pointqueryTime_umli1 += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_umli1 = avg_pointqueryTime_umli1 / POINT_QUERY_SIZE;
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli1 << endl;
    delete[] pointquery_time_arr_umli1;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    int* pointquery_time_arr_umli2 = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_umli2 = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        umli2.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_umli2[i] = duration.count();
        avg_pointqueryTime_umli2 += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_umli2 = avg_pointqueryTime_umli2 / POINT_QUERY_SIZE;
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli2 << endl;
    delete[] pointquery_time_arr_umli2;


    //// Range Query
    cout << "=================================================" << endl;
    cout << "Range Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    int* rangequery_time_arr_umli1 = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_umli1 = 0.0;
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
        umli1.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_umli1[i] = duration.count();
        avg_rangequeryTime_umli1 += duration.count();
        //sort(Result.begin(), Result.end());
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli1 = avg_rangequeryTime_umli1 / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli1 << endl;
    delete[] rangequery_time_arr_umli1;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    int* rangequery_time_arr_umli2 = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_umli2 = 0.0;
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
        umli2.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_umli2[i] = duration.count();
        avg_rangequeryTime_umli2 += duration.count();
        //sort(Result.begin(), Result.end());
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli2 = avg_rangequeryTime_umli2 / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli2 << endl;
    delete[] rangequery_time_arr_umli2;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli1 = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli1 = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli1.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli1[i] = duration.count();
            avg_knnqueryTime_umli1 += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli1 = avg_knnqueryTime_umli1 / KNN_QUERY_SIZE;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli1 << endl;
        delete[] knnquery_time_arr_umli1;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli2 = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli2 = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli2.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli2[i] = duration.count();
            avg_knnqueryTime_umli2 += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli2 = avg_knnqueryTime_umli2 / KNN_QUERY_SIZE;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli2 << endl;
        delete[] knnquery_time_arr_umli2;
    }


    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void a1test_normal_4d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "4D TEST" << endl;

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
    int BUILD_SIZE = 999000;
    vector<vector<double>> build_arr;
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        build_arr.push_back(data_arr[i]);
    }


    //// Generate point & KNN query
    cout << "Generate point & KNN query" << endl;
    int POINT_QUERY_SIZE = 1000;
    int KNN_QUERY_SIZE = 10;
    double** point_query = new double* [POINT_QUERY_SIZE];
    double** knn_query = new double* [KNN_QUERY_SIZE];
    for (int i = 0; i < POINT_QUERY_SIZE; ++i)
    {
        point_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            point_query[i][j] = data_arr[i * 998][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i * 998][j];
        }
    }

    //// Generate insert dataset
    cout << "Generate insert dataset" << endl;
    int INSERT_SIZE = 1000;
    vector<vector<double>> insert_arr;
    for (int i = 999000; i < 1000000; ++i)
    {
        insert_arr.push_back(data_arr[i]);
    }


    //// Generate delete dataset
    cout << "Generate delete dataset" << endl;
    int DELETE_SIZE = 100;
    vector<vector<double>> delete_arr;
    for (int i = 500000; i < 500100; ++i)
    {
        delete_arr.push_back(data_arr[i]);
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


    //// Build all indexes
    cout << "=================================================" << endl;
    cout << "Build all indexes: " << endl;
    
    //vector<double> scale1 = { 0,2.5,5,7.5,10 };
    vector<double> lscale1 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale2 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale3 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale4 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    //vector<double> lscale5 = { 0,2,4,6,8,10 };
    vector<double> scale1 = { 0, 3.7, 4.15, 4.489, 4.7755, 5.0, 5.27, 5.532, 5.842, 6.278, 10 };
    //vector<double> scale1 = { 0, 4.15, 4.7755, 5.27, 5.842, 10 };
    vector<double> scale2 = { 0, 3.7, 4.15, 4.489, 4.7755, 5.0, 5.27, 5.532, 5.842, 6.278, 10 };
    vector<double> scale3 = { 0, 3.7, 4.15, 4.489, 4.7755, 5.0, 5.27, 5.532, 5.842, 6.278, 10 };
    vector<double> scale4 = { 0, 3.7, 4.15, 4.489, 4.7755, 5.0, 5.27, 5.532, 5.842, 6.278, 10 };
    //vector<double> scale4 = { 0, 3.35, 3.7, 3.95, 4.15, 4.345, 4.489, 4.6496, 4.7755, 4.8979, 5.0, 5.12, 5.27, 5.39, 5.532, 5.6811, 5.842, 6.04, 6.278, 6.638, 10 };
    //vector<double> scale5 = { 0, 3.7, 4.15, 4.489, 4.7755, 5.0, 5.27, 5.532, 5.842, 6.278, 10 };

    // UMLI1
    cout << "UMLI1 with optimized grid:\t\t";
    vector<int> id_arr1;
    vector<vector<double>> umli_build_arr1;
    sort(build_arr.begin(), build_arr.end());
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        id_arr1.push_back(build_arr[i][dim]);
        vector<double> point;
        for (int j = 0; j < dim; ++j)
        {
            point.push_back(build_arr[i][j]);
        }
        umli_build_arr1.push_back(point);
    }
    vector<vector<double>> linear_scales_UMLI;
    linear_scales_UMLI.push_back(scale1);
    linear_scales_UMLI.push_back(scale2);
    linear_scales_UMLI.push_back(scale3);
    linear_scales_UMLI.push_back(scale4);
    UMLI<int, double, 4> umli1(id_arr1, umli_build_arr1, linear_scales_UMLI, 6);
    // UMLI2
    cout << "UMLI2:\t\t";
    vector<int> id_arr2;
    vector<vector<double>> umli_build_arr2;
    sort(build_arr.begin(), build_arr.end());
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        id_arr2.push_back(build_arr[i][dim]);
        vector<double> point;
        for (int j = 0; j < dim; ++j)
        {
            point.push_back(build_arr[i][j]);
        }
        umli_build_arr2.push_back(point);
    }
    vector<vector<double>> linear_scales_UMLI2;
    linear_scales_UMLI2.push_back(lscale1);
    linear_scales_UMLI2.push_back(lscale2);
    linear_scales_UMLI2.push_back(lscale3);
    linear_scales_UMLI2.push_back(lscale4);
    UMLI<int, double, 4> umli2(id_arr2, umli_build_arr2, linear_scales_UMLI2, 6);





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    int* pointquery_time_arr_umli1 = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_umli1 = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        umli1.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_umli1[i] = duration.count();
        avg_pointqueryTime_umli1 += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_umli1 = avg_pointqueryTime_umli1 / POINT_QUERY_SIZE;
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli1 << endl;
    delete[] pointquery_time_arr_umli1;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    int* pointquery_time_arr_umli2 = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_umli2 = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        umli2.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_umli2[i] = duration.count();
        avg_pointqueryTime_umli2 += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_umli2 = avg_pointqueryTime_umli2 / POINT_QUERY_SIZE;
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli2 << endl;
    delete[] pointquery_time_arr_umli2;


    //// Range Query
    cout << "=================================================" << endl;
    cout << "Range Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    int* rangequery_time_arr_umli1 = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_umli1 = 0.0;
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
        umli1.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_umli1[i] = duration.count();
        avg_rangequeryTime_umli1 += duration.count();
        //sort(Result.begin(), Result.end());
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli1 = avg_rangequeryTime_umli1 / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli1 << endl;
    delete[] rangequery_time_arr_umli1;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    int* rangequery_time_arr_umli2 = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_umli2 = 0.0;
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
        umli2.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_umli2[i] = duration.count();
        avg_rangequeryTime_umli2 += duration.count();
        //sort(Result.begin(), Result.end());
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli2 = avg_rangequeryTime_umli2 / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli2 << endl;
    delete[] rangequery_time_arr_umli2;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli1 = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli1 = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli1.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli1[i] = duration.count();
            avg_knnqueryTime_umli1 += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli1 = avg_knnqueryTime_umli1 / KNN_QUERY_SIZE;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli1 << endl;
        delete[] knnquery_time_arr_umli1;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli2 = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli2 = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli2.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli2[i] = duration.count();
            avg_knnqueryTime_umli2 += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli2 = avg_knnqueryTime_umli2 / KNN_QUERY_SIZE;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli2 << endl;
        delete[] knnquery_time_arr_umli2;
    }



    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void a1test_chisquare_4d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "4D TEST" << endl;

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
    int BUILD_SIZE = 999000;
    vector<vector<double>> build_arr;
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        build_arr.push_back(data_arr[i]);
    }


    //// Generate point & KNN query
    cout << "Generate point & KNN query" << endl;
    int POINT_QUERY_SIZE = 1000;
    int KNN_QUERY_SIZE = 10;
    double** point_query = new double* [POINT_QUERY_SIZE];
    double** knn_query = new double* [KNN_QUERY_SIZE];
    for (int i = 0; i < POINT_QUERY_SIZE; ++i)
    {
        point_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            point_query[i][j] = data_arr[i * 998][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i * 998][j];
        }
    }

    //// Generate insert dataset
    cout << "Generate insert dataset" << endl;
    int INSERT_SIZE = 1000;
    vector<vector<double>> insert_arr;
    for (int i = 999000; i < 1000000; ++i)
    {
        insert_arr.push_back(data_arr[i]);
    }


    //// Generate delete dataset
    cout << "Generate delete dataset" << endl;
    int DELETE_SIZE = 100;
    vector<vector<double>> delete_arr;
    for (int i = 500000; i < 500100; ++i)
    {
        delete_arr.push_back(data_arr[i]);
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


    //// Build all indexes
    cout << "=================================================" << endl;
    cout << "Build all indexes: " << endl;
    
    vector<double> lscale1 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale2 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale3 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale4 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    //vector<double> lscale5 = { 0,2,4,6,8,10 };
    vector<double> scale1 = { 0, 3.61, 4.5879, 5.3341, 6.01, 6.63, 7.233, 7.876, 8.54, 9.244, 10 };
    vector<double> scale2 = { 0, 3.61, 4.5879, 5.3341, 6.01, 6.63, 7.233, 7.876, 8.54, 9.244, 10 };
    vector<double> scale3 = { 0, 3.61, 4.5879, 5.3341, 6.01, 6.63, 7.233, 7.876, 8.54, 9.244, 10 };
    vector<double> scale4 = { 0, 3.61, 4.5879, 5.3341, 6.01, 6.63, 7.233, 7.876, 8.54, 9.244, 10 };
    //vector<double> scale4 = { 0, 3.35, 3.7, 3.95, 4.15, 4.345, 4.489, 4.6496, 4.7755, 4.8979, 5.0, 5.12, 5.27, 5.39, 5.532, 5.6811, 5.842, 6.04, 6.278, 6.638, 10 };
    //vector<double> scale5 = { 0, 3.7, 4.15, 4.489, 4.7755, 5.0, 5.27, 5.532, 5.842, 6.278, 10 };

    // UMLI1
    cout << "UMLI1 with optimized grid:\t\t";
    vector<int> id_arr1;
    vector<vector<double>> umli_build_arr1;
    sort(build_arr.begin(), build_arr.end());
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        id_arr1.push_back(build_arr[i][dim]);
        vector<double> point;
        for (int j = 0; j < dim; ++j)
        {
            point.push_back(build_arr[i][j]);
        }
        umli_build_arr1.push_back(point);
    }
    vector<vector<double>> linear_scales_UMLI;
    linear_scales_UMLI.push_back(scale1);
    linear_scales_UMLI.push_back(scale2);
    linear_scales_UMLI.push_back(scale3);
    linear_scales_UMLI.push_back(scale4);
    UMLI<int, double, 4> umli1(id_arr1, umli_build_arr1, linear_scales_UMLI, 6);
    // UMLI2
    cout << "UMLI2:\t\t";
    vector<int> id_arr2;
    vector<vector<double>> umli_build_arr2;
    sort(build_arr.begin(), build_arr.end());
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        id_arr2.push_back(build_arr[i][dim]);
        vector<double> point;
        for (int j = 0; j < dim; ++j)
        {
            point.push_back(build_arr[i][j]);
        }
        umli_build_arr2.push_back(point);
    }
    vector<vector<double>> linear_scales_UMLI2;
    linear_scales_UMLI2.push_back(lscale1);
    linear_scales_UMLI2.push_back(lscale2);
    linear_scales_UMLI2.push_back(lscale3);
    linear_scales_UMLI2.push_back(lscale4);
    UMLI<int, double, 4> umli2(id_arr2, umli_build_arr2, linear_scales_UMLI2, 6);





    ////// Point Query
    //cout << "=================================================" << endl;
    //cout << "Point Query:" << endl;
    //cout << "-------------------------------------------------" << endl;
    //cout << "UMLI1:" << endl;
    //int* pointquery_time_arr_umli1 = new int[POINT_QUERY_SIZE];//microseconds
    //double avg_pointqueryTime_umli1 = 0.0;
    //for (int i = 0; i < POINT_QUERY_SIZE; i++) {
    //    //cout << "Query" << i + 1 << ": \t";
    //    vector<int> Result;
    //    auto start = high_resolution_clock::now();
    //    umli1.PointQuery(Result, point_query[i]);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    pointquery_time_arr_umli1[i] = duration.count();
    //    avg_pointqueryTime_umli1 += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //}
    //avg_pointqueryTime_umli1 = avg_pointqueryTime_umli1 / POINT_QUERY_SIZE;
    //cout << "average point query time(us):\t" << avg_pointqueryTime_umli1 << endl;
    //delete[] pointquery_time_arr_umli1;
    //cout << "-------------------------------------------------" << endl;
    //cout << "UMLI2:" << endl;
    //int* pointquery_time_arr_umli2 = new int[POINT_QUERY_SIZE];//microseconds
    //double avg_pointqueryTime_umli2 = 0.0;
    //for (int i = 0; i < POINT_QUERY_SIZE; i++) {
    //    //cout << "Query" << i + 1 << ": \t";
    //    vector<int> Result;
    //    auto start = high_resolution_clock::now();
    //    umli2.PointQuery(Result, point_query[i]);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    pointquery_time_arr_umli2[i] = duration.count();
    //    avg_pointqueryTime_umli2 += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //}
    //avg_pointqueryTime_umli2 = avg_pointqueryTime_umli2 / POINT_QUERY_SIZE;
    //cout << "average point query time(us):\t" << avg_pointqueryTime_umli2 << endl;
    //delete[] pointquery_time_arr_umli2;


    ////// Range Query
    //cout << "=================================================" << endl;
    //cout << "Range Query:" << endl;
    //cout << "-------------------------------------------------" << endl;
    //cout << "UMLI1:" << endl;
    //int* rangequery_time_arr_umli1 = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_umli1 = 0.0;
    //for (int i = 0; i < RANGE_QUERY_SIZE; i++) {
    //    //cout << "Query" << i + 1 << ": \t";
    //    vector<int> Result;
    //    double* min = new double[dim];
    //    double* max = new double[dim];
    //    for (int j = 0; j < dim; j++) {
    //        min[j] = range_query[i][j];
    //        max[j] = range_query[i][j + dim];
    //    }
    //    auto start = high_resolution_clock::now();
    //    umli1.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_umli1[i] = duration.count();
    //    avg_rangequeryTime_umli1 += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //cout << Result.size() << endl;
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_umli1 = avg_rangequeryTime_umli1 / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_umli1 << endl;
    //delete[] rangequery_time_arr_umli1;
    //cout << "-------------------------------------------------" << endl;
    //cout << "UMLI2:" << endl;
    //int* rangequery_time_arr_umli2 = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_umli2 = 0.0;
    //for (int i = 0; i < RANGE_QUERY_SIZE; i++) {
    //    //cout << "Query" << i + 1 << ": \t";
    //    vector<int> Result;
    //    double* min = new double[dim];
    //    double* max = new double[dim];
    //    for (int j = 0; j < dim; j++) {
    //        min[j] = range_query[i][j];
    //        max[j] = range_query[i][j + dim];
    //    }
    //    auto start = high_resolution_clock::now();
    //    umli2.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_umli2[i] = duration.count();
    //    avg_rangequeryTime_umli2 += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //cout << Result.size() << endl;
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_umli2 = avg_rangequeryTime_umli2 / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_umli2 << endl;
    //delete[] rangequery_time_arr_umli2;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli1 = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli1 = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli1.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli1[i] = duration.count();
            avg_knnqueryTime_umli1 += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli1 = avg_knnqueryTime_umli1 / KNN_QUERY_SIZE;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli1 << endl;
        delete[] knnquery_time_arr_umli1;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli2 = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli2 = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli2.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli2[i] = duration.count();
            avg_knnqueryTime_umli2 += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli2 = avg_knnqueryTime_umli2 / KNN_QUERY_SIZE;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli2 << endl;
        delete[] knnquery_time_arr_umli2;
    }



    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void a1test_f_4d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "4D TEST" << endl;

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
    int BUILD_SIZE = 999000;
    vector<vector<double>> build_arr;
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        build_arr.push_back(data_arr[i]);
    }


    //// Generate point & KNN query
    cout << "Generate point & KNN query" << endl;
    int POINT_QUERY_SIZE = 1000;
    int KNN_QUERY_SIZE = 10;
    double** point_query = new double* [POINT_QUERY_SIZE];
    double** knn_query = new double* [KNN_QUERY_SIZE];
    for (int i = 0; i < POINT_QUERY_SIZE; ++i)
    {
        point_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            point_query[i][j] = data_arr[i * 998][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i * 998][j];
        }
    }

    //// Generate insert dataset
    cout << "Generate insert dataset" << endl;
    int INSERT_SIZE = 1000;
    vector<vector<double>> insert_arr;
    for (int i = 999000; i < 1000000; ++i)
    {
        insert_arr.push_back(data_arr[i]);
    }


    //// Generate delete dataset
    cout << "Generate delete dataset" << endl;
    int DELETE_SIZE = 100;
    vector<vector<double>> delete_arr;
    for (int i = 500000; i < 500100; ++i)
    {
        delete_arr.push_back(data_arr[i]);
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


    //// Build all indexes
    cout << "=================================================" << endl;
    cout << "Build all indexes: " << endl;
    
    vector<double> lscale1 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale2 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale3 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    vector<double> lscale4 = { 0, 1.25, 2.5, 3.75, 5, 6.25, 7.5, 8.75, 10 };
    //vector<double> lscale5 = { 0,2,4,6,8,10 };
    vector<double> scale1 = { 0, 0.788, 0.8509, 0.904, 0.955, 1.0, 1.056, 1.1142, 1.19, 1.3038, 10 };
    //vector<double> scale1 = { 0, 4.15, 4.7755, 5.27, 5.842, 10 };
    vector<double> scale2 = { 0, 0.788, 0.8509, 0.904, 0.955, 1.0, 1.056, 1.1142, 1.19, 1.3038, 10 };
    vector<double> scale3 = { 0, 0.788, 0.8509, 0.904, 0.955, 1.0, 1.056, 1.1142, 1.19, 1.3038, 10 };
    vector<double> scale4 = { 0, 0.788, 0.8509, 0.904, 0.955, 1.0, 1.056, 1.1142, 1.19, 1.3038, 10 };
    //vector<double> scale4 = { 0, 3.35, 3.7, 3.95, 4.15, 4.345, 4.489, 4.6496, 4.7755, 4.8979, 5.0, 5.12, 5.27, 5.39, 5.532, 5.6811, 5.842, 6.04, 6.278, 6.638, 10 };
    //vector<double> scale5 = { 0, 3.7, 4.15, 4.489, 4.7755, 5.0, 5.27, 5.532, 5.842, 6.278, 10 };


    // UMLI1
    cout << "UMLI1 with optimized grid:\t\t";
    vector<int> id_arr1;
    vector<vector<double>> umli_build_arr1;
    sort(build_arr.begin(), build_arr.end());
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        id_arr1.push_back(build_arr[i][dim]);
        vector<double> point;
        for (int j = 0; j < dim; ++j)
        {
            point.push_back(build_arr[i][j]);
        }
        umli_build_arr1.push_back(point);
    }
    vector<vector<double>> linear_scales_UMLI;
    linear_scales_UMLI.push_back(scale1);
    linear_scales_UMLI.push_back(scale2);
    linear_scales_UMLI.push_back(scale3);
    linear_scales_UMLI.push_back(scale4);
    UMLI<int, double, 4> umli1(id_arr1, umli_build_arr1, linear_scales_UMLI, 6);
    // UMLI2
    cout << "UMLI2:\t\t";
    vector<int> id_arr2;
    vector<vector<double>> umli_build_arr2;
    sort(build_arr.begin(), build_arr.end());
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        id_arr2.push_back(build_arr[i][dim]);
        vector<double> point;
        for (int j = 0; j < dim; ++j)
        {
            point.push_back(build_arr[i][j]);
        }
        umli_build_arr2.push_back(point);
    }
    vector<vector<double>> linear_scales_UMLI2;
    linear_scales_UMLI2.push_back(lscale1);
    linear_scales_UMLI2.push_back(lscale2);
    linear_scales_UMLI2.push_back(lscale3);
    linear_scales_UMLI2.push_back(lscale4);
    UMLI<int, double, 4> umli2(id_arr2, umli_build_arr2, linear_scales_UMLI2, 6);





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    int* pointquery_time_arr_umli1 = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_umli1 = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        umli1.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_umli1[i] = duration.count();
        avg_pointqueryTime_umli1 += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_umli1 = avg_pointqueryTime_umli1 / POINT_QUERY_SIZE;
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli1 << endl;
    delete[] pointquery_time_arr_umli1;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    int* pointquery_time_arr_umli2 = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_umli2 = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        umli2.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_umli2[i] = duration.count();
        avg_pointqueryTime_umli2 += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_umli2 = avg_pointqueryTime_umli2 / POINT_QUERY_SIZE;
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli2 << endl;
    delete[] pointquery_time_arr_umli2;


    //// Range Query
    cout << "=================================================" << endl;
    cout << "Range Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    int* rangequery_time_arr_umli1 = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_umli1 = 0.0;
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
        umli1.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_umli1[i] = duration.count();
        avg_rangequeryTime_umli1 += duration.count();
        //sort(Result.begin(), Result.end());
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli1 = avg_rangequeryTime_umli1 / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli1 << endl;
    delete[] rangequery_time_arr_umli1;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    int* rangequery_time_arr_umli2 = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_umli2 = 0.0;
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
        umli2.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_umli2[i] = duration.count();
        avg_rangequeryTime_umli2 += duration.count();
        //sort(Result.begin(), Result.end());
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli2 = avg_rangequeryTime_umli2 / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli2 << endl;
    delete[] rangequery_time_arr_umli2;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "UMLI1:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli1 = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli1 = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli1.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli1[i] = duration.count();
            avg_knnqueryTime_umli1 += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli1 = avg_knnqueryTime_umli1 / KNN_QUERY_SIZE;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli1 << endl;
        delete[] knnquery_time_arr_umli1;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI2:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli2 = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli2 = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli2.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli2[i] = duration.count();
            avg_knnqueryTime_umli2 += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli2 = avg_knnqueryTime_umli2 / KNN_QUERY_SIZE;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli2 << endl;
        delete[] knnquery_time_arr_umli2;
    }





    cout << "==============================================================================================================" << endl;
    cout << endl;
}


int maina2() {

    //cout << "uniform_4d_1000000" << endl;
    //a1test_uniform_4d("./Datasets/uniform_4d_1000000.csv", "./Datasets/4d_1000_range.csv", 4);
    //cout << "exponential_4d_1000000" << endl;
    //a1test_exponential_4d("./Datasets/exponential_4d_1000000.csv", "./Datasets/_range_exponential_4d_1000.csv", 4);
    //cout << "gamma_4d_1000000" << endl;
    //a1test_gamma_4d("./Datasets/gamma_4d_1000000.csv", "./Datasets/_range_gamma_4d_1000.csv", 4);
    //cout << "normal_4d_1000000" << endl;
    //a1test_normal_4d("./Datasets/normal_4d_1000000.csv", "./Datasets/_range_normal_4d_1000.csv", 4);
    //cout << "chisquared_4d_1000000" << endl;
    //a1test_chisquare_4d("./Datasets/chisquared_4d_1000000.csv", "./Datasets/_range_chisquared_4d_1000.csv", 4);
    cout << "f_4d_1000000" << endl;
    a1test_f_4d("./Datasets/f_4d_1000000.csv", "./Datasets/_range_f_4d_1000.csv", 4);


    system("pause");
    return 0;
}