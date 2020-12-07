/*****************************************************************************
*  距离模板基类                                               *
*  Copyright (C) 2020 胡森 2309780282@qq.com.                         *
*                                                                            *
*  该文件是距离模板基类
*
*  @file     CDistanceTemp.h                                                       *
*  @brief    距离模板抽象基类                                                     *
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
/**
 * @brief 距离模板基类
 */
class CDistanceTmp
{
public:
	virtual unsigned int Size() ; 
	virtual int getOffX(unsigned int i) ;
	virtual int getOffY(unsigned int i) ;
	virtual float getTmpDis(unsigned int i) ;
};

