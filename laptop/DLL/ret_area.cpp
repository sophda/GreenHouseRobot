
// ���˲������ǻ��������
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


///////////////////////��������С���˷�
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

////����ڲ�
const double camera_factor = 1000;
const double camera_cx = 682.3;
const double camera_cy = 254.9;
const double camera_fx = 979.8; 
const double camera_fy = 942.8;
PointCloud::Ptr construct_pointcloud(cv::Mat img_rgb, cv::Mat img_dep);
float Heron_area(pcl::PolygonMesh mesh);
//�ӿں���
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
	// �²���
	pcl::VoxelGrid<PointT> downSampled;  //�����˲�����
	downSampled.setInputCloud(cloud);            //������Ҫ���˵ĵ��Ƹ��˲�����
	downSampled.setLeafSize(0.01f, 0.01f, 0.01f);  //�����˲�ʱ�������������Ϊ1cm��������
	downSampled.filter(*cloud_downSampled);           //ִ���˲������洢���


	pcl::StatisticalOutlierRemoval<PointT> statisOutlierRemoval;       //�����˲�������
	statisOutlierRemoval.setInputCloud(cloud_downSampled);            //���ô��˲��ĵ���
	statisOutlierRemoval.setMeanK(50);                                //�����ڽ���ͳ��ʱ���ǲ�ѯ���ٽ�����
	statisOutlierRemoval.setStddevMulThresh(1.0);                     //�����ж��Ƿ�Ϊ��Ⱥ��ķ�ֵ:��ֵ+1.0*��׼��
	statisOutlierRemoval.filter(*cloud_filtered);                     //�˲�����洢��cloud_filtered


	// ���� KD-Tree
	pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGB>);

	// Output has the PointNormal type in order to store the normals calculated by MLS
	pcl::PointCloud<pcl::PointXYZRGB> mls_points;

	// ������С����ʵ�ֵĶ���mls
	pcl::MovingLeastSquares<pcl::PointXYZRGB, pcl::PointXYZRGB> mls;
	mls.setComputeNormals(true);  //��������С���˼�������Ҫ���з��߹���
	// Set parameters
	mls.setInputCloud(cloud_filtered);
	mls.setPolynomialFit(true);
	mls.setSearchMethod(tree);
	mls.setSearchRadius(0.03);
	// Reconstruct
	mls.process(mls_points);
	// Save output   ����õ�������С���˷�����Ϻ�ĵ���
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

	// ���Ʒ�����  
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


	//��ʼ��GreedyProjectionTriangulation���󣬲����ò���  
	pcl::GreedyProjectionTriangulation<pcl::PointNormal> gp3;
	//����������������ڴ洢���  
	pcl::PolygonMesh triangles;

	//����GreedyProjectionTriangulation����Ĳ���  
	//��һ������Ӱ��ܴ�  
	gp3.setSearchRadius(200.0f);           //�������ӵ�֮��������루���߳�������ȷ��k���ڵ���뾶��Ĭ��ֵ 0��  
	gp3.setMu(2.5f);                       //��������ھ���ĳ��ӣ��Եõ�ÿ��������������뾶��Ĭ��ֵ 0��  
	gp3.setMaximumNearestNeighbors(100);   //��������������ڵ���������  
	gp3.setMaximumSurfaceAngle(M_PI / 4);  // 45 degrees��pi�����ƽ���  
	gp3.setMinimumAngle(M_PI / 18);        // 10 degrees ÿ�����ǵ���С�Ƕ�  
	gp3.setMaximumAngle(2 * M_PI / 3);     // 120 degrees ÿ�����ǵ����Ƕ�  
	gp3.setNormalConsistency(false);       //���������һ�£�����Ϊtrue  

	//���������������������  
	gp3.setInputCloud(cloud_with_normals);
	gp3.setSearchMethod(tree2);

	//ִ���ع������������triangles��  
	gp3.reconstruct(triangles);

	//float heron = Heron_area(triangles);
	//��������ͼ  
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
	//ת����

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
	// �²���
	pcl::VoxelGrid<PointT> downSampled;  //�����˲�����
	downSampled.setInputCloud(cloud);            //������Ҫ���˵ĵ��Ƹ��˲�����
	downSampled.setLeafSize(0.01f, 0.01f, 0.01f);  //�����˲�ʱ�������������Ϊ1cm��������
	downSampled.filter(*cloud_downSampled);           //ִ���˲������洢���


	pcl::StatisticalOutlierRemoval<PointT> statisOutlierRemoval;       //�����˲�������
	statisOutlierRemoval.setInputCloud(cloud_downSampled);            //���ô��˲��ĵ���
	statisOutlierRemoval.setMeanK(50);                                //�����ڽ���ͳ��ʱ���ǲ�ѯ���ٽ�����
	statisOutlierRemoval.setStddevMulThresh(1.0);                     //�����ж��Ƿ�Ϊ��Ⱥ��ķ�ֵ:��ֵ+1.0*��׼��
	statisOutlierRemoval.filter(*cloud_filtered);                     //�˲�����洢��cloud_filtered



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