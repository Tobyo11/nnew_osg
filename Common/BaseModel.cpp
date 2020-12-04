#include "BaseModel.h"
#include "Utility.h"
#include "CallBack.h"

BaseModel::BaseModel()
{
	_modelGroup = new osg::Group;
	_modelGroup->setName("modelGroup");
	_modelSelf = new osg::MatrixTransform;
	_modelSelf->setName("modelSelf");
	_modelAutoTransform = new osg::AutoTransform;
	_modelAutoTransform->setName("modelAutoTransform");
	_model = new osg::MatrixTransform;
	_modelLabel = NULL;
	_modelTrack = NULL;
	_modelTargetLock = new NTargetLock();					/*新加 目标锁定 2020.11.27*//*修改2020.11.30*/
	//_model->addChild(_modelTargetLock->getLockGroup());		/*                        *//*修改2020.11.30*/
	_modelAutoTransform->addChild(_modelSelf);
	_model->addChild(_modelAutoTransform);
	_modelGroup->addChild(_model);

		//_model->addChild(_modelSelf);
		//_modelAutoTransform->addChild(_model);
		//_modelGroup->addChild(_modelAutoTransform);

	_modelAutoTransform->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
	_modelAutoTransform->setAutoScaleToScreen(true);
	_modelAutoTransform->setMinimumScale(20.0);
	//_modelAutoTransform->setAutoScaleTransitionWidthRatio(0.05);

	_adjustVar = 0.0;
}

BaseModel::BaseModel(ModelConfig &mc)
{
	_modelGroup = new osg::Group;
	_modelGroup->setName("modelGroup");
	_modelSelf = new osg::MatrixTransform;
	_modelSelf->setName("modelSelf");
	_modelAutoTransform = new osg::AutoTransform;
	_modelAutoTransform->setName("modelAutoTransform");
	_model = new osg::MatrixTransform;
	_modelLabel = NULL;
	_modelTrack = NULL;
	_modelTargetLock = new NTargetLock();					/*新加 目标锁定 2020.11.27*//*修改2020.11.30*/
	//_model->addChild(_modelTargetLock->getLockGroup());		/*                        *//*修改2020.11.30*/
	_modelAutoTransform->addChild(_modelSelf); 
	_model->addChild(_modelAutoTransform);
	_modelGroup->addChild(_model);

		//_model->addChild(_modelSelf);
		//_modelAutoTransform->addChild(_model);
		//_modelGroup->addChild(_modelAutoTransform);

	_modelAutoTransform->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
	_modelAutoTransform->setAutoScaleToScreen(true);
	_modelAutoTransform->setMinimumScale(20.0);
	//_modelAutoTransform->setAutoScaleTransitionWidthRatio(0.05);

	_adjustVar = 0.0;

	applyModelConfig(mc);
}

//BaseModel::BaseModel(const BaseModel& model, const osg::CopyOp& copyop)
//{
//	//
//}

BaseModel::~BaseModel()
{
	if (_modelTrack)
		delete _modelTrack;
	if (_modelLabel)
		delete _modelLabel;
	if (_modelTargetLock)			/*                      */
		delete 	_modelTargetLock; /*新加 目标锁定 2020.11.27*/
}

void BaseModel::applyModelConfig(ModelConfig& mc)
{
	_modelName = mc.modelName;
	_modelColor = mc.modelColor;
	_modelFilePath = mc.modelFilePath;
	_trackType = mc.trackType;

	_model->setName(_modelName);

	initTrackLabel();
}
/*2020.11.27 新加  添加目标锁定*/
void BaseModel::addTargetLock(std::vector<osg::Vec3d> vecTargetPos/*std::vector<Target*> vecTarget*/)
{
	if (_modelTargetLock)
	{
		_modelGroup->addChild(_modelTargetLock->getLock());
		for (int i = 0; i < vecTargetPos.size(); i++)
		{
			_modelTargetLock->addLock(_modelColor);
		}
		//_modelTargetLock->getLock()->addUpdateCallback(new NTargetLockCallback(_model));
	}
}
/*2020.11.30 新加 重载目标锁定*/
void BaseModel::addTargetLock(std::vector<osg::MatrixTransform*> vecModel)
{
	if (_modelTargetLock)
	{
		_modelGroup->addChild(_modelTargetLock->getLock());
		for (int i = 0; i < vecModel.size(); i++)
		{
			_modelTargetLock->addLock(_modelColor);
		}
		_modelTargetLock->getLock()->addUpdateCallback(new NTargetLockCallback(_model, vecModel));
	}
}

void BaseModel::initTrackLabel()
{
	////添加尾迹
	//if (_modelTrack == NULL)
	//{
	//	//_modelTrack = new CTrack(_trackType, /*osg::Vec4f(0.0, 1.0, 0.0, 1.0)*/_modelColor);
	//	//_modelGroup->addChild(_modelTrack->getTrackGeode());
	//	_modelTrack = new NTrack(_trackType, _modelColor);
	//	_modelGroup->addChild(_modelTrack->getTrackGeode());
	//}

	//添加标牌
	if (_modelLabel == NULL)
	{
		_modelLabel = new CLabel("test\ntest\ntest\n", _modelColor, osg::Vec4f(_modelColor.x(), _modelColor.y(), _modelColor.z(), 0.2), 1);
		_modelGroup->addChild(_modelLabel->getLabelRoot());
	}
}

void BaseModel::setTrackVisible(bool state)
{
	if (_modelTrack)
		_modelTrack->getTrackGeode()->setNodeMask(state);
		//_modelTrack->getTrackAutoTransform()->setNodeMask(state);
}

