/*
	Track.h
	����һ��class CTrack
	���ڴ�����,�߻�Ʈ����β��
*/
#pragma once
#include "DataStruct.h"

#include <osg/Geometry>
#include <osg/AutoTransform>
#include <osg/MatrixTransform>

class CTrack
{
public:
	CTrack(TrackType trackType, osg::Vec4 color);
	~CTrack();

	/*��ȡβ������*/
	TrackType getTrackType() { return _trackType; }
	/*��ȡβ������ͼԪ*/
	osg::ref_ptr<osg::Geometry> getTrackGeom() { return _trackGeom; }
	/*��ȡβ��Ҷ�ڵ�*/
	osg::ref_ptr<osg::Geode> getTrackGeode() { return _trackGeode; }
	/*��ȡβ��������*/
	//osg::ref_ptr<osg::MatrixTransform> getTrackSelf() { return _trackSelf; }
	/*��ȡ�Զ��任�ڵ�*/
	//osg::ref_ptr<osg::AutoTransform> getTrackAutoTransform() { return _trackAutoTransform; }
	/*����β����С*/
	//void setTrackScale(osg::Vec3d scale) { if (_trackSelf) _trackSelf->setMatrix(osg::Matrix::scale(scale)); }

private:
	void initTrackGeom();

private:
	//osg::ref_ptr<osg::AutoTransform> _trackAutoTransform;
	//osg::ref_ptr<osg::MatrixTransform> _trackSelf;
	osg::ref_ptr<osg::Geode>    _trackGeode;
	osg::ref_ptr<osg::Geometry> _trackGeom;
	TrackType _trackType;
	osg::Vec4 _trackColor;
};


class NTrack
{
	/*
		��β��:�任������ԭ��������¼ӱ任������Զ��任����
	*/
public:
	NTrack(TrackType trackType, osg::Vec4 color);
	~NTrack();

	/*��ȡβ������*/
	TrackType getTrackType() { return _trackType; }
	/*��ȡβ������ͼԪ*/
	osg::ref_ptr<osg::Geometry> getTrackGeom() { return _trackGeom; }
	/*��ȡβ��Ҷ�ڵ�*/
	osg::ref_ptr<osg::Geode> getTrackGeode() { return _trackGeode; }
	/*��ȡβ��������*/
	osg::ref_ptr<osg::MatrixTransform> getTrackSelf() { return _trackSelf; }
	/*��ȡ�Զ��任�ڵ�*/
	osg::ref_ptr<osg::AutoTransform> getTrackAutoTransform() { return _trackAutoTransform; }
	/*����β����С*/
	//void setTrackScale(osg::Vec3d scale) { if (_trackSelf) _trackSelf->setMatrix(osg::Matrix::scale(scale)); }

private:
	void initTrackGeom();

private:
	osg::ref_ptr<osg::AutoTransform> _trackAutoTransform;
	osg::ref_ptr<osg::MatrixTransform> _trackSelf;
	osg::ref_ptr<osg::Geode>    _trackGeode;
	osg::ref_ptr<osg::Geometry> _trackGeom;
	TrackType _trackType;
	osg::Vec4 _trackColor;
};