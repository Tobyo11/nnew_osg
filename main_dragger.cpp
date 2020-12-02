///*
//	osgManipulator::selection完全可以用osg::MatrixTransform替代
//*/
//#include <iostream>
//#include <windows.h>
//
//#include <osg/Geode>
//#include <osg/LineWidth>
//#include <osg/AutoTransform>
//#include <osgViewer/Viewer>
//#include <osgViewer/ViewerEventHandlers>
//#include <osgManipulator/Selection>
//#include <osgManipulator/TranslateAxisDragger>
//#include <osgManipulator/Translate2DDragger>
//
//#include "Common//Model3D.h"
//#include "Common/PickHandle.h"
//
//osg::ref_ptr<osg::Geode> createBox()
//{
//	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//	geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0, 0.0, 0.0), 1.0, 1.0, 1.0)));
//
//	return geode;
//}
//
//int main(int argc, char** argv)
//{
//	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//	osg::ref_ptr<osg::Geode> geode = createBox();
//
//
//	osg::ref_ptr<osg::MatrixTransform> _modelSelf = new osg::MatrixTransform;
//	osg::ref_ptr<osg::AutoTransform> _modelAutoTransform = new osg::AutoTransform;
//	osg::ref_ptr<osg::MatrixTransform> _model = new osg::MatrixTransform;
//	osg::ref_ptr<osg::Group> _modelGroup = new osg::Group;
//
//	_modelSelf->addChild(geode);
//	_modelAutoTransform->addChild(_modelSelf);
//	_model->addChild(_modelAutoTransform);
//	_modelGroup->addChild(_model);
//	root->addChild(_modelGroup);
//
//	_modelAutoTransform->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
//	_modelAutoTransform->setAutoScaleToScreen(true);
//	_modelAutoTransform->setMinimumScale(15.0);
//
//	//osg::Quat rr = osg::Quat(30, osg::Z_AXIS);
//
//	//_modelSelf->setMatrix(osg::Matrix::rotate(osg::PI_2, osg::Z_AXIS)/* * osg::Matrix::scale(10, 10, 10)*//* * osg::Matrix::rotate(45.0, osg::Z_AXIS)*/);
//	//_model->setMatrix(osg::Matrix::translate(10, 0, 0));
//
//	//osg::Quat rotate = _modelSelf->getMatrix().getRotate();
//	//osg::Vec3d scale = _modelSelf->getMatrix().getScale();
//	//scale = osg::Vec3d(scale.x() * 5, scale.y() * 5, scale.z() * 5);
//	//osg::Matrixd mat = osg::Matrix::rotate(rotate)/* * osg::Matrix::scale(scale)*/;
//
//	osg::ref_ptr<osgManipulator::TranslateAxisDragger> _modelDragger = new osgManipulator::TranslateAxisDragger();
//	_modelDragger->setupDefaultGeometry();
//	_modelDragger->setMatrix(osg::Matrix::scale(5, 5, 5) * _modelSelf->getMatrix());
//	_modelDragger->addTransformUpdating(_modelSelf);
//	_modelDragger->setHandleEvents(true);
//	
//	_modelAutoTransform->addChild(_modelDragger);
//	viewer->addEventHandler(new osgViewer::StatsHandler());
//	viewer->setSceneData(root);
//	return viewer->run();
//}