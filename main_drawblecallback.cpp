//#include <iostream>
//#include <Windows.h>
//
//#include <osg/Node>
//
//#include <osgViewer/Viewer>
//#include <osgViewer/ViewerEventHandlers>
//
//#include <osgDB/ReadFile>
//#include <osgUtil/Optimizer>
//
//#include <osgEarth/ImageLayer>
//#include <osgEarth/ElevationLayer>
//#include <osgEarth/TerrainEngineNode>
//
//#include <osgEarthDrivers/tms/TMSOptions>
//#include <osgEarthDrivers/xyz/XYZOptions>
//#include <osgEarthDrivers/gdal/GDALOptions>
//#include <osgEarthDrivers/vpb/VPBOptions>
//#include <osgEarthDrivers/engine_mp/MPTerrainEngineOptions>
//#include <osg/LineWidth>
//#include <osgEarthUtil/EarthManipulator>
//#include <osgEarthUtil/Sky>
//#include <osg/AutoTransform>
//using namespace osgEarth;
//using namespace osgEarth::Drivers;
//class DynamicLineCallback : public osg::Drawable::UpdateCallback
//{
//public:
//	DynamicLineCallback() : _angle(0.0) {}
//
//	virtual void update(osg::NodeVisitor* nv, osg::Drawable* drawable)
//	{
//		osg::Geometry* geom = dynamic_cast<osg::Geometry*>(drawable);
//		if (!geom) return;
//
//		osg::Vec3Array* vertices = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
//		if (vertices)
//		{
//			for (osg::Vec3Array::iterator itr = vertices->begin(); itr != vertices->end() - 1; ++itr)
//				itr->set((*(itr + 1)));
//
//			_angle += 1.0 / 10.0;
//			osg::Vec3& pt = vertices->back();
//			pt.set(400.0*cos(_angle), 0.0, 400.0*sin(_angle));
//			vertices->dirty();
//		}
//	}
//
//protected:
//	float _angle;
//};
//int main(int argc, char **argv)
//{
//	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//
//	osgEarth::Config conf_3D("type", "geocentric");
//	osgEarth::MapOptions options_3D;
//	options_3D.mergeConfig(conf_3D);
//	options_3D.elevationInterpolation() = INTERP_BILINEAR;
//	osg::ref_ptr<Map> map = new osgEarth::Map(options_3D);
//	map->setName("3DMap ");
//
//	osgEarth::MapNodeOptions mapNodeOptions;
//	mapNodeOptions.overlayMipMapping() = true;
//	mapNodeOptions.overlayResolutionRatio() = 3;
//	mapNodeOptions.overlayTextureSize() = 4096;
//	MPTerrainEngine::MPTerrainEngineOptions mpoptions;
//	mpoptions.normalizeEdges() = true;
//	mapNodeOptions.setTerrainOptions(mpoptions);
//	osg::ref_ptr<MapNode> mapNode = new osgEarth::MapNode(map, mapNodeOptions);
//
//	/*添加图像层*/
//	osg::ref_ptr<ImageLayer> imageLayer = NULL;
//	TMSOptions tms;
//	tms.url() = "F:\\data\\google\\image\\tms.xml";
//	imageLayer = new ImageLayer("GoogleImage", tms);
//	mapNode->getMap()->addLayer(imageLayer);
//
//	/*添加高程*/
//	osg::ref_ptr<ElevationLayer> elevationLayer = NULL;
//	VPBOptions vpb;
//	vpb.url() = "F:\\data\\Earth\\output\\earth.ive";
//	vpb.primarySplitLevel() = 2;
//	vpb.secondarySplitLevel() = 5;
//	vpb.directoryStructure() = VPBOptions::DS_TASK;
//	osgEarth::Config conf_Profile("profile", "global-geodetic");
//	osgEarth::ProfileOptions profile(conf_Profile);
//	vpb.profile() = profile;
//	elevationLayer = new ElevationLayer("3DElevation", vpb);
//	mapNode->getMap()->addLayer(elevationLayer);
//
//	root->addChild(mapNode);
//
//	osgUtil::Optimizer optimizer;
//	optimizer.optimize(root);
//
//	osg::ref_ptr<osgEarth::Util::EarthManipulator> earthManipulator = new osgEarth::Util::EarthManipulator;
//	if (mapNode.valid())
//	{
//		earthManipulator->setNode(mapNode);
//	}
//	earthManipulator->getSettings()->setArcViewpointTransitions(true);
//	viewer->setCameraManipulator(earthManipulator);
//
//	viewer->addEventHandler(new osgViewer::StatsHandler());
//	viewer->getCamera()->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
//	viewer->getCamera()->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//
//	osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array(10);
//	osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array(10);
//	for (unsigned int i = 0; i < 10; ++i)
//	{
//		(*vertices)[i].set(float(i), 0.0, 0.0);
//		(*colors)[i].set(1.0, 0.0, 0.0, 1.0);
//	}
//
//	osg::ref_ptr<osg::Geometry> lineGeom = new osg::Geometry;
//	lineGeom->setVertexArray(vertices.get());
//	lineGeom->addPrimitiveSet(new osg::DrawArrays(osg::DrawArrays::LINE_STRIP, 0, 10));
//
//	lineGeom->setColorArray(colors);
//	lineGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
//	lineGeom->addPrimitiveSet(new osg::DrawArrays(GL_QUAD_STRIP, 0, 10));
//
//	//lineGeom->setInitialBound(osg::BoundingBox(osg::Vec3(-10.0, -10.0, -10.0), osg::Vec3(10.0, 10.0, 10.0)));
//	lineGeom->setUpdateCallback(new DynamicLineCallback);
//	lineGeom->setUseDisplayList(false);
//	lineGeom->setUseVertexBufferObjects(true);
//
//	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
//	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
//	geode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(2.0));
//	geode->addDrawable(lineGeom.get());
//
//	osg::AutoTransform* at = new osg::AutoTransform;
//
//	at->setAutoScaleToScreen(TRUE);
//	//at->setAutoScaleTransitionWidthRatio(0.0001);
//	at->setMinimumScale(0.0);
//	at->setMaximumScale(FLT_MAX);
//	at->addChild(geode);
//
//	root->addChild(at);
//
//	viewer->setSceneData(root);
//
//	return viewer->run();
//}