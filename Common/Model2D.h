/*
	Model2D.h
	����һ��class CModel2D
	���ڴ����͹���2Dģ��
*/
#pragma once
#include "BaseModel.h"

class CModel2D :public BaseModel
{
public:
	CModel2D(ModelConfig &mc);
	CModel2D(const CModel2D& model, const osg::CopyOp& copyop);
	~CModel2D();

	/*��������*/
	CModel2D* clone(const osg::CopyOp& copyop) { return new CModel2D(*this, copyop); }
	/*��ȡģ�ͽڵ�*/
	osg::ref_ptr<osg::Node> getModelNode() { return _modelNode; }
	/*����ģ���˶�·������(�Ӷ�������)*/
	virtual void setModelAnimationPathCallback(osg::ref_ptr<osg::Vec3Array> pointArray, double speed, osg::Matrix matToWorld) override;
	///*����ģ�͵�������λ��*/
	//virtual void setPositionLLA(osg::Vec3f lla) override;

protected:
	virtual void initModel() override;

private:
	osg::ref_ptr<osg::Node> _modelNode;
};