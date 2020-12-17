/*
	BaseModel.h
	����class BaseModel
	ģ�ʹ������,�ṩ����ģ�ͺ͵���ģ�͵Ĺ����ӿ�
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

	/*��������*/
	//virtual	BaseModel* clone(const osg::CopyOp& copyop) { return new BaseModel(*this, copyop); }
	/*Ӧ��ģ������*/
	void applyModelConfig(ModelConfig& mc);
	/*����ģ�ͷŴ���*/
	void setModelScale(osg::Vec3 scale) { if(_modelSelf) _modelSelf->setMatrix(osg::Matrix::scale(scale)); }
	/*��ȡģ�ͷŴ���*/
	osg::Vec3d getModelScale() { if (_modelSelf) return _modelSelf->getMatrix().getScale(); }
	/*����ģ����ת*/
	void setModelRotate(osg::Quat quat) { if(_modelSelf) _modelSelf->setMatrix(osg::Matrix::rotate(quat)); }
	/*����ģ�;���*/
	void setModelMatrix(osg::Matrix mat) { _model->setMatrix(mat); }
	/*����ģ������*/
	void setModelName(const std::string name) { _model->setName(name); }
	/*��ȡģ������*/
	std::string getModelName() { return _model->getName(); }
	/*��ȡmodel�����*/
	osg::ref_ptr<osg::Group> getModelGroup() { return _modelGroup; }
	/*����β������*/
	void setTrackVisible(bool state);
	/*���ñ��ƿ���*/
	void setLabelVisible(bool state);
	/*����ģ���˶�·������(�Ӷ�������)*/
	/*void setModelAnimationPathCallback(osg::ref_ptr<osg::Vec3Array> pointArray, double speed, osg::Matrix matToWorld);*/
	virtual void setModelAnimationPathCallback(osg::ref_ptr<osg::Vec3Array> pointArray, double speed, osg::Matrix matToWorld) = 0;
	/*����ģ���˶�·������(�Ӷ���·��)*/
	void setModelAnimationPathCallback1(osg::ref_ptr<osg::AnimationPath> ap);
	/*�Ƴ����ƻص�*/
	void removeLabelCallback();
	/*�Ƴ�β���ص�*/
	void removeTrackCallback();
	/*�ر�ģ���Զ�����*/
	void closeModelAutoTransform();
	///*����ģ�͵�������λ��*/
	//virtual void setPositionLLA(osg::Vec3f lla) = 0;

	/*��ȡβ�����*/
	//osg::ref_ptr<osg::MatrixTransform> getTrackSelf() { return _modelTrack->getTrackSelf(); }
	osg::ref_ptr<osg::Geode> getTrackGeode() { return _modelTrack->getTrackGeode(); }

	/*2020.11.27 �¼� Ŀ��������ؽӿ�*/
	/*���Ŀ������ �ֽ׶β���Ϊλ����Ϣ,������Ҫ�޸�ΪĿ����Ϣ,��Ӧ�Ļص��β�ҲҪ�����޸�*/
	void addTargetLock(std::vector<osg::Vec3d> vecTargetPos/*std::vector<Target*> vecTarget*/);
	/*�Ƴ�Ŀ������*/
	void removeTargetLockAll() { if (_modelTargetLock) _modelTargetLock->clearLock(); }
	void removeTargetLock(unsigned int i) { if (_modelTargetLock) _modelTargetLock->deleteLock(i); }
	/*2020.11.30 �¼� ���ص�addTargetLock*/
	void addTargetLock(std::vector<osg::MatrixTransform*> vecModel);

protected:
	/*��ʼ��β���ͱ���*/
	void initTrackLabel();
	/*��ʼ��ģ��*/
	virtual void initModel() = 0;
	/*�������������������ɶ���·��,3Dģ����Ҫ����ֵrotate,2D����Ҫ*/
	osg::ref_ptr<osg::AnimationPath> getAnimationPathfromArray(osg::ref_ptr<osg::Vec3Array> pointArray, bool rotate, double speed, osg::Matrix matToWorld);
	/*Ϊβ�����ӻص�*/
	void addTrackUpdateCallback(bool render3D);
	/*Ϊ�������ӻص�*/
	void addLabelUpdateCallback(bool render3D);
	/*�����ģ������ڵ�*/
	template<class T>
	bool addModelSelfChild(const osg::ref_ptr<T>& child) { return _modelSelf ? _modelSelf->addChild(child) : false; }
	/*��ȡģ������*/
	const ModelConfig getModelConfig();
	/*�Ƴ�ģ������ڵ�����к��ӽڵ�*/
	void removeAllModelSelfChildren() 
	{
		if (_modelSelf->getNumChildren() != 0)
		{//�����Ƴ����к��ӽڵ�
			_modelSelf->removeChildren(0, _modelSelf->getNumChildren());
		}
	}

	/*2020.11.30�޸� ����ȡģ�͵ĺ�����Ϊ����*/
public:
	/*��ȡmodel*/
	osg::ref_ptr<osg::MatrixTransform> getModel() { return _model; }
protected:
	osg::Vec4f _modelColor;
	std::string _modelFilePath;
	std::string _modelName;
	TrackType _trackType;

	double _adjustVar;	//���ڽ�β��������ģ��β��
private:
	osg::ref_ptr<osg::Group> _modelGroup;
	osg::ref_ptr<osg::MatrixTransform> _modelSelf;
	osg::ref_ptr<osg::AutoTransform> _modelAutoTransform;
	osg::ref_ptr<osg::MatrixTransform> _model;

	CLabel* _modelLabel;
	NTrack/*CTrack*/* _modelTrack;
	/*�¼� Ŀ������ 2020.11.27*/
	NTargetLock* _modelTargetLock; /*�޸�2020.11.30(TargetLock*)*/
};