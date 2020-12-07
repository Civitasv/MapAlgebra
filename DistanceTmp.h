/*****************************************************************************
*  ����ģ�����                                               *
*  Copyright (C) 2020 ��ɭ 2309780282@qq.com.                         *
*                                                                            *
*  ���ļ��Ǿ���ģ�����
*
*  @file     CDistanceTemp.h                                                       *
*  @brief    ����ģ��������                                                     *
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
/**
 * @brief ����ģ�����
 */
class CDistanceTmp
{
public:
	virtual unsigned int Size() ; 
	virtual int getOffX(unsigned int i) ;
	virtual int getOffY(unsigned int i) ;
	virtual float getTmpDis(unsigned int i) ;
};

