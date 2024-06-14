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

void s4test_uniform_4d(const char* fileName, const char* rfileName, int dim)
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



    //// Build all indexes
    cout << "=================================================" << endl;
    cout << "Build all indexes: " << endl;
    // Full Scan
    cout << "Full Scan:\t";
    auto start_fs = high_resolution_clock::now();
    FullScan<int, double, 4> fullscan;
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
    cout << duration_fs.count() << " (us)" << endl;
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
    //vector<double> scale1 = { 0, 4.15, 4.7755, 5.27, 5.842, 10 };
    vector<double> scale1 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    vector<double> scale2 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    vector<double> scale3 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    vector<double> scale4 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
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
    UMLI<int, double, 4> umli(id_arr, umli_build_arr, linear_scales_UMLI, 6);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();



    // KNN Query
    cout << "=================================================" << endl;
    cout << "KNN Query:" << endl;
    cout << "-------------------------------------------------" << endl;
    cout << "Full Scan:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_fs = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_fs = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < 1/*KNN_QUERY_SIZE*/; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            fullscan.KNNQuery(Result, point_query[i], K);
            auto stop = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(stop - start);
            knnquery_time_arr_fs[i] = duration.count();
            avg_knnqueryTime_fs += duration.count();
            //sort(Result, Result + K);
            //for (int j = 0; j < K; j++) {
            //    cout << Result[j] << "\t";
            //}
            //cout << endl;
            delete[] Result;
        }
        avg_knnqueryTime_fs = avg_knnqueryTime_fs;
        cout << "average knn query time(us):\t" << avg_knnqueryTime_fs << endl;
        delete[] knnquery_time_arr_fs;
    }


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
    for (int K = 201; K <= 501; K = K + 100) {
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
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
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
    cout << "Grid:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
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
    cout << "UMLI:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
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




    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void s4test_exponential_4d(const char* fileName, const char* rfileName, int dim)
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
    // Full Scan
    cout << "Full Scan:\t";
    auto start_fs = high_resolution_clock::now();
    FullScan<int, double, 4> fullscan;
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
    cout << duration_fs.count() << " (us)" << endl;
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
    vector<double> scale1 = { 0,0.2,0.444,0.69,1.0,1.35,1.81,2.38,3.14,4.46,10 };
    //vector<double> scale1 = { 0, 4.15, 4.7755, 5.27, 5.842, 10 };
    vector<double> scale2 = { 0,0.2,0.444,0.69,1.0,1.35,1.81,2.38,3.14,4.46,10 };
    vector<double> scale3 = { 0,0.2,0.444,0.69,1.0,1.35,1.81,2.38,3.14,4.46,10 };
    vector<double> scale4 = { 0,0.2,0.444,0.69,1.0,1.35,1.81,2.38,3.14,4.46,10 };
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
    for (int K = 201; K <= 501; K = K + 100) {
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
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
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
    cout << "Grid:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
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
    cout << "UMLI:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
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





    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void s4test_gamma_4d(const char* fileName, const char* rfileName, int dim)
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
    // Full Scan
    cout << "Full Scan:\t";
    auto start_fs = high_resolution_clock::now();
    FullScan<int, double, 4> fullscan;
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
    cout << duration_fs.count() << " (us)" << endl;
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
    vector<double> scale1 = { 0, 6.64, 7.42, 7.935, 8.3545, 8.735, 9.0376, 9.3027, 9.562, 9.79, 10 };
    vector<double> scale2 = { 0, 6.64, 7.42, 7.935, 8.3545, 8.735, 9.0376, 9.3027, 9.562, 9.79, 10 };
    vector<double> scale3 = { 0, 6.64, 7.42, 7.935, 8.3545, 8.735, 9.0376, 9.3027, 9.562, 9.79, 10 };
    vector<double> scale4 = { 0, 6.64, 7.42, 7.935, 8.3545, 8.735, 9.0376, 9.3027, 9.562, 9.79, 10 };
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
    UMLI<int, double, 4> umli(id_arr, umli_build_arr, linear_scales_UMLI, 8);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();



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
    for (int K = 201; K <= 501; K = K + 100) {
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
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
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
    cout << "Grid:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
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
    cout << "UMLI:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
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





    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void s4test_normal_4d(const char* fileName, const char* rfileName, int dim)
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
    // Full Scan
    cout << "Full Scan:\t";
    auto start_fs = high_resolution_clock::now();
    FullScan<int, double, 4> fullscan;
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
    cout << duration_fs.count() << " (us)" << endl;
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
    for (int K = 201; K <= 501; K = K + 100) {
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
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
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
    cout << "Grid:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
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
    cout << "UMLI:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
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




    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void s4test_chisquare_4d(const char* fileName, const char* rfileName, int dim)
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
    // Full Scan
    cout << "Full Scan:\t";
    auto start_fs = high_resolution_clock::now();
    FullScan<int, double, 4> fullscan;
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
    cout << duration_fs.count() << " (us)" << endl;
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
    vector<double> scale1 = { 0, 3.61, 4.5879, 5.3341, 6.01, 6.63, 7.233, 7.876, 8.54, 9.244, 10 };
    vector<double> scale2 = { 0, 3.61, 4.5879, 5.3341, 6.01, 6.63, 7.233, 7.876, 8.54, 9.244, 10 };
    vector<double> scale3 = { 0, 3.61, 4.5879, 5.3341, 6.01, 6.63, 7.233, 7.876, 8.54, 9.244, 10 };
    vector<double> scale4 = { 0, 3.61, 4.5879, 5.3341, 6.01, 6.63, 7.233, 7.876, 8.54, 9.244, 10 };
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
    UMLI<int, double, 4> umli(id_arr, umli_build_arr, linear_scales_UMLI, 8);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();



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
    for (int K = 201; K <= 501; K = K + 100) {
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
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
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
    cout << "Grid:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
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
    cout << "UMLI:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
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





    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void s4test_f_4d(const char* fileName, const char* rfileName, int dim)
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
    // Full Scan
    cout << "Full Scan:\t";
    auto start_fs = high_resolution_clock::now();
    FullScan<int, double, 4> fullscan;
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
    cout << duration_fs.count() << " (us)" << endl;
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
    vector<double> scale1 = { 0, 0.788, 0.8509, 0.904, 0.955, 1.0, 1.056, 1.1142, 1.19, 1.3038, 10 };
    //vector<double> scale1 = { 0, 4.15, 4.7755, 5.27, 5.842, 10 };
    vector<double> scale2 = { 0, 0.788, 0.8509, 0.904, 0.955, 1.0, 1.056, 1.1142, 1.19, 1.3038, 10 };
    vector<double> scale3 = { 0, 0.788, 0.8509, 0.904, 0.955, 1.0, 1.056, 1.1142, 1.19, 1.3038, 10 };
    vector<double> scale4 = { 0, 0.788, 0.8509, 0.904, 0.955, 1.0, 1.056, 1.1142, 1.19, 1.3038, 10 };
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
    UMLI<int, double, 4> umli(id_arr, umli_build_arr, linear_scales_UMLI, 8);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
    //umli.PrintGridSize();
    //umli.PrintGridCDF();



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
    for (int K = 201; K <= 501; K = K + 100) {
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
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            rtree.KNNQuery(Result, point_query[i], K);
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
    cout << "Grid:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            grid.KNNQuery(Result, point_query[i], K);
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
    cout << "UMLI:" << endl;
    for (int K = 1; K <= 101; K = K + 10) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
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
    for (int K = 201; K <= 501; K = K + 100) {
        int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
        double avg_knnqueryTime_kd = 0.0;
        cout << "K=" << K << endl;
        for (int i = 0; i < KNN_QUERY_SIZE; i++) {
            //cout << "Query" << i + 1 << ": \t";
            int* Result = new int[K];
            auto start = high_resolution_clock::now();
            umli.KNNQuery(Result, point_query[i], K);
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





    cout << "==============================================================================================================" << endl;
    cout << endl;
}


int mainS4() {


    //s4test_uniform_4d("./Datasets/uniform_4d_1000000.csv", "./Datasets/4d_1000_range.csv", 4);
    //s4test_exponential_4d("./Datasets/exponential_4d_1000000.csv", "./Datasets/_range_exponential_4d_1000.csv", 4);
    //s4test_gamma_4d("./Datasets/gamma_4d_1000000.csv", "./Datasets/_range_gamma_4d_1000.csv", 4);
    s4test_normal_4d("./Datasets/normal_4d_1000000.csv", "./Datasets/_range_normal_4d_1000.csv", 4);
    //s4test_chisquare_4d("./Datasets/chisquared_4d_1000000.csv", "./Datasets/_range_chisquared_4d_1000.csv", 4);
    //s4test_f_4d("./Datasets/f_4d_1000000.csv", "./Datasets/_range_f_4d_1000.csv", 4);


    system("pause");
    return 0;
}