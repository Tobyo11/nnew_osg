/*
	DataStruct.h
	各种自定义数据类型
	说明详见各个定义区域
*/
#pragma once

#include <iostream>
#include <windows.h>

#include <osg/Array>

enum OEDriverType
{//加载瓦片或高程时用到的驱动类型
	TMS,
	VBP,
	GDAL,
};

enum MapType
{//需要渲染的地图类型
	Map2D = 1,
	Map3D,
	Map2D_3D
};

enum TrackType
{//尾迹类型
	Point,
	Line,
	Band
};

enum ShpType
{
	shpPoint,
	shpPolygon,
	shpLine,
	shpIcon,
	shpName

};

struct ModelConfig
{//模型初始配置
	std::string modelName;				//模型名字
	//std::string symbolFilePath;
	std::string modelFilePath;			//模型所在文件路径
	//bool modelTextureShow;
	osg::Vec4 modelColor;				//模型颜色
	//osg::Vec4 symbolColor;
	TrackType trackType;				//模型需要的尾迹类型

	ModelConfig()
	{
		modelName = "Aircraft";
		//symbolFilePath = "./PanoISRResource/texture/TYPE_AIRCRAFT.png";
		modelFilePath = "f35.fbx";
		//modelTextureShow = false;
		modelColor = osg::Vec4(1.0, 0.0, 0.0, 1.0);
		//symbolColor = osg::Vec4(1.0, 0.0, 0.0, 1.0);
		trackType = TrackType::Line;
	}
};

struct RadarParam
{//雷达包络参数
	double minDis;                                 //雷达近界	
	double maxDis;								   //雷达远界
	double minPitch;                               //雷达俯仰角-低角
	double maxPitch;							   //雷达俯仰角-高角
	double intPitch;							   //雷达俯仰角-间隔角度
	double minHeading;							   //雷达方位角-起始角
	double maxHeading;							   //雷达方位角-终止角
	double intHeading;							   //雷达方位角-间隔角度
	osg::Vec4d  lineColor;                         //雷达包络线颜色
	osg::Vec4d  quadColor;						   //雷达包络颜色

	RadarParam()
	{
		minDis = 0.0;
		maxDis = 400000;
		minPitch = 0.0;
		maxPitch = 90.0;
		intPitch = 1.0;
		minHeading = 0.0;
		maxHeading = 360.0;
		intHeading = 1.0;
		lineColor = osg::Vec4d(1.0, 0.0, 0.0, 0.7);
		quadColor = osg::Vec4d(1.0, 0.0, 0.0, 0.3);
	}
};

