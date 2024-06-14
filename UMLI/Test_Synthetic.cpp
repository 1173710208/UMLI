/*
This is a test program on sysnthetic datasets.
*/

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

using namespace std;
using namespace std::chrono;


/**
* Test all 3d dataset
*
* @param fileName File name of dataset
* @param rfileName File name of range query dataset
* @param dim NUmber of dimensions
*/
void test_synthetic_3d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "3D TEST" << endl;

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
    for (int i = 800000; i < 800100; ++i)
    {
        delete_arr.push_back(data_arr[i]);
    }

    //// Generate point & KNN query
    cout << "Generate point & KNN query" << endl;
    int POINT_QUERY_SIZE = 10000;
    int KNN_QUERY_SIZE = 10000;
    double** point_query = new double* [POINT_QUERY_SIZE];
    double** knn_query = new double* [KNN_QUERY_SIZE];
    for (int i = 0; i < POINT_QUERY_SIZE; ++i)
    {
        point_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            point_query[i][j] = data_arr[i][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i][j];
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
    int RANGE_QUERY_SIZE = range_query.size()/100;


    //// Build all indexes
    cout << "=================================================" << endl;
    cout << "Build all indexes: " << endl;
    // Full Scan
    cout << "Full Scan:\t";
    auto start_fs = high_resolution_clock::now();
    FullScan<int, double, 3> fullscan;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* point = new double[dim];
        for (int j = 0; j < dim; ++j)
        {
            point[j] = build_arr[i][j];
        }
        int id = build_arr[i][dim];
        fullscan.Insert(point, id);
        delete[] point;
    }
    auto stop_fs = high_resolution_clock::now();
    auto duration_fs = duration_cast<microseconds>(stop_fs - start_fs);
    cout << duration_fs.count() << " (ms)" << endl;
    // K-d Tree
    cout << "K-D Tree:\t";
    auto start_kd = high_resolution_clock::now();
    KDTree<int, double, 3> kdtree;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* point = new double[dim];
        for (int j = 0; j < dim; ++j)
        {
            point[j] = build_arr[i][j];
        }
        int id = build_arr[i][dim];
        kdtree.Insert(point, id);
        delete[] point;
    }
    auto stop_kd = high_resolution_clock::now();
    auto duration_kd = duration_cast<microseconds>(stop_kd - start_kd);
    cout << duration_kd.count() << " (ms)" << endl;
    // R Tree
    cout << "R Tree:\t\t";
    auto start_r = high_resolution_clock::now();
    RTree<int, double, 3> rtree;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* min = new double[dim];
        double* max = new double[dim];
        for (int j = 0; j < dim; ++j) {
            min[j] = build_arr[i][j];
            max[j] = build_arr[i][j];
        }
        rtree.Insert(min, max, build_arr[i][dim]);
        delete[] min;
        delete[] max;
    }
    auto stop_r = high_resolution_clock::now();
    auto duration_r = duration_cast<microseconds>(stop_r - start_r);
    cout << duration_r.count() << " (ms)" << endl;
    // Grid
    cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
    for (int i = 0; i < dim; ++i)
    {
        vector<double> scales = { 0,1,2,3,4,5,6,7,8,9,10 };
        linear_scales.push_back(scales);
    }
    auto start_grid = high_resolution_clock::now();
    Grid<int, double, 3> grid(linear_scales);
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
    cout << duration_grid.count() << " (ms)" << endl;
    //grid.PrintGridSize();
    // UMLI
    cout << "UMLI:\t\t";
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
        vector<double> scales = { 0,1,2,3,4,5,6,7,8,9,10 };
        linear_scales_UMLI.push_back(scales);
    }
    auto start_umli = high_resolution_clock::now();
    UMLI<int, double, 3> umli(id_arr, umli_build_arr, linear_scales_UMLI, 10);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (ms)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();


    //// Footprint
    cout << "=================================================" << endl;
    cout << "Footprint:" << endl;
    cout << "Fullscan:\t" << "0" << endl;
    cout << "K-d Tree:\t" << kdtree.Footprint() << endl;
    cout << "R Tree:\t\t" << rtree.Footprint() << endl;
    cout << "Grid:\t\t" << grid.Footprint() << endl;
    cout << "UMLI:\t\t" << umli.Footprint() << endl;


    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Full Scan:" << endl;
    int* pointquery_time_arr_fs = new int[POINT_QUERY_SIZE]; //microseconds
    double avg_pointqueryTime_fs = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        fullscan.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_fs[i] = duration.count();
        avg_pointqueryTime_fs += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_fs = avg_pointqueryTime_fs / POINT_QUERY_SIZE;
    cout << "average point query time(ms):\t" << avg_pointqueryTime_fs << endl;
    delete[] pointquery_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* pointquery_time_arr_kd = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_kd = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        kdtree.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_kd[i] = duration.count();
        avg_pointqueryTime_kd += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int i = 0; i < Result.size(); i++) {
        //    cout << Result[i] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_kd = avg_pointqueryTime_kd / POINT_QUERY_SIZE;
    cout << "average point query time(ms):\t" << avg_pointqueryTime_kd << endl;
    delete[] pointquery_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* pointquery_time_arr_r = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_r = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        rtree.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_r[i] = duration.count();
        avg_pointqueryTime_r += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_r = avg_pointqueryTime_r / POINT_QUERY_SIZE;
    cout << "average point query time(ms):\t" << avg_pointqueryTime_r << endl;
    delete[] pointquery_time_arr_r;
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    int* pointquery_time_arr_grid = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_grid = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        grid.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_grid[i] = duration.count();
        avg_pointqueryTime_grid += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_grid = avg_pointqueryTime_grid / POINT_QUERY_SIZE;
    cout << "average point query time(ms):\t" << avg_pointqueryTime_grid << endl;
    delete[] pointquery_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
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
    cout << "average point query time(ms):\t" << avg_pointqueryTime_umli << endl;
    delete[] pointquery_time_arr_umli;



    //// Range Query
    cout << "=================================================" << endl;
    cout << "Range Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Full Scan:" << endl;
    int* rangequery_time_arr_fs = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_fs = 0.0;
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
        fullscan.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_fs[i] = duration.count();
        avg_rangequeryTime_fs += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_fs = avg_rangequeryTime_fs / RANGE_QUERY_SIZE;
    cout << "average range query time(ms):\t" << avg_rangequeryTime_fs << endl;
    delete[] rangequery_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* rangequery_time_arr_kd = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_kd = 0.0;
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
        kdtree.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_kd[i] = duration.count();
        avg_rangequeryTime_kd += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_kd = avg_rangequeryTime_kd / RANGE_QUERY_SIZE;
    cout << "average range query time(ms):\t" << avg_rangequeryTime_kd << endl;
    delete[] rangequery_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* rangequery_time_arr_r = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_r = 0.0;
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
        rtree.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_r[i] = duration.count();
        avg_rangequeryTime_r += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_r = avg_rangequeryTime_r / RANGE_QUERY_SIZE;
    cout << "average range query time(ms):\t" << avg_rangequeryTime_r << endl;
    delete[] rangequery_time_arr_r;
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
    cout << "average range query time(ms):\t" << avg_rangequeryTime_grid << endl;
    delete[] rangequery_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
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
    cout << "average range query time(ms):\t" << avg_rangequeryTime_umli << endl;
    delete[] rangequery_time_arr_umli;



    //// KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Full Scan:" << endl;
    int* knnquery_time_arr_fs = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_fs = 0.0;
    for (int K = 1; K <= 1001; K = K + 100) {
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            fullscan.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_fs[i] = duration.count();
            avg_knnqueryTime_fs += duration.count();
            /*sort(Result, Result + K);
            for (int j = 0; j < K; j++) {
                cout << Result[j] << "\t";
            }
            cout << endl;*/
            delete[] Result;
        }
        avg_knnqueryTime_fs = avg_knnqueryTime_fs / KNN_QUERY_SIZE;
        cout << "average knn query time(ms):\t" << avg_knnqueryTime_fs << endl;
    }
    delete[] knnquery_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_kd = 0.0;
    for (int K = 1; K <= 1001; K = K + 100) {
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            kdtree.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_kd[i] = duration.count();
            avg_knnqueryTime_kd += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_kd = avg_knnqueryTime_kd / KNN_QUERY_SIZE;
        cout << "average knn query time(ms):\t" << avg_knnqueryTime_kd << endl;
    }
    delete[] knnquery_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_r = 0.0;
    for (int K = 1; K <= 1001; K = K + 100) {
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_r[i] = duration.count();
            avg_knnqueryTime_r += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_r = avg_knnqueryTime_r / KNN_QUERY_SIZE;
        cout << "average knn query time(ms):\t" << avg_knnqueryTime_r << endl;
    }
    delete[] knnquery_time_arr_r;
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_grid = 0.0;
    for (int K = 1; K <= 1001; K = K + 100) {
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_grid[i] = duration.count();
            avg_knnqueryTime_grid += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_grid = avg_knnqueryTime_grid / KNN_QUERY_SIZE;
        cout << "average knn query time(ms):\t" << avg_knnqueryTime_grid << endl;
    }
    delete[] knnquery_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_umli = 0.0;
    for (int K = 1; K <= 1001; K = K + 100) {
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_umli[i] = duration.count();
            avg_knnqueryTime_umli += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_umli = avg_knnqueryTime_umli / KNN_QUERY_SIZE;
        cout << "average knn query time(ms):\t" << avg_knnqueryTime_umli << endl;
    }
    delete[] knnquery_time_arr_umli;



    //// Insertion
    cout << "=================================================" << endl;
    cout << "Insertion:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Full Scan:" << endl;
    int* insert_time_arr_fs = new int[INSERT_SIZE]; //microseconds
    double avg_insertTime_fs = 0.0;
    for (int i = 0; i < INSERT_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = insert_arr[i][j];
        }
        int a_id = insert_arr[i][dim];
        auto start = high_resolution_clock::now();
        fullscan.Insert(a_point, a_id);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        insert_time_arr_fs[i] = duration.count();
        avg_insertTime_fs += duration.count();
        //vector<int> tmp_Result;
        //fullscan.PointQuery(tmp_Result, a_point);
        //for (int j = 0; j < tmp_Result.size(); j++) {
        //    cout << tmp_Result[j] << "\t";
        //}
        delete[] a_point;
    }
    cout << endl;
    avg_insertTime_fs = avg_insertTime_fs / INSERT_SIZE;
    cout << "average insert time(ms):\t" << avg_insertTime_fs << endl;
    delete[] insert_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* insert_time_arr_kd = new int[INSERT_SIZE]; //microseconds
    double avg_insertTime_kd = 0.0;
    for (int i = 0; i < INSERT_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = insert_arr[i][j];
        }
        int a_id = insert_arr[i][dim];
        auto start = high_resolution_clock::now();
        kdtree.Insert(a_point, a_id);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        insert_time_arr_kd[i] = duration.count();
        avg_insertTime_kd += duration.count();
        //vector<int> tmp_Result;
        //fullscan.PointQuery(tmp_Result, a_point);
        //for (int j = 0; j < tmp_Result.size(); j++) {
        //    cout << tmp_Result[j] << "\t";
        //}
        delete[] a_point;
    }
    cout << endl;
    avg_insertTime_kd = avg_insertTime_kd / INSERT_SIZE;
    cout << "average insert time(ms):\t" << avg_insertTime_kd << endl;
    delete[] insert_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* insert_time_arr_r = new int[INSERT_SIZE]; //microseconds
    double avg_insertTime_r = 0.0;
    for (int i = 0; i < INSERT_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = insert_arr[i][j];
        }
        int a_id = insert_arr[i][dim];
        auto start = high_resolution_clock::now();
        rtree.Insert(a_point, a_point, a_id);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        insert_time_arr_r[i] = duration.count();
        avg_insertTime_r += duration.count();
        //vector<int> tmp_Result;
        //fullscan.PointQuery(tmp_Result, a_point);
        //for (int j = 0; j < tmp_Result.size(); j++) {
        //    cout << tmp_Result[j] << "\t";
        //}
        delete[] a_point;
    }
    cout << endl;
    avg_insertTime_r = avg_insertTime_r / INSERT_SIZE;
    cout << "average insert time(ms):\t" << avg_insertTime_r << endl;
    delete[] insert_time_arr_r;
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    int* insert_time_arr_grid = new int[INSERT_SIZE]; //microseconds
    double avg_insertTime_grid = 0.0;
    for (int i = 0; i < INSERT_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = insert_arr[i][j];
        }
        int a_id = insert_arr[i][dim];
        auto start = high_resolution_clock::now();
        grid.Insert(a_point, a_id);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        insert_time_arr_grid[i] = duration.count();
        avg_insertTime_grid += duration.count();
        //vector<int> tmp_Result;
        //grid.PointQuery(tmp_Result, a_point);
        //for (int j = 0; j < tmp_Result.size(); j++) {
        //    cout << tmp_Result[j] << "\t";
        //}
        //delete[] a_point;
    }
    cout << endl;
    avg_insertTime_grid = avg_insertTime_grid / INSERT_SIZE;
    cout << "average insert time(ms):\t" << avg_insertTime_grid << endl;
    delete[] insert_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    int* insert_time_arr_umli = new int[INSERT_SIZE]; //microseconds
    double avg_insertTime_umli = 0.0;
    for (int i = 0; i < INSERT_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = insert_arr[i][j];
        }
        int a_id = insert_arr[i][dim];
        auto start = high_resolution_clock::now();
        umli.Insert(a_point, a_id);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        insert_time_arr_umli[i] = duration.count();
        avg_insertTime_umli += duration.count();
        //vector<int> tmp_Result;
        //umli.PointQuery(tmp_Result, a_point);
        //for (int j = 0; j < tmp_Result.size(); j++) {
        //    cout << tmp_Result[j] << "\t";
        //}
        //delete[] a_point;
    }
    cout << endl;
    avg_insertTime_umli = avg_insertTime_umli / INSERT_SIZE;
    cout << "average insert time(ms):\t" << avg_insertTime_umli << endl;
    delete[] insert_time_arr_umli;



    //// Deletion
    cout << "=================================================" << endl;
    cout << "Deletion:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Full Scan:" << endl;
    int* delete_time_arr_fs = new int[DELETE_SIZE]; //microseconds
    double avg_deleteTime_fs = 0.0;
    for (int i = 0; i < DELETE_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = delete_arr[i][j];
        }
        auto start = high_resolution_clock::now();
        fullscan.Delete(a_point);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        delete_time_arr_fs[i] = duration.count();
        avg_deleteTime_fs += duration.count();
        //vector<int> tmp_Result;
        //if (!fullscan.PointQuery(tmp_Result, a_point))
        //{
        //    cout << delete_arr[i][dim] << "\t";
        //}
        delete[] a_point;
    }
    cout << endl;
    avg_deleteTime_fs = avg_deleteTime_fs / DELETE_SIZE;
    cout << "average delete time(ms):\t" << avg_deleteTime_fs << endl;
    delete[] delete_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* delete_time_arr_kd = new int[DELETE_SIZE]; //microseconds
    double avg_deleteTime_kd = 0.0;
    for (int i = 0; i < DELETE_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = delete_arr[i][j];
        }
        auto start = high_resolution_clock::now();
        kdtree.Delete(a_point);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        delete_time_arr_kd[i] = duration.count();
        avg_deleteTime_kd += duration.count();
        //vector<int> tmp_Result;
        //if (!kdtree.PointQuery(tmp_Result, a_point))
        //{
        //    cout << delete_arr[i][dim] << "\t";
        //}
        delete[] a_point;
    }
    cout << endl;
    avg_deleteTime_kd = avg_deleteTime_kd / DELETE_SIZE;
    cout << "average delete time(ms):\t" << avg_deleteTime_kd << endl;
    delete[] delete_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* delete_time_arr_r = new int[DELETE_SIZE]; //microseconds
    double avg_deleteTime_r = 0.0;
    for (int i = 0; i < DELETE_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = delete_arr[i][j];
        }
        int a_id = delete_arr[i][dim];
        auto start = high_resolution_clock::now();
        rtree.Delete(a_point, a_point);
        //rtree.Remove(a_point, a_point, a_id);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        delete_time_arr_r[i] = duration.count();
        avg_deleteTime_r += duration.count();
        //vector<int> tmp_Result;
        //if (!rtree.PointQuery(tmp_Result, a_point))
        //{
        //    cout << delete_arr[i][dim] << "\t";
        //}
        delete[] a_point;
    }
    cout << endl;
    avg_deleteTime_r = avg_deleteTime_r / DELETE_SIZE;
    cout << "average delete time(ms):\t" << avg_deleteTime_r << endl;
    delete[] delete_time_arr_r;
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    int* delete_time_arr_grid = new int[DELETE_SIZE]; //microseconds
    double avg_deleteTime_grid = 0.0;
    for (int i = 0; i < DELETE_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = delete_arr[i][j];
        }
        int a_id = delete_arr[i][dim];
        auto start = high_resolution_clock::now();
        grid.Delete(a_point);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        delete_time_arr_grid[i] = duration.count();
        avg_deleteTime_grid += duration.count();
        //vector<int> tmp_Result;
        //if (!grid.PointQuery(tmp_Result, a_point))
        //{
        //    cout << delete_arr[i][dim] << "\t";
        //}
        delete[] a_point;
    }
    cout << endl;
    avg_deleteTime_grid = avg_deleteTime_grid / DELETE_SIZE;
    cout << "average delete time(ms):\t" << avg_deleteTime_grid << endl;
    delete[] delete_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    int* delete_time_arr_umli = new int[DELETE_SIZE]; //microseconds
    double avg_deleteTime_umli = 0.0;
    for (int i = 0; i < DELETE_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = delete_arr[i][j];
        }
        int a_id = delete_arr[i][dim];
        auto start = high_resolution_clock::now();
        umli.Delete(a_point);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        delete_time_arr_umli[i] = duration.count();
        avg_deleteTime_umli += duration.count();
        //vector<int> tmp_Result;
        //if (!umli.PointQuery(tmp_Result, a_point))
        //{
        //    cout << delete_arr[i][dim] << "\t";
        //}
        delete[] a_point;
    }
    cout << endl;
    avg_deleteTime_umli = avg_deleteTime_umli / DELETE_SIZE;
    cout << "average delete time(ms):\t" << avg_deleteTime_umli << endl;
    delete[] delete_time_arr_umli;




    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void test_build_2d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "2D TEST" << endl;
    cout << fileName << endl;

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
    int BUILD_SIZE = 1000000;
    vector<vector<double>> build_arr;
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        build_arr.push_back(data_arr[i]);
    }





    //// Build all indexes
    cout << "=================================================" << endl;
    cout << "Build all indexes: " << endl;
    // Full Scan
    cout << "Full Scan:\t";
    auto start_fs = high_resolution_clock::now();
    FullScan<int, double, 2> fullscan;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* point = new double[dim];
        for (int j = 0; j < dim; ++j)
        {
            point[j] = build_arr[i][j];
        }
        int id = build_arr[i][dim];
        fullscan.Insert(point, id);
        delete[] point;
    }
    auto stop_fs = high_resolution_clock::now();
    auto duration_fs = duration_cast<microseconds>(stop_fs - start_fs);
    cout << duration_fs.count() << " (ms)" << endl;
    // K-d Tree
    cout << "K-D Tree:\t";
    auto start_kd = high_resolution_clock::now();
    KDTree<int, double, 2> kdtree;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* point = new double[dim];
        for (int j = 0; j < dim; ++j)
        {
            point[j] = build_arr[i][j];
        }
        int id = build_arr[i][dim];
        kdtree.Insert(point, id);
        delete[] point;
    }
    auto stop_kd = high_resolution_clock::now();
    auto duration_kd = duration_cast<microseconds>(stop_kd - start_kd);
    cout << duration_kd.count() << " (ms)" << endl;
    // R Tree
    cout << "R Tree:\t\t";
    auto start_r = high_resolution_clock::now();
    RTree<int, double, 2> rtree;
    auto stop_r = high_resolution_clock::now();
    auto duration_r = duration_cast<microseconds>(stop_r - start_r);
    int build_time_r = duration_r.count();
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* min = new double[dim];
        double* max = new double[dim];
        for (int j = 0; j < dim; ++j) {
            min[j] = build_arr[i][j];
            max[j] = build_arr[i][j];
        }
        auto start = high_resolution_clock::now();
        rtree.Insert(min, max, build_arr[i][dim]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop_r - start_r);
        build_time_r += duration.count();
        delete[] min;
        delete[] max;
    }
    cout << build_time_r << " (ms)" << endl;
    // Grid
    cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
    for (int i = 0; i < dim; ++i)
    {
        vector<double> scales = { 0,1,2,3,4,5,6,7,8,9,10 };
        linear_scales.push_back(scales);
    }
    auto start_grid = high_resolution_clock::now();
    Grid<int, double, 2> grid(linear_scales);
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
    cout << duration_grid.count() << " (ms)" << endl;
    //grid.PrintGridSize();
    // UMLI
    cout << "UMLI:\t\t";
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
        vector<double> scales = { 0,1,2,3,4,5,6,7,8,9,10 };
        linear_scales_UMLI.push_back(scales);
    }
    auto start_umli = high_resolution_clock::now();
    UMLI<int, double, 2> umli(id_arr, umli_build_arr, linear_scales_UMLI, 5);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (ms)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();


    //// Footprint
    cout << "=================================================" << endl;
    cout << "Footprint:" << endl;
    cout << "Fullscan:\t" << "0" << endl;
    cout << "K-d Tree:\t" << kdtree.Footprint() << endl;
    cout << "R Tree:\t\t" << rtree.Footprint() << endl;
    cout << "Grid:\t\t" << grid.Footprint() << endl;
    cout << "UMLI:\t\t" << umli.Footprint() << endl;

}

