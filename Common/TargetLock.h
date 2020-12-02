/*
	TargetLock.h
	����class TargetLock��
	ʵ��Ŀ������
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
	/*��������״̬*/
	void updateLockPos(osg::Vec3d platPos, std::vector<osg::Vec3d> vecTargetPos);
	/*�������״̬*/
	void addLock(osg::Vec4d color);
	/*�Ƴ�����״̬*/
	void deleteLock();

	/*��ȡ�������Geode*/
	osg::ref_ptr<osg::Geode> getLockGeode() { return _lockGeode ? _lockGeode : NULL; }

	/*2020.11.30 �¼� updateLockPos����*/
	void updateLockPos(osg::Vec3d platPos, std::vector<osg::MatrixTransform*> vecModel);
private:
	bool _lockPosUpdate;	//�Ƿ��������λ��

	osg::Vec3d _platPos;
	std::vector<osg::Vec3d> _vecTargetPos;
	osg::ref_ptr<osg::Geode> _lockGeode;
	osg::ref_ptr<osg::Geometry> _lockLineGeom;
	/*2020.11.30 �¼�*/
	std::vector<osg::MatrixTransform*> _vecModel;
};

/*2020.11.30 Ŀ���������� �ı����߱��ַ�ʽ*/
class NTargetLock
{
public:
	NTargetLock();
	~NTargetLock();

	/*�������״̬*/
	void addLock(osg::Vec4d color);
	/*�����������״̬*/
	void clearLock();
	/*�Ƴ�ĳ������״̬*/
	void deleteLock(unsigned int n);
	/*��ȡ��������*/
	unsigned int getNumLock() { return _lock->getNumChildren(); }
	/*��ȡlockGroup*/
	osg::ref_ptr<osg::MatrixTransform> getLock() { return _lock; }
protected:
	osg::ref_ptr<osg::MatrixTransform> createLock(osg::Vec4d color);
private:
	bool _lockPosUpdate;	//�Ƿ����
	unsigned int _goalNum;	//Ŀ������

	osg::ref_ptr<osg::MatrixTransform> _lock;	//_lock�µ�ÿһ���ӽڵ㶼��matrixtransform->geode->geometry���
};