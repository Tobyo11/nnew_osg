/*
	DataStruct.h
	�����Զ�����������
	˵�����������������
*/
#pragma once

#include <iostream>
#include <windows.h>

#include <osg/Array>

enum OEDriverType
{//������Ƭ��߳�ʱ�õ�����������
	TMS,
	VBP,
	GDAL,
};

enum MapType
{//��Ҫ��Ⱦ�ĵ�ͼ����
	Map2D = 1,
	Map3D,
	Map2D_3D
};

enum TrackType
{//β������
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
{//ģ�ͳ�ʼ����
	std::string modelName;				//ģ������
	//std::string symbolFilePath;
	std::string modelFilePath;			//ģ�������ļ�·��
	//bool modelTextureShow;
	osg::Vec4 modelColor;				//ģ����ɫ
	//osg::Vec4 symbolColor;
	TrackType trackType;				//ģ����Ҫ��β������

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
{//�״�������
	double minDis;                                 //�״����	
	double maxDis;								   //�״�Զ��
	double minPitch;                               //�״︩����-�ͽ�
	double maxPitch;							   //�״︩����-�߽�
	double intPitch;							   //�״︩����-����Ƕ�
	double minHeading;							   //�״﷽λ��-��ʼ��
	double maxHeading;							   //�״﷽λ��-��ֹ��
	double intHeading;							   //�״﷽λ��-����Ƕ�
	osg::Vec4d  lineColor;                         //�״��������ɫ
	osg::Vec4d  quadColor;						   //�״������ɫ

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