void test_build_3d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "3D TEST" << endl;
    cout << fileName << endl;

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
    int BUILD_SIZE = 1000000;
    vector<vector<double>> build_arr;
    for (int i = 0; i < BUILD_SIZE; ++i)
    {
        build_arr.push_back(data_arr[i]);
    }





    //// Build all indexes
    cout << "=================================================" << endl;
    cout << "Build all indexes: " << endl;
    // Full Scan
    cout << "Full Scan:\t";
    auto start_fs = high_resolution_clock::now();
    FullScan<int, double, 3> fullscan;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* point = new double[dim];
        for (int j = 0; j < dim; ++j)
        {
            point[j] = build_arr[i][j];
        }
        int id = build_arr[i][dim];
        fullscan.Insert(point, id);
        delete[] point;
    }
    auto stop_fs = high_resolution_clock::now();
    auto duration_fs = duration_cast<microseconds>(stop_fs - start_fs);
    cout << duration_fs.count() << " (ms)" << endl;
    // K-d Tree
    cout << "K-D Tree:\t";
    auto start_kd = high_resolution_clock::now();
    KDTree<int, double, 3> kdtree;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* point = new double[dim];
        for (int j = 0; j < dim; ++j)
        {
            point[j] = build_arr[i][j];
        }
        int id = build_arr[i][dim];
        kdtree.Insert(point, id);
        delete[] point;
    }
    auto stop_kd = high_resolution_clock::now();
    auto duration_kd = duration_cast<microseconds>(stop_kd - start_kd);
    cout << duration_kd.count() << " (ms)" << endl;
    // R Tree
    cout << "R Tree:\t\t";
    auto start_r = high_resolution_clock::now();
    RTree<int, double, 3> rtree;
    auto stop_r = high_resolution_clock::now();
    auto duration_r = duration_cast<microseconds>(stop_r - start_r);
    int build_time_r = duration_r.count();
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* min = new double[dim];
        double* max = new double[dim];
        for (int j = 0; j < dim; ++j) {
            min[j] = build_arr[i][j];
            max[j] = build_arr[i][j];
        }
        auto start = high_resolution_clock::now();
        rtree.Insert(min, max, build_arr[i][dim]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop_r - start_r);
        build_time_r += duration.count();
        delete[] min;
        delete[] max;
    }
    cout << build_time_r << " (ms)" << endl;
    // Grid
    cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
    for (int i = 0; i < dim; ++i)
    {
        vector<double> scales = { 0,1,2,3,4,5,6,7,8,9,10 };
        linear_scales.push_back(scales);
    }
    auto start_grid = high_resolution_clock::now();
    Grid<int, double, 3> grid(linear_scales);
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
    cout << duration_grid.count() << " (ms)" << endl;
    //grid.PrintGridSize();
    // UMLI
    cout << "UMLI:\t\t";
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
        vector<double> scales = { 0,1,2,3,4,5,6,7,8,9,10 };
        linear_scales_UMLI.push_back(scales);
    }
    auto start_umli = high_resolution_clock::now();
    UMLI<int, double, 3> umli(id_arr, umli_build_arr, linear_scales_UMLI, 5);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (ms)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();


    //// Footprint
    cout << "=================================================" << endl;
    cout << "Footprint:" << endl;
    cout << "Fullscan:\t" << "0" << endl;
    cout << "K-d Tree:\t" << kdtree.Footprint() << endl;
    cout << "R Tree:\t\t" << rtree.Footprint() << endl;
    cout << "Grid:\t\t" << grid.Footprint() << endl;
    cout << "UMLI:\t\t" << umli.Footprint() << endl;

}

