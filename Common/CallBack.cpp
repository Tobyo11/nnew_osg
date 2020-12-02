/*
	标牌显示内容需要和发送的信息一致,而不是简单取包围盒中心
	只有当发送的信息不包含地理位置时,才选取包围盒中心

*/

#include "CallBack.h"
#include "Utility.h"

#include <osg/Switch>
#include <osg/Timer>
#include <osg/AnimationPath>
#include <osg/LineWidth>

TrackCallback::TrackCallback(osg::ref_ptr<osg::MatrixTransform> model, TrackType trackType, bool is3D, double adjustVar, int length, osg::Vec4f color, int width /*= 0*/) :
	_model(model),
	_trackType(trackType),
	_length(length),
	_width(width),
	_color(color),
	_is3D(is3D),
	_first(true)
{
	_adjustVar = 0.1 * adjustVar;
}

void TrackCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{
	osg::ref_ptr<osg::Geometry> geom = dynamic_cast<osg::Geometry*>(node);
	if (geom && _model)
	{
		switch (_trackType)
		{
		case TrackType::Point:
		{//点尾迹
			osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*> (geom->getVertexArray());
			if (verArray->size() < _length)
			{//小于设定长度时,长度增加
				if (verArray->at(0) == osg::Vec3())
					verArray->clear();
				//if (!_is3D)
				//{
					verArray->push_back(_model->getBound().center());//取模型中心2D
				//}
				//else
				//{
				//	osg::Vec3d tailPoint = (osg::Vec3d(0.0, _adjustVar * _model->getBound().radius(), 0.0) * _model->getMatrix());
				//	verArray->push_back(tailPoint);//取模型尾部中心3D
				//}
			}
			else
			{//大于等于长度时,保持长度不变并更新顶点数组
				/****************去掉顶点数组第一个顶点********************/
				verArray->erase(verArray->begin(), ++verArray->begin());
				//if (!_is3D)
				//{
					verArray->push_back(_model->getBound().center());//取模型中心2D
				//}
				//else
				//{
				//	osg::Vec3d tailPoint = (osg::Vec3d(0.0, _adjustVar * _model->getBound().radius(), 0.0) * _model->getMatrix());
				//	verArray->push_back(tailPoint);//取模型尾部中心3D
				//}
			}
			if (_first)
			{//首次更新时,改变初始化的图元颜色(仅首次)
				osg::ref_ptr<osg::Vec4Array> colorArray = dynamic_cast<osg::Vec4Array*> (geom->getColorArray());
				colorArray->clear();
				colorArray->push_back(_color);
				colorArray->dirty();
				_first = false;
			}

			//更新图元集
			for (int i = 0; i < geom->getPrimitiveSetList().size(); i++)
				geom->removePrimitiveSet(i);
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, verArray->size()));

			verArray->dirty();
			geom->dirtyBound();
		}
		break;
		case TrackType::Line:
		{//线尾迹
			osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*> (geom->getVertexArray());
			if (verArray->size() < _length)
			{//小于设定长度时,长度增加
				if (verArray->at(0) == osg::Vec3())
					verArray->clear();
				//if (!_is3D)
				//{
					verArray->push_back(_model->getBound().center());//取模型中心2D
				//}
				//else
				//{
				//	osg::Vec3d tailPoint = (osg::Vec3d(0.0, _adjustVar * _model->getBound().radius(), 0.0) * _model->getMatrix());
				//	verArray->push_back(tailPoint);//取模型尾部中心3D
				//}
			}
			else
			{//大于等于长度时,保持长度不变并更新顶点数组
				/****************去掉顶点数组第一个顶点********************/
				verArray->erase(verArray->begin(), ++verArray->begin());
				//if (!_is3D)
				//{
					verArray->push_back(_model->getBound().center());//取模型中心2D
				//}
				//else
				//{
				//	osg::Vec3d tailPoint = (osg::Vec3d(0.0, _adjustVar * _model->getBound().radius(), 0.0) * _model->getMatrix());
				//	verArray->push_back(tailPoint);//取模型尾部中心3D
				//}
			}
			if (_first)
			{//首次更新时,改变初始化的图元颜色(仅首次)
				osg::ref_ptr<osg::Vec4Array> colorArray = dynamic_cast<osg::Vec4Array*> (geom->getColorArray());
				colorArray->clear();
				colorArray->push_back(_color);
				colorArray->dirty();
				_first = false;
			}

			//更新图元集
			for (int i = 0; i < geom->getPrimitiveSetList().size(); i++)
				geom->removePrimitiveSet(i);
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, verArray->size()));

			verArray->dirty();
			geom->dirtyBound();
		}
		break;
		case TrackType::Band:
		{
			/*	20201125
				尾迹缩放新策略,动态读取AutoTransform和modelSelf的scale以及模型自身大小,根据这些参数进行飘带尾迹的宽度调整
			*/
			//osg::ref_ptr<osg::AutoTransform> modelAutoTransform = dynamic_cast<osg::AutoTransform*>(_model->getChild(0));
			//_width = modelAutoTransform->getBound().radius();
			osg::ref_ptr<osg::Geode> geode = dynamic_cast<osg::Geode*> (geom->getParent(0));
			osg::ref_ptr<osg::Group> modelgroup = dynamic_cast<osg::Group*>(geode->getParent(0));
			osg::ref_ptr<osg::MatrixTransform> model = dynamic_cast<osg::MatrixTransform*>(modelgroup->getChild(0));
			osg::ref_ptr<osg::AutoTransform> modelat = dynamic_cast<osg::AutoTransform*>(model->getChild(0));
			osg::ref_ptr<osg::MatrixTransform> modelSelf = dynamic_cast<osg::MatrixTransform*>(modelat->getChild(0));
			osg::ref_ptr<osg::Node> modelNode = dynamic_cast<osg::Node*> (modelSelf->getChild(0));

			//std::cout << model->getBound().radius() << std::endl;
			//std::cout << modelat->getScale().x() <<" "<< modelat->getScale().y() << " "<< modelat->getScale().z() <<std::endl;
			//std::cout << modelSelf->getMatrix().getScale().x() << " " << modelSelf->getMatrix().getScale().y() << " " << modelSelf->getMatrix().getScale().z() << std::endl;
			//std::cout << geode->getBound().center().x() << std::endl;

			float modelRadius = modelNode->getBound().radius();
			float modelSelfScale = modelSelf->getMatrix().getScale().x();
			float modelAtScale = modelat->getScale().x();
			float width = modelRadius * modelSelfScale * modelAtScale;

			_width = width / 10;

			osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*> (geom->getVertexArray());
			osg::ref_ptr<osg::Vec4Array> colorArray = dynamic_cast<osg::Vec4Array*> (geom->getColorArray());
			if (verArray->size() < _length * 2)
			{
				if (verArray->at(0) == osg::Vec3())
					verArray->clear();
				//if (!_is3D)
				//{
					osg::Vec3d leftPoint = (-osg::Vec3d(_width / 2, 0, 0)) * _model->getMatrix();
					osg::Vec3d rightPoint = (osg::Vec3d(_width / 2, 0, 0)) * _model->getMatrix();
					verArray->push_back(leftPoint);
					verArray->push_back(rightPoint);
				//}
				//else
				//{
				//	osg::Vec3d leftPoint = osg::Vec3d(-_width / 2, _adjustVar * _model->getBound().radius(), 0) * _model->getMatrix();
				//	osg::Vec3d rightPoint = (osg::Vec3d(_width / 2, _adjustVar * _model->getBound().radius(), 0)) * _model->getMatrix();
				//	verArray->push_back(leftPoint);
				//	verArray->push_back(rightPoint);
				//}
				colorArray->clear();
				for (int i = 0; i < verArray->size(); i = i + 2)
				{
					double alpha = ((double)i / verArray->size());
					colorArray->push_back(osg::Vec4f(_color.x(), _color.y(), _color.z(), alpha));
					colorArray->push_back(osg::Vec4f(_color.x(), _color.y(), _color.z(), alpha));
				}
			}
			else
			{
				verArray->erase(verArray->begin(), ++(++verArray->begin()));
				//if (!_is3D)
				//{
					osg::Vec3d leftPoint = (-osg::Vec3d(_width / 2, 0, 0)) * _model->getMatrix();
					osg::Vec3d rightPoint = (osg::Vec3d(_width / 2, 0, 0)) * _model->getMatrix();
					verArray->push_back(leftPoint);
					verArray->push_back(rightPoint);
				//}
				//else
				//{
				//	osg::Vec3d leftPoint = osg::Vec3d(-_width / 2, _adjustVar * _model->getBound().radius(), 0) * _model->getMatrix();
				//	osg::Vec3d rightPoint = (osg::Vec3d(_width / 2, _adjustVar * _model->getBound().radius(), 0)) * _model->getMatrix();
				//	verArray->push_back(leftPoint);
				//	verArray->push_back(rightPoint);
				//}
			}

			for (int i = 0; i < geom->getPrimitiveSetList().size(); i++)
				geom->removePrimitiveSet(i);
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, verArray->size()));

			verArray->dirty();
			colorArray->dirty();
			geom->dirtyBound();
		}
		break;
		default:
			break;
		}
	}

	traverse(node, nv);
}

