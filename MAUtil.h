/*****************************************************************************
*  ��ͼ����������                                                *
*  Copyright (C) 2020 ��ɭ 2309780282@qq.com.                         *
*                                                                            *
*  ���ļ��ǵ�ͼ�����������ͷ�ļ�
*
*  @file     MAUtil.h                                                       *
*  @brief    ��ͼ����������                                                      *
*  Details.                                                                  *
*                                                                            *
*  @author   ��ɭ                                                      *
*  @email    2309780282@qq.com                                              *
*  @version  1.0.0.0(�汾��)                                                  *
*  @date     2019                                           *
*  @license                                  *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : Description                                              *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2019 | 1.0.0.0   | ��ɭ     | Create file                     *
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
 * @brief ��ͼ����������
 * ʵ���˻����ĵ�ͼ��������
 */
class MAUtil
{
public:
	static unsigned char** readBmp(const char* sourceFileName);
	/**
	  * @brief λͼ��ɫ����
	  * @param sourceFileName  λͼͼ������·��
	  * @param destFileName    λͼͼ�����·��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool inverseBmp(const char* sourceFileName, const char* destFileName);

	/**
	  * @brief ���÷���
	  * @param sourceFileName1  λͼͼ������·��
	  * @param sourceFileName2  λͼͼ������·��
	  * @param destFileName    λͼͼ�����·��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool overlayAnalysis(const char* sourceFileName1, const char* sourceFileName2, const char* destFileName);

	/**
	  * @brief ��Դ256λͼתΪ 32bit λͼ
	  * @param sourceFileNames  λͼͼ������·��
	  * @param destFileName    λͼͼ�����·��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool bmp256to32b(const char* sourceFileName, const char* destFileName);

	/**
	  * @brief ʹ�þ�ֵ�˲�ƽ��
	  * @param sourceFileNames  λͼͼ������·��
	  * @param destFileName    λͼͼ�����·��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool averageFilter(const char* sourceFileName, const char* destFileName);

	/**
	  * @brief ʹ����ֵ�˲�ƽ��
	  * @param sourceFileNames  λͼͼ������·��
	  * @param destFileName    λͼͼ�����·��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool midFilter(const char* sourceFileName, const char* destFileName);

	/**
	  * @brief ����任
	  * @param sourceFileNames  λͼͼ������·��
	  * @param destFileName1    ���볡λͼͼ�����·��
	  * @param destFileName2    ���䳡λͼͼ�����·��
	  * @param templet    ����ģ��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool distanceTransform(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet);
	
	/**
	  * @brief ����任2
	  * @param sourceFileNames  λͼͼ������·��
	  * @param destFileName1    ���볡λͼͼ�����·��
	  * @param destFileName2    ���䳡λͼͼ�����·��
	  * @param templet    ����ģ��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool distanceTransformIn(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet);
	
	/**
	  * @brief ����任���������תΪ32λ
	  * @param sourceFileNames  λͼͼ������·��
	  * @param destFileName1    ���볡λͼͼ�����·��
	  * @param destFileName2    ���䳡λͼͼ�����·��
	  * @param templet    ����ģ��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool distanceTransform32b(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet);

	/**
	  * @brief �ϰ�����任
	  * @param sourceFileNames  λͼͼ������·��
	  * @param destFileName1    ���볡λͼͼ�����·��
	  * @param destFileName2    ���䳡λͼͼ�����·��
	  * @param templet    ����ģ��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool barrierDistanceTransform(const char* sourceFileName, const char* destFileName1, const char* destFileName2, CDistanceTmp* templet);

	/**
	  * @brief ���䳡�߽���ȡvoronoiͼ
	  * @param LocFileName  ���䳡ͼ������·��
	  * @param destFileName    �߽�λͼͼ�����·��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool getBdyBmp(const char* LocFileName, const char* destFileName);

	/**
	  * @brief ��������ȡ
	  * @param disFileName     256ɫͼ���볡ͼ������·��
	  * @param bufWidth			���������
	  * @param destFileName1    ������ͼ�����·��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool getBufferBmp(const char* disFileName, int bufWidth, const char* destFileName);

	/**
	  * @brief �ӿǱ任
	  * @param disFileName     256ɫͼ���볡ͼ������·��
	  * @param bufWidth			���������
	  * @param destFileName1    �ӿǱ任ͼ�����·��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool shellAddTransform(const char* disFileName, int bufWidth, const char* destFileName);

	/**
	  * @brief ��Ƥ�任
	  * @param disFileName     256ɫͼ���볡ͼ������·��
	  * @param bufWidth			���������
	  * @param destFileName1    ��Ƥ�任ͼ�����·��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool shellMinusTransform(const char* disFileName, int bufWidth, const char* destFileName);

	/**
	  * @brief ճ���任
	  * @param sourcefilename     256ɫͼԭʼͼ������·��
	  * @param bufWidth			���������
	  * @param destFileName1    ճ���任ͼ�����·��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool adhesionTransform(const char* sourcefilename, int bufWidth, const char* disFile, const char* locFile, CDistanceTmp* templet,const char* outFileName);

	/**
	  * @brief ʴ�ϱ任
	  * @param sourcefilename     256ɫͼԭʼͼ������·��
	  * @param bufWidth			���������
	  * @param destFileName1    ʴ�ϱ任ͼ�����·��
	  *
	  * @return ����˵��
	  *     -<em>false</em> fail
	  *     -<em>true</em> succeed
	  */
	static bool corrosionTransform(const char* sourcefilename, int bufWidth, const char* disFile, const char* locFile, CDistanceTmp* templet, const char* outFileName);
	/**
	* @brief ��ȡ��ɫ�����Ӧ��
	* @param sourceFileName      ԭʼͼ��·��
	* @param colorsTableFile    ��ɫ�����Ӧ���ļ�·��
	*
	* @return ����˵��
	*     -<em>false</em> fail
	*     -<em>true</em> succeed
	*/
	static bool scanSrcPtCoors(const char* sourceFileName, const char* colorsTableFile);

	/**
	* @brief ɨ����䳡��ȡ�߽�����
	* @param locFileName      ���䳡ͼ��·��
	* @param ptPairsFile    �洢Voronoi���ڹ�ϵ���
	*
	* @return ����˵��
	*     -<em>false</em> fail
	*     -<em>true</em> succeed
	*/
	static bool getTinPtPairs(const char* locFileName, const char* ptPairsFile);

	/**
	* @brief    ɨ����䳡��ȡ�߽�����
	* @param sourceFileName      ԭʼͼ��·��
	* @param ptPairsFile      �洢Voronoi���ڹ�ϵ���
	* @param colorsTableFile    ��ɫ�����Ӧ���ļ�·��
	*
	* @return ����˵��
	*     -<em>false</em> fail
	*     -<em>true</em> succeed
	*/
	static bool linkPts(const char* sourceFileName, const char* ptPairsFile, const char* colorsTableFile, const char* destFileName);


};

