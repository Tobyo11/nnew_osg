/*
	逻辑:
		ctrl + 左键点击模型后出现拖拽坐标轴
*/
#include "PickDragHandler.h"
#include "Utility.h"
#include "CallBack.h"

#include <iostream>
#include <osgViewer/Viewer>

PickDragHandler::PickDragHandler(/*bool is3D*/):
	_is3D(true),
	_mouseX(0.0f),
	_mouseY(0.0f),
	_leftCtrlKeyDown(false),
	_mouseLeftPush(false)
{
	_modelTranslateDragger = new osgManipulator::TranslateAxisDragger;
	_modelTranslateDragger->setName("modelTranslateDragger");
	_modelTranslateDragger->setupDefaultGeometry();
	_modelTranslateDragger->setAxisLineWidth(10.0);

	_modelRotateDragger = new osgManipulator::TrackballDragger;
	_modelRotateDragger->setName("modelRotateDragger");
	_modelRotateDragger->setupDefaultGeometry();
	_modelRotateDragger->setupDefaultGeometry();
	_modelRotateDragger->setAxisLineWidth(10.0);

	_modelSelf = NULL;
}

PickDragHandler::~PickDragHandler()
{

}

bool PickDragHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{

	osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*> (&aa);
	if (NULL == view)
	{
		return false;
	}

	switch (ea.getEventType())
	{
	case osgGA::GUIEventAdapter::KEYDOWN:
	{
		if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Control_L)
		{//左ctrl按下
			_leftCtrlKeyDown = true;
		}
	}
	break;
	case osgGA::GUIEventAdapter::KEYUP:
	{
		if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Control_L)
		{//左ctrl释放
			_leftCtrlKeyDown = false;
		}
	}
	break;
	case osgGA::GUIEventAdapter::PUSH:
	{
		if (_leftCtrlKeyDown)
		{
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{//鼠标左键按下
				//记录鼠标位置
				_mouseLeftPush = true;
				_mouseX = ea.getX();
				_mouseY = ea.getY();
			}
		}
	}
	break;
	case osgGA::GUIEventAdapter::SCROLL:
	{//鼠标滚轮滚动
		if (_leftCtrlKeyDown && _modelSelf && !_mouseLeftPush)
		{//左ctrl按下模型存在且鼠标左键没有按下
			changeDragger();
		}
	}
	break;
	case osgGA::GUIEventAdapter::RELEASE:
	{
		if (_leftCtrlKeyDown)
		{
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{//鼠标左键释放
				_mouseLeftPush = false;
				if (_mouseX == ea.getX() && _mouseY == ea.getY() && _modelSelf == NULL)
				{
					//if (_modelSelf != NULL)
					//	clearDragger();
					//显示拖拽坐标轴
					pick(ea, aa);
				}
			}
			if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{//鼠标右键释放
				if(!_mouseLeftPush)	//左键没有按下时,避免误操作
					clearDragger();
			}
		}
	}
	break;
	default:
		break;
	}

	return false;
}

void PickDragHandler::pick(const osgGA::GUIEventAdapter & ea, osgGA::GUIActionAdapter & aa)
{//是否需要关闭自动缩放,是否变更model矩阵(因为此拖拽器变更了modelSelf矩阵),是否需要隐藏标牌和尾迹等其他结点
//23维未同步
	osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*> (&aa);

	osgUtil::LineSegmentIntersector::Intersections hits;

	if (view->computeIntersections(ea.getX(), ea.getY(), hits))
	{
		osgUtil::LineSegmentIntersector::Intersection intersection = *hits.begin();
		osg::NodePath& nodePath = intersection.nodePath;

		_modelSelf = dynamic_cast<osg::MatrixTransform*>(findNodebyName(nodePath, "modelSelf").get());
		if (_modelSelf != NULL)
		{
			osg::ref_ptr<osg::AutoTransform> modelAutoTransform = dynamic_cast<osg::AutoTransform*>(_modelSelf->getParent(0));
			osg::ref_ptr<osg::MatrixTransform> model = dynamic_cast<osg::MatrixTransform*>(modelAutoTransform->getParent(0));
			osg::ref_ptr<osg::Group> modelGroup = dynamic_cast<osg::Group*>(model->getParent(0));

			for (unsigned int i = 1; i < modelGroup->getNumChildren(); i++)
			{
				modelGroup->getChild(i)->setNodeMask(0);	//隐藏尾迹和标牌
			}

			_modelTranslateDragger->setMatrix(osg::Matrix::scale(10, 10, 10) * _modelSelf->getMatrix());
			_modelTranslateDragger->addTransformUpdating(_modelSelf);
			_modelTranslateDragger->setHandleEvents(true);
			modelAutoTransform->addChild(_modelTranslateDragger);
		}
	}
}

void PickDragHandler::changeDragger()
{
	osg::ref_ptr<osg::AutoTransform> modelAutoTransform = dynamic_cast<osg::AutoTransform*>(_modelSelf->getParent(0));
	if (_modelTranslateDragger->getHandleEvents())
	{
		_modelTranslateDragger->removeTransformUpdating(_modelSelf);
		_modelTranslateDragger->setHandleEvents(false);
		modelAutoTransform->removeChild(_modelTranslateDragger);

		_modelRotateDragger->setMatrix(osg::Matrix::scale(10, 10, 10) * _modelSelf->getMatrix());
		_modelRotateDragger->addTransformUpdating(_modelSelf);
		_modelRotateDragger->setHandleEvents(true);
		modelAutoTransform->addChild(_modelRotateDragger);

	}
	else
	{
		_modelRotateDragger->removeTransformUpdating(_modelSelf);
		_modelRotateDragger->setHandleEvents(false);
		modelAutoTransform->removeChild(_modelRotateDragger);

		_modelTranslateDragger->setMatrix(osg::Matrix::scale(10, 10, 10) * _modelSelf->getMatrix());
		_modelTranslateDragger->addTransformUpdating(_modelSelf);
		_modelTranslateDragger->setHandleEvents(true);
		modelAutoTransform->addChild(_modelTranslateDragger);
	}
}

void PickDragHandler::clearDragger()
{
	if (_modelSelf != NULL)
	{
		osg::ref_ptr<osg::AutoTransform> modelAutoTransform = dynamic_cast<osg::AutoTransform*>(_modelSelf->getParent(0));
		osg::ref_ptr<osg::MatrixTransform> model = dynamic_cast<osg::MatrixTransform*>(modelAutoTransform->getParent(0));
		osg::ref_ptr<osg::Group> modelGroup = dynamic_cast<osg::Group*>(model->getParent(0));

		for (unsigned int i = 1; i < modelGroup->getNumChildren(); i++)
		{
			modelGroup->getChild(i)->setNodeMask(1);	//显示尾迹和标牌
		}

		_modelTranslateDragger->removeTransformUpdating(_modelSelf);
		_modelTranslateDragger->setHandleEvents(false);
		modelAutoTransform->removeChild(_modelTranslateDragger);

		_modelRotateDragger->removeTransformUpdating(_modelSelf);
		_modelRotateDragger->setHandleEvents(false);
		modelAutoTransform->removeChild(_modelRotateDragger);
	}
	_modelSelf = NULL;
}