void test_query_3d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "3D TEST" << endl;

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
    int KNN_QUERY_SIZE = 1000;
    double** point_query = new double* [POINT_QUERY_SIZE];
    double** knn_query = new double* [KNN_QUERY_SIZE];
    for (int i = 0; i < POINT_QUERY_SIZE; ++i)
    {
        point_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            point_query[i][j] = data_arr[i][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i][j];
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
    //cout << "=================================================" << endl;
    //cout << "Build all indexes: " << endl;
    // Full Scan
    //cout << "Full Scan:\t";
    auto start_fs = high_resolution_clock::now();
    FullScan<int, double, 3> fullscan;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* point = new double[dim];
        for (int j = 0; j < dim; ++j)
        {
            point[j] = build_arr[i][j];
        }
        int id = build_arr[i][dim];
        fullscan.Insert(point, id);
        delete[] point;
    }
    auto stop_fs = high_resolution_clock::now();
    auto duration_fs = duration_cast<microseconds>(stop_fs - start_fs);
    //cout << duration_fs.count() << " (ms)" << endl;
    // K-d Tree
    //cout << "K-D Tree:\t";
    auto start_kd = high_resolution_clock::now();
    KDTree<int, double, 3> kdtree;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* point = new double[dim];
        for (int j = 0; j < dim; ++j)
        {
            point[j] = build_arr[i][j];
        }
        int id = build_arr[i][dim];
        kdtree.Insert(point, id);
        delete[] point;
    }
    auto stop_kd = high_resolution_clock::now();
    auto duration_kd = duration_cast<microseconds>(stop_kd - start_kd);
    //cout << duration_kd.count() << " (ms)" << endl;
    // R Tree
    //cout << "R Tree:\t\t";
    auto start_r = high_resolution_clock::now();
    RTree<int, double, 3> rtree;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* min = new double[dim];
        double* max = new double[dim];
        for (int j = 0; j < dim; ++j) {
            min[j] = build_arr[i][j];
            max[j] = build_arr[i][j];
        }
        rtree.Insert(min, max, build_arr[i][dim]);
        delete[] min;
        delete[] max;
    }
    auto stop_r = high_resolution_clock::now();
    auto duration_r = duration_cast<microseconds>(stop_r - start_r);
    //cout << duration_r.count() << " (ms)" << endl;
    // Grid
    //cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
    for (int i = 0; i < dim; ++i)
    {
        vector<double> scales = { 0,1,2,3,4,5,6,7,8,9,10 };
        linear_scales.push_back(scales);
    }
    auto start_grid = high_resolution_clock::now();
    Grid<int, double, 3> grid(linear_scales);
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
    //cout << duration_grid.count() << " (ms)" << endl;
    //grid.PrintGridSize();
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
        vector<double> scales = { 0,1,2,3,4,5,6,7,8,9,10 };
        linear_scales_UMLI.push_back(scales);
    }
    auto start_umli = high_resolution_clock::now();
    UMLI<int, double, 3> umli(id_arr, umli_build_arr, linear_scales_UMLI, 10);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    //cout << duration_umli.count() << " (ms)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();



    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Full Scan:" << endl;
    int* pointquery_time_arr_fs = new int[POINT_QUERY_SIZE]; //microseconds
    double avg_pointqueryTime_fs = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        fullscan.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_fs[i] = duration.count();
        avg_pointqueryTime_fs += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_fs = avg_pointqueryTime_fs / POINT_QUERY_SIZE;
    cout << "average point query time(ms):\t" << avg_pointqueryTime_fs << endl;
    delete[] pointquery_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* pointquery_time_arr_kd = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_kd = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        kdtree.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_kd[i] = duration.count();
        avg_pointqueryTime_kd += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int i = 0; i < Result.size(); i++) {
        //    cout << Result[i] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_kd = avg_pointqueryTime_kd / POINT_QUERY_SIZE;
    cout << "average point query time(ms):\t" << avg_pointqueryTime_kd << endl;
    delete[] pointquery_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* pointquery_time_arr_r = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_r = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        rtree.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_r[i] = duration.count();
        avg_pointqueryTime_r += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_r = avg_pointqueryTime_r / POINT_QUERY_SIZE;
    cout << "average point query time(ms):\t" << avg_pointqueryTime_r << endl;
    delete[] pointquery_time_arr_r;
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    int* pointquery_time_arr_grid = new int[POINT_QUERY_SIZE];//microseconds
    double avg_pointqueryTime_grid = 0.0;
    for (int i = 0; i < POINT_QUERY_SIZE; i++) {
        //cout << "Query" << i + 1 << ": \t";
        vector<int> Result;
        auto start = high_resolution_clock::now();
        grid.PointQuery(Result, point_query[i]);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        pointquery_time_arr_grid[i] = duration.count();
        avg_pointqueryTime_grid += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
    }
    avg_pointqueryTime_grid = avg_pointqueryTime_grid / POINT_QUERY_SIZE;
    cout << "average point query time(ms):\t" << avg_pointqueryTime_grid << endl;
    delete[] pointquery_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
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
    cout << "average point query time(ms):\t" << avg_pointqueryTime_umli << endl;
    delete[] pointquery_time_arr_umli;



    //// Range Query
    cout << "=================================================" << endl;
    cout << "Range Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Full Scan:" << endl;
    int* rangequery_time_arr_fs = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_fs = 0.0;
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
        fullscan.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_fs[i] = duration.count();
        avg_rangequeryTime_fs += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_fs = avg_rangequeryTime_fs / RANGE_QUERY_SIZE;
    cout << "average range query time(ms):\t" << avg_rangequeryTime_fs << endl;
    delete[] rangequery_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* rangequery_time_arr_kd = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_kd = 0.0;
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
        kdtree.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_kd[i] = duration.count();
        avg_rangequeryTime_kd += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_kd = avg_rangequeryTime_kd / RANGE_QUERY_SIZE;
    cout << "average range query time(ms):\t" << avg_rangequeryTime_kd << endl;
    delete[] rangequery_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* rangequery_time_arr_r = new int[RANGE_QUERY_SIZE];//microseconds
    double avg_rangequeryTime_r = 0.0;
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
        rtree.RangeQuery(Result, min, max);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        rangequery_time_arr_r[i] = duration.count();
        avg_rangequeryTime_r += duration.count();
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_r = avg_rangequeryTime_r / RANGE_QUERY_SIZE;
    cout << "average range query time(ms):\t" << avg_rangequeryTime_r << endl;
    delete[] rangequery_time_arr_r;
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
    cout << "average range query time(ms):\t" << avg_rangequeryTime_grid << endl;
    delete[] rangequery_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
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
    cout << "average range query time(ms):\t" << avg_rangequeryTime_umli << endl;
    delete[] rangequery_time_arr_umli;




    cout << "==============================================================================================================" << endl;
    cout << endl;
}

