//#include <iostream>
//#include <windows.h>
//
//#include <osg/Geode>
//#include <osg/LineWidth>
//#include <osg/AutoTransform>
//#include <osgViewer/Viewer>
//#include <osgViewer/ViewerEventHandlers>
//
//#include "Common/PickHandle.h"
//
//int main(int argc, char** argv)
//{
//	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//
//	osg::ref_ptr<osg::AutoTransform> autoNode = new osg::AutoTransform;
//	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
//	osg::ref_ptr<osg::Vec3Array> vecArray = new osg::Vec3Array;
//	osg::ref_ptr<osg::Vec4Array> colorArray = new osg::Vec4Array;
//
//
//	vecArray->push_back(osg::Vec3d(1, 0, 1));
//	vecArray->push_back(osg::Vec3d(-1, 0, 1));
//	vecArray->push_back(osg::Vec3d(-1, 0, -1));
//	vecArray->push_back(osg::Vec3d(1, 0, -1));
//	colorArray->push_back(osg::Vec4d(1.0, 0.0, 0.0, 0.2));
//
//	geom->setVertexArray(vecArray);
//	geom->setColorArray(colorArray);
//	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
//	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vecArray->size()));
//	//geom->getOrCreateStateSet()->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
//	geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//	geom->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
//	//geom->getOrCreateStateSet()->setAttributeAndModes(new osg::LineWidth(3), osg::StateAttribute::ON);
//
//	geode->addDrawable(geom);
//	autoNode->addChild(geode);
//	root->addChild(autoNode);
//
//	//autoNode->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
//	//autoNode->setAutoScaleToScreen(true);
//
//	viewer->setSceneData(root);
//
//	viewer->addEventHandler(new PickHandler());
//
//	return viewer->run();
//}