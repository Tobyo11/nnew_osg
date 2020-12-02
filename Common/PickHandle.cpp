/*
	�����߼�:
		���ctrl����ʱ������¼�����Ӧ��Ӧ:
			�������:��¼���λ�ò���⵱ǰview�����λ�ô���֮�ཻ�Ľ�㲢������Ľ��Ŵ�
			��������������£�:
				������ǰ����:(δ�ƶ����,�����λ�������ص�ģ��)�ɽ���Զ���ηŴ�ģ��(�ԷŴ��ģ����Ҫ�ָ�ԭ����С)
				����������:(δ�ƶ����,�����λ�������ص�ģ��)��Զ�������ηŴ�ģ��(ͬ��)
				�ƶ�		:�ԷŴ��ģ�ͽ����ƶ�(3ά�е��ƶ��������Ϊ:ģ����.���ཻģ�����ĵ����������ķ���������ɵ�ƽ��.���ƶ�)
							�Ƴ�(����ͷ�	:(δ�ƶ����)�ԷŴ��ģ�ͽ��е�ѡЧ��չʾ)
*/
#include "PickHandle.h"
#include "Utility.h"
#include "CallBack.h"

#include <iostream>
#include <osgViewer/Viewer>

PickHandler::PickHandler(bool is3D) :
	_is3D(is3D),
	_mouseX(0.0f),
	_mouseY(0.0f),
	_leftCtrlKeyDown(false),
	_mouseLeftPush(false),
	_mouseMove(false),
	_enlargeVar(2.0),
	_enlargeNodeNum(-1),
	_enlargeNodeLast(-1),
	_count(0)
{
	//_modelSelection = new osgManipulator::Selection;
	//if (is3D)
	//	_modelDragger = new osgManipulator::TranslateAxisDragger;
	//else
	//	_modelDragger = new osgManipulator::Translate2DDragger(osg::Plane(0.0, 0.0, 1.0, 0.0));
}

PickHandler::~PickHandler()
{

}

bool PickHandler::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
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
				_mouseX = ea.getX();
				_mouseY = ea.getY();

				_mouseLeftPush = true;
				clearNodePath();

				//��¼��λ�������н��
				appendNodePath(ea, aa);
				if (_sceneNodePath.size() != 0)
				{//��¼�Ľ��ǿ�ʱ,�ı�ģ��״̬(true�Ŵ�false��С)
					_enlargeNodeNum = 0;
					changeModelScale();
				}
			}
		}
	}
		break;
	case osgGA::GUIEventAdapter::SCROLL:
	{//�����ֹ���
		if (_leftCtrlKeyDown && _mouseLeftPush && !_mouseMove &&_sceneNodePath.size() > 1)
		{//��ctrl����+����������δ�ƶ�+�ཻģ����������1
			if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_UP)
			{//����/ǰ����(����ģ�ͱ���Ϊ�ɽ���Զ)
				_enlargeNodeLast = _enlargeNodeNum;
				_enlargeNodeNum = (++_enlargeNodeNum == _sceneNodePath.size() ? _sceneNodePath.size() - 1 : _enlargeNodeNum);
			}
			if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_DOWN)
			{//����/�����(����ģ�ͱ���Ϊ��Զ����)
				_enlargeNodeLast = _enlargeNodeNum;
				_enlargeNodeNum = (--_enlargeNodeNum == -1 ? 0 : _enlargeNodeNum);
			}
			changeModelScale();
		}
	}
		break;
	case osgGA::GUIEventAdapter::RELEASE:
	{
		if (_leftCtrlKeyDown)
		{
			if (ea.getButton() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{//�������ͷ�

				if (_sceneNodePath.size() != 0)
				{
					//if(_mouseX == ea.getX() && _mouseY == ea.getY())
					//	pick(ea, aa);
					//��ʾģ��
					for (int i = 0; i < _sceneNodePath.size(); i++)
						_sceneNodePath[i].second->setNodeMask(true);

					//���ø�������
					_mouseLeftPush = false;
					_mouseMove = false;
					changeModelScale();
					_enlargeNodeNum = -1;
					_enlargeNodeLast = -1;
				}
			}
		}
	}
		break;
	case osgGA::GUIEventAdapter::DRAG:
		if (_leftCtrlKeyDown)
		{
			if (ea.getButtonMask() == osgGA::GUIEventAdapter::LEFT_MOUSE_BUTTON)
			{//����������ʱ�϶�
				_mouseMove = true;
				if (_sceneNodePath.size() != 0)
				{
					//�ƶ�ǰ���÷�ѡ�нڵ�����,�������ͷź�����ʾ
					for (int i = 0; i < _sceneNodePath.size(); i++)
					{
						if (i != _enlargeNodeNum)
							_sceneNodePath[i].second->setNodeMask(false);
					}
					move(ea, aa);
				}
				
			}
		}
	default:
		break;
	}

	return false;
}

