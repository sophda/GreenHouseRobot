
// 从滤波到三角化到求面积
#include <pcl/point_types.h>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/radius_outlier_removal.h>
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/surface/mls.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/surface/poisson.h> 
#include <pcl/io/obj_io.h>


///////////////////////加入了最小二乘法
#include <pcl/point_types.h>    
#include <pcl/io/ply_io.h>  
#include <pcl/io/obj_io.h>
#include <pcl/PolygonMesh.h>
#include <pcl/io/vtk_lib_io.h>
#include <pcl/kdtree/kdtree_flann.h>  
#include <pcl/features/normal_3d.h>    
#include <pcl/visualization/pcl_visualizer.h>  
#include <boost/thread/thread.hpp>  
#include <fstream>  
#include <iostream>  
#include <stdio.h>  
#include <string.h>  
#include <string>  
#include <vtkOBJReader.h>
#include <vtkTriangleFilter.h>
#include <vtkSmartPointer.h>
#include <vtkMassProperties.h>
using namespace std;
/////
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

typedef pcl::PointXYZRGB  PointT;
typedef pcl::PointCloud<PointT> PointCloudT;
typedef pcl::PointCloud<PointT> PointCloud;

////相机内参
const double camera_factor = 1000;
const double camera_cx = 682.3;
const double camera_cy = 254.9;
const double camera_fx = 979.8; 
const double camera_fy = 942.8;
PointCloud::Ptr construct_pointcloud(cv::Mat img_rgb, cv::Mat img_dep);
float Heron_area(pcl::PolygonMesh mesh);
//接口函数
extern "C"
{
	__declspec(dllexport) float ret_area(int rows, int cols, unsigned char* jpg, unsigned char* dep);
	__declspec(dllexport) int construct_pcd_with_rawdata(int rows, int cols, unsigned char* jpg, 
		unsigned char* dep);
}

float ret_area(int rows, int cols, unsigned char* jpg, unsigned char* dep)

