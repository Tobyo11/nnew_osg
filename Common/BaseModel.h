/*
	BaseModel.h
	包含class BaseModel
	模型纯虚基类,提供控制模型和调整模型的公共接口
*/
#pragma once

#include "DataStruct.h"
#include "Label.h"
#include "Track.h"
#include "TargetLock.h"

#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/AnimationPath>
#include <osg/AutoTransform>
class BaseModel
{
public:
	BaseModel();
	BaseModel(ModelConfig &mc);
	//BaseModel(const BaseModel& model, const osg::CopyOp& copyop);
	~BaseModel();

	/*拷贝函数*/
	//virtual	BaseModel* clone(const osg::CopyOp& copyop) { return new BaseModel(*this, copyop); }
	/*应用模型配置*/
	void applyModelConfig(ModelConfig& mc);
	/*设置模型放大倍数*/
	void setModelScale(osg::Vec3 scale) { if(_modelSelf) _modelSelf->setMatrix(osg::Matrix::scale(scale)); }
	/*获取模型放大倍数*/
	osg::Vec3d getModelScale() { if (_modelSelf) return _modelSelf->getMatrix().getScale(); }
	/*设置模型旋转*/
	void setModelRotate(osg::Quat quat) { if(_modelSelf) _modelSelf->setMatrix(osg::Matrix::rotate(quat)); }
	/*设置模型矩阵*/
	void setModelMatrix(osg::Matrix mat) { _model->setMatrix(mat); }
	/*设置模型名字*/
	void setModelName(const std::string name) { _model->setName(name); }
	/*获取模型名字*/
	std::string getModelName() { return _model->getName(); }
	/*获取model根结点*/
	osg::ref_ptr<osg::Group> getModelGroup() { return _modelGroup; }
	/*设置尾迹开关*/
	void setTrackVisible(bool state);
	/*设置标牌开关*/
	void setLabelVisible(bool state);
	/*设置模型运动路径动画(从顶点数组)*/
	/*void setModelAnimationPathCallback(osg::ref_ptr<osg::Vec3Array> pointArray, double speed, osg::Matrix matToWorld);*/
	virtual void setModelAnimationPathCallback(osg::ref_ptr<osg::Vec3Array> pointArray, double speed, osg::Matrix matToWorld) = 0;
	/*设置模型运动路径动画(从动画路径)*/
	void setModelAnimationPathCallback1(osg::ref_ptr<osg::AnimationPath> ap);
	/*移除标牌回调*/
	void removeLabelCallback();
	/*移除尾迹回调*/
	void removeTrackCallback();
	/*关闭模型自动缩放*/
	void closeModelAutoTransform();
	///*设置模型地理坐标位置*/
	//virtual void setPositionLLA(osg::Vec3f lla) = 0;

	/*获取尾迹结点*/
	//osg::ref_ptr<osg::MatrixTransform> getTrackSelf() { return _modelTrack->getTrackSelf(); }
	osg::ref_ptr<osg::Geode> getTrackGeode() { return _modelTrack->getTrackGeode(); }

	/*2020.11.27 新加 目标锁定相关接口*/
	/*添加目标锁定 现阶段参数为位置信息,后期需要修改为目标信息,对应的回调形参也要进行修改*/
	void addTargetLock(std::vector<osg::Vec3d> vecTargetPos/*std::vector<Target*> vecTarget*/);
	/*移除目标锁定*/
	void removeTargetLockAll() { if (_modelTargetLock) _modelTargetLock->clearLock(); }
	void removeTargetLock(unsigned int i) { if (_modelTargetLock) _modelTargetLock->deleteLock(i); }
	/*2020.11.30 新加 重载的addTargetLock*/
	void addTargetLock(std::vector<osg::MatrixTransform*> vecModel);

protected:
	/*初始化尾迹和标牌*/
	void initTrackLabel();
	/*初始化模型*/
	virtual void initModel() = 0;
	/*根据世界坐标数组生成动画路径,3D模型需要设置值rotate,2D不需要*/
	osg::ref_ptr<osg::AnimationPath> getAnimationPathfromArray(osg::ref_ptr<osg::Vec3Array> pointArray, bool rotate, double speed, osg::Matrix matToWorld);
	/*为尾迹增加回调*/
	void addTrackUpdateCallback(bool render3D);
	/*为标牌增加回调*/
	void addLabelUpdateCallback(bool render3D);
	/*添加至模型自身节点*/
	template<class T>
	bool addModelSelfChild(const osg::ref_ptr<T>& child) { return _modelSelf ? _modelSelf->addChild(child) : false; }
	/*获取模型配置*/
	const ModelConfig getModelConfig();
	/*移除模型自身节点的所有孩子节点*/
	void removeAllModelSelfChildren() 
	{
		if (_modelSelf->getNumChildren() != 0)
		{//首先移除所有孩子节点
			_modelSelf->removeChildren(0, _modelSelf->getNumChildren());
		}
	}

	/*2020.11.30修改 将获取模型的函数改为公有*/
public:
	/*获取model*/
	osg::ref_ptr<osg::MatrixTransform> getModel() { return _model; }
protected:
	osg::Vec4f _modelColor;
	std::string _modelFilePath;
	std::string _modelName;
	TrackType _trackType;

	double _adjustVar;	//用于将尾迹调整至模型尾部
private:
	osg::ref_ptr<osg::Group> _modelGroup;
	osg::ref_ptr<osg::MatrixTransform> _modelSelf;
	osg::ref_ptr<osg::AutoTransform> _modelAutoTransform;
	osg::ref_ptr<osg::MatrixTransform> _model;

	CLabel* _modelLabel;
	NTrack/*CTrack*/* _modelTrack;
	/*新加 目标锁定 2020.11.27*/
	NTargetLock* _modelTargetLock; /*修改2020.11.30(TargetLock*)*/
};