void test_update_3d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "3D TEST" << endl;

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
    for (int i = 990000; i < 990100; ++i)
    {
        delete_arr.push_back(data_arr[i]);
    }



    //// Build all indexes
    //cout << "=================================================" << endl;
    //cout << "Build all indexes: " << endl;
    // Full Scan
    //cout << "Full Scan:\t";
    auto start_fs = high_resolution_clock::now();
    FullScan<int, double, 3> fullscan;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* point = new double[dim];
        for (int j = 0; j < dim; ++j)
        {
            point[j] = build_arr[i][j];
        }
        int id = build_arr[i][dim];
        fullscan.Insert(point, id);
        delete[] point;
    }
    auto stop_fs = high_resolution_clock::now();
    auto duration_fs = duration_cast<microseconds>(stop_fs - start_fs);
    //cout << duration_fs.count() << " (ms)" << endl;
    // K-d Tree
    //cout << "K-D Tree:\t";
    auto start_kd = high_resolution_clock::now();
    KDTree<int, double, 3> kdtree;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* point = new double[dim];
        for (int j = 0; j < dim; ++j)
        {
            point[j] = build_arr[i][j];
        }
        int id = build_arr[i][dim];
        kdtree.Insert(point, id);
        delete[] point;
    }
    auto stop_kd = high_resolution_clock::now();
    auto duration_kd = duration_cast<microseconds>(stop_kd - start_kd);
    //cout << duration_kd.count() << " (ms)" << endl;
    // R Tree
    //cout << "R Tree:\t\t";
    auto start_r = high_resolution_clock::now();
    RTree<int, double, 3> rtree;
    for (int i = 0; i < BUILD_SIZE; i++)
    {
        double* min = new double[dim];
        double* max = new double[dim];
        for (int j = 0; j < dim; ++j) {
            min[j] = build_arr[i][j];
            max[j] = build_arr[i][j];
        }
        rtree.Insert(min, max, build_arr[i][dim]);
        delete[] min;
        delete[] max;
    }
    auto stop_r = high_resolution_clock::now();
    auto duration_r = duration_cast<microseconds>(stop_r - start_r);
    //cout << duration_r.count() << " (ms)" << endl;
    // Grid
    //cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
    for (int i = 0; i < dim; ++i)
    {
        vector<double> scales = { 0,1,2,3,4,5,6,7,8,9,10 };
        linear_scales.push_back(scales);
    }
    auto start_grid = high_resolution_clock::now();
    Grid<int, double, 3> grid(linear_scales);
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
    //cout << duration_grid.count() << " (ms)" << endl;
    //grid.PrintGridSize();
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
        vector<double> scales = { 0,1,2,3,4,5,6,7,8,9,10 };
        linear_scales_UMLI.push_back(scales);
    }
    auto start_umli = high_resolution_clock::now();
    UMLI<int, double, 3> umli(id_arr, umli_build_arr, linear_scales_UMLI, 6);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    //cout << duration_umli.count() << " (ms)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();




    //// Insertion
    cout << "=================================================" << endl;
    cout << "Insertion:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Full Scan:" << endl;
    int* insert_time_arr_fs = new int[INSERT_SIZE]; //microseconds
    double avg_insertTime_fs = 0.0;
    for (int i = 0; i < INSERT_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = insert_arr[i][j];
        }
        int a_id = insert_arr[i][dim];
        auto start = high_resolution_clock::now();
        fullscan.Insert(a_point, a_id);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        insert_time_arr_fs[i] = duration.count();
        avg_insertTime_fs += duration.count();
        //vector<int> tmp_Result;
        //fullscan.PointQuery(tmp_Result, a_point);
        //for (int j = 0; j < tmp_Result.size(); j++) {
        //    cout << tmp_Result[j] << "\t";
        //}
        delete[] a_point;
    }
    cout << endl;
    avg_insertTime_fs = avg_insertTime_fs / INSERT_SIZE;
    cout << "average insert time(ms):\t" << avg_insertTime_fs << endl;
    delete[] insert_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* insert_time_arr_kd = new int[INSERT_SIZE]; //microseconds
    double avg_insertTime_kd = 0.0;
    for (int i = 0; i < INSERT_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = insert_arr[i][j];
        }
        int a_id = insert_arr[i][dim];
        auto start = high_resolution_clock::now();
        kdtree.Insert(a_point, a_id);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        insert_time_arr_kd[i] = duration.count();
        avg_insertTime_kd += duration.count();
        //vector<int> tmp_Result;
        //fullscan.PointQuery(tmp_Result, a_point);
        //for (int j = 0; j < tmp_Result.size(); j++) {
        //    cout << tmp_Result[j] << "\t";
        //}
        delete[] a_point;
    }
    cout << endl;
    avg_insertTime_kd = avg_insertTime_kd / INSERT_SIZE;
    cout << "average insert time(ms):\t" << avg_insertTime_kd << endl;
    delete[] insert_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* insert_time_arr_r = new int[INSERT_SIZE]; //microseconds
    double avg_insertTime_r = 0.0;
    for (int i = 0; i < INSERT_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = insert_arr[i][j];
        }
        int a_id = insert_arr[i][dim];
        auto start = high_resolution_clock::now();
        rtree.Insert(a_point, a_point, a_id);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        insert_time_arr_r[i] = duration.count();
        avg_insertTime_r += duration.count();
        //vector<int> tmp_Result;
        //fullscan.PointQuery(tmp_Result, a_point);
        //for (int j = 0; j < tmp_Result.size(); j++) {
        //    cout << tmp_Result[j] << "\t";
        //}
        delete[] a_point;
    }
    cout << endl;
    avg_insertTime_r = avg_insertTime_r / INSERT_SIZE;
    cout << "average insert time(ms):\t" << avg_insertTime_r << endl;
    delete[] insert_time_arr_r;
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    int* insert_time_arr_grid = new int[INSERT_SIZE]; //microseconds
    double avg_insertTime_grid = 0.0;
    for (int i = 0; i < INSERT_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = insert_arr[i][j];
        }
        int a_id = insert_arr[i][dim];
        auto start = high_resolution_clock::now();
        grid.Insert(a_point, a_id);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        insert_time_arr_grid[i] = duration.count();
        avg_insertTime_grid += duration.count();
        //vector<int> tmp_Result;
        //grid.PointQuery(tmp_Result, a_point);
        //for (int j = 0; j < tmp_Result.size(); j++) {
        //    cout << tmp_Result[j] << "\t";
        //}
        //delete[] a_point;
    }
    cout << endl;
    avg_insertTime_grid = avg_insertTime_grid / INSERT_SIZE;
    cout << "average insert time(ms):\t" << avg_insertTime_grid << endl;
    delete[] insert_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    int* insert_time_arr_umli = new int[INSERT_SIZE]; //microseconds
    double avg_insertTime_umli = 0.0;
    for (int i = 0; i < INSERT_SIZE; i++) {
        double* a_point = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            a_point[j] = insert_arr[i][j];
        }
        int a_id = insert_arr[i][dim];
        auto start = high_resolution_clock::now();
        umli.Insert(a_point, a_id);
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        insert_time_arr_umli[i] = duration.count();
        avg_insertTime_umli += duration.count();
        //vector<int> tmp_Result;
        //umli.PointQuery(tmp_Result, a_point);
        //for (int j = 0; j < tmp_Result.size(); j++) {
        //    cout << tmp_Result[j] << "\t";
        //}
        //delete[] a_point;
    }
    cout << endl;
    avg_insertTime_umli = avg_insertTime_umli / INSERT_SIZE;
    cout << "average insert time(ms):\t" << avg_insertTime_umli << endl;
    delete[] insert_time_arr_umli;



    cout << "==============================================================================================================" << endl;
    cout << endl;
}

