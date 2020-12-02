/*
	AirborneRadar.h
	包含class AirborneRadar类
	实现机载雷达包络圆绘制
*/
#pragma once

#include "DataStruct.h"

#include <osg/MatrixTransform>
#include <osg/Geode>

class AirborneRadar
{
public:
	AirborneRadar(RadarParam* radarParam);
	~AirborneRadar();

	void addAirborneRadar(bool is3D);

	/*得到radarSelf*/
	osg::ref_ptr<osg::MatrixTransform> getAirborneRadar() { return _airborneRadar; }
protected:
	/*创建2维视角下的雷达*/
	osg::ref_ptr<osg::Geode> create2DRadar();
	/*创建3维视角下的雷达*/
	osg::ref_ptr<osg::Geode> create3DRadar();
private:
	RadarParam* _radarParam;

	osg::ref_ptr<osg::MatrixTransform> _airborneRadar; //_airborneRadar下的每一个子节点都由matrixtransform->geode->geometry组成
};