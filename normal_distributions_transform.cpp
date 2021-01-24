// SPDX-License-Identifier: BSD 3-Clause License
/*
     Copyright (c) 2021, Kentotomi. All rights reserved.
*/

#include <iostream>
#include <thread>

#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>

#include <pcl/registration/ndt.h>
#include <pcl/filters/approximate_voxel_grid.h>

#include <pcl/visualization/pcl_visualizer.h>

#include <Eigen/Core>
#include <Eigen/LU>

#include <time.h>

using namespace std::chrono_literals;

int
main (int argc, char** argv)
{
  // 地図データの読み込み
  pcl::PointCloud<pcl::PointXYZ>::Ptr target_cloud (new pcl::PointCloud<pcl::PointXYZ>);
  if (pcl::io::loadPCDFile<pcl::PointXYZ> ("3dmap_filtered.pcd", *target_cloud) == -1)
  {
    PCL_ERROR ("Couldn't read file 3dmap_filtered.pcd\n");
    return (-1);
  }
  std::cout << "Loaded " << target_cloud->size () << " data points from 3dmap_filtered" << std::endl;
  // スキャンデータの読み込み
  pcl::PointCloud<pcl::PointXYZ>::Ptr input_cloud (new pcl::PointCloud<pcl::PointXYZ>);
  if (pcl::io::loadPCDFile<pcl::PointXYZ> ("scan.pcd", *input_cloud) == -1)
  {
    PCL_ERROR ("Couldn't read file scan.pcd \n");
    return (-1);
  }
  std::cout << "Loaded " << input_cloud->size () << " data points from scan.pcd" << std::endl;

  //スキャンデータをフィルタリング
  pcl::PointCloud<pcl::PointXYZ>::Ptr filtered_cloud (new pcl::PointCloud<pcl::PointXYZ>);
  pcl::ApproximateVoxelGrid<pcl::PointXYZ> approximate_voxel_filter;
  approximate_voxel_filter.setLeafSize (0.1, 0.1, 0.1);
  approximate_voxel_filter.setInputCloud (input_cloud);
  approximate_voxel_filter.filter (*filtered_cloud);
  std::cout << "Filtered cloud contains " << filtered_cloud->size ()
            << " data points from scan.pcd" << std::endl;

  
  pcl::NormalDistributionsTransform<pcl::PointXYZ, pcl::PointXYZ> ndt;

  //パラメータの決定
  ndt.setTransformationEpsilon (0.0001);
  ndt.setStepSize (0.10);
  ndt.setResolution (1.0);
  ndt.setMaximumIterations (50);
  
  ndt.setInputSource (filtered_cloud);
  ndt.setInputTarget (target_cloud);

  //初期位置の決定
  Eigen::Matrix3f rot;
  rot = Eigen::AngleAxisf(0, Eigen::Vector3f::UnitX())
  * Eigen::AngleAxisf(0, Eigen::Vector3f::UnitY())
  * Eigen::AngleAxisf(0, Eigen::Vector3f::UnitZ());
  Eigen::Translation3f init_translation (0.0, 0.0, 0.0);
  Eigen::Matrix4f init_guess = (init_translation * rot).matrix ();
  
  clock_t start = clock();
  
  
  pcl::PointCloud<pcl::PointXYZ>::Ptr output_cloud (new pcl::PointCloud<pcl::PointXYZ>);
  ndt.align (*output_cloud, init_guess);
  
  //評価値の表示
  std::cout << "Normal Distributions Transform has converged:" << ndt.hasConverged ()
            << " score: " << ndt.getFitnessScore () << std::endl;

  //スキャン点群を変換
  pcl::transformPointCloud (*input_cloud, *output_cloud, ndt.getFinalTransformation());

  //変換後のスキャン点群を保存
  pcl::io::savePCDFileASCII ("transformed.pcd", *output_cloud);
  
  clock_t end = clock();

  const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;
  printf("time %lf[ms]\n", time);


  return (0);
}

