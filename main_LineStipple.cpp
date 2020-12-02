//#include <iostream>
//#include <windows.h>
//
//#include <osg/Geometry>
//#include <osg/LineWidth>
//#include <osg/MatrixTransform>
//#include <osgViewer/Viewer>
//#include <osgViewer/ViewerEventHandlers>
//#include <osg/ShapeDrawable>
//
//class SliderCallback :public osg::NodeCallback
//{
//public:
//	SliderCallback(osg::Vec3d startPos, osg::Vec3d endPos, float offset)
//	{
//		_startPos = startPos;
//		_endPos = endPos;
//		_length = (startPos - endPos).length() / 50;
//		_norVec = (endPos - startPos) / (endPos - startPos).length();
//		_lastPos = startPos + _norVec * offset;
//		_step = 0.005;
//	}
//	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
//	{
//		osg::ref_ptr<osg::Geometry> geom = dynamic_cast<osg::Geometry*> (node);
//		if (geom)
//		{
//			auto verArray = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
//			计算起点和终点
//			osg::Vec3d sPos = _lastPos + _norVec * _step;
//			osg::Vec3d ePos = sPos - _norVec * _length;
//
//			if ((sPos - _startPos).length() < _length)
//			{
//				(*verArray)[0] = _startPos;
//				(*verArray)[1] = sPos;
//			}
//			else if ((sPos - _startPos).length() < (_endPos - _startPos).length())
//			{
//				(*verArray)[0] = ePos;
//				(*verArray)[1] = sPos;
//			}
//			else if((sPos - _startPos).length() < (_endPos - _startPos).length() + _length)
//			{
//				(*verArray)[0] = ePos;
//				(*verArray)[1] = _endPos;
//			}
//			else
//			{
//				sPos = _startPos;
//			}
//			_lastPos = sPos;
//
//			for (int i = 0; i < geom->getPrimitiveSetList().size(); i++)
//				geom->removePrimitiveSet(i);
//			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray->size()));
//
//			verArray->dirty();
//		}
//		traverse(node, nv);
//	}
//protected:
//	osg::Vec3d _startPos;
//	osg::Vec3d _endPos;
//	float _length;
//	osg::Vec3d _norVec;
//	osg::Vec3d _lastPos;
//	float _step;	//	移动步长
//};
//
//class RayLineCallback :public osg::NodeCallback
//{
//public:
//	RayLineCallback(osg::Vec3d startPos, osg::Vec3d endPos, float partLength/*总长度占比*/, float blank/*间隔长度(总长度占比)*/, float step/*每次移动长度*/)
//	{
//		_startPos = startPos;
//		_endPos = endPos;
//		_length = (endPos - startPos).length() * partLength;
//		_norVec = (endPos - startPos) / (endPos - startPos).length();
//		_blankLength = (endPos - startPos).length() * blank;
//		_step = step;
//		
//		_first = true;
//	}
//	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
//	{
//		osg::ref_ptr<osg::Geometry> geom = dynamic_cast<osg::Geometry*> (node);
//		if (geom)
//		{
//			osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
//			if (_first)
//			{
//				verArray->clear();
//				osg::Vec3d point = _startPos;
//				int i = 0;
//				while ((point - _startPos).length() < (_endPos - _startPos).length())
//				{
//					verArray->push_back(point);
//					if (i % 2 == 0)
//					{
//						point = point + _norVec * _length;
//					}
//					else
//					{
//						point = point + _norVec * _blankLength;
//					}
//					i++;
//				}
//				if (i % 2 == 1)
//					verArray->push_back(_endPos);
//				_first = false;
//			}
//			else
//			{
//				先对每个顶点进行移动
//				auto iter = verArray->begin();
//				while (iter != verArray->end())
//				{
//					(*iter) += _norVec * _step;
//					iter++;
//				}
//				再对顶点进行判断(是否超过线段起点或者终点)
//				最前面的两个顶点
//				osg::Vec3d firstPos = verArray->at(0);
//				osg::Vec3d secondPos = verArray->at(1);
//				if ((firstPos - secondPos).length() < _length)
//				{
//					verArray->erase(verArray->begin());
//					verArray->insert(verArray->begin(), _startPos);
//				}
//				else
//				{
//					if ((firstPos - _endPos).length() < (_startPos + _norVec * _blankLength - _endPos).length())
//					{
//						verArray->insert(verArray->begin(), _startPos);
//						verArray->insert(verArray->begin(), _startPos + _norVec * _blankLength);
//					}
//				}
//				最后面的一个顶点
//				osg::Vec3d rfirstPos = verArray->at(verArray->size() - 1);
//				osg::Vec3d rsecondPos = verArray->at(verArray->size() - 2);
//				if ((rsecondPos - _startPos).length() >= (_endPos - _startPos).length())
//				{
//					verArray->pop_back();
//					verArray->pop_back();
//				}
//				rfirstPos = verArray->at(verArray->size() - 1);
//				rsecondPos = verArray->at(verArray->size() - 2);
//				if ((rfirstPos - _startPos).length() > (_endPos - _startPos).length())
//				{
//					verArray->pop_back();
//					verArray->push_back(_endPos);
//				}
//			}
//
//			for (int i = 0; i < geom->getPrimitiveSetList().size(); i++)
//				geom->removePrimitiveSet(i);
//			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray->size()));
//
//			verArray->dirty();
//		}
//		traverse(node, nv);
//	}
//protected:
//	osg::Vec3d _startPos;
//	osg::Vec3d _endPos;
//	float _length;
//	float _blankLength;
//	osg::Vec3d _norVec;
//	osg::Vec3d _lastPos;
//	float _step;	//	移动步长
//
//	bool _first;
//};
//
//class RayLinesCallback :public osg::NodeCallback
//{
//public:
//	RayLinesCallback(osg::Vec3d startPos, std::vector<osg::Vec3d> endPos, float partLength/*总长度占比*/, float blank/*间隔长度(总长度占比)*/, float step/*每次移动长度*/)
//	{
//		_startPos = startPos;
//		_endPos = endPos;
//		for (int i = 0; i < _endPos.size(); i++)
//		{
//			_length.push_back((endPos[i] - startPos).length() * partLength);
//			_norVec.push_back((endPos[i] - startPos) / (endPos[i] - startPos).length());
//			_blankLength.push_back((endPos[i] - startPos).length() * blank);
//		}
//		_step = step;
//
//		_first = true;
//	}
//	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
//	{
//		osg::ref_ptr<osg::Geometry> geom = dynamic_cast<osg::Geometry*> (node);
//		if (geom)
//		{
//			osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
//			for (int n = 0; n < _endPos.size(); n++)
//			{
//				if (_first)
//				{
//					verArray->clear();
//					osg::Vec3d point = _startPos;
//					int i = 0;
//					while ((point - _startPos).length() < (_endPos[n] - _startPos).length())
//					{
//						verArray->push_back(point);
//						if (i % 2 == 0)
//						{
//							point = point + _norVec[n] * _length[n];
//						}
//						else
//						{
//							point = point + _norVec[n] * _blankLength[n];
//						}
//						i++;
//					}
//					if (i % 2 == 1)
//						verArray->push_back(_endPos[n]);
//					_first = false;
//				}
//				else
//				{
//					//先对每个顶点进行移动
//					auto iter = verArray->begin();
//					while (iter != verArray->end())
//					{
//						(*iter) += _norVec[n] * _step;
//						iter++;
//					}
//					//再对顶点进行判断(是否超过线段起点或者终点)
//					//最前面的两个顶点
//					osg::Vec3d firstPos = verArray->at(0);
//					osg::Vec3d secondPos = verArray->at(1);
//					if ((firstPos - secondPos).length() < _length[n])
//					{
//						verArray->erase(verArray->begin());
//						verArray->insert(verArray->begin(), _startPos);
//					}
//					else
//					{
//						if ((firstPos - _endPos[n]).length() < (_startPos + _norVec[n] * _blankLength[n] - _endPos[n]).length())
//						{
//							verArray->insert(verArray->begin(), _startPos);
//							verArray->insert(verArray->begin(), _startPos + _norVec[n] * _blankLength[n]);
//						}
//					}
//					//最后面的一个顶点
//					osg::Vec3d rfirstPos = verArray->at(verArray->size() - 1);
//					osg::Vec3d rsecondPos = verArray->at(verArray->size() - 2);
//					if ((rsecondPos - _startPos).length() >= (_endPos[n] - _startPos).length())
//					{
//						verArray->pop_back();
//						verArray->pop_back();
//					}
//					rfirstPos = verArray->at(verArray->size() - 1);
//					rsecondPos = verArray->at(verArray->size() - 2);
//					if ((rfirstPos - _startPos).length() > (_endPos[n] - _startPos).length())
//					{
//						verArray->pop_back();
//						verArray->push_back(_endPos[n]);
//					}
//				}
//			}
//			for (int i = 0; i < geom->getPrimitiveSetList().size(); i++)
//				geom->removePrimitiveSet(i);
//			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray->size()));
//
//			verArray->dirty();
//		}
//		traverse(node, nv);
//	}
//protected:
//	osg::Vec3d _startPos;
//	//osg::Vec3d _endPos;
//	//float _length;
//	//float _blankLength;
//	//osg::Vec3d _norVec;
//	std::vector<osg::Vec3d> _endPos;
//	std::vector<float> _length;
//	std::vector<float> _blankLength;
//	std::vector<osg::Vec3d> _norVec;
//
//	float _step;	//	移动步长
//
//	bool _first;
//};
//
//osg::ref_ptr<osg::Node> createBox()
//{
//	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//	geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0, 0.0, 0.0), 1.0, 1.0, 1.0)));
//
//	return geode;
//}
//
//osg::ref_ptr<osg::Geometry> createSlider(osg::Vec3d startPos, osg::Vec3d endPos/*附着于线段的起始位置*/, float offset)
//{
//	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
//	osg::ref_ptr<osg::Vec3Array> verArray = new osg::Vec3Array(2);
//	osg::ref_ptr<osg::Vec4Array> colArray = new osg::Vec4Array;
//	colArray->push_back(osg::Vec4d(1.0, 1.0, 1.0, 0.0));
//	geom->setVertexArray(verArray);
//	geom->setColorArray(colArray);
//	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
//	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray->size()));
//
//	osg::ref_ptr < osg::StateSet> pStateSet = geom->getOrCreateStateSet();
//	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
//	pStateSet->setRenderBinDetails(90, "RenderBin");
//	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
//	pStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
//	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//	pStateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
//	pStateSet->setAttributeAndModes(new osg::LineWidth(5), osg::StateAttribute::ON);
//
//	geom->addUpdateCallback(new SliderCallback(startPos, endPos, offset));
//
//	return geom;
//}
//
//osg::ref_ptr<osg::Node> createLine(osg::Vec3d startPos, std::vector<osg::Vec3d> endPos)
//{
//	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
//
//	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
//	osg::ref_ptr<osg::Vec3Array> verArray = new osg::Vec3Array;
//	osg::ref_ptr<osg::Vec4Array> colArray = new osg::Vec4Array;
//	verArray->push_back(startPos);
//	verArray->push_back(endPos[0]);
//	colArray->push_back(osg::Vec4d(1.0, 0.0, 0.0, 1.0));
//	geom->setVertexArray(verArray);
//	geom->setColorArray(colArray);
//	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
//	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray->size()));
//
//	osg::ref_ptr < osg::StateSet> pStateSet = geom->getOrCreateStateSet();
//
//	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
//	pStateSet->setRenderBinDetails(200, "RenderBin");
//	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
//	pStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
//	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//	pStateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
//	pStateSet->setAttributeAndModes(new osg::LineWidth(3), osg::StateAttribute::ON);
//
//	
//
//	geode->addDrawable(geom);
//		/*全透明滑块式,在视角移动时会闪烁*/
//		geode->addDrawable(createSlider(startPos, endPos, 0));
//		geode->addDrawable(createSlider(startPos, endPos, 2));
//		geode->addDrawable(createSlider(startPos, endPos, 4));
//		geode->addDrawable(createSlider(startPos, endPos, 6));
//		geode->addDrawable(createSlider(startPos, endPos, 8));
//	/*自主线段生成式*/
//	geom->addUpdateCallback(new RayLineCallback(startPos, endPos[0], 0.2, 0.05, 0.005));
//	mt->setMatrix(osg::Matrix::scale(5, 5, 5));
//	mt->addChild(geode);
//
//	return geode;
//}
//
//
//
//int main(int argc, char** argv)
//{
//	osg::ref_ptr<osgViewer::Viewer> view = new osgViewer::Viewer;
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//
//	std::vector<osg::Vec3d> endPos;
//	endPos.push_back(osg::Vec3d(10, 0, 0));
//	endPos.push_back(osg::Vec3d(0, 10, 0));
//	endPos.push_back(osg::Vec3d(0, 0, 10));
//	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
//	mt->addChild(createLine(osg::Vec3d(), endPos));
//	root->addChild(mt);
//	root->addChild(createBox());
//
//	mt->setMatrix(osg::Matrix::rotate(osg::PI_2, osg::Y_AXIS));
//
//	view->setSceneData(root);
//	view->addEventHandler(new osgViewer::StatsHandler());
//
//	return view->run();
//}