/*
	CallBack.h
	��������class TrackCallback
			class LabelCallback
	����β���ͱ��Ƶĸ��»ص�,������ģ��λ�ý����α�������Ϣ�ĸ���
*/
#pragma once

#include "DataStruct.h"
#include "Label.h"
#include "TargetLock.h"
#include "Explosion.h"

#include <osg/NodeCallback>
#include <osg/MatrixTransform>

class TrackCallback :public osg::NodeCallback
{//����ΪƮ��ʱ,��ɫalphaֵ�����ڲ�����
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
	//�㡢����
	bool _first;
	//Ʈ����
	float _width;
};

/*2020.11.26��β����,��β������*/
class NTrackCallback :public osg::NodeCallback
{//����ΪƮ��ʱ,��ɫalphaֵ�����ڲ�����
	/*
		�µ�β�����Ʋ���:
			��ԭ��Ϊ���ĵ㿪ʼ���ƺ͸���,���Զ��任����Ϊλ�ƾ����β��λ�ý��б任
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
	//�㡢����
	bool _first;
	//Ʈ����
	float _width;
	//��¼�״α任����model->getMatrix�������
	osg::Matrix _firstMat;
	//��¼�׸��任�����λ��
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

/*2020.11.27����Ŀ��������*/
class TargetLockCallback : public osg::NodeCallback
{
public:
	TargetLockCallback(osg::ref_ptr<osg::MatrixTransform> model, TargetLock* targetLock, std::vector<osg::Vec3d> vecTargetPos/*std::vector<Target*> vecTarget*/);
	/*2020.11.30 �¼� ��������*/
	TargetLockCallback(osg::ref_ptr<osg::MatrixTransform> model, TargetLock* targetLock, std::vector<osg::MatrixTransform*> vecModel);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
protected:
	TargetLock* _pTargetLock;
	osg::ref_ptr<osg::MatrixTransform> _model;
	std::vector<osg::Vec3d> _vecTargetPos;
	/*2020.11.30 �¼� _vecModel*/
	std::vector<osg::MatrixTransform*> _vecModel;
};

/*2020.11.30 ��Ŀ��������*/
class NTargetLockCallback : public osg::NodeCallback
{
public:
	NTargetLockCallback(osg::ref_ptr<osg::MatrixTransform> model, std::vector<osg::MatrixTransform*> vecModel);
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

protected:
	osg::ref_ptr<osg::MatrixTransform> _model;
	std::vector<osg::MatrixTransform*> _vecModel;
};

/*2020.11.30����Ŀ����������Ч��,��ʱδʹ��*/
/*ʹ�÷�ʽ��main_LineStipple.cpp*/
//class RayLineCallback :public osg::NodeCallback
//{
//public:
//	RayLineCallback(osg::Vec3d startPos, osg::Vec3d endPos, float partLength/*�ܳ���ռ��*/, float blank/*�������(�ܳ���ռ��)*/, float step/*ÿ���ƶ�����*/);
//	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
//protected:
//	osg::Vec3d _startPos;
//	osg::Vec3d _endPos;
//	float _length;
//	float _blankLength;
//	osg::Vec3d _norVec;
//	//osg::Vec3d _lastPos;
//	float _step;	//	�ƶ�����
//
//	bool _first;
//};