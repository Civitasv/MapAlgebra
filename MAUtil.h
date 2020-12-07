/*****************************************************************************
*  地图代数工具类                                                *
*  Copyright (C) 2020 胡森 2309780282@qq.com.                         *
*                                                                            *
*  该文件是地图代数工具类的头文件
*
*  @file     MAUtil.h                                                       *
*  @brief    地图代数工具类                                                      *
*  Details.                                                                  *
*                                                                            *
*  @author   胡森                                                      *
*  @email    2309780282@qq.com                                              *
*  @version  1.0.0.0(版本号)                                                  *
*  @date     2019                                           *
*  @license                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : Description                                              *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019 | 1.0.0.0   | 胡森     | Create file                     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#pragma once
#include <cassert>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include "AverageFilter.h"
#include "DistanceTmp.h"
#define MaxFloat 9999999999999.0
#define MinFloat 0.00000001

using namespace std;

/**
 * @brief 地图代数工具类
 * 实现了基础的地图代数操作
 */
class MAUtil
{
public:
	static unsigned char** readBmp(const char* sourceFileName);
	/**
	  * @brief 位图反色处理
	  * @param sourceFileName  位图图像输入路径
	  * @param destFileName    位图图像输出路径
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool inverseBmp(const char* sourceFileName, const char* destFileName);

	/**
	  * @brief 叠置分析
	  * @param sourceFileName1  位图图像输入路径
	  * @param sourceFileName2  位图图像输入路径
	  * @param destFileName    位图图像输出路径
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool overlayAnalysis(const char* sourceFileName1, const char* sourceFileName2, const char* destFileName);

	/**
	  * @brief 将源256位图转为 32bit 位图
	  * @param sourceFileNames  位图图像输入路径
	  * @param destFileName    位图图像输出路径
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool bmp256to32b(const char* sourceFileName, const char* destFileName);

	/**
	  * @brief 使用均值滤波平滑
	  * @param sourceFileNames  位图图像输入路径
	  * @param destFileName    位图图像输出路径
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool averageFilter(const char* sourceFileName, const char* destFileName);

	/**
	  * @brief 使用中值滤波平滑
	  * @param sourceFileNames  位图图像输入路径
	  * @param destFileName    位图图像输出路径
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool midFilter(const char* sourceFileName, const char* destFileName);

	/**
	  * @brief 距离变换
	  * @param sourceFileNames  位图图像输入路径
	  * @param destFileName1    距离场位图图像输出路径
	  * @param destFileName2    分配场位图图像输出路径
	  * @param templet    距离模板
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool distanceTransform(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet);
	
	/**
	  * @brief 距离变换2
	  * @param sourceFileNames  位图图像输入路径
	  * @param destFileName1    距离场位图图像输出路径
	  * @param destFileName2    分配场位图图像输出路径
	  * @param templet    距离模板
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool distanceTransformIn(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet);
	
	/**
	  * @brief 距离变换，并将结果转为32位
	  * @param sourceFileNames  位图图像输入路径
	  * @param destFileName1    距离场位图图像输出路径
	  * @param destFileName2    分配场位图图像输出路径
	  * @param templet    距离模板
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool distanceTransform32b(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet);

	/**
	  * @brief 障碍距离变换
	  * @param sourceFileNames  位图图像输入路径
	  * @param destFileName1    距离场位图图像输出路径
	  * @param destFileName2    分配场位图图像输出路径
	  * @param templet    距离模板
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool barrierDistanceTransform(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet);

	/**
	  * @brief 分配场边界提取voronoi图
	  * @param LocFileName  分配场图像输入路径
	  * @param destFileName    边界位图图像输出路径
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool getBdyBmp(const char* LocFileName, const char* destFileName);

	/**
	  * @brief 缓冲区提取
	  * @param disFileName     256色图距离场图像输入路径
	  * @param bufWidth			缓冲区宽度
	  * @param destFileName1    缓冲区图像输出路径
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool getBufferBmp(const char* disFileName, int bufWidth, const char* destFileName);

	/**
	  * @brief 加壳变换
	  * @param disFileName     256色图距离场图像输入路径
	  * @param bufWidth			缓冲区宽度
	  * @param destFileName1    加壳变换图像输出路径
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool shellAddTransform(const char* disFileName, int bufWidth, const char* destFileName);

	/**
	  * @brief 蜕皮变换
	  * @param disFileName     256色图距离场图像输入路径
	  * @param bufWidth			缓冲区宽度
	  * @param destFileName1    蜕皮变换图像输出路径
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool shellMinusTransform(const char* disFileName, int bufWidth, const char* destFileName);

	/**
	  * @brief 粘连变换
	  * @param sourcefilename     256色图原始图像输入路径
	  * @param bufWidth			缓冲区宽度
	  * @param destFileName1    粘连变换图像输出路径
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool adhesionTransform(const char* sourcefilename, int bufWidth, const char* disFile, const char* locFile, CDistanceTmp* templet,const char* outFileName);

	/**
	  * @brief 蚀断变换
	  * @param sourcefilename     256色图原始图像输入路径
	  * @param bufWidth			缓冲区宽度
	  * @param destFileName1    蚀断变换图像输出路径
	  *
	  * @return 返回说明
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool corrosionTransform(const char* sourcefilename, int bufWidth, const char* disFile, const char* locFile, CDistanceTmp* templet, const char* outFileName);
	/**
	* @brief 获取颜色坐标对应表
	* @param sourceFileName      原始图像路径
	* @param colorsTableFile    颜色坐标对应表文件路径
	*
	* @return 返回说明
	*     -<em>false</em> fail
	*     -<em>true</em> succeed
	*/
	static bool scanSrcPtCoors(const char* sourceFileName, const char* colorsTableFile);

	/**
	* @brief 扫描分配场获取边界点对组
	* @param locFileName      分配场图像路径
	* @param ptPairsFile    存储Voronoi相邻关系点对
	*
	* @return 返回说明
	*     -<em>false</em> fail
	*     -<em>true</em> succeed
	*/
	static bool getTinPtPairs(const char* locFileName, const char* ptPairsFile);

	/**
	* @brief    扫描分配场获取边界点对组
	* @param sourceFileName      原始图像路径
	* @param ptPairsFile      存储Voronoi相邻关系点对
	* @param colorsTableFile    颜色坐标对应表文件路径
	*
	* @return 返回说明
	*     -<em>false</em> fail
	*     -<em>true</em> succeed
	*/
	static bool linkPts(const char* sourceFileName, const char* ptPairsFile, const char* colorsTableFile, const char* destFileName);


};

