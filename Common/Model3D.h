/*
	Model3D.h
	����һ��class CModel3D
	���ڴ����͹���3Dģ��
*/
#pragma once
#include "BaseModel.h"

class CModel3D :public BaseModel
{
public:
	CModel3D(ModelConfig &mc);
	CModel3D(const CModel3D& model, const osg::CopyOp& copyop);
	~CModel3D();

	/*��������*/
	CModel3D* clone(const osg::CopyOp& copyop) { return new CModel3D(*this, copyop); }
	/*��ȡģ�ͽڵ�*/
	osg::ref_ptr<osg::Node> getModelNode() { return _modelNode; }
	/*����3Dģ���Ƿ���������*/
	void setModelTextureShow(bool show, osg::Vec4 color = osg::Vec4(0, 0, 0, 0));
	/*����ģ���˶�·������(�Ӷ�������)*/
	virtual void setModelAnimationPathCallback(osg::ref_ptr<osg::Vec3Array> pointArray, double speed, osg::Matrix matToWorld) override;

protected:
	virtual void initModel() override;
	void setModelShaderUniform(osg::ref_ptr<osg::Node> node, bool enableColor, osg::Vec4 color, bool enableLight);
private:
	osg::ref_ptr<osg::Node> _modelNode;
};