NTrackCallback::NTrackCallback(osg::ref_ptr<osg::MatrixTransform> model, TrackType trackType, bool is3D, double adjustVar, int length, osg::Vec4f color, int width /*= 0*/):
	_model(model),
	_trackType(trackType),
	_length(length),
	_width(width),
	_color(color),
	_is3D(is3D),
	_first(true)
{
	_adjustVar = 2 * adjustVar;
}

void NTrackCallback::operator()(osg::Node * node, osg::NodeVisitor * nv)
{
	osg::ref_ptr<osg::Geometry> geom = dynamic_cast<osg::Geometry*> (node);
	if (geom && _model)
	{
		osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*> (geom->getVertexArray());
		osg::ref_ptr<osg::Vec4Array> colorArray = dynamic_cast<osg::Vec4Array*> (geom->getColorArray());
		switch (_trackType)
		{
		case TrackType::Point:
		{//点尾迹
			osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*> (geom->getVertexArray());
			if (verArray->size() < _length)
			{//小于设定长度时,长度增加
				if (verArray->at(0) == osg::Vec3())
					verArray->clear();
				if (!_is3D)
				{
					verArray->push_back(_model->getBound().center());//取模型中心2D
				}
				else
				{
					osg::Vec3d tailPoint = (osg::Vec3d(0.0, _adjustVar * _model->getBound().radius(), 0.0) * _model->getMatrix());
					verArray->push_back(tailPoint);//取模型尾部中心3D
				}
			}
			else
			{//大于等于长度时,保持长度不变并更新顶点数组
				/****************去掉顶点数组第一个顶点********************/
				verArray->erase(verArray->begin(), ++verArray->begin());
				if (!_is3D)
				{
					verArray->push_back(_model->getBound().center());//取模型中心2D
				}
				else
				{
					osg::Vec3d tailPoint = (osg::Vec3d(0.0, _adjustVar * _model->getBound().radius(), 0.0) * _model->getMatrix());
					verArray->push_back(tailPoint);//取模型尾部中心3D
				}
			}
			if (_first)
			{//首次更新时,改变初始化的图元颜色(仅首次)
				osg::ref_ptr<osg::Vec4Array> colorArray = dynamic_cast<osg::Vec4Array*> (geom->getColorArray());
				colorArray->clear();
				colorArray->push_back(_color);
				colorArray->dirty();
				_first = false;
			}

			//更新图元集
			for (int i = 0; i < geom->getPrimitiveSetList().size(); i++)
				geom->removePrimitiveSet(i);
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, verArray->size()));

			verArray->dirty();
			geom->dirtyBound();
		}
		break;
		case TrackType::Line:
		{//线尾迹
			osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*> (geom->getVertexArray());
			if (verArray->size() < _length)
			{//小于设定长度时,长度增加
				if (verArray->at(0) == osg::Vec3())
					verArray->clear();
				if (!_is3D)
				{
					verArray->push_back(_model->getBound().center());//取模型中心2D
				}
				else
				{
					osg::Vec3d tailPoint = (osg::Vec3d(0.0, _adjustVar * _model->getBound().radius(), 0.0) * _model->getMatrix());
					verArray->push_back(tailPoint);//取模型尾部中心3D
				}
			}
			else
			{//大于等于长度时,保持长度不变并更新顶点数组
				/****************去掉顶点数组第一个顶点********************/
				verArray->erase(verArray->begin(), ++verArray->begin());
				if (!_is3D)
				{
					verArray->push_back(_model->getBound().center());//取模型中心2D
				}
				else
				{
					osg::Vec3d tailPoint = (osg::Vec3d(0.0, _adjustVar * _model->getBound().radius(), 0.0) * _model->getMatrix());
					verArray->push_back(tailPoint);//取模型尾部中心3D
				}
			}
			if (_first)
			{//首次更新时,改变初始化的图元颜色(仅首次)
				osg::ref_ptr<osg::Vec4Array> colorArray = dynamic_cast<osg::Vec4Array*> (geom->getColorArray());
				colorArray->clear();
				colorArray->push_back(_color);
				colorArray->dirty();
				_first = false;
			}

			//更新图元集
			for (int i = 0; i < geom->getPrimitiveSetList().size(); i++)
				geom->removePrimitiveSet(i);
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, verArray->size()));

			verArray->dirty();
			geom->dirtyBound();
		}
		break;
		case TrackType::Band:
		{
			osg::ref_ptr<osg::AutoTransform> modelat = dynamic_cast<osg::AutoTransform*>(_model->getChild(0));
			osg::ref_ptr<osg::MatrixTransform> modelSelf = dynamic_cast<osg::MatrixTransform*>(modelat->getChild(0));
			osg::ref_ptr<osg::Node> modelNode = dynamic_cast<osg::Node*> (modelSelf->getChild(0));

			float modelRadius = modelNode->getBound().radius();
			float modelSelfScale = modelSelf->getMatrix().getScale().x();
			float modelAtScale = modelat->getScale().x();

			_width = modelRadius * modelSelfScale * modelAtScale / 10;

			if (verArray->size() < _length)
			{
				if (verArray->at(0) == osg::Vec3())
					verArray->clear();

				if (!_is3D)
				{
					verArray->push_back(_model->getBound().center());
				}
				else
				{
					osg::Vec3d tailPoint = (osg::Vec3d(0.0, _adjustVar * _model->getBound().radius(), 0.0) * _model->getMatrix());
					verArray->push_back(tailPoint);//取模型尾部中心3D
				}
				colorArray->clear();

				//透明度最小值于透明度增量
				double minAlpha = 0.3;
				double dxAlpha = (1 - minAlpha) / verArray->size();
				for (int i = 0; i < verArray->size(); i = i + 2)
				{
					double alpha = minAlpha + i * dxAlpha/*((double)i / verArray->size())*/;
					colorArray->push_back(osg::Vec4f(_color.x(), _color.y(), _color.z(), alpha));
					colorArray->push_back(osg::Vec4f(_color.x(), _color.y(), _color.z(), alpha));
				}

				colorArray->dirty();
			}
			else
			{
				verArray->erase(verArray->begin(), ++verArray->begin());
				if (!_is3D)
				{
					verArray->push_back(_model->getBound().center());
				}
				else
				{
					osg::Vec3d tailPoint = (osg::Vec3d(0.0, _adjustVar * _model->getBound().radius(), 0.0) * _model->getMatrix());
					verArray->push_back(tailPoint);//取模型尾部中心3D
				}
			}

			//更新图元集
			for (int i = 0; i < geom->getPrimitiveSetList().size(); i++)
				geom->removePrimitiveSet(i);
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, verArray->size()));
			geom->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(_width), osg::StateAttribute::ON);


			verArray->dirty();
			geom->dirtyBound();
		}
		break;
		default:
			break;
		}
	}
	traverse(node, nv);
}

