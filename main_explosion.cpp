//#include <iostream>
//#include <windows.h>
//
//#include <osg/Geode>
//#include <osg/LineWidth>
//#include <osg/AutoTransform>
//#include <osgViewer/Viewer>
//#include <osgViewer/ViewerEventHandlers>
//
//#include "Common/Explosion.h"
//
//int main(int argc, char** argv)
//{
//	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//
//	CExplosion* exp = new CExplosion(1);
//	root->addChild(exp->createExplosionNode(osg::Vec3d()));
//
//	viewer->addEventHandler(new osgViewer::StatsHandler());
//	viewer->setSceneData(root);
//	return viewer->run();
//}