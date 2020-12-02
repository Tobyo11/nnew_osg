///*2020.12.01 新加 用作测试函数computeMatrix2Vector*/
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
//#include "Common/Utility.h"
//
//osg::ref_ptr<osg::Geode> createAxis()
//{
//	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//	osg::ref_ptr<osg::Geometry> geom_x = new osg::Geometry;
//	osg::ref_ptr<osg::Geometry> geom_y = new osg::Geometry;
//	osg::ref_ptr<osg::Geometry> geom_z = new osg::Geometry;
//	osg::ref_ptr<osg::Vec3Array> verArray_x = new osg::Vec3Array(1);
//	osg::ref_ptr<osg::Vec3Array> verArray_y = new osg::Vec3Array(1);
//	osg::ref_ptr<osg::Vec3Array> verArray_z = new osg::Vec3Array(1);
//	osg::ref_ptr<osg::Vec4Array> colArray = new osg::Vec4Array;
//
//	verArray_x->push_back(osg::Vec3d(1.0, 0.0, 0.0));
//	verArray_y->push_back(osg::Vec3d(0.0, 1.0, 0.0));
//	verArray_z->push_back(osg::Vec3d(0.0, 0.0, 1.0));
//
//	colArray->push_back(osg::Vec4d(1.0, 1.0, 1.0, 1.0));
//
//	geom_x->setVertexArray(verArray_x);
//	geom_x->setColorArray(colArray);
//	geom_x->setColorBinding(osg::Geometry::BIND_OVERALL);
//	geom_x->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray_x->size()));
//
//	geom_y->setVertexArray(verArray_y);
//	geom_y->setColorArray(colArray);
//	geom_y->setColorBinding(osg::Geometry::BIND_OVERALL);
//	geom_y->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray_y->size()));
//
//	geom_z->setVertexArray(verArray_z);
//	geom_z->setColorArray(colArray);
//	geom_z->setColorBinding(osg::Geometry::BIND_OVERALL);
//	geom_z->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray_z->size()));
//
//	osg::ref_ptr < osg::StateSet> pStateSet = geom_x->getOrCreateStateSet();
//	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
//	pStateSet->setRenderBinDetails(200, "RenderBin");
//	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
//	pStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
//	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//	pStateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
//	pStateSet->setAttributeAndModes(new osg::LineWidth(3), osg::StateAttribute::ON);
//
//	pStateSet = geom_y->getOrCreateStateSet();
//	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
//	pStateSet->setRenderBinDetails(200, "RenderBin");
//	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
//	pStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
//	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//	pStateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
//	pStateSet->setAttributeAndModes(new osg::LineWidth(3), osg::StateAttribute::ON);
//
//	pStateSet = geom_z->getOrCreateStateSet();
//	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
//	pStateSet->setRenderBinDetails(200, "RenderBin");
//	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
//	pStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
//	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//	pStateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
//	pStateSet->setAttributeAndModes(new osg::LineWidth(3), osg::StateAttribute::ON);
//
//	geode->addDrawable(geom_x);
//	geode->addDrawable(geom_y);
//	geode->addDrawable(geom_z);
//
//	return geode;
//}
//
//osg::ref_ptr<osg::Geode> createVec3(osg::Vec3d v)
//{
//	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
//	osg::ref_ptr<osg::Vec3Array> verArray = new osg::Vec3Array(1);
//	osg::ref_ptr<osg::Vec4Array> colArray = new osg::Vec4Array;
//	verArray->push_back(v);
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
//	geode->addDrawable(geom);
//
//	return geode;
//}
//
//int main(int argc, char** argv)
//{
//	osg::ref_ptr<osgViewer::Viewer> view = new osgViewer::Viewer;
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//
//	osg::Vec3d v1 = osg::Vec3d(0.0, 1.0, 0.0);
//	osg::Vec3d v2 = osg::Vec3d(7.0, 4.0, -1.0);
//
//	osg::Quat quat = computeMatrix2Vector(v1, v2);
//
//	v1 = v1 * osg::Matrix::rotate(quat);
//
//	root->addChild(createAxis());
//	root->addChild(createVec3(v1));
//	root->addChild(createVec3(v2));
//
//	view->setSceneData(root);
//	view->addEventHandler(new osgViewer::StatsHandler());
//
//	return view->run();
//}