void BaseModel::setLabelVisible(bool state)
{
	if(_modelLabel)
		_modelLabel->getLabelRoot()->setNodeMask(state);
}

/*void BaseModel::setModelAnimationPathCallback(osg::ref_ptr<osg::Vec3Array> pointArray, double speed, osg::Matrix matToWorld)
{
	osg::ref_ptr<osg::AnimationPath> ap = getAnimationPathfromArray(pointArray, true, speed, matToWorld);
	_model->addUpdateCallback(new osg::AnimationPathCallback(ap));
}*/

void BaseModel::setModelAnimationPathCallback(osg::ref_ptr<osg::AnimationPath> ap)
{
	_model->addUpdateCallback(new osg::AnimationPathCallback(ap));
}

osg::ref_ptr<osg::AnimationPath> BaseModel::getAnimationPathfromArray(osg::ref_ptr<osg::Vec3Array> pointArray, bool rotate, double speed, osg::Matrix matToWorld)
{
	/*特例(圆)*/
	osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath;
	animationPath->setLoopMode(osg::AnimationPath::LOOP);

	double head = 0;
	double time = 0;

	osg::Quat rotation;

	//当前点
	osg::Vec3d positionCur;
	//下一点
	osg::Vec3d positionNext;

	for (auto iter = pointArray->begin(); iter != pointArray->end(); iter++)
	{
		auto iter2 = iter;
		iter2++;

		osg::Vec3d center = osg::Vec3d() * matToWorld;

		//需要判断是不是已经到顶
		//iter2++
		if (iter2 == pointArray->end())
		{
			break;
		}

		positionCur = osg::Vec3d(iter->x(), iter->y(), iter->z());
		positionNext = osg::Vec3d(iter2->x(), iter2->y(), iter2->z());

		//求出水平夹角(以下均以以圆心为原点)
		if (positionCur.x() > center.x())
		{//机头方向由东变西(1 4象限)
			if (positionCur.y() > center.y())
			{//1象限
				head = atan((positionCur.y() - center.y()) / (positionCur.x() - center.x()));
			}
			else
			{//4象限
				head = atan((center.y() - positionCur.y()) / (positionCur.x() - center.x()));
				head = 2 * osg::PI - head;
			}
		}
		if (positionCur.x() == center.x())
		{//y轴方向
			if (positionCur.y() > center.y())
			{//正y轴
				head = osg::PI_2;
			}
			else
			{//负y轴
				head = 3 * osg::PI_2;
			}
		}
		if (positionCur.x() < center.x())
		{//机头方向由西变东(2 3象限)
			if (positionCur.y() > center.y())
			{//2象限
				head = atan((positionCur.y() - center.y()) / (center.x() - positionCur.x()));
				head += osg::PI;
				head = -head;
			}
			else
			{//3象限
				head = atan((center.y() - positionCur.y()) / (center.x() - positionCur.x()));
				head += osg::PI;
			}
		}

		if (!rotate)
			rotation.makeRotate(0, osg::X_AXIS, 0, osg::Y_AXIS, head, osg::Z_AXIS);
		else
			rotation.makeRotate(osg::DegreesToRadians(0.0), osg::X_AXIS, osg::DegreesToRadians(-45.0), osg::Y_AXIS, head - osg::PI, osg::Z_AXIS);

		animationPath->insert(time, osg::AnimationPath::ControlPoint(positionCur, rotation * matToWorld.getRotate()));

		time += (speed == 0 ? 1000000000 : getDistance(positionCur, positionNext) / speed);
	}

	animationPath->insert(time, osg::AnimationPath::ControlPoint(positionNext, rotation * matToWorld.getRotate()));
	return animationPath;
}

void BaseModel::addTrackUpdateCallback(bool render3D)
{
	if (_modelTrack)
		//_modelTrack->getTrackGeom()->setUpdateCallback(new TrackCallback(_model, _modelTrack->getTrackType(), render3D, _adjustVar, 3000, osg::Vec4f(0.0, 1.0, 0.0, 1.0)/*_modelColor*/, 100));
		_modelTrack->getTrackGeom()->setUpdateCallback(new NTrackCallback(_model, _modelTrack->getTrackType(), render3D, _adjustVar, 300, /*osg::Vec4f(0.0, 1.0, 0.0, 1.0)*/_modelColor, 100));
}

void BaseModel::addLabelUpdateCallback(bool render3D)
{
	if (_modelLabel)
		_modelLabel->getLabelRoot()->setUpdateCallback(new LabelCallback(_model, _modelLabel, render3D));
}

const ModelConfig BaseModel::getModelConfig()
{
	ModelConfig mc;
	mc.modelColor = _modelColor;
	mc.modelFilePath = _modelFilePath;
	mc.modelName = _modelName;
	mc.trackType = _trackType;

	return mc;
}

void BaseModel::removeLabelCallback()
{
	if (_modelLabel)
		_modelLabel->getLabelRoot()->removeUpdateCallback(_modelLabel->getLabelRoot()->getUpdateCallback());
}

void BaseModel::removeTrackCallback()
{
	if (_modelTrack)
		_modelTrack->getTrackGeom()->removeUpdateCallback(_modelTrack->getTrackGeom()->getUpdateCallback());
}

void BaseModel::closeModelAutoTransform()
{ 
	if (_modelAutoTransform) 
		_modelAutoTransform->setAutoScaleToScreen(false); 
	//osg::Vec3d scale = getModelScale();
	//scale = osg::Vec3d(scale.x() * 20, scale.y() * 20, scale.z() * 20);
	//setModelScale(scale);
	_modelAutoTransform->setScale(osg::Vec3d(1.0, 1.0, 1.0));
}