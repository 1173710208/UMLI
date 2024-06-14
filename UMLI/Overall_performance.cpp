/*
This is a program for evaluating overall performance.
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

void test_build_synthetic(const char* fileName, const char* rfileName, int dim)
{
    cout << "==============================================================================================================" << endl;
    cout << "dataset:"<< fileName <<endl;

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
    vector<double> lscale1 = { 0,1,2,3,4,5,6,7,8,9,10 };
    vector<double> lscale2 = { 0,1,2,3,4,5,6,7,8,9,10 };
    vector<double> lscale3 = { 0,1,2,3,4,5,6,7,8,9,10 };
    vector<double> lscale4 = { 0,1,2,3,4,5,6,7,8,9,10 };
    //vector<double> lscale5 = { 0,2,4,6,8,10 };
    vector<double> scale1 = { 0,1,2,3,4,5,6,7,8,9,10 };
    vector<double> scale2 = { 0,1,2,3,4,5,6,7,8,9,10 };
    vector<double> scale3 = { 0,1,2,3,4,5,6,7,8,9,10 };
    vector<double> scale4 = { 0,1,2,3,4,5,6,7,8,9,10 };
    //vector<double> scale1 = { 0, 3.35, 3.7, 3.95, 4.15, 4.345, 4.489, 4.6496, 4.7755, 4.8979, 5.0, 5.12, 5.27, 5.39, 5.532, 5.6811, 5.842, 6.04, 6.278, 6.638, 10 };
    //vector<double> scale2 = { 0, 3.35, 3.7, 3.95, 4.15, 4.345, 4.489, 4.6496, 4.7755, 4.8979, 5.0, 5.12, 5.27, 5.39, 5.532, 5.6811, 5.842, 6.04, 6.278, 6.638, 10 };
    //vector<double> scale3 = { 0, 3.35, 3.7, 3.95, 4.15, 4.345, 4.489, 4.6496, 4.7755, 4.8979, 5.0, 5.12, 5.27, 5.39, 5.532, 5.6811, 5.842, 6.04, 6.278, 6.638, 10 };
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
    UMLI<int, double, 4> umli(id_arr, umli_build_arr, linear_scales_UMLI, 10);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
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

//
//
//
//
void test_uniform_4d(const char* fileName, const char* rfileName, int dim)
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
    int BUILD_SIZE = 4999000;
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
            point_query[i][j] = data_arr[i * 4988][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i * 4988][j];
        }
    }

    //// Generate insert dataset
    cout << "Generate insert dataset" << endl;
    int INSERT_SIZE = 1000;
    vector<vector<double>> insert_arr;
    for (int i = 4999000; i < 5000000; ++i)
    {
        insert_arr.push_back(data_arr[i]);
    }


    //// Generate delete dataset
    cout << "Generate delete dataset" << endl;
    int DELETE_SIZE = 100;
    vector<vector<double>> delete_arr;
    for (int i = 4000000; i < 4000100; ++i)
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
    UMLI<int, double, 4> umli(id_arr, umli_build_arr, linear_scales_UMLI, 21);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_fs << endl;
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
    cout << "average range query time(us):\t" << avg_rangequeryTime_fs << endl;
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
    cout << "Full Scan:" << endl;
    int* knnquery_time_arr_fs = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_fs = 0.0;
    for (int K = 1; K <= 101; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_kd = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_r = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_r;
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_grid = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_umli = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_umli;



    //// Insertion
    cout << "=================================================" << endl;
    cout << "Insertion:" << endl;
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
        //cout << i << endl;
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
        //cout << i << endl;
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






    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void test_exponential_4d(const char* fileName, const char* rfileName, int dim)
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
    int BUILD_SIZE = 4999000;
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
            point_query[i][j] = data_arr[i * 4998][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i * 499800][j];
        }
    }

    //// Generate insert dataset
    cout << "Generate insert dataset" << endl;
    int INSERT_SIZE = 1000;
    vector<vector<double>> insert_arr;
    for (int i = 4999000; i < 5000000; ++i)
    {
        insert_arr.push_back(data_arr[i]);
    }


    //// Generate delete dataset
    cout << "Generate delete dataset" << endl;
    int DELETE_SIZE = 100;
    vector<vector<double>> delete_arr;
    for (int i = 4000000; i < 4000100; ++i)
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
    vector<double> scale1 = { 0,0.2,0.444,0.69,1.0,1.35,1.81,2.38,3.14,4.46,10};
    //vector<double> scale1 = { 0, 4.15, 4.7755, 5.27, 5.842, 10 };
    vector<double> scale2 = { 0,0.2,0.444,0.69,1.0,1.35,1.81,2.38,3.14,4.46,10};
    vector<double> scale3 = { 0,0.2,0.444,0.69,1.0,1.35,1.81,2.38,3.14,4.46,10};
    vector<double> scale4 = { 0,0.2,0.444,0.69,1.0,1.35,1.81,2.38,3.14,4.46,10};
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
    UMLI<int, double, 4> umli(id_arr, umli_build_arr, linear_scales_UMLI, 21);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
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
    cout << "average point query time(us):\t" << avg_pointqueryTime_fs << endl;
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
    cout << "average range query time(us):\t" << avg_rangequeryTime_fs << endl;
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
    cout << "Full Scan:" << endl;
    int* knnquery_time_arr_fs = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_fs = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_kd = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_r = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_r;
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_grid = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_umli = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_umli;






    //// Insertion
    cout << "=================================================" << endl;
    cout << "Insertion:" << endl;
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
        //cout << i << endl;
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
        //cout << i << endl;
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



    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void test_gamma_4d(const char* fileName, const char* rfileName, int dim)
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
    int BUILD_SIZE = 4999000;
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
            point_query[i][j] = data_arr[i * 4998][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i * 499800][j];
        }
    }

    //// Generate insert dataset
    cout << "Generate insert dataset" << endl;
    int INSERT_SIZE = 1000;
    vector<vector<double>> insert_arr;
    for (int i = 4999000; i < 5000000; ++i)
    {
        insert_arr.push_back(data_arr[i]);
    }


    //// Generate delete dataset
    cout << "Generate delete dataset" << endl;
    int DELETE_SIZE = 100;
    vector<vector<double>> delete_arr;
    for (int i = 4500000; i < 4500100; ++i)
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
    //// Full Scan
    //cout << "Full Scan:\t";
    //auto start_fs = high_resolution_clock::now();
    //FullScan<int, double, 4> fullscan;
    //for (int i = 0; i < BUILD_SIZE; i++)
    //{
    //    double* point = new double[dim];
    //    for (int j = 0; j < dim; ++j)
    //    {
    //        point[j] = build_arr[i][j];
    //    }
    //    int id = build_arr[i][dim];
    //    fullscan.Insert(point, id);
    //    delete[] point;
    //}
    //auto stop_fs = high_resolution_clock::now();
    //auto duration_fs = duration_cast<microseconds>(stop_fs - start_fs);
    //cout << duration_fs.count() << " (us)" << endl;
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
    UMLI<int, double, 4> umli(id_arr, umli_build_arr, linear_scales_UMLI, 21);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
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
    //cout << "-------------------------------------------------" << endl;
    //cout << "Full Scan:" << endl;
    //int* pointquery_time_arr_fs = new int[POINT_QUERY_SIZE]; //microseconds
    //double avg_pointqueryTime_fs = 0.0;
    //for (int i = 0; i < POINT_QUERY_SIZE; i++) {
    //    //cout << "Query" << i + 1 << ": \t";
    //    vector<int> Result;
    //    auto start = high_resolution_clock::now();
    //    fullscan.PointQuery(Result, point_query[i]);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    pointquery_time_arr_fs[i] = duration.count();
    //    avg_pointqueryTime_fs += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //}
    //avg_pointqueryTime_fs = avg_pointqueryTime_fs / POINT_QUERY_SIZE;
    //cout << "average point query time(us):\t" << avg_pointqueryTime_fs << endl;
    //delete[] pointquery_time_arr_fs;
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
    //cout << "-------------------------------------------------" << endl;
    //cout << "Full Scan:" << endl;
    //int* rangequery_time_arr_fs = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_fs = 0.0;
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
    //    fullscan.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_fs[i] = duration.count();
    //    avg_rangequeryTime_fs += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_fs = avg_rangequeryTime_fs / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_fs << endl;
    //delete[] rangequery_time_arr_fs;
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
    //cout << "-------------------------------------------------" << endl;
    //cout << "Full Scan:" << endl;
    //int* knnquery_time_arr_fs = new int[KNN_QUERY_SIZE];//microseconds
    //double avg_knnqueryTime_fs = 0.0;
    //for (int K = 51; K <= 51; K = K + 10) {
    //    cout << "K=" << K << endl;
    //    for (int i = 0; i < 1/*KNN_QUERY_SIZE*/; i++) {
    //        //cout << "Query" << i + 1 << ": \t";
    //        int* Result = new int[K];
    //        auto start = high_resolution_clock::now();
    //        fullscan.KNNQuery(Result, point_query[i], K);
    //        auto stop = high_resolution_clock::now();
    //        auto duration = duration_cast<microseconds>(stop - start);
    //        knnquery_time_arr_fs[i] = duration.count();
    //        avg_knnqueryTime_fs += duration.count();
    //        //sort(Result, Result + K);
    //        //for (int j = 0; j < K; j++) {
    //        //    cout << Result[j] << "\t";
    //        //}
    //        //cout << endl;
    //        delete[] Result;
    //    }
    //    avg_knnqueryTime_fs = avg_knnqueryTime_fs;
    //    cout << "average knn query time(us):\t" << avg_knnqueryTime_fs << endl;
    //}
    //delete[] knnquery_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_kd = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_r = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_r;
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_grid = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_umli = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_umli;




    //// Insertion
    cout << "=================================================" << endl;
    cout << "Insertion:" << endl;
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
        //cout << i << endl;
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
    cout << "UMLI:" << endl;
    int* delete_time_arr_umli = new int[DELETE_SIZE]; //microseconds
    double avg_deleteTime_umli = 0.0;
    for (int i = 0; i < DELETE_SIZE; i++) {
        //cout << i << endl;
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


void test_normal_4d(const char* fileName, const char* rfileName, int dim)
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
    int BUILD_SIZE = 4999000;
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
            point_query[i][j] = data_arr[i * 4998][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i * 499800][j];
        }
    }

    //// Generate insert dataset
    cout << "Generate insert dataset" << endl;
    int INSERT_SIZE = 1000;
    vector<vector<double>> insert_arr;
    for (int i = 4999000; i < 5000000; ++i)
    {
        insert_arr.push_back(data_arr[i]);
    }


    //// Generate delete dataset
    cout << "Generate delete dataset" << endl;
    int DELETE_SIZE = 100;
    vector<vector<double>> delete_arr;
    for (int i = 4500000; i < 4500100; ++i)
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
    UMLI<int, double, 4> umli(id_arr, umli_build_arr, linear_scales_UMLI,21);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
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
    //cout << "Full Scan:" << endl;
    //int* pointquery_time_arr_fs = new int[POINT_QUERY_SIZE]; //microseconds
    //double avg_pointqueryTime_fs = 0.0;
    //for (int i = 0; i < POINT_QUERY_SIZE; i++) {
    //    //cout << "Query" << i + 1 << ": \t";
    //    vector<int> Result;
    //    auto start = high_resolution_clock::now();
    //    fullscan.PointQuery(Result, point_query[i]);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    pointquery_time_arr_fs[i] = duration.count();
    //    avg_pointqueryTime_fs += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //}
    //avg_pointqueryTime_fs = avg_pointqueryTime_fs / POINT_QUERY_SIZE;
    //cout << "average point query time(us):\t" << avg_pointqueryTime_fs << endl;
    //delete[] pointquery_time_arr_fs;
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
    //cout << "-------------------------------------------------" << endl;
    //cout << "Full Scan:" << endl;
    //int* rangequery_time_arr_fs = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_fs = 0.0;
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
    //    fullscan.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_fs[i] = duration.count();
    //    avg_rangequeryTime_fs += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_fs = avg_rangequeryTime_fs / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_fs << endl;
    //delete[] rangequery_time_arr_fs;
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
    //cout << "-------------------------------------------------" << endl;
    //cout << "Full Scan:" << endl;
    //int* knnquery_time_arr_fs = new int[KNN_QUERY_SIZE];//microseconds
    //double avg_knnqueryTime_fs = 0.0;
    //for (int K = 51; K <= 51; K = K + 10) {
    //    cout << "K=" << K << endl;
    //    for (int i = 0; i < 1/*KNN_QUERY_SIZE*/; i++) {
    //        //cout << "Query" << i + 1 << ": \t";
    //        int* Result = new int[K];
    //        auto start = high_resolution_clock::now();
    //        fullscan.KNNQuery(Result, point_query[i], K);
    //        auto stop = high_resolution_clock::now();
    //        auto duration = duration_cast<microseconds>(stop - start);
    //        knnquery_time_arr_fs[i] = duration.count();
    //        avg_knnqueryTime_fs += duration.count();
    //        //sort(Result, Result + K);
    //        //for (int j = 0; j < K; j++) {
    //        //    cout << Result[j] << "\t";
    //        //}
    //        //cout << endl;
    //        delete[] Result;
    //    }
    //    avg_knnqueryTime_fs = avg_knnqueryTime_fs;
    //    cout << "average knn query time(us):\t" << avg_knnqueryTime_fs << endl;
    //}
    //delete[] knnquery_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_kd = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_r = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_r;
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_grid = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_umli = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_umli;





    //// Insertion
    cout << "=================================================" << endl;
    cout << "Insertion:" << endl;
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
        //cout << i << endl;
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
        //cout << i << endl;
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




    cout << "==============================================================================================================" << endl;
    cout << endl;
}


