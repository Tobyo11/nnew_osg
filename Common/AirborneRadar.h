/*
	AirborneRadar.h
	����class AirborneRadar��
	ʵ�ֻ����״����Բ����
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

	/*�õ�radarSelf*/
	osg::ref_ptr<osg::MatrixTransform> getAirborneRadar() { return _airborneRadar; }
protected:
	/*����2ά�ӽ��µ��״�*/
	osg::ref_ptr<osg::Geode> create2DRadar();
	/*����3ά�ӽ��µ��״�*/
	osg::ref_ptr<osg::Geode> create3DRadar();
private:
	RadarParam* _radarParam;

	osg::ref_ptr<osg::MatrixTransform> _airborneRadar; //_airborneRadar�µ�ÿһ���ӽڵ㶼��matrixtransform->geode->geometry���
};