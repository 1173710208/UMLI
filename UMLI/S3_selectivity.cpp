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


void s3test_normal_4d(const char* fileName, const char* rfileName, int dim)
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
    int BUILD_SIZE = DATA_SIZE;
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
            point_query[i][j] = data_arr[i *998][j];
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






    cout << "==============================================================================================================" << endl;
    cout << endl;
}



int mains3() {

    s3test_normal_4d("./Datasets/normal_4d_1000000.csv", "./Datasets/0.05_range_normal_4d_1000.csv", 4);
    s3test_normal_4d("./Datasets/normal_4d_1000000.csv", "./Datasets/0.15_range_normal_4d_1000.csv", 4);
    s3test_normal_4d("./Datasets/normal_4d_1000000.csv", "./Datasets/0.2_range_normal_4d_1000.csv", 4);


    system("pause");
    return 0;
}