/*
	Model3D.h
	包含一个class CModel3D
	用于创建和管理3D模型
*/
#pragma once
#include "BaseModel.h"

class CModel3D :public BaseModel
{
public:
	CModel3D(ModelConfig &mc);
	CModel3D(const CModel3D& model, const osg::CopyOp& copyop);
	~CModel3D();

	/*拷贝函数*/
	CModel3D* clone(const osg::CopyOp& copyop) { return new CModel3D(*this, copyop); }
	/*获取模型节点*/
	osg::ref_ptr<osg::Node> getModelNode() { return _modelNode; }
	/*设置3D模型是否启用纹理*/
	void setModelTextureShow(bool show, osg::Vec4 color = osg::Vec4(0, 0, 0, 0));
	/*设置模型运动路径动画(从顶点数组)*/
	virtual void setModelAnimationPathCallback(osg::ref_ptr<osg::Vec3Array> pointArray, double speed, osg::Matrix matToWorld) override;

protected:
	virtual void initModel() override;
	void setModelShaderUniform(osg::ref_ptr<osg::Node> node, bool enableColor, osg::Vec4 color, bool enableLight);
private:
	osg::ref_ptr<osg::Node> _modelNode;
};