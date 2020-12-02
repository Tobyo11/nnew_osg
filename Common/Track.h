/*
	Track.h
	包含一个class CTrack
	用于创建点,线或飘带形尾迹
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

	/*获取尾迹类型*/
	TrackType getTrackType() { return _trackType; }
	/*获取尾迹基本图元*/
	osg::ref_ptr<osg::Geometry> getTrackGeom() { return _trackGeom; }
	/*获取尾迹叶节点*/
	osg::ref_ptr<osg::Geode> getTrackGeode() { return _trackGeode; }
	/*获取尾迹自身结点*/
	//osg::ref_ptr<osg::MatrixTransform> getTrackSelf() { return _trackSelf; }
	/*获取自动变换节点*/
	//osg::ref_ptr<osg::AutoTransform> getTrackAutoTransform() { return _trackAutoTransform; }
	/*设置尾迹大小*/
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
		新尾迹:变换策略由原点出发且新加变换矩阵和自动变换矩阵
	*/
public:
	NTrack(TrackType trackType, osg::Vec4 color);
	~NTrack();

	/*获取尾迹类型*/
	TrackType getTrackType() { return _trackType; }
	/*获取尾迹基本图元*/
	osg::ref_ptr<osg::Geometry> getTrackGeom() { return _trackGeom; }
	/*获取尾迹叶节点*/
	osg::ref_ptr<osg::Geode> getTrackGeode() { return _trackGeode; }
	/*获取尾迹自身结点*/
	osg::ref_ptr<osg::MatrixTransform> getTrackSelf() { return _trackSelf; }
	/*获取自动变换节点*/
	osg::ref_ptr<osg::AutoTransform> getTrackAutoTransform() { return _trackAutoTransform; }
	/*设置尾迹大小*/
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