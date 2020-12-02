/*
	TargetLock.h
	包含class TargetLock类
	实现目标锁定
*/
#pragma once

#include <windows.h>
#include <osg/Array>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/MatrixTransform>

class TargetLock
{
public:
	TargetLock();
	~TargetLock();
	/*更新锁定状态*/
	void updateLockPos(osg::Vec3d platPos, std::vector<osg::Vec3d> vecTargetPos);
	/*添加锁定状态*/
	void addLock(osg::Vec4d color);
	/*移除锁定状态*/
	void deleteLock();

	/*获取锁定类的Geode*/
	osg::ref_ptr<osg::Geode> getLockGeode() { return _lockGeode ? _lockGeode : NULL; }

	/*2020.11.30 新加 updateLockPos重载*/
	void updateLockPos(osg::Vec3d platPos, std::vector<osg::MatrixTransform*> vecModel);
private:
	bool _lockPosUpdate;	//是否更新锁定位置

	osg::Vec3d _platPos;
	std::vector<osg::Vec3d> _vecTargetPos;
	osg::ref_ptr<osg::Geode> _lockGeode;
	osg::ref_ptr<osg::Geometry> _lockLineGeom;
	/*2020.11.30 新加*/
	std::vector<osg::MatrixTransform*> _vecModel;
};

/*2020.11.30 目标锁定新类 改变连线表现方式*/
class NTargetLock
{
public:
	NTargetLock();
	~NTargetLock();

	/*添加锁定状态*/
	void addLock(osg::Vec4d color);
	/*清空所有锁定状态*/
	void clearLock();
	/*移除某个锁定状态*/
	void deleteLock(unsigned int n);
	/*获取锁定个数*/
	unsigned int getNumLock() { return _lock->getNumChildren(); }
	/*获取lockGroup*/
	osg::ref_ptr<osg::MatrixTransform> getLock() { return _lock; }
protected:
	osg::ref_ptr<osg::MatrixTransform> createLock(osg::Vec4d color);
private:
	bool _lockPosUpdate;	//是否更新
	unsigned int _goalNum;	//目标数量

	osg::ref_ptr<osg::MatrixTransform> _lock;	//_lock下的每一个子节点都由matrixtransform->geode->geometry组成
};