LabelCallback::LabelCallback(osg::ref_ptr<osg::MatrixTransform> model, CLabel* label, bool is3D)
{
	_model = model;
	_pLabel = label;
	_is3D = is3D;
}

void LabelCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
{//更新位置以及文本内容
	osg::ref_ptr<osg::AutoTransform> labelAutoTransform = dynamic_cast<osg::AutoTransform*>(node);
	if (labelAutoTransform && _pLabel && _model)
	{
		//osg::ref_ptr<osg::AutoTransform> _modelAutoTransform = dynamic_cast<osg::AutoTransform*> (_model->getChild(0));
		//osg::ref_ptr<osg::MatrixTransform> _modelSelf = dynamic_cast<osg::MatrixTransform*> (_modelAutoTransform->getChild(0));
		//osg::ref_ptr<osg::Node> _modelNode = dynamic_cast<osg::Node*> (_modelSelf->getChild(0));

		osg::Vec3d entityCenterPos = _model->getBound().center()/*_model->getMatrix().getTrans()*/;
		labelAutoTransform->setPosition(/*_model->getBound().center()*/entityCenterPos);

		osg::ref_ptr<osg::Group> modelGroup = dynamic_cast<osg::Group*> (_model->getParent(0));
		osg::ref_ptr<osg::Switch> switchMapNode = dynamic_cast<osg::Switch*> (modelGroup->getParent(0));
		osg::ref_ptr<osgEarth::MapNode> mapNode = dynamic_cast<osgEarth::MapNode*> (switchMapNode->getChild(0));
		if (mapNode)
		{
			double lon, lat, height;
			//getLonLatHeightfromWorld(mapNode, entityCenterPos, lon, lat, height);
			getLonLatHeightfromWorld(_is3D, entityCenterPos, lon, lat, height);
			std::string strLon = convertDoubletoString(lon, 2);
			std::string strLat = convertDoubletoString(lat, 2);
			std::string strHei = convertDoubletoString(height, 2);
			std::string strInfo =
				_model->getName() + "\n" +
				"Lon: " + strLon + "\n" +
				"Lat: " + strLat + "\n" +
				"Alt: " + strHei;
			_pLabel->updateTextInfo(strInfo);
			_pLabel->updateTextShowBoxGeom();
			_pLabel->updateTextShowLineGeom();
			_pLabel->updateLableObj();
		}
	}

	traverse(node, nv);
}

