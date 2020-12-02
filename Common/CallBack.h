/*
	CallBack.h
	包含两个class TrackCallback
			class LabelCallback
	用于尾迹和标牌的更新回调,即根据模型位置进行形变和相关信息的更新
*/
#pragma once

#include "DataStruct.h"
#include "Label.h"
#include "TargetLock.h"
#include "Explosion.h"

#include <osg/NodeCallback>
#include <osg/MatrixTransform>

class TrackCallback :public osg::NodeCallback
{//类型为飘带时,颜色alpha值仅由内部控制
public:
	TrackCallback(osg::ref_ptr<osg::MatrixTransform> model, TrackType trackType, bool is3D, double adjustVar, int length, osg::Vec4f color, int width = 0);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
protected:
	osg::ref_ptr<osg::MatrixTransform> _model;
	TrackType _trackType;
	int _length;
	osg::Vec4f _color;
	bool _is3D;
	double _adjustVar;
	//点、线用
	bool _first;
	//飘带用
	float _width;
};

/*2020.11.26新尾迹类,旧尾迹弃用*/
class NTrackCallback :public osg::NodeCallback
{//类型为飘带时,颜色alpha值仅由内部控制
	/*
		新的尾迹绘制策略:
			以原点为中心点开始绘制和更新,以自动变换矩阵为位移矩阵对尾迹位置进行变换
	*/
public:
	NTrackCallback(osg::ref_ptr<osg::MatrixTransform> model, TrackType trackType, bool is3D, double adjustVar, int length, osg::Vec4f color, int width = 0);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
protected:
	osg::ref_ptr<osg::MatrixTransform> _model;
	TrackType _trackType;
	int _length;
	osg::Vec4f _color;
	bool _is3D;
	double _adjustVar;
	//点、线用
	bool _first;
	//飘带用
	float _width;
	//记录首次变换矩阵model->getMatrix的逆矩阵
	osg::Matrix _firstMat;
	//记录首个变换矩阵的位置
	osg::Vec3d _firstPos;
};

class LabelCallback :public osg::NodeCallback
{
public:
	LabelCallback(osg::ref_ptr<osg::MatrixTransform> model, CLabel* label, bool is3D);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
protected:
	CLabel* _pLabel;
	osg::ref_ptr<osg::MatrixTransform> _model;
	bool _is3D;
};

/*2020.11.27新增目标锁定类*/
class TargetLockCallback : public osg::NodeCallback
{
public:
	TargetLockCallback(osg::ref_ptr<osg::MatrixTransform> model, TargetLock* targetLock, std::vector<osg::Vec3d> vecTargetPos/*std::vector<Target*> vecTarget*/);
	/*2020.11.30 新加 构造重载*/
	TargetLockCallback(osg::ref_ptr<osg::MatrixTransform> model, TargetLock* targetLock, std::vector<osg::MatrixTransform*> vecModel);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
protected:
	TargetLock* _pTargetLock;
	osg::ref_ptr<osg::MatrixTransform> _model;
	std::vector<osg::Vec3d> _vecTargetPos;
	/*2020.11.30 新加 _vecModel*/
	std::vector<osg::MatrixTransform*> _vecModel;
};

/*2020.11.30 新目标锁定类*/
class NTargetLockCallback : public osg::NodeCallback
{
public:
	NTargetLockCallback(osg::ref_ptr<osg::MatrixTransform> model, std::vector<osg::MatrixTransform*> vecModel);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

protected:
	osg::ref_ptr<osg::MatrixTransform> _model;
	std::vector<osg::MatrixTransform*> _vecModel;
};

/*2020.11.30新增目标锁定连线效果,暂时未使用*/
/*使用方式见main_LineStipple.cpp*/
//class RayLineCallback :public osg::NodeCallback
//{
//public:
//	RayLineCallback(osg::Vec3d startPos, osg::Vec3d endPos, float partLength/*总长度占比*/, float blank/*间隔长度(总长度占比)*/, float step/*每次移动长度*/);
//	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
//protected:
//	osg::Vec3d _startPos;
//	osg::Vec3d _endPos;
//	float _length;
//	float _blankLength;
//	osg::Vec3d _norVec;
//	//osg::Vec3d _lastPos;
//	float _step;	//	移动步长
//
//	bool _first;
//};