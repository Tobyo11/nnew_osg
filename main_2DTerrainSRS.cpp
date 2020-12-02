///*
//	结果:二维坐标系->osgEarth::SpatialReference::get("spherical-mercator");
//*/
//
//#include "Common/DataStruct.h"
//#include "Common/DigitalEarth.h"
//#include "Common/Model2D.h"
//#include "Common//Model3D.h"
//#include "Common/Utility.h"
//#include "Common/Explosion.h"
//#include "Common/PickHandle.h"
//
//#include <iostream>
//
//int main(int argc, char** argv)
//{
//	DigitalEarth* earth = new DigitalEarth();
//	earth->addImageLayer("GoogleImage2D", false, OEDriverType::TMS, "D:/EarthMap/Openstreet/tms.xml");
//	earth->addImageLayer("GoogleImage3D", true, OEDriverType::TMS, "D:/EarthMap/yingxiang/tms.xml");
//	earth->addElevationLayer("3DElevation", true, OEDriverType::VBP, "D:/EarthMap/yingxiang/output/earth.ive");
//
//	const osgEarth::SpatialReference* wgs84 = osgEarth::SpatialReference::get("wgs84");
//	const osgEarth::SpatialReference* terrainSRS = earth->getMapNode2D()->getTerrain()->getSRS();
//	const osgEarth::SpatialReference* srs = osgEarth::SpatialReference::get("spherical-mercator");
//
//	if (srs == terrainSRS)
//		std::cout << "==" << std::endl;
//
//	return earth->start();
//}