TargetLockCallback::TargetLockCallback(osg::ref_ptr<osg::MatrixTransform> model, TargetLock * targetLock, std::vector<osg::Vec3d> vecTargetPos/*std::vector<Target*> vecTarget*/)
{
	_model = model;
	_pTargetLock = targetLock;
	_vecTargetPos = vecTargetPos;
}

TargetLockCallback::TargetLockCallback(osg::ref_ptr<osg::MatrixTransform> model, TargetLock* targetLock, std::vector<osg::MatrixTransform*> vecModel)
{
	_model = model;
	_pTargetLock = targetLock;
	_vecModel = vecModel;
}

void TargetLockCallback::operator()(osg::Node * node, osg::NodeVisitor * nv)
{
	osg::ref_ptr<osg::Geode> lockGeode = dynamic_cast<osg::Geode*>(node);
	if (lockGeode && _model)
	{
		if (_vecTargetPos.size() != 0)
		{
			_pTargetLock->updateLockPos(_model->getBound().center(), _vecTargetPos);
		}
		/*2020.11.30 新加 根据model位置更新*/
		if (_vecModel.size() != 0)
		{
			_pTargetLock->updateLockPos(_model->getBound().center(), _vecModel);
		}
	}
	traverse(node, nv);
}

//RayLineCallback::RayLineCallback(osg::Vec3d startPos, osg::Vec3d endPos, float partLength/*总长度占比*/, float blank/*间隔长度(总长度占比)*/, float step/*每次移动长度*/)
//{
//	_startPos = startPos;
//	_endPos = endPos;
//	_length = (endPos - startPos).length() * partLength;
//	_norVec = (endPos - startPos) / (endPos - startPos).length();
//	_blankLength = (endPos - startPos).length() * blank;
//	_step = step;
//
//	_first = true;
//}
//
//void RayLineCallback::operator()(osg::Node* node, osg::NodeVisitor* nv)
//{
//	osg::ref_ptr<osg::Geometry> geom = dynamic_cast<osg::Geometry*> (node);
//	if (geom)
//	{
//		osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
//		if (_first)
//		{
//			verArray->clear();
//			osg::Vec3d point = _startPos;
//			int i = 0;
//			while ((point - _startPos).length() < (_endPos - _startPos).length())
//			{
//				verArray->push_back(point);
//				if (i % 2 == 0)
//				{
//					point = point + _norVec * _length;
//				}
//				else
//				{
//					point = point + _norVec * _blankLength;
//				}
//				i++;
//			}
//			if (i % 2 == 1)
//				verArray->push_back(_endPos);
//			_first = false;
//		}
//		else
//		{
//			//先对每个顶点进行移动
//			auto iter = verArray->begin();
//			while (iter != verArray->end())
//			{
//				(*iter) += _norVec * _step;
//				iter++;
//			}
//			//再对顶点进行判断(是否超过线段起点或者终点)
//			//最前面的两个顶点
//			osg::Vec3d firstPos = verArray->at(0);
//			osg::Vec3d secondPos = verArray->at(1);
//			if ((firstPos - secondPos).length() < _length)
//			{
//				verArray->erase(verArray->begin());
//				verArray->insert(verArray->begin(), _startPos);
//			}
//			else
//			{
//				if ((firstPos - _endPos).length() < (_startPos + _norVec * _blankLength - _endPos).length())
//				{
//					verArray->insert(verArray->begin(), _startPos);
//					verArray->insert(verArray->begin(), _startPos + _norVec * _blankLength);
//				}
//			}
//			//最后面的一个顶点
//			osg::Vec3d rfirstPos = verArray->at(verArray->size() - 1);
//			osg::Vec3d rsecondPos = verArray->at(verArray->size() - 2);
//			if ((rsecondPos - _startPos).length() >= (_endPos - _startPos).length())
//			{
//				verArray->pop_back();
//				verArray->pop_back();
//			}
//			rfirstPos = verArray->at(verArray->size() - 1);
//			rsecondPos = verArray->at(verArray->size() - 2);
//			if ((rfirstPos - _startPos).length() > (_endPos - _startPos).length())
//			{
//				verArray->pop_back();
//				verArray->push_back(_endPos);
//			}
//		}
//
//		for (int i = 0; i < geom->getPrimitiveSetList().size(); i++)
//			geom->removePrimitiveSet(i);
//		geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray->size()));
//
//		verArray->dirty();
//	}
//	traverse(node, nv);
//}