int mainS() {
	test_synthetic_3d("./Datasets/uniform_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
	//test_synthetic_3d(3);


    //test_build_2d("./Datasets/chisquared_2d_1000000.csv", "./Datasets/2d_1000_range.csv", 2);
    //test_build_2d("./Datasets/exponential_2d_1000000.csv", "./Datasets/2d_1000_range.csv", 2);
    //test_build_2d("./Datasets/f_2d_1000000.csv", "./Datasets/2d_1000_range.csv", 2);
    //test_build_2d("./Datasets/gamma_2d_1000000.csv", "./Datasets/2d_1000_range.csv", 2);
    //test_build_2d("./Datasets/normal_2d_1000000.csv", "./Datasets/2d_1000_range.csv", 2);
    //test_build_2d("./Datasets/piecewiselinear_2d_1000000.csv", "./Datasets/2d_1000_range.csv", 2);
    //test_build_2d("./Datasets/uniform_2d_1000000.csv", "./Datasets/2d_1000_range.csv", 2);


    //test_build_3d("./Datasets/chisquared_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    //test_build_3d("./Datasets/exponential_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    //test_build_3d("./Datasets/f_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    //test_build_3d("./Datasets/gamma_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    //test_build_3d("./Datasets/normal_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    //test_build_3d("./Datasets/piecewiselinear_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    //test_build_3d("./Datasets/uniform_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);



    //test_build_4d("./Datasets/chisquared_4d_1000000.csv", "./Datasets/4d_1000_range.csv", 4);
    //test_build_4d("./Datasets/exponential_4d_1000000.csv", "./Datasets/4d_1000_range.csv", 4);
    //test_build_4d("./Datasets/f_4d_1000000.csv", "./Datasets/4d_1000_range.csv", 4);
    //test_build_4d("./Datasets/gamma_4d_1000000.csv", "./Datasets/4d_1000_range.csv", 4);
    //test_build_4d("./Datasets/normal_4d_1000000.csv", "./Datasets/4d_1000_range.csv", 4);
    //test_build_4d("./Datasets/piecewiselinear_4d_1000000.csv", "./Datasets/4d_1000_range.csv", 4);
    //test_build_4d("./Datasets/uniform_4d_1000000.csv", "./Datasets/4d_1000_range.csv", 4);


    //test_build_5d("./Datasets/chisquared_5d_1000000.csv", "./Datasets/5d_1000_range.csv", 5);
    //test_build_5d("./Datasets/exponential_5d_1000000.csv", "./Datasets/5d_1000_range.csv", 5);
    //test_build_5d("./Datasets/f_5d_1000000.csv", "./Datasets/5d_1000_range.csv", 5);
    //test_build_5d("./Datasets/gamma_5d_1000000.csv", "./Datasets/5d_1000_range.csv", 5);
    //test_build_5d("./Datasets/normal_5d_1000000.csv", "./Datasets/5d_1000_range.csv", 5);
    //test_build_5d("./Datasets/piecewiselinear_5d_1000000.csv", "./Datasets/5d_1000_range.csv", 5);
    //test_build_5d("./Datasets/uniform_5d_1000000.csv", "./Datasets/5d_1000_range.csv", 5);


    //test_build_6d("./Datasets/chisquared_6d_1000000.csv", "./Datasets/6d_1000_range.csv", 6);
    //test_build_6d("./Datasets/exponential_6d_1000000.csv", "./Datasets/6d_1000_range.csv", 6);
    //test_build_6d("./Datasets/f_6d_1000000.csv", "./Datasets/6d_1000_range.csv", 6);
    //test_build_6d("./Datasets/gamma_6d_1000000.csv", "./Datasets/6d_1000_range.csv", 6);
    //test_build_6d("./Datasets/normal_6d_1000000.csv", "./Datasets/6d_1000_range.csv", 6);
    //test_build_6d("./Datasets/piecewiselinear_6d_1000000.csv", "./Datasets/6d_1000_range.csv", 6);
    //test_build_6d("./Datasets/uniform_6d_1000000.csv", "./Datasets/6d_1000_range.csv", 6);

    //test_query_3d("./Datasets/chisquared_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    //test_query_3d("./Datasets/exponential_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    //test_query_3d("./Datasets/f_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    //test_query_3d("./Datasets/gamma_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    //test_query_3d("./Datasets/normal_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    //test_query_3d("./Datasets/piecewiselinear_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    //test_query_3d("./Datasets/uniform_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);


    test_update_3d("./Datasets/chisquared_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    test_update_3d("./Datasets/exponential_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    test_update_3d("./Datasets/f_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    test_update_3d("./Datasets/gamma_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    test_update_3d("./Datasets/normal_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    test_update_3d("./Datasets/piecewiselinear_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);
    test_update_3d("./Datasets/uniform_3d_1000000.csv", "./Datasets/3d_1000_range.csv", 3);



	system("pause");
	return 0;
}