{	

	cv::Mat img_rgb = cv::Mat(rows, cols, CV_8UC1, jpg);
	cv::Mat img_dep = cv::Mat(rows, cols,CV_16U, dep);

	PointCloud::Ptr cloud(new PointCloud);
	
	cloud = construct_pointcloud(img_rgb, img_dep);


	pcl::PointCloud<PointT>::Ptr cloud_downSampled(new pcl::PointCloud<PointT>);
	pcl::PointCloud<PointT>::Ptr cloud_filtered(new pcl::PointCloud<PointT>);
	pcl::PointCloud<PointT>::Ptr cloud_smoothed(new pcl::PointCloud<PointT>);
	// 下采样
	pcl::VoxelGrid<PointT> downSampled;  //创建滤波对象
	downSampled.setInputCloud(cloud);            //设置需要过滤的点云给滤波对象
	downSampled.setLeafSize(0.01f, 0.01f, 0.01f);  //设置滤波时创建的体素体积为1cm的立方体
	downSampled.filter(*cloud_downSampled);           //执行滤波处理，存储输出


	pcl::StatisticalOutlierRemoval<PointT> statisOutlierRemoval;       //创建滤波器对象
	statisOutlierRemoval.setInputCloud(cloud_downSampled);            //设置待滤波的点云
	statisOutlierRemoval.setMeanK(50);                                //设置在进行统计时考虑查询点临近点数
	statisOutlierRemoval.setStddevMulThresh(1.0);                     //设置判断是否为离群点的阀值:均值+1.0*标准差
	statisOutlierRemoval.filter(*cloud_filtered);                     //滤波结果存储到cloud_filtered


	// 创建 KD-Tree
	pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGB>);

	// Output has the PointNormal type in order to store the normals calculated by MLS
	pcl::PointCloud<pcl::PointXYZRGB> mls_points;

	// 定义最小二乘实现的对象mls
	pcl::MovingLeastSquares<pcl::PointXYZRGB, pcl::PointXYZRGB> mls;
	mls.setComputeNormals(true);  //设置在最小二乘计算中需要进行法线估计
	// Set parameters
	mls.setInputCloud(cloud_filtered);
	mls.setPolynomialFit(true);
	mls.setSearchMethod(tree);
	mls.setSearchRadius(0.03);
	// Reconstruct
	mls.process(mls_points);
	// Save output   这个得到的是最小二乘法的拟合后的点云
	pcl::io::savePCDFile("2-4.pcd", mls_points);

	//pcl::PointCloud<pcl::PointXYZ>::Ptr mlscloud(new pcl::PointCloud<pcl::PointXYZ>);
	typedef pcl::PointXYZ PointT;
	typedef pcl::PointCloud<PointT> PointCloudT;
	PointCloudT::Ptr mlscloud(new PointCloudT);

	//mlscloud = mls_points.makeShared();
	if (pcl::io::loadPCDFile<pcl::PointXYZ>("2-4.pcd", *mlscloud) == -1)
	{
		PCL_ERROR("Couldn't read file \n");
		return (-1);
	}

	//if (pcl::io::loadPCDFile<pcl::PointXYZ>("2-4.pcd", *mlscloud) == -1)
	//{
	//	PCL_ERROR("Cloudn't read file!");
	//	return -1;
	//}


	//cout << "there are " << mlscloud->points.size() << " afer mls" << endl;

	// 估计法向量  
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree_1(new pcl::search::KdTree<pcl::PointXYZ>);
	tree_1->setInputCloud(mlscloud);
	n.setInputCloud(mlscloud);
	n.setSearchMethod(tree_1);
	n.setKSearch(20);
	n.compute(*normals);

	pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);
	pcl::concatenateFields(*mlscloud, *normals, *cloud_with_normals); 
	pcl::search::KdTree<pcl::PointNormal>::Ptr tree2(new pcl::search::KdTree<pcl::PointNormal>);
	tree2->setInputCloud(cloud_with_normals);


	//初始化GreedyProjectionTriangulation对象，并设置参数  
	pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
	//创建多变形网格，用于存储结果  
	pcl::PolygonMesh triangles;

	//设置GreedyProjectionTriangulation对象的参数  
	//第一个参数影响很大  
	gp3.setSearchRadius(200.0f);           //设置连接点之间的最大距离（最大边长）用于确定k近邻的球半径【默认值 0】  
	gp3.setMu(2.5f);                       //设置最近邻距离的乘子，以得到每个点的最终搜索半径【默认值 0】  
	gp3.setMaximumNearestNeighbors(100);   //设置搜索的最近邻点的最大数量  
	gp3.setMaximumSurfaceAngle(M_PI / 4);  // 45 degrees（pi）最大平面角  
	gp3.setMinimumAngle(M_PI / 18);        // 10 degrees 每个三角的最小角度  
	gp3.setMaximumAngle(2 * M_PI / 3);     // 120 degrees 每个三角的最大角度  
	gp3.setNormalConsistency(false);       //如果法向量一致，设置为true  

	//设置搜索方法和输入点云  
	gp3.setInputCloud(cloud_with_normals);
	gp3.setSearchMethod(tree2);

	//执行重构，结果保存在triangles中  
	gp3.reconstruct(triangles);

	//float heron = Heron_area(triangles);
	//保存网格图  
	//pcl::io::saveOBJFile("result.obj", triangles);
	pcl::io::saveOBJFile("201.obj", triangles);

	vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
	reader->SetFileName("201.obj");
	reader->Update();

	vtkSmartPointer< vtkTriangleFilter > triangleFilter = vtkSmartPointer< vtkTriangleFilter >::New();
	triangleFilter->SetInputData(reader->GetOutput());
	triangleFilter->Update();

	vtkSmartPointer<vtkMassProperties> polygonProperties = vtkSmartPointer< vtkMassProperties >::New();
	polygonProperties->SetInputData(triangleFilter->GetOutput());
	polygonProperties->Update();


	double vol = polygonProperties->GetVolume();
	double area = polygonProperties->GetSurfaceArea();

	return area;

}

PointCloud::Ptr construct_pointcloud(cv::Mat img_rgb, cv::Mat img_dep)
{

	//cv::Mat img_rgb = cv::imread("a.jpg");
	//cv::Mat img_dep = cv::imread("0.png",-1);
	//转点云

	PointCloud::Ptr cloud(new PointCloud);
	for (int m = 0; m < img_dep.rows; m++)
		for (int n = 0; n < img_dep.cols; n++)
		{
			//g_pers.at<cv::Vec3b>(i, j);
			ushort d = img_dep.ptr<ushort>(m)[n];
			//cout << rgb_n << "   " << d << endl;
			if (d == 0 || d >= 4096)
				continue;
			PointT p;

			p.z = double(d) / camera_factor;
			p.x = (n - camera_cx) * p.z / camera_fx;
			p.y = (m - camera_cy) * p.z / camera_fy;
			p.b = img_rgb.ptr<uchar>(m)[n * 3];
			p.g = img_rgb.ptr<uchar>(m)[n * 3 + 1];
			p.r = img_rgb.ptr<uchar>(m)[n * 3 + 2];

			if (p.b + p.g + p.r > 10)

			{
				cloud->points.push_back(p);
			}

		}

	cloud->height = 1;
	cloud->width = cloud->points.size();
	cloud->is_dense = false;
	return cloud;
}

