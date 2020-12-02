//#include <iostream>
//#include <windows.h>
//
//#include <osgViewer/Viewer>
//#include <osg/Geode>
//#include <osg/Depth>
//#include <osg/ShapeDrawable>
//#include <osgViewer/ViewerEventHandlers>
//#include <osg/MatrixTransform>
//#include <osg/AutoTransform>
//#include <osg/Point>
//
//class CTrackCallback :public osg::NodeCallback
//{
//public:
//	CTrackCallback(osg::Matrix trans)
//	{
//		_trans = trans;
//		_count = 0;
//		_size = 360;
//		_width = 4;
//		_radius = 50;
//		_color = osg::Vec4f(0.0, 1.0, 0.0, 1.0);
//	}
//	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
//	{
//		if (_count > 360)
//			_count -= 360;
//		osg::ref_ptr<osg::Geometry> geom = dynamic_cast<osg::Geometry*>(node);
//		if (geom)
//		{
//			osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*> (geom->getVertexArray());
//			osg::ref_ptr<osg::Vec4Array> colorArray = dynamic_cast<osg::Vec4Array*> (geom->getColorArray());
//			if (verArray->size() < _size * 2)
//			{
//				if (verArray->at(0) == osg::Vec3())
//					verArray->clear();
//
//				//计算中心点
//				double x = _radius * cos(osg::DegreesToRadians((float)_count));
//				double y = _radius * sin(osg::DegreesToRadians((float)_count));
//				osg::Vec3d centerPoint = osg::Vec3d(x, y, 0);
//				//计算中心点到原点的方向向量
//				osg::Vec3d directionVec = centerPoint / centerPoint.length();
//				//计算在方向向量上距离中心点为_width / 2的点
//				osg::Vec3d leftPoint = centerPoint + directionVec * osg::Matrix::scale(_width / 2, _width / 2, _width / 2);
//				osg::Vec3d rightPoint = centerPoint - directionVec * osg::Matrix::scale(_width / 2, _width / 2, _width / 2);
//				verArray->push_back(leftPoint * _trans);
//				verArray->push_back(rightPoint * _trans);
//				
//				colorArray->clear();
//				for (int i = 0; i < verArray->size(); i = i + 2)
//				{
//					double alpha = ((double)i / verArray->size());
//					colorArray->push_back(osg::Vec4f(_color.x(), _color.y(), _color.z(), alpha));
//					colorArray->push_back(osg::Vec4f(_color.x(), _color.y(), _color.z(), alpha));
//				}
//			}
//			else
//			{
//				verArray->erase(verArray->begin(), ++(++verArray->begin()));
//
//				//计算中心点
//				double x = _radius * cos(osg::DegreesToRadians((float)_count));
//				double y = _radius * sin(osg::DegreesToRadians((float)_count));
//				osg::Vec3d centerPoint = osg::Vec3d(x, y, 0);
//				//计算中心点到原点的方向向量
//				osg::Vec3d directionVec = centerPoint / centerPoint.length();
//				//计算在方向向量上距离中心点为_width / 2的点
//				osg::Vec3d leftPoint = centerPoint + directionVec * osg::Matrix::scale(_width / 2, _width / 2, _width / 2);
//				osg::Vec3d rightPoint = centerPoint - directionVec * osg::Matrix::scale(_width / 2, _width / 2, _width / 2);
//				verArray->push_back(leftPoint * _trans);
//				verArray->push_back(rightPoint * _trans);
//			}
//
//			for (int i = 0; i < geom->getPrimitiveSetList().size(); i++)
//				geom->removePrimitiveSet(i);
//			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, verArray->size()));
//
//			verArray->dirty();
//			colorArray->dirty();
//			geom->dirtyBound();
//		}
//		_count++;
//		traverse(node, nv);
//	}
//protected:
//	int _count;
//	int _size;
//	float _width;
//	float _radius;
//	osg::Vec4f _color;
//	osg::Matrix _trans;
//};
//
//int main(int argc, char** argv)
//{
//	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//	osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
//	osg::ref_ptr<osg::AutoTransform> at = new osg::AutoTransform;
//	at->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
//	at->setAutoScaleToScreen(true);
//	at->setMinimumScale(0.0);
//	//at->setMinimumScale(1.0);
//
//	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
//	osg::ref_ptr<osg::Vec3Array> verArray = new osg::Vec3Array(1);
//	osg::ref_ptr<osg::Vec4Array> colArray = new osg::Vec4Array(1);
//	/*verArray->setDataVariance(osg::Object::DYNAMIC);
//	colArray->setDataVariance(osg::Object::DYNAMIC);*/
//
//	geom->setDataVariance(osg::Object::DYNAMIC);
//	geom->setUseDisplayList(false);
//	geom->setUseVertexBufferObjects(true);
//	geom->setVertexArray(verArray);
//	geom->setColorArray(colArray);
//
//	osg::ref_ptr < osg::StateSet> pStateSet = geom->getOrCreateStateSet();
//	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
//	pStateSet->setRenderBinDetails(299, "RenderBin");
//	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
//	pStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
//	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//	pStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::LESS, 0.0, 1.0, true), osg::StateAttribute::ON);
//
//	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
//	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, verArray->size()));
//
//	
//	geode->addDrawable(geom);
//	geom->setUpdateCallback(new CTrackCallback(osg::Matrix::rotate(osg::PI_2, osg::X_AXIS)/* * osg::Matrix::translate(10, 10, 0)*/));
//
//	//osg::ref_ptr<osg::Geometry> geomPoint = new osg::Geometry;
//	//osg::ref_ptr<osg::Vec3Array> verArrayPoint = new osg::Vec3Array;
//	//verArrayPoint->push_back(osg::Vec3d());
//	//osg::ref_ptr<osg::Vec4Array> colArrayPoint = new osg::Vec4Array;
//	//colArrayPoint->push_back(osg::Vec4d(1.0, 0.0, 0.0, 1.0));
//	//geomPoint->setVertexArray(verArrayPoint);
//	//geomPoint->setColorArray(colArrayPoint);
//	//geomPoint->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
//	//geomPoint->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, verArrayPoint->size()));
//	//geomPoint->getOrCreateStateSet()->setAttributeAndModes(new osg::Point(2), osg::StateAttribute::ON);
//	//geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0, 0.0, 0.0), 20.0, 20.0, 20.0)));
//	geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0, 0.0, 0.0), 1.0, 1.0, 1.0)));
//
//
//
//	mt->addChild(geode);
//	//at->addChild(mt);
//	//mt->setMatrix(osg::Matrix::scale(10, 10, 10));
//	//geodeBox->setNodeMask(false);
//	root->addChild(mt);
//	viewer->addEventHandler(new osgViewer::StatsHandler());
//	viewer->setSceneData(root);
//	return viewer->run();
//}