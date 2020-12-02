/*
	处理逻辑:
		左侧ctrl按下时对鼠标事件做相应响应:
			左键按下:记录鼠标位置并检测当前view在鼠标位置处与之相交的结点并将最近的结点放大
			（左键按下条件下）:
				滚轮向前滚动:(未移动鼠标,且鼠标位置下有重叠模型)由近至远依次放大模型(以放大的模型需要恢复原来大小)
				滚轮向后滚动:(未移动鼠标,且鼠标位置下有重叠模型)由远至近依次放大模型(同上)
				移动		:对放大的模型进行移动(3维中的移动可以理解为:模型在.以相交模型中心点与相机朝里的方向向量组成的平面.上移动)
							移除(左键释放	:(未移动鼠标)对放大的模型进行点选效果展示)
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
				_mouseX = ea.getX();
				_mouseY = ea.getY();

				_mouseLeftPush = true;
				clearNodePath();

				//记录该位置下所有结点
				appendNodePath(ea, aa);
				if (_sceneNodePath.size() != 0)
				{//记录的结点非空时,改变模型状态(true放大，false缩小)
					_enlargeNodeNum = 0;
					changeModelScale();
				}
			}
		}
	}
		break;
	case osgGA::GUIEventAdapter::SCROLL:
	{//鼠标滚轮滚动
		if (_leftCtrlKeyDown && _mouseLeftPush && !_mouseMove &&_sceneNodePath.size() > 1)
		{//左ctrl按下+鼠标左键按下未移动+相交模型数量大于1
			if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_UP)
			{//向上/前滚动(场景模型表现为由近至远)
				_enlargeNodeLast = _enlargeNodeNum;
				_enlargeNodeNum = (++_enlargeNodeNum == _sceneNodePath.size() ? _sceneNodePath.size() - 1 : _enlargeNodeNum);
			}
			if (ea.getScrollingMotion() == osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_DOWN)
			{//向下/后滚动(场景模型表现为由远至近)
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
			{//鼠标左键释放

				if (_sceneNodePath.size() != 0)
				{
					//if(_mouseX == ea.getX() && _mouseY == ea.getY())
					//	pick(ea, aa);
					//显示模型
					for (int i = 0; i < _sceneNodePath.size(); i++)
						_sceneNodePath[i].second->setNodeMask(true);

					//重置各个变量
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
			{//鼠标左键按下时拖动
				_mouseMove = true;
				if (_sceneNodePath.size() != 0)
				{
					//移动前先让非选中节点隐藏,待按键释放后再显示
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
		osg::ref_ptr<osg::Node> nodeLast = new osg::Node;	//用于剔除重复结点
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
{//(改变_sceneNodePath的主要函数, 其余地方仅对当前需要放大的结点序号进行改变)
	if (_mouseLeftPush) 
	{//鼠标左键按下时
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
	{//鼠标左键未按下
		if (_enlargeNodeNum != -1 && (_enlargeNodeNum < _sceneNodePath.size()))
		{
			osg::ref_ptr<osg::MatrixTransform> modelSelf = dynamic_cast<osg::MatrixTransform*> (_sceneNodePath[_enlargeNodeNum].second);
			modelSelf->setMatrix(osg::Matrix::scale(modelSelf->getMatrix().getScale() - osg::Vec3d(_enlargeVar, _enlargeVar, _enlargeVar)));
		}
	}
}

void PickHandler::move(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
{//（待修改,目前仅作功能实现,等场景管理类有后,能够更方便的获取模型和设置模型位置）
	osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*> (&aa);
	//通过视口与场景结点交点进行移动
		//没有模型管理类的情况
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
				//	//创建以相交模型中心点与相机朝里的方向向量组成的平面
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
//{//选中时闪烁(仅针对模型没有上下重叠的情况)
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
//	//	//	////模型闪烁策略(不推荐)
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
//	//	//	//模型透明度改变策略(仅部分被改变且不太好看)
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
//	//	//	//轮廓选中策略(缺陷，轮廓颜色与模型颜色相近)
//	//	//	//osg::ref_ptr<osg::Node> parent = node->getParent(0);
//	//	//	//if (parent && parent->getName() != "outLine")
//	//	//	//{//未选中
//	//	//	//	osg::ref_ptr<osgFX::Outline> outline = new osgFX::Outline();
//	//	//	//	outline->setColor(_outLineColor);
//	//	//	//	outline->setWidth(_outLineWidth);
//	//	//	//	outline->addChild(node);
//	//	//	//	outline->setName("outLine");
//	//	//	//	parent->asGroup()->replaceChild(node, outline);
//	//	//	//}
//	//	//	//if (parent && parent->getName() == "outLine")
//	//	//	//{//选中
//	//	//	//	osg::ref_ptr<osg::Node> grandParent = parent->getParent(0);
//	//	//	//	if (grandParent)
//	//	//	//	{
//	//	//	//		grandParent->asGroup()->replaceChild(parent, node);
//	//	//	//	}
//	//	//	//}
//	//	//}
//	//	////Node隐藏策略
//	//	////int nNodeSize = static_cast<int> (nodePath.size());
//	//	////if (nNodeSize > 0)
//	//	////{
//	//	////	osg::ref_ptr<osg::Node> node = nodePath[nNodeSize - 1];
//	//	////	node->setNodeMask(!node->getNodeMask());
//	//	////}
//	//}
//}

