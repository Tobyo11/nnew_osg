/*
	Model2D.h
	包含一个class CModel2D
	用于创建和管理2D模型
*/
#pragma once
#include "BaseModel.h"

class CModel2D :public BaseModel
{
public:
	CModel2D(ModelConfig &mc);
	CModel2D(const CModel2D& model, const osg::CopyOp& copyop);
	~CModel2D();

	/*拷贝函数*/
	CModel2D* clone(const osg::CopyOp& copyop) { return new CModel2D(*this, copyop); }
	/*获取模型节点*/
	osg::ref_ptr<osg::Node> getModelNode() { return _modelNode; }
	/*设置模型运动路径动画(从顶点数组)*/
	virtual void setModelAnimationPathCallback(osg::ref_ptr<osg::Vec3Array> pointArray, double speed, osg::Matrix matToWorld) override;
	///*设置模型地理坐标位置*/
	//virtual void setPositionLLA(osg::Vec3f lla) override;

protected:
	virtual void initModel() override;

private:
	osg::ref_ptr<osg::Node> _modelNode;
};