NTargetLockCallback::NTargetLockCallback(osg::ref_ptr<osg::MatrixTransform> model, std::vector<osg::MatrixTransform*> vecModel)
{
	_model = model;
	_vecModel = vecModel;
}

void NTargetLockCallback::operator()(osg::Node * node, osg::NodeVisitor * nv)
{
	osg::ref_ptr<osg::MatrixTransform> lock = dynamic_cast<osg::MatrixTransform*>(node);
	if (lock && _model)
	{
		lock->setMatrix(osg::Matrix::translate(_model->getMatrix().getTrans()));
		if (_vecModel.size() == lock->getNumChildren())
		{
			for (unsigned int i = 0; i < lock->getNumChildren(); i++)
			{
				osg::ref_ptr<osg::MatrixTransform> lockSelf = dynamic_cast<osg::MatrixTransform*>(lock->getChild(i));
				osg::Vec3d vec1 = osg::Vec3d(0.0, 1.0, 0.0);
				osg::Vec3d vec2 = _vecModel[i]->getBound().center() - (_model->getBound().center());
				double scale = vec2.length();
				lockSelf->setMatrix(osg::Matrix::scale(osg::Vec3d(scale, scale, scale)) * osg::Matrix::rotate(computeMatrix2Vector(vec1, vec2)));
			}
		}
		
	}

	traverse(node, nv);
}