void test_chisquare_4d(const char* fileName, const char* rfileName, int dim)
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
    int BUILD_SIZE = 4999000;
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
            point_query[i][j] = data_arr[i * 4998][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i * 499800][j];
        }
    }

    //// Generate insert dataset
    cout << "Generate insert dataset" << endl;
    int INSERT_SIZE = 1000;
    vector<vector<double>> insert_arr;
    for (int i = 4999000; i < 5000000; ++i)
    {
        insert_arr.push_back(data_arr[i]);
    }


    //// Generate delete dataset
    cout << "Generate delete dataset" << endl;
    int DELETE_SIZE = 100;
    vector<vector<double>> delete_arr;
    for (int i = 4500000; i < 4500100; ++i)
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
    vector<double> scale1 = { 0, 3.61, 4.5879, 5.3341, 6.01, 6.63, 7.233, 7.876, 8.54, 9.244, 10};
    vector<double> scale2 = { 0, 3.61, 4.5879, 5.3341, 6.01, 6.63, 7.233, 7.876, 8.54, 9.244, 10};
    vector<double> scale3 = { 0, 3.61, 4.5879, 5.3341, 6.01, 6.63, 7.233, 7.876, 8.54, 9.244, 10};
    vector<double> scale4 = { 0, 3.61, 4.5879, 5.3341, 6.01, 6.63, 7.233, 7.876, 8.54, 9.244, 10};
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
    UMLI<int, double, 4> umli(id_arr, umli_build_arr, linear_scales_UMLI, 21);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
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
    //cout << "-------------------------------------------------" << endl;
    //cout << "Full Scan:" << endl;
    //int* pointquery_time_arr_fs = new int[POINT_QUERY_SIZE]; //microseconds
    //double avg_pointqueryTime_fs = 0.0;
    //for (int i = 0; i < POINT_QUERY_SIZE; i++) {
    //    //cout << "Query" << i + 1 << ": \t";
    //    vector<int> Result;
    //    auto start = high_resolution_clock::now();
    //    fullscan.PointQuery(Result, point_query[i]);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    pointquery_time_arr_fs[i] = duration.count();
    //    avg_pointqueryTime_fs += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //}
    //avg_pointqueryTime_fs = avg_pointqueryTime_fs / POINT_QUERY_SIZE;
    //cout << "average point query time(us):\t" << avg_pointqueryTime_fs << endl;
    //delete[] pointquery_time_arr_fs;
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
    //cout << "-------------------------------------------------" << endl;
    //cout << "Full Scan:" << endl;
    //int* rangequery_time_arr_fs = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_fs = 0.0;
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
    //    fullscan.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_fs[i] = duration.count();
    //    avg_rangequeryTime_fs += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_fs = avg_rangequeryTime_fs / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_fs << endl;
    //delete[] rangequery_time_arr_fs;
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
    //cout << "-------------------------------------------------" << endl;
    //cout << "Full Scan:" << endl;
    //int* knnquery_time_arr_fs = new int[KNN_QUERY_SIZE];//microseconds
    //double avg_knnqueryTime_fs = 0.0;
    //for (int K = 1; K <= 101; K = K + 10) {
    //    cout << "K=" << K << endl;
    //    for (int i = 0; i < 1/*KNN_QUERY_SIZE*/; i++) {
    //        //cout << "Query" << i + 1 << ": \t";
    //        int* Result = new int[K];
    //        auto start = high_resolution_clock::now();
    //        fullscan.KNNQuery(Result, point_query[i], K);
    //        auto stop = high_resolution_clock::now();
    //        auto duration = duration_cast<microseconds>(stop - start);
    //        knnquery_time_arr_fs[i] = duration.count();
    //        avg_knnqueryTime_fs += duration.count();
    //        //sort(Result, Result + K);
    //        //for (int j = 0; j < K; j++) {
    //        //    cout << Result[j] << "\t";
    //        //}
    //        //cout << endl;
    //        delete[] Result;
    //    }
    //    avg_knnqueryTime_fs = avg_knnqueryTime_fs;
    //    cout << "average knn query time(us):\t" << avg_knnqueryTime_fs << endl;
    //}
    //delete[] knnquery_time_arr_fs;
    //cout << "-------------------------------------------------" << endl;
    //cout << "K-d Tree:" << endl;
    //int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
    //double avg_knnqueryTime_kd = 0.0;
    //for (int K = 1; K <= 101; K = K + 10) {
    //    cout << "K=" << K << endl;
    //    for (int i = 0; i < KNN_QUERY_SIZE; i++) {
    //        //cout << "Query" << i + 1 << ": \t";
    //        int* Result = new int[K];
    //        auto start = high_resolution_clock::now();
    //        kdtree.KNNQuery(Result, point_query[i], K);
    //        auto stop = high_resolution_clock::now();
    //        auto duration = duration_cast<microseconds>(stop - start);
    //        knnquery_time_arr_kd[i] = duration.count();
    //        avg_knnqueryTime_kd += duration.count();
    //        //sort(Result, Result + K);
    //        //for (int j = 0; j < K; j++) {
    //        //    cout << Result[j] << "\t";
    //        //}
    //        //cout << endl;
    //        delete[] Result;
    //    }
    //    avg_knnqueryTime_kd = avg_knnqueryTime_kd / KNN_QUERY_SIZE;
    //    cout << "average knn query time(us):\t" << avg_knnqueryTime_kd << endl;
    //}
    //delete[] knnquery_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_r = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_r;
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_grid = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_umli = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_umli;




    //// Insertion
    cout << "=================================================" << endl;
    cout << "Insertion:" << endl;
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
        //cout << i << endl;
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
    cout << "UMLI:" << endl;
    int* delete_time_arr_umli = new int[DELETE_SIZE]; //microseconds
    double avg_deleteTime_umli = 0.0;
    for (int i = 0; i < DELETE_SIZE; i++) {
        //cout << i << endl;
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


void test_f_4d(const char* fileName, const char* rfileName, int dim)
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
    int BUILD_SIZE = 4999000;
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
            point_query[i][j] = data_arr[i * 4998][j];
        }
    }
    for (int i = 0; i < KNN_QUERY_SIZE; ++i)
    {
        knn_query[i] = new double[dim];
        for (int j = 0; j < dim; j++)
        {
            knn_query[i][j] = data_arr[i * 499800][j];
        }
    }

    //// Generate insert dataset
    cout << "Generate insert dataset" << endl;
    int INSERT_SIZE = 1000;
    vector<vector<double>> insert_arr;
    for (int i = 4999000; i < 5000000; ++i)
    {
        insert_arr.push_back(data_arr[i]);
    }


    //// Generate delete dataset
    cout << "Generate delete dataset" << endl;
    int DELETE_SIZE = 100;
    vector<vector<double>> delete_arr;
    for (int i = 4500000; i < 4500100; ++i)
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
    UMLI<int, double, 4> umli(id_arr, umli_build_arr, linear_scales_UMLI, 15);
    auto stop_umli = high_resolution_clock::now();
    auto duration_umli = duration_cast<microseconds>(stop_umli - start_umli);
    cout << duration_umli.count() << " (us)" << endl;
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
    //cout << "-------------------------------------------------" << endl;
    //cout << "Full Scan:" << endl;
    //int* pointquery_time_arr_fs = new int[POINT_QUERY_SIZE]; //microseconds
    //double avg_pointqueryTime_fs = 0.0;
    //for (int i = 0; i < POINT_QUERY_SIZE; i++) {
    //    //cout << "Query" << i + 1 << ": \t";
    //    vector<int> Result;
    //    auto start = high_resolution_clock::now();
    //    fullscan.PointQuery(Result, point_query[i]);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    pointquery_time_arr_fs[i] = duration.count();
    //    avg_pointqueryTime_fs += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //}
    //avg_pointqueryTime_fs = avg_pointqueryTime_fs / POINT_QUERY_SIZE;
    //cout << "average point query time(us):\t" << avg_pointqueryTime_fs << endl;
    //delete[] pointquery_time_arr_fs;
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
    //cout << "-------------------------------------------------" << endl;
    //cout << "Full Scan:" << endl;
    //int* rangequery_time_arr_fs = new int[RANGE_QUERY_SIZE];//microseconds
    //double avg_rangequeryTime_fs = 0.0;
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
    //    fullscan.RangeQuery(Result, min, max);
    //    auto stop = high_resolution_clock::now();
    //    auto duration = duration_cast<microseconds>(stop - start);
    //    rangequery_time_arr_fs[i] = duration.count();
    //    avg_rangequeryTime_fs += duration.count();
    //    //sort(Result.begin(), Result.end());
    //    //for (int j = 0; j < Result.size(); j++) {
    //    //    cout << Result[j] << "\t";
    //    //}
    //    //cout << endl;
    //    delete[] min;
    //    delete[] max;
    //}
    //avg_rangequeryTime_fs = avg_rangequeryTime_fs / RANGE_QUERY_SIZE;
    //cout << "average range query time(us):\t" << avg_rangequeryTime_fs << endl;
    //delete[] rangequery_time_arr_fs;
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
    //cout << "-------------------------------------------------" << endl;
    //cout << "Full Scan:" << endl;
    //int* knnquery_time_arr_fs = new int[KNN_QUERY_SIZE];//microseconds
    //double avg_knnqueryTime_fs = 0.0;
    //for (int K = 1; K <= 101; K = K + 10) {
    //    cout << "K=" << K << endl;
    //    for (int i = 0; i < 1/*KNN_QUERY_SIZE*/; i++) {
    //        //cout << "Query" << i + 1 << ": \t";
    //        int* Result = new int[K];
    //        auto start = high_resolution_clock::now();
    //        fullscan.KNNQuery(Result, point_query[i], K);
    //        auto stop = high_resolution_clock::now();
    //        auto duration = duration_cast<microseconds>(stop - start);
    //        knnquery_time_arr_fs[i] = duration.count();
    //        avg_knnqueryTime_fs += duration.count();
    //        //sort(Result, Result + K);
    //        //for (int j = 0; j < K; j++) {
    //        //    cout << Result[j] << "\t";
    //        //}
    //        //cout << endl;
    //        delete[] Result;
    //    }
    //    avg_knnqueryTime_fs = avg_knnqueryTime_fs;
    //    cout << "average knn query time(us):\t" << avg_knnqueryTime_fs << endl;
    //}
    //delete[] knnquery_time_arr_fs;
    cout << "-------------------------------------------------" << endl;
    cout << "K-d Tree:" << endl;
    int* knnquery_time_arr_kd = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_kd = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_kd;
    cout << "-------------------------------------------------" << endl;
    cout << "R Tree:" << endl;
    int* knnquery_time_arr_r = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_r = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_r;
    cout << "-------------------------------------------------" << endl;
    cout << "Grid:" << endl;
    int* knnquery_time_arr_grid = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_grid = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_grid;
    cout << "-------------------------------------------------" << endl;
    cout << "UMLI:" << endl;
    int* knnquery_time_arr_umli = new int[KNN_QUERY_SIZE];//microseconds
    double avg_knnqueryTime_umli = 0.0;
    for (int K = 51; K <= 51; K = K + 10) {
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
    }
    delete[] knnquery_time_arr_umli;





    //// Insertion
    cout << "=================================================" << endl;
    cout << "Insertion:" << endl;
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
        //cout << i << endl;
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
        //cout << i << endl;
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




    cout << "==============================================================================================================" << endl;
    cout << endl;
}


