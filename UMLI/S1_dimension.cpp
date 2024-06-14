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



void s1test_normal_2d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "2D TEST" << endl;

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
    cout << duration_kd.count() << " (us)" << endl;
    // R Tree
    cout << "R Tree:\t\t";
    auto start_r = high_resolution_clock::now();
    RTree<int, double, 2> rtree;
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
    cout << duration_r.count() << " (us)" << endl;
    // Grid
    cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
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


    vector<vector<double>> linear_scales_UMLI;
    for (int di = 0; di < dim; di++) {
        linear_scales_UMLI.push_back(scale1);
        linear_scales.push_back(lscale1);
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
    cout << duration_grid.count() << " (us)" << endl;
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
    auto start_umli = high_resolution_clock::now();
    UMLI<int, double, 2> umli(id_arr, umli_build_arr, linear_scales_UMLI, 7);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;
   
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_kd << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_r << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_grid << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli << endl;
    delete[] pointquery_time_arr_umli;



    //// Range Query
    cout << "=================================================" << endl;
    cout << "Range Query:" << endl;
    
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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_kd = avg_rangequeryTime_kd / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_kd << endl;
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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_r = avg_rangequeryTime_r / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_r << endl;
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
        //cout << Result.size() << endl;
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
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli = avg_rangequeryTime_umli / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli << endl;
    delete[] rangequery_time_arr_umli;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;
    
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_kd << endl;
        delete[] knnquery_time_arr_kd;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_r = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_r << endl;
        delete[] knnquery_time_arr_r;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_grid = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_grid << endl;
        delete[] knnquery_time_arr_grid;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli << endl;
        delete[] knnquery_time_arr_umli;
    }





    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void s1test_normal_3d(const char* fileName, const char* rfileName, int dim)
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
    cout << duration_kd.count() << " (us)" << endl;
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
    cout << duration_r.count() << " (us)" << endl;
    // Grid
    cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
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

    vector<vector<double>> linear_scales_UMLI;
    for (int di = 0; di < dim; di++) {
        linear_scales_UMLI.push_back(scale1);
        linear_scales.push_back(lscale1);
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
    cout << duration_grid.count() << " (us)" << endl;
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
    auto start_umli = high_resolution_clock::now();
    UMLI<int, double, 3> umli(id_arr, umli_build_arr, linear_scales_UMLI, 7);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;

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
    cout << "average point query time(us):\t" << avg_pointqueryTime_kd << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_r << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_grid << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli << endl;
    delete[] pointquery_time_arr_umli;



    //// Range Query
    cout << "=================================================" << endl;
    cout << "Range Query:" << endl;

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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_kd = avg_rangequeryTime_kd / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_kd << endl;
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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_r = avg_rangequeryTime_r / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_r << endl;
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
        //cout << Result.size() << endl;
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
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli = avg_rangequeryTime_umli / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli << endl;
    delete[] rangequery_time_arr_umli;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_kd << endl;
        delete[] knnquery_time_arr_kd;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_r = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_r << endl;
        delete[] knnquery_time_arr_r;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_grid = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_grid << endl;
        delete[] knnquery_time_arr_grid;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli << endl;
        delete[] knnquery_time_arr_umli;
    }





    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void s1test_normal_4d(const char* fileName, const char* rfileName, int dim)
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

    // K-d Tree
    cout << "K-D Tree:\t";
    auto start_kd = high_resolution_clock::now();
    KDTree<int, double, 4> kdtree;
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
    cout << duration_kd.count() << " (us)" << endl;
    // R Tree
    cout << "R Tree:\t\t";
    auto start_r = high_resolution_clock::now();
    RTree<int, double, 4> rtree;
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
    cout << duration_r.count() << " (us)" << endl;
    // Grid
    cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
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

    //vector<double> scale4 = { 0, 0.4, 0.8, 1.2, 1.6, 2, 2.4, 2.8, 3.2, 3.6, 4, 4.4, 4.8, 5.2, 5.6, 6, 6.4, 6.8, 7.2, 7.6, 8, 8.4, 8.8, 9.2, 9.6, 10 }; // NoC = 25
    linear_scales.push_back(lscale1);
    linear_scales.push_back(lscale2);
    linear_scales.push_back(lscale3);
    linear_scales.push_back(lscale4);
    //linear_scales.push_back(lscale5);
    auto start_grid = high_resolution_clock::now();
    Grid<int, double, 4> grid(linear_scales);
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
    cout << duration_grid.count() << " (us)" << endl;
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
    linear_scales_UMLI.push_back(scale1);
    linear_scales_UMLI.push_back(scale2);
    linear_scales_UMLI.push_back(scale3);
    linear_scales_UMLI.push_back(scale4);
    //linear_scales_UMLI.push_back(scale5);
    auto start_umli = high_resolution_clock::now();
    UMLI<int, double, 4> umli(id_arr, umli_build_arr, linear_scales_UMLI, 7);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;

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
    cout << "average point query time(us):\t" << avg_pointqueryTime_kd << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_r << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_grid << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli << endl;
    delete[] pointquery_time_arr_umli;



    //// Range Query
    cout << "=================================================" << endl;
    cout << "Range Query:" << endl;

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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_kd = avg_rangequeryTime_kd / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_kd << endl;
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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_r = avg_rangequeryTime_r / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_r << endl;
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
        //cout << Result.size() << endl;
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
        //cout << Result.size() << endl;
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_umli = avg_rangequeryTime_umli / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_umli << endl;
    delete[] rangequery_time_arr_umli;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_kd << endl;
        delete[] knnquery_time_arr_kd;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_r = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_r << endl;
        delete[] knnquery_time_arr_r;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_grid = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_grid << endl;
        delete[] knnquery_time_arr_grid;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli << endl;
        delete[] knnquery_time_arr_umli;
    }





    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void s1test_normal_5d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "5D TEST" << endl;

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

    // K-d Tree
    cout << "K-D Tree:\t";
    auto start_kd = high_resolution_clock::now();
    KDTree<int, double, 5> kdtree;
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
    cout << duration_kd.count() << " (us)" << endl;
    // R Tree
    cout << "R Tree:\t\t";
    auto start_r = high_resolution_clock::now();
    RTree<int, double, 5> rtree;
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
    cout << duration_r.count() << " (us)" << endl;
    // Grid
    cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
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


    vector<vector<double>> linear_scales_UMLI;
    for (int di = 0; di < dim; di++) {
        linear_scales_UMLI.push_back(scale1);
        linear_scales.push_back(lscale1);
    }
    auto start_grid = high_resolution_clock::now();
    Grid<int, double, 5> grid(linear_scales);
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
    cout << duration_grid.count() << " (us)" << endl;
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
    //linear_scales_UMLI.push_back(scale5);
    auto start_umli = high_resolution_clock::now();
    UMLI<int, double, 5> umli(id_arr, umli_build_arr, linear_scales_UMLI, 7);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;

    //cout << "-------------------------------------------------" << endl;
    //cout << "K-d Tree:" << endl;
    //int* pointquery_time_arr_kd = new int[POINT_QUERY_SIZE];//microseconds
    //double avg_pointqueryTime_kd = 0.0;
    //for (int i = 0; i < POINT_QUERY_SIZE; i++) {
    //    //cout << "Query" << i + 1 << ": \t";
    //    vector<int> Result;
    //    auto start = high_resolution_clock::now();
    //    kdtree.PointQuery(Result, point_query[i]);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    pointquery_time_arr_kd[i] = duration.count();
    //    avg_pointqueryTime_kd += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int i = 0; i < Result.size(); i++) {
    //    //    cout << Result[i] << "\t";
    //    //}
    //    //cout << endl;
    //}
    //avg_pointqueryTime_kd = avg_pointqueryTime_kd / POINT_QUERY_SIZE;
    //cout << "average point query time(us):\t" << avg_pointqueryTime_kd << endl;
    //delete[] pointquery_time_arr_kd;
    //cout << "-------------------------------------------------" << endl;
    //cout << "R Tree:" << endl;
    //int* pointquery_time_arr_r = new int[POINT_QUERY_SIZE];//microseconds
    //double avg_pointqueryTime_r = 0.0;
    //for (int i = 0; i < POINT_QUERY_SIZE; i++) {
    //    //cout << "Query" << i + 1 << ": \t";
    //    vector<int> Result;
    //    auto start = high_resolution_clock::now();
    //    rtree.PointQuery(Result, point_query[i]);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    pointquery_time_arr_r[i] = duration.count();
    //    avg_pointqueryTime_r += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //}
    //avg_pointqueryTime_r = avg_pointqueryTime_r / POINT_QUERY_SIZE;
    //cout << "average point query time(us):\t" << avg_pointqueryTime_r << endl;
    //delete[] pointquery_time_arr_r;
    //cout << "-------------------------------------------------" << endl;
    //cout << "Grid:" << endl;
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
    //cout << "average point query time(us):\t" << avg_pointqueryTime_grid << endl;
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

    //cout << "-------------------------------------------------" << endl;
    //cout << "K-d Tree:" << endl;
    //int* rangequery_time_arr_kd = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_kd = 0.0;
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
    //    kdtree.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_kd[i] = duration.count();
    //    avg_rangequeryTime_kd += duration.count();
    //    //cout << Result.size() << endl;
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_kd = avg_rangequeryTime_kd / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_kd << endl;
    //delete[] rangequery_time_arr_kd;
    //cout << "-------------------------------------------------" << endl;
    //cout << "R Tree:" << endl;
    //int* rangequery_time_arr_r = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_r = 0.0;
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
    //    rtree.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_r[i] = duration.count();
    //    avg_rangequeryTime_r += duration.count();
    //    //cout << Result.size() << endl;
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_r = avg_rangequeryTime_r / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_r << endl;
    //delete[] rangequery_time_arr_r;
    //cout << "-------------------------------------------------" << endl;
    //cout << "Grid:" << endl;
    //int* rangequery_time_arr_grid = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_grid = 0.0;
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
    //    grid.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_grid[i] = duration.count();
    //    avg_rangequeryTime_grid += duration.count();
    //    //cout << Result.size() << endl;
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_grid = avg_rangequeryTime_grid / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_grid << endl;
    //delete[] rangequery_time_arr_grid;
    //cout << "-------------------------------------------------" << endl;
    //cout << "UMLI:" << endl;
    //int* rangequery_time_arr_umli = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_umli = 0.0;
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
    //    umli.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_umli[i] = duration.count();
    //    avg_rangequeryTime_umli += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //cout << Result.size() << endl;
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_umli = avg_rangequeryTime_umli / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_umli << endl;
    //delete[] rangequery_time_arr_umli;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_kd << endl;
        delete[] knnquery_time_arr_kd;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_r = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_r << endl;
        delete[] knnquery_time_arr_r;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_grid = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_grid << endl;
        delete[] knnquery_time_arr_grid;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli << endl;
        delete[] knnquery_time_arr_umli;
    }





    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void s1test_normal_6d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "6D TEST" << endl;

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

    // K-d Tree
    cout << "K-D Tree:\t";
    auto start_kd = high_resolution_clock::now();
    KDTree<int, double, 6> kdtree;
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
    cout << duration_kd.count() << " (us)" << endl;
    // R Tree
    cout << "R Tree:\t\t";
    auto start_r = high_resolution_clock::now();
    RTree<int, double, 6> rtree;
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
    cout << duration_r.count() << " (us)" << endl;
    // Grid
    cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
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


    vector<vector<double>> linear_scales_UMLI;
    for (int di = 0; di < dim; di++) {
        linear_scales_UMLI.push_back(scale1);
        linear_scales.push_back(lscale1);
    }
    auto start_grid = high_resolution_clock::now();
    Grid<int, double, 6> grid(linear_scales);
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
    cout << duration_grid.count() << " (us)" << endl;
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
    //linear_scales_UMLI.push_back(scale5);
    auto start_umli = high_resolution_clock::now();
    UMLI<int, double, 6> umli(id_arr, umli_build_arr, linear_scales_UMLI, 7);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;

    //cout << "-------------------------------------------------" << endl;
    //cout << "K-d Tree:" << endl;
    //int* pointquery_time_arr_kd = new int[POINT_QUERY_SIZE];//microseconds
    //double avg_pointqueryTime_kd = 0.0;
    //for (int i = 0; i < POINT_QUERY_SIZE; i++) {
    //    //cout << "Query" << i + 1 << ": \t";
    //    vector<int> Result;
    //    auto start = high_resolution_clock::now();
    //    kdtree.PointQuery(Result, point_query[i]);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    pointquery_time_arr_kd[i] = duration.count();
    //    avg_pointqueryTime_kd += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int i = 0; i < Result.size(); i++) {
    //    //    cout << Result[i] << "\t";
    //    //}
    //    //cout << endl;
    //}
    //avg_pointqueryTime_kd = avg_pointqueryTime_kd / POINT_QUERY_SIZE;
    //cout << "average point query time(us):\t" << avg_pointqueryTime_kd << endl;
    //delete[] pointquery_time_arr_kd;
    //cout << "-------------------------------------------------" << endl;
    //cout << "R Tree:" << endl;
    //int* pointquery_time_arr_r = new int[POINT_QUERY_SIZE];//microseconds
    //double avg_pointqueryTime_r = 0.0;
    //for (int i = 0; i < POINT_QUERY_SIZE; i++) {
    //    //cout << "Query" << i + 1 << ": \t";
    //    vector<int> Result;
    //    auto start = high_resolution_clock::now();
    //    rtree.PointQuery(Result, point_query[i]);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    pointquery_time_arr_r[i] = duration.count();
    //    avg_pointqueryTime_r += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //}
    //avg_pointqueryTime_r = avg_pointqueryTime_r / POINT_QUERY_SIZE;
    //cout << "average point query time(us):\t" << avg_pointqueryTime_r << endl;
    //delete[] pointquery_time_arr_r;
    //cout << "-------------------------------------------------" << endl;
    //cout << "Grid:" << endl;
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
    //cout << "average point query time(us):\t" << avg_pointqueryTime_grid << endl;
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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_kd = avg_rangequeryTime_kd / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_kd << endl;
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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_r = avg_rangequeryTime_r / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_r << endl;
    delete[] rangequery_time_arr_r;
    //cout << "-------------------------------------------------" << endl;
    //cout << "Grid:" << endl;
    //int* rangequery_time_arr_grid = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_grid = 0.0;
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
    //    grid.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_grid[i] = duration.count();
    //    avg_rangequeryTime_grid += duration.count();
    //    //cout << Result.size() << endl;
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_grid = avg_rangequeryTime_grid / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_grid << endl;
    //delete[] rangequery_time_arr_grid;
    //cout << "-------------------------------------------------" << endl;
    //cout << "UMLI:" << endl;
    //int* rangequery_time_arr_umli = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_umli = 0.0;
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
    //    umli.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_umli[i] = duration.count();
    //    avg_rangequeryTime_umli += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //cout << Result.size() << endl;
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_umli = avg_rangequeryTime_umli / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_umli << endl;
    //delete[] rangequery_time_arr_umli;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_kd << endl;
        delete[] knnquery_time_arr_kd;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_r = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_r << endl;
        delete[] knnquery_time_arr_r;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_grid = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_grid << endl;
        delete[] knnquery_time_arr_grid;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli << endl;
        delete[] knnquery_time_arr_umli;
    }





    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void s1test_normal_7d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "7D TEST" << endl;

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

    // K-d Tree
    cout << "K-D Tree:\t";
    auto start_kd = high_resolution_clock::now();
    KDTree<int, double, 7> kdtree;
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
    cout << duration_kd.count() << " (us)" << endl;
    // R Tree
    cout << "R Tree:\t\t";
    auto start_r = high_resolution_clock::now();
    RTree<int, double, 7> rtree;
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
    cout << duration_r.count() << " (us)" << endl;
    // Grid
    cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
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

    vector<vector<double>> linear_scales_UMLI;
    for (int di = 0; di < dim; di++) {
        linear_scales_UMLI.push_back(scale1);
        linear_scales.push_back(lscale1);
    }
    //linear_scales.push_back(lscale5);
    auto start_grid = high_resolution_clock::now();
    Grid<int, double, 7> grid(linear_scales);
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
    cout << duration_grid.count() << " (us)" << endl;
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
    //linear_scales_UMLI.push_back(scale5);
    auto start_umli = high_resolution_clock::now();
    UMLI<int, double, 7> umli(id_arr, umli_build_arr, linear_scales_UMLI, 7);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;

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
    cout << "average point query time(us):\t" << avg_pointqueryTime_kd << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_r << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_grid << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_umli << endl;
    delete[] pointquery_time_arr_umli;



    //// Range Query
    cout << "=================================================" << endl;
    cout << "Range Query:" << endl;

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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_kd = avg_rangequeryTime_kd / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_kd << endl;
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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_r = avg_rangequeryTime_r / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_r << endl;
    delete[] rangequery_time_arr_r;
    //cout << "-------------------------------------------------" << endl;
    //cout << "Grid:" << endl;
    //int* rangequery_time_arr_grid = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_grid = 0.0;
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
    //    grid.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_grid[i] = duration.count();
    //    avg_rangequeryTime_grid += duration.count();
    //    //cout << Result.size() << endl;
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_grid = avg_rangequeryTime_grid / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_grid << endl;
    //delete[] rangequery_time_arr_grid;
    //cout << "-------------------------------------------------" << endl;
    //cout << "UMLI:" << endl;
    //int* rangequery_time_arr_umli = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_umli = 0.0;
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
    //    umli.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_umli[i] = duration.count();
    //    avg_rangequeryTime_umli += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //cout << Result.size() << endl;
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_umli = avg_rangequeryTime_umli / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_umli << endl;
    //delete[] rangequery_time_arr_umli;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_kd << endl;
        delete[] knnquery_time_arr_kd;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_r = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_r << endl;
        delete[] knnquery_time_arr_r;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_grid = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_grid << endl;
        delete[] knnquery_time_arr_grid;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_umli = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_umli << endl;
        delete[] knnquery_time_arr_umli;
    }





    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void s1test_normal_8d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "8D TEST" << endl;

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

    // K-d Tree
    cout << "K-D Tree:\t";
    auto start_kd = high_resolution_clock::now();
    KDTree<int, double, 8> kdtree;
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
    cout << duration_kd.count() << " (us)" << endl;
    // R Tree
    cout << "R Tree:\t\t";
    auto start_r = high_resolution_clock::now();
    RTree<int, double, 8> rtree;
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
    cout << duration_r.count() << " (us)" << endl;
    // Grid
    cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
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

    //vector<double> scale4 = { 0, 0.4, 0.8, 1.2, 1.6, 2, 2.4, 2.8, 3.2, 3.6, 4, 4.4, 4.8, 5.2, 5.6, 6, 6.4, 6.8, 7.2, 7.6, 8, 8.4, 8.8, 9.2, 9.6, 10 }; // NoC = 25

    //vector<vector<double>> linear_scales_UMLI;
    //for (int di = 0; di < dim; di++) {
    //    linear_scales_UMLI.push_back(scale1);
    //    linear_scales.push_back(lscale1);
    //}
    //auto start_grid = high_resolution_clock::now();
    //Grid<int, double, 8> grid(linear_scales);
    //for (int i = 0; i < BUILD_SIZE; i++)
    //{
    //    double* point = new double[dim];
    //    for (int j = 0; j < dim; ++j)
    //    {
    //        point[j] = build_arr[i][j];
    //    }
    //    int id = build_arr[i][dim];
    //    grid.Insert(point, id);
    //    delete[] point;
    //}
    //auto stop_grid = high_resolution_clock::now();
    //auto duration_grid = duration_cast<microseconds>(stop_grid - start_grid);
    //cout << duration_grid.count() << " (us)" << endl;
    ////grid.PrintGridSize();
    //// UMLI
    //cout << "UMLI:\t\t";
    //vector<int> id_arr;
    //vector<vector<double>> umli_build_arr;
    //sort(build_arr.begin(), build_arr.end());
    //for (int i = 0; i < BUILD_SIZE; ++i)
    //{
    //    id_arr.push_back(build_arr[i][dim]);
    //    vector<double> point;
    //    for (int j = 0; j < dim; ++j)
    //    {
    //        point.push_back(build_arr[i][j]);
    //    }
    //    umli_build_arr.push_back(point);
    //}
    ////linear_scales_UMLI.push_back(scale5);
    //auto start_umli = high_resolution_clock::now();
    //UMLI<int, double, 8> umli(id_arr, umli_build_arr, linear_scales_UMLI, 7);
    //auto stop_umli = high_resolution_clock::now();
    //auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    //cout << duration_umli.count() << " (us)" << endl;
    ////umli.PrintGridSize();
    ////umli.PrintGridCDF();





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;

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
    cout << "average point query time(us):\t" << avg_pointqueryTime_kd << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_r << endl;
    delete[] pointquery_time_arr_r;
    //cout << "-------------------------------------------------" << endl;
    //cout << "Grid:" << endl;
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
    //cout << "average point query time(us):\t" << avg_pointqueryTime_grid << endl;
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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_kd = avg_rangequeryTime_kd / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_kd << endl;
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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_r = avg_rangequeryTime_r / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_r << endl;
    delete[] rangequery_time_arr_r;
    //cout << "-------------------------------------------------" << endl;
    //cout << "Grid:" << endl;
    //int* rangequery_time_arr_grid = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_grid = 0.0;
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
    //    grid.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_grid[i] = duration.count();
    //    avg_rangequeryTime_grid += duration.count();
    //    //cout << Result.size() << endl;
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_grid = avg_rangequeryTime_grid / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_grid << endl;
    //delete[] rangequery_time_arr_grid;
    //cout << "-------------------------------------------------" << endl;
    //cout << "UMLI:" << endl;
    //int* rangequery_time_arr_umli = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_umli = 0.0;
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
    //    umli.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_umli[i] = duration.count();
    //    avg_rangequeryTime_umli += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //cout << Result.size() << endl;
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_umli = avg_rangequeryTime_umli / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_umli << endl;
    //delete[] rangequery_time_arr_umli;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_kd << endl;
        delete[] knnquery_time_arr_kd;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_r = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_r << endl;
        delete[] knnquery_time_arr_r;
    }
    //cout << "-------------------------------------------------" << endl;
    //cout << "Grid:" << endl;
    //for (int K = 1; K <= 101; K = K + 10) {
    //    int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
    //    double avg_knnqueryTime_grid = 0.0;
    //    cout << "K=" << K << endl;
    //    for (int i = 0; i < KNN_QUERY_SIZE; i++) {
    //        //cout << "Query" << i + 1 << ": \t";
    //        int* Result = new int[K];
    //        auto start = high_resolution_clock::now();
    //        grid.KNNQuery(Result, point_query[i], K);
    //        auto stop = high_resolution_clock::now();
    //        auto duration = duration_cast<microseconds>(stop - start);
    //        knnquery_time_arr_grid[i] = duration.count();
    //        avg_knnqueryTime_grid += duration.count();
    //        //sort(Result, Result + K);
    //        //for (int j = 0; j < K; j++) {
    //        //    cout << Result[j] << "\t";
    //        //}
    //        //cout << endl;
    //        delete[] Result;
    //    }
    //    avg_knnqueryTime_grid = avg_knnqueryTime_grid / KNN_QUERY_SIZE;
    //    cout << "average knn query time(us):\t" << avg_knnqueryTime_grid << endl;
    //    delete[] knnquery_time_arr_grid;
    //}
    //cout << "-------------------------------------------------" << endl;
    //cout << "UMLI:" << endl;
    //for (int K = 1; K <= 101; K = K + 10) {
    //    int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
    //    double avg_knnqueryTime_umli = 0.0;
    //    cout << "K=" << K << endl;
    //    for (int i = 0; i < KNN_QUERY_SIZE; i++) {
    //        //cout << "Query" << i + 1 << ": \t";
    //        int* Result = new int[K];
    //        auto start = high_resolution_clock::now();
    //        umli.KNNQuery(Result, point_query[i], K);
    //        auto stop = high_resolution_clock::now();
    //        auto duration = duration_cast<microseconds>(stop - start);
    //        knnquery_time_arr_umli[i] = duration.count();
    //        avg_knnqueryTime_umli += duration.count();
    //        //sort(Result, Result + K);
    //        //for (int j = 0; j < K; j++) {
    //        //    cout << Result[j] << "\t";
    //        //}
    //        //cout << endl;
    //        delete[] Result;
    //    }
    //    avg_knnqueryTime_umli = avg_knnqueryTime_umli / KNN_QUERY_SIZE;
    //    cout << "average knn query time(us):\t" << avg_knnqueryTime_umli << endl;
    //    delete[] knnquery_time_arr_umli;
   // }






    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void s1test_normal_9d(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "9D TEST" << endl;

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

    // K-d Tree
    cout << "K-D Tree:\t";
    auto start_kd = high_resolution_clock::now();
    KDTree<int, double, 9> kdtree;
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
    cout << duration_kd.count() << " (us)" << endl;
    // R Tree
    cout << "R Tree:\t\t";
    auto start_r = high_resolution_clock::now();
    RTree<int, double, 9> rtree;
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
    cout << duration_r.count() << " (us)" << endl;
    // Grid
    cout << "Grid:\t\t";
    vector<vector<double>> linear_scales;
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

    //vector<double> scale4 = { 0, 0.4, 0.8, 1.2, 1.6, 2, 2.4, 2.8, 3.2, 3.6, 4, 4.4, 4.8, 5.2, 5.6, 6, 6.4, 6.8, 7.2, 7.6, 8, 8.4, 8.8, 9.2, 9.6, 10 }; // NoC = 25

    //vector<vector<double>> linear_scales_UMLI;
    //for (int di = 0; di < dim; di++) {
    //    linear_scales_UMLI.push_back(scale1);
    //    linear_scales.push_back(lscale1);
    //}
    ////linear_scales.push_back(lscale5);
    //auto start_grid = high_resolution_clock::now();
    //Grid<int, double, 9> grid(linear_scales);
    //for (int i = 0; i < BUILD_SIZE; i++)
    //{
    //    double* point = new double[dim];
    //    for (int j = 0; j < dim; ++j)
    //    {
    //        point[j] = build_arr[i][j];
    //    }
    //    int id = build_arr[i][dim];
    //    grid.Insert(point, id);
    //    delete[] point;
    //}
    //auto stop_grid = high_resolution_clock::now();
    //auto duration_grid = duration_cast<microseconds>(stop_grid - start_grid);
    //cout << duration_grid.count() << " (us)" << endl;
    ////grid.PrintGridSize();
    //// UMLI
    //cout << "UMLI:\t\t";
    //vector<int> id_arr;
    //vector<vector<double>> umli_build_arr;
    //sort(build_arr.begin(), build_arr.end());
    //for (int i = 0; i < BUILD_SIZE; ++i)
    //{
    //    id_arr.push_back(build_arr[i][dim]);
    //    vector<double> point;
    //    for (int j = 0; j < dim; ++j)
    //    {
    //        point.push_back(build_arr[i][j]);
    //    }
    //    umli_build_arr.push_back(point);
    //}
    ////linear_scales_UMLI.push_back(scale5);
    //auto start_umli = high_resolution_clock::now();
    //UMLI<int, double, 9> umli(id_arr, umli_build_arr, linear_scales_UMLI, 7);
    //auto stop_umli = high_resolution_clock::now();
    //auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    //cout << duration_umli.count() << " (us)" << endl;
    ////umli.PrintGridSize();
    ////umli.PrintGridCDF();





    //// Point Query
    cout << "=================================================" << endl;
    cout << "Point Query:" << endl;

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
    cout << "average point query time(us):\t" << avg_pointqueryTime_kd << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_r << endl;
    delete[] pointquery_time_arr_r;
    //cout << "-------------------------------------------------" << endl;
    //cout << "Grid:" << endl;
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
    //cout << "average point query time(us):\t" << avg_pointqueryTime_grid << endl;
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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_kd = avg_rangequeryTime_kd / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_kd << endl;
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
        //cout << Result.size() << endl;
        //sort(Result.begin(), Result.end());
        //for (int j = 0; j < Result.size(); j++) {
        //    cout << Result[j] << "\t";
        //}
        //cout << endl;
        delete[] min;
        delete[] max;
    }
    avg_rangequeryTime_r = avg_rangequeryTime_r / RANGE_QUERY_SIZE;
    cout << "average range query time(us):\t" << avg_rangequeryTime_r << endl;
    delete[] rangequery_time_arr_r;
    //cout << "-------------------------------------------------" << endl;
    //cout << "Grid:" << endl;
    //int* rangequery_time_arr_grid = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_grid = 0.0;
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
    //    grid.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_grid[i] = duration.count();
    //    avg_rangequeryTime_grid += duration.count();
    //    //cout << Result.size() << endl;
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_grid = avg_rangequeryTime_grid / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_grid << endl;
    //delete[] rangequery_time_arr_grid;
    //cout << "-------------------------------------------------" << endl;
    //cout << "UMLI:" << endl;
    //int* rangequery_time_arr_umli = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_umli = 0.0;
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
    //    umli.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_umli[i] = duration.count();
    //    avg_rangequeryTime_umli += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //cout << Result.size() << endl;
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_umli = avg_rangequeryTime_umli / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_umli << endl;
    //delete[] rangequery_time_arr_umli;



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;

    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_kd << endl;
        delete[] knnquery_time_arr_kd;
    }
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_r = 0.0;
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
        cout << "average knn query time(us):\t" << avg_knnqueryTime_r << endl;
        delete[] knnquery_time_arr_r;
    }
    //cout << "-------------------------------------------------" << endl;
    //cout << "Grid:" << endl;
    //for (int K = 1; K <= 101; K = K + 10) {
    //    int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
    //    double avg_knnqueryTime_grid = 0.0;
    //    cout << "K=" << K << endl;
    //    for (int i = 0; i < KNN_QUERY_SIZE; i++) {
    //        //cout << "Query" << i + 1 << ": \t";
    //        int* Result = new int[K];
    //        auto start = high_resolution_clock::now();
    //        grid.KNNQuery(Result, point_query[i], K);
    //        auto stop = high_resolution_clock::now();
    //        auto duration = duration_cast<microseconds>(stop - start);
    //        knnquery_time_arr_grid[i] = duration.count();
    //        avg_knnqueryTime_grid += duration.count();
    //        //sort(Result, Result + K);
    //        //for (int j = 0; j < K; j++) {
    //        //    cout << Result[j] << "\t";
    //        //}
    //        //cout << endl;
    //        delete[] Result;
    //    }
    //    avg_knnqueryTime_grid = avg_knnqueryTime_grid / KNN_QUERY_SIZE;
    //    cout << "average knn query time(us):\t" << avg_knnqueryTime_grid << endl;
    //    delete[] knnquery_time_arr_grid;
    //}
    //cout << "-------------------------------------------------" << endl;
    //cout << "UMLI:" << endl;
    //for (int K = 1; K <= 101; K = K + 10) {
    //    int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
    //    double avg_knnqueryTime_umli = 0.0;
    //    cout << "K=" << K << endl;
    //    for (int i = 0; i < KNN_QUERY_SIZE; i++) {
    //        //cout << "Query" << i + 1 << ": \t";
    //        int* Result = new int[K];
    //        auto start = high_resolution_clock::now();
    //        umli.KNNQuery(Result, point_query[i], K);
    //        auto stop = high_resolution_clock::now();
    //        auto duration = duration_cast<microseconds>(stop - start);
    //        knnquery_time_arr_umli[i] = duration.count();
    //        avg_knnqueryTime_umli += duration.count();
    //        //sort(Result, Result + K);
    //        //for (int j = 0; j < K; j++) {
    //        //    cout << Result[j] << "\t";
    //        //}
    //        //cout << endl;
    //        delete[] Result;
    //    }
    //    avg_knnqueryTime_umli = avg_knnqueryTime_umli / KNN_QUERY_SIZE;
    //    cout << "average knn query time(us):\t" << avg_knnqueryTime_umli << endl;
    //    delete[] knnquery_time_arr_umli;
    //}





    cout << "==============================================================================================================" << endl;
    cout << endl;
}

int main() {


    //s1test_normal_2d("./Datasets/normal_2d_1000000.csv", "./Datasets/_range_normal_2d_1000.csv", 2);
    //s1test_normal_3d("./Datasets/normal_3d_1000000.csv", "./Datasets/_range_normal_3d_1000.csv", 3);
    //s1test_normal_4d("./Datasets/normal_4d_1000000.csv", "./Datasets/_range_normal_4d_1000.csv", 4);
    //s1test_normal_5d("./Datasets/normal_5d_1000000.csv", "./Datasets/_range_normal_5d_1000.csv", 5);
    //s1test_normal_6d("./Datasets/normal_6d_1000000.csv", "./Datasets/_range_normal_6d_1000.csv", 6);
    //s1test_normal_7d("./Datasets/normal_7d_1000000.csv", "./Datasets/_range_normal_7d_1000.csv", 7);
    s1test_normal_8d("./Datasets/normal_8d_1000000.csv", "./Datasets/_range_normal_8d_1000.csv", 8);
    s1test_normal_9d("./Datasets/normal_9d_1000000.csv", "./Datasets/_range_normal_9d_1000.csv", 9);


    system("pause");
    return 0;
}