int construct_pcd_with_rawdata(int rows, int cols, unsigned char* jpg,
	unsigned char* dep)
{

	cv::Mat img_rgb = cv::Mat(rows, cols, CV_8UC1, jpg);
	cv::Mat img_dep = cv::Mat(rows, cols, CV_16U, dep);

	PointCloud::Ptr cloud(new PointCloud);

	cloud = construct_pointcloud(img_rgb, img_dep);

	pcl::PointCloud<PointT>::Ptr cloud_downSampled(new pcl::PointCloud<PointT>);
	pcl::PointCloud<PointT>::Ptr cloud_filtered(new pcl::PointCloud<PointT>);
	pcl::PointCloud<PointT>::Ptr cloud_smoothed(new pcl::PointCloud<PointT>);
	// 下采样
	pcl::VoxelGrid<PointT> downSampled;  //创建滤波对象
	downSampled.setInputCloud(cloud);            //设置需要过滤的点云给滤波对象
	downSampled.setLeafSize(0.01f, 0.01f, 0.01f);  //设置滤波时创建的体素体积为1cm的立方体
	downSampled.filter(*cloud_downSampled);           //执行滤波处理，存储输出


	pcl::StatisticalOutlierRemoval<PointT> statisOutlierRemoval;       //创建滤波器对象
	statisOutlierRemoval.setInputCloud(cloud_downSampled);            //设置待滤波的点云
	statisOutlierRemoval.setMeanK(50);                                //设置在进行统计时考虑查询点临近点数
	statisOutlierRemoval.setStddevMulThresh(1.0);                     //设置判断是否为离群点的阀值:均值+1.0*标准差
	statisOutlierRemoval.filter(*cloud_filtered);                     //滤波结果存储到cloud_filtered



	pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGB>);
	pcl::PointCloud<pcl::PointXYZRGB> mls_points;
	pcl::MovingLeastSquares<pcl::PointXYZRGB, pcl::PointXYZRGB> mls;
	mls.setComputeNormals(true);
	mls.setInputCloud(cloud_filtered);
	mls.setPolynomialFit(true);
	mls.setSearchMethod(tree);
	mls.setSearchRadius(0.03);
	mls.process(mls_points);
	pcl::io::savePCDFile("raw_data.pcd", mls_points);

	return 0;
}

float Heron_area(pcl::PolygonMesh mesh)
{


	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);
	fromPCLPointCloud2(mesh.cloud, *cloud);

	vector<pcl::Vertices>::iterator it;
	int i;
	long double sums = 0;
	for (it = mesh.polygons.begin(); i < mesh.polygons.size(); i++)
	{

		long double ax, ay, az, bx, by, bz, cx, cy, cz;
		//ax = it[i].vertices[0];

		ax = cloud->points[it[i].vertices[0]].x;
		ay = cloud->points[it[i].vertices[0]].y;
		az = cloud->points[it[i].vertices[0]].z;
		bx = cloud->points[it[i].vertices[1]].x;
		by = cloud->points[it[i].vertices[1]].y;
		bz = cloud->points[it[i].vertices[1]].z;
		cx = cloud->points[it[i].vertices[2]].x;
		cy = cloud->points[it[i].vertices[2]].y;
		cz = cloud->points[it[i].vertices[2]].z;


		long double a = sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by) + (az - bz) * (az - bz));
		long double b = sqrt((ax - cx) * (ax - cx) + (ay - cy) * (ay - cy) + (az - cz) * (az - cz));
		long double c = sqrt((cx - bx) * (cx - bx) + (cy - by) * (cy - by) + (cz - bz) * (cz - bz));

		long double p = (a + b + c) / 2;

		long double s = sqrt(p * (p - a) * (p - b) * (p - c));

		sums = sums + s;

	}
	return sums;
}