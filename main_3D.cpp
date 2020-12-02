//#include "Common/DataStruct.h"
//#include "Common/DigitalEarth.h"
//#include "Common/Model2D.h"
//#include "Common//Model3D.h"
//#include "Common/Utility.h"
//#include "Common/Explosion.h"
//#include "Common/PickHandle.h"
//#include "Common//PickDragHandler.h"
//
//#include <iostream>
//
//int main(int argc, char** argv)
//{
//	DigitalEarth* earth = new DigitalEarth(MapType::Map3D);
//	earth->addImageLayer("GoogleImage2D", false, OEDriverType::TMS, "D:/EarthMap/Openstreet/tms.xml");
//	earth->addImageLayer("GoogleImage3D", true, OEDriverType::TMS, "D:/EarthMap/yingxiang/tms.xml");
//	earth->addElevationLayer("3DElevation", true, OEDriverType::VBP, "D:/EarthMap/yingxiang/output/earth.ive");
//
//	ModelConfig mc = ModelConfig();
//	CModel3D* m3 = new CModel3D(mc);
//
//	m3->setModelName(m3->getModelName() + "-" + (std::to_string(0) + std::to_string(0)));
//	m3->setModelScale(osg::Vec3(5, 5, 5));
//
//	osg::Matrix mat3D = getWorldMatrixfromLonLatHeight(true, 106.56, 29.55, 2000);
//	osg::ref_ptr<osg::Vec3Array> vecArray3D = createCirclePath(7000, mat3D);
//	m3->setModelAnimationPathCallback(vecArray3D, 1000, mat3D);
//	//m3->setModelMatrix(mat3D);
//	earth->addChild3D(m3->getModelGroup());
//
//	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//	geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0, 0.0, 0.0), 10000000.0, 1000000.0, 10000.0)));
//	earth->addChild3D(geode);
//
//	//m3->closeModelAutoTransform();
//
//	//earth->setTetherNode(m3->getTrackGeode());
//
//	return earth->start();
//}