void PickHandler::appendNodePath(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{
	osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*> (&aa);
	osgUtil::LineSegmentIntersector::Intersections hits;
	if (view->computeIntersections(ea.getX(), ea.getY(), hits))
	{
		osg::ref_ptr<osg::Node> nodeLast = new osg::Node;	//�����޳��ظ����
		auto iter = hits.begin();
		while (iter != hits.end())
		{
			osgUtil::LineSegmentIntersector::Intersection intersection = *iter;
			osg::NodePath& nodePath = intersection.nodePath;
			osg::ref_ptr<osg::Node> node = findNodebyName(nodePath, "modelSelf");
			if (node != NULL && node != nodeLast)
			{
				nodeLast = node;
				_sceneNodePath.push_back(std::make_pair(false, node));
			}
			iter++;
		}
		//osgUtil::LineSegmentIntersector::Intersection intersection = *hits.begin();
		//osg::NodePath& nodePath = intersection.nodePath;
		//osg::NodePath& sceneNodePath = findNodePathbyName(nodePath, "modelSelf");
		//for (int i = 0; i < sceneNodePath.size(); i++)
		//{
		//	if(i == 0)
		//		_sceneNodePath.push_back(std::make_pair(true, sceneNodePath[i]));
		//	else
		//		_sceneNodePath.push_back(std::make_pair(false, sceneNodePath[i]));
		//}	
	}
}

void PickHandler::changeModelScale()
{//(�ı�_sceneNodePath����Ҫ����, ����ط����Ե�ǰ��Ҫ�Ŵ�Ľ����Ž��иı�)
	if (_mouseLeftPush) 
	{//����������ʱ
		if (_enlargeNodeNum != -1 && (_enlargeNodeNum < _sceneNodePath.size()))
		{
			_sceneNodePath[_enlargeNodeNum].first = true;
			osg::ref_ptr<osg::MatrixTransform> modelSelf = dynamic_cast<osg::MatrixTransform*> (_sceneNodePath[_enlargeNodeNum].second);
			modelSelf->setMatrix(osg::Matrix::scale(modelSelf->getMatrix().getScale() + osg::Vec3d(_enlargeVar, _enlargeVar, _enlargeVar)));
			if (_enlargeNodeLast != -1 && (_enlargeNodeLast < _sceneNodePath.size()))
			{
				_sceneNodePath[_enlargeNodeLast].first = false;
				osg::ref_ptr<osg::MatrixTransform> modelSelf = dynamic_cast<osg::MatrixTransform*> (_sceneNodePath[_enlargeNodeLast].second);
				modelSelf->setMatrix(osg::Matrix::scale(modelSelf->getMatrix().getScale() - osg::Vec3d(_enlargeVar, _enlargeVar, _enlargeVar)));
			}
		}
	}
	else
	{//������δ����
		if (_enlargeNodeNum != -1 && (_enlargeNodeNum < _sceneNodePath.size()))
		{
			osg::ref_ptr<osg::MatrixTransform> modelSelf = dynamic_cast<osg::MatrixTransform*> (_sceneNodePath[_enlargeNodeNum].second);
			modelSelf->setMatrix(osg::Matrix::scale(modelSelf->getMatrix().getScale() - osg::Vec3d(_enlargeVar, _enlargeVar, _enlargeVar)));
		}
	}
}

void PickHandler::move(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{//�����޸�,Ŀǰ��������ʵ��,�ȳ����������к�,�ܹ�������Ļ�ȡģ�ͺ�����ģ��λ�ã�
	osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*> (&aa);
	//ͨ���ӿ��볡����㽻������ƶ�
		//û��ģ�͹���������
	osgUtil::LineSegmentIntersector::Intersections hits;
	if (view->computeIntersections(ea.getX(), ea.getY(), hits))
	{
		osgUtil::LineSegmentIntersector::Intersection intersection = *hits.begin();
		osg::Vec3f pos = intersection.getWorldIntersectPoint();
		double lon, lat, height;
		getLonLatHeightfromWorld(_is3D, pos, lon, lat, height);
		std::cout << lon << " " << lat << " " << height << std::endl;
		osg::NodePath& nodePath = intersection.nodePath;
		osg::ref_ptr<osg::Node> node = findNodebyName(nodePath, "modelSelf");
		if (node != NULL)
		{
			osg::ref_ptr<osg::AutoTransform> modelAutoTransform = dynamic_cast<osg::AutoTransform*> (node->getParent(0));
			osg::ref_ptr<osg::MatrixTransform> model = dynamic_cast<osg::MatrixTransform*> (modelAutoTransform->getParent(0));
			if (model)
			{
				//if (!_is3D)
				//{//2D
					osg::Matrix mat = model->getMatrix();
					model->setMatrix(osg::Matrix::rotate(model->getMatrix().getRotate()) * osg::Matrix::translate(pos));
				//}
				//else
				//{//3D
				//	//�������ཻģ�����ĵ����������ķ���������ɵ�ƽ��
				//	//osg::Vec3d eye, center, up;
				//	//view->getCamera()->getViewMatrixAsLookAt(eye, center, up);
				//	//osg::Vec3d planeNormal = model->getBound().center() - pos;
				//	//planeNormal.normalize();
				//	//double x = planeNormal.x()*up.x() + planeNormal.y()*up.y() + planeNormal.z()*up.z();
				//	//osg::Plane modelPlane = osg::Plane(planeNormal, model->getBound().center());
				//}
			}
		}
	}

	
}

//void PickHandler::pick(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
//{//ѡ��ʱ��˸(�����ģ��û�������ص������)
//
//	//if (_enlargeNodeNum != -1 && (_enlargeNodeNum < _sceneNodePath.size()))
//	//{
//	//	osg::ref_ptr<osg::Node> node = _sceneNodePath[_enlargeNodeNum].second;
//	//	if (node != NULL)
//	//	{
//	//		//if (node->getUpdateCallback() == NULL)
//	//		//	node->setUpdateCallback(new ModelPickCallback(250));
//	//		if (node->getUpdateCallback() != NULL)
//	//		{
//	//			std::cout << "node->getUpdateCallback() != NULL" << std::endl;
//	//			node->setUpdateCallback(NULL);
//	//			//node->setNodeMask(true);
//	//		}
//	//		else
//	//		{
//	//			std::cout << "node->setUpdateCallback(new ModelPickCallback(250))" << std::endl;
//	//			node->setUpdateCallback(new ModelPickCallback(250));
//	//		}
//	//	}
//	//}
// 
// 
//	//osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*> (&aa);
//
//	//osgUtil::LineSegmentIntersector::Intersections hits;
//
//	//if (view->computeIntersections(ea.getX(), ea.getY(), hits))
//	//{
//	//	osgUtil::LineSegmentIntersector::Intersection intersection = *hits.begin();
//	//	osg::NodePath& nodePath = intersection.nodePath;
//
//	//	osg::ref_ptr<osg::Node> node = findNodebyName(nodePath, "modelSelf");
//	//	if (node != NULL)
//	//	{
//	//		if (node->getUpdateCallback() != NULL)
//	//		{
//	//			node->removeUpdateCallback(node->getUpdateCallback());
//	//			node->setUpdateCallback(NULL);
//	//			node->setNodeMask(true);
//	//		}
//	//		else
//	//		{
//	//			node->setUpdateCallback(new ModelPickCallback(250));
//	//		}
//	//	}
//	//}
//	//	//	////ģ����˸����(���Ƽ�)
//	//	//	//osg::ref_ptr<osg::Uniform> uniform = node->getOrCreateStateSet()->getUniform("uColor");
//	//	//	//if (uniform->getUpdateCallback() != NULL)
//	//	//	//{
//	//	//	//	uniform->setUpdateCallback(NULL);
//	//	//	//	uniform->set(_modelColor);
//	//	//	//}
//	//	//	//else
//	//	//	//{
//	//	//	//	uniform->get(_modelColor);
//	//	//	//	uniform->setUpdateCallback(new ModelTwinkleCallback(0.001, _modelColor));
//	//	//	//}
//	//	//	//ģ��͸���ȸı����(�����ֱ��ı��Ҳ�̫�ÿ�)
//	//	//	//osg::Uniform* uniform = node->getOrCreateStateSet()->getUniform("uColor");
//	//	//	//osg::Vec4f color;
//	//	//	//uniform->get(color);
//	//	//	//if (color.a() != 1.0)
//	//	//	//{
//	//	//	//	color.a() = 1.0;
//	//	//	//	setModelTransparent(node, 1.0);
//	//	//	//}
//	//	//	//else
//	//	//	//{
//	//	//	//	color.a() = _alpha;
//	//	//	//	setModelTransparent(node, _alpha);
//	//	//	//}
//	//	//	//uniform->set(color);
//	//	//	//����ѡ�в���(ȱ�ݣ�������ɫ��ģ����ɫ���)
//	//	//	//osg::ref_ptr<osg::Node> parent = node->getParent(0);
//	//	//	//if (parent && parent->getName() != "outLine")
//	//	//	//{//δѡ��
//	//	//	//	osg::ref_ptr<osgFX::Outline> outline = new osgFX::Outline();
//	//	//	//	outline->setColor(_outLineColor);
//	//	//	//	outline->setWidth(_outLineWidth);
//	//	//	//	outline->addChild(node);
//	//	//	//	outline->setName("outLine");
//	//	//	//	parent->asGroup()->replaceChild(node, outline);
//	//	//	//}
//	//	//	//if (parent && parent->getName() == "outLine")
//	//	//	//{//ѡ��
//	//	//	//	osg::ref_ptr<osg::Node> grandParent = parent->getParent(0);
//	//	//	//	if (grandParent)
//	//	//	//	{
//	//	//	//		grandParent->asGroup()->replaceChild(parent, node);
//	//	//	//	}
//	//	//	//}
//	//	//}
//	//	////Node���ز���
//	//	////int nNodeSize = static_cast<int> (nodePath.size());
//	//	////if (nNodeSize > 0)
//	//	////{
//	//	////	osg::ref_ptr<osg::Node> node = nodePath[nNodeSize - 1];
//	//	////	node->setNodeMask(!node->getNodeMask());
//	//	////}
//	//}
//}

