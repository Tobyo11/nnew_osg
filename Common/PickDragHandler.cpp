/*
	�߼�:
		ctrl + ������ģ�ͺ������ק������
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
		{//��ctrl����
			_leftCtrlKeyDown = true;
		}
	}
	break;
	case osgGA::GUIEventAdapter::KEYUP:
	{
		if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Control_L)
		{//��ctrl�ͷ�
			_leftCtrlKeyDown = false;
		}
	}
	break;
	case osgGA::GUIEventAdapter::PUSH:
	{
		if (_leftCtrlKeyDown)
		{
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{//����������
				//��¼���λ��
				_mouseLeftPush = true;
				_mouseX = ea.getX();
				_mouseY = ea.getY();
			}
		}
	}
	break;
	case osgGA::GUIEventAdapter::SCROLL:
	{//�����ֹ���
		if (_leftCtrlKeyDown && _modelSelf && !_mouseLeftPush)
		{//��ctrl����ģ�ʹ�����������û�а���
			changeDragger();
		}
	}
	break;
	case osgGA::GUIEventAdapter::RELEASE:
	{
		if (_leftCtrlKeyDown)
		{
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{//�������ͷ�
				_mouseLeftPush = false;
				if (_mouseX == ea.getX() && _mouseY == ea.getY() && _modelSelf == NULL)
				{
					//if (_modelSelf != NULL)
					//	clearDragger();
					//��ʾ��ק������
					pick(ea, aa);
				}
			}
			if (ea.getButton() == osgGA::GUIEventAdapter::RIGHT_MOUSE_BUTTON)
			{//����Ҽ��ͷ�
				if(!_mouseLeftPush)	//���û�а���ʱ,���������
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
{//�Ƿ���Ҫ�ر��Զ�����,�Ƿ���model����(��Ϊ����ק�������modelSelf����),�Ƿ���Ҫ���ر��ƺ�β�����������
//23άδͬ��
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
				modelGroup->getChild(i)->setNodeMask(0);	//����β���ͱ���
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
			modelGroup->getChild(i)->setNodeMask(1);	//��ʾβ���ͱ���
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