int main22() {

    //test_build_synthetic("./Datasets/uniform_4d_1000000.csv", "./Datasets/_range_uniform_4d_1000.csv", 4);
    //test_build_synthetic("./Datasets/exponential_4d_1000000.csv", "./Datasets/_range_uniform_4d_1000.csv", 4);
    //test_build_synthetic("./Datasets/gamma_4d_1000000.csv", "./Datasets/_range_uniform_4d_1000.csv", 4);
    //test_build_synthetic("./Datasets/normal_4d_1000000.csv", "./Datasets/_range_uniform_4d_1000.csv", 4);
    //test_build_synthetic("./Datasets/chisquared_4d_1000000.csv", "./Datasets/_range_uniform_4d_1000.csv", 4);
    //test_build_synthetic("./Datasets/f_4d_1000000.csv", "./Datasets/_range_uniform_4d_1000.csv", 4);

    //test_uniform_4d("./Datasets/uniform_4d_5000000.csv", "./Datasets/4d_1000_range.csv", 4);
    //test_exponential_4d("./Datasets/exponential_4d_5000000.csv", "./Datasets/_range_exponential_4d_1000.csv", 4);
    test_gamma_4d("./Datasets/gamma_4d_5000000.csv", "./Datasets/_range_gamma_4d_1000.csv", 4);
    //test_normal_4d("./Datasets/normal_4d_5000000.csv", "./Datasets/_range_normal_4d_1000.csv", 4);
    //test_chisquare_4d("./Datasets/chisquared_4d_5000000.csv", "./Datasets/_range_chisquared_4d_1000.csv", 4);
    //test_f_4d("./Datasets/f_4d_5000000.csv", "./Datasets/_range_f_4d_1000.csv", 4);


    system("pause");
    return 0;
}