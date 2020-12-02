#include "Model2D.h"
#include "Utility.h"

CModel2D::CModel2D(ModelConfig &mc):BaseModel(mc)
{
	initModel();

	addTrackUpdateCallback(false);
	addLabelUpdateCallback(false);
}

CModel2D::CModel2D(const CModel2D& model, const osg::CopyOp& copyop)
{
	_modelColor = model._modelColor;
	_modelFilePath = model._modelFilePath;
	_modelName = model._modelName;
	_trackType = model._trackType;

	initTrackLabel();

	_modelNode = (osg::Node*)(model._modelNode->clone(copyop));
	addModelSelfChild(_modelNode);

	getModel()->setName(_modelName);

	addTrackUpdateCallback(false);
	addLabelUpdateCallback(false);
}

CModel2D::~CModel2D()
{
	//
}

void CModel2D::initModel()
{
	//model2D
	removeAllModelSelfChildren();
	osg::Texture2D* pTex = createTexture2D(_modelFilePath, osg::Texture::CLAMP_TO_BORDER, osg::Texture::CLAMP_TO_BORDER);
	_modelNode = createNode(pTex, 0, 64, 64, 0, 0, _modelColor, false, true);
	if (_modelNode)
	{
		addModelSelfChild(_modelNode);
		_adjustVar = 0.0;

		_modelNode->setName("modelNode");
		//for (int i = 0; i < _modelNode->asGroup()->getNumChildren(); i++)
		//{
		//	_modelNode->asGroup()->getChild(i)->setName("_modelNodelChild");
		//}
	}
	else
	{
		std::cout << "CModel2D::initModel() Error" << std::endl;
		std::cout << "	load 2D symbol fail" << std::endl;
	}
}

void CModel2D::setModelAnimationPathCallback(osg::ref_ptr<osg::Vec3Array> pointArray, double speed, osg::Matrix matToWorld)
{
	osg::ref_ptr<osg::AnimationPath> ap = getAnimationPathfromArray(pointArray, false, speed, matToWorld);
	getModel()->addUpdateCallback(new osg::AnimationPathCallback(ap));
}

//void CModel2D::setPositionLLA(osg::Vec3f lla)
//{
//
//}