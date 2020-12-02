#include "DigitalEarth.h"

#include <osgViewer/ViewerEventHandlers>

#include <osgEarth/ImageLayer>
#include <osgEarthDrivers/engine_mp/MPTerrainEngineOptions>
#include <osgEarthDrivers/colorramp/ColorRampOptions>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/vpb/VPBOptions>
#include <osgGA/StateSetManipulator>
#include <osgEarthDrivers/tms/TMSOptions>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthDrivers/vpb/VPBOptions>
#include <osg/PolygonOffset>
#include <osgEarthDrivers/feature_ogr/OGRFeatureOptions>
#include <osgEarthFeatures/FeatureModelLayer>
#include <osgEarthFeatures/FeatureSourceLayer>
#include <osgEarth/GLUtils>
#include <osgEarth/ImageLayer>
#include <osgGA/StateSetManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
class SysncOperation :public osg::GraphicsOperation
{

public:
	SysncOperation() :osg::GraphicsOperation("SysncOperation", false)
	{

	}
	virtual void operator() (osg::GraphicsContext* context)
	{
		if (context)
		{
			context->makeCurrent();
			if (osgViewer::GraphicsWindow* window = dynamic_cast<osgViewer::GraphicsWindow*>(context))
			{
				window->setSyncToVBlank(false); // 关闭显卡垂直同步
			}
		}
	}
};
DigitalEarth::DigitalEarth() :
	_mapType(MapType::Map2D_3D)
{
	init();
}

DigitalEarth::DigitalEarth(MapType mapType) :
	_mapType(mapType)
{
	init();
}

DigitalEarth::~DigitalEarth()
{
	//
}

int DigitalEarth::start()
{
	return _viewer->run();
}

void DigitalEarth::setRenderMapType(bool render2D, bool render3D)
{
	if (_mapType == MapType::Map3D)
	{
		std::cout << "DigitalEarth Warning:" << std::endl;
		std::cout << "	current render mode is Map3D, can not change the rendering type!" << std::endl;
		return;
	}
	if(_mapType == MapType::Map2D_3D)
	{
		if (!render2D && !render3D)
		{
			std::cout << "DigitalEarth::setRenderMapType() Error" << std::endl;
			std::cout << "	must choose one map type rendering!" << std::endl;
			return;
		}
		if (render2D && render3D)
		{//暂时不考虑两种图都渲染的情况
			std::cout << "DigitalEarth::setRenderMapType() Warning:" << std::endl;
			std::cout << "	rendering both maptype not be realized." << std::endl;
			return;
		}
		if (render2D && !render3D)
		{//仅2D
			if (_render2D == false)
			{//状态不同,改变状态
				_render2D = true;
				_render3D = false;

				//相机视图移动
				const osg::GraphicsContext::Traits* traits2D = _viewer2D->getCamera()->getGraphicsContext()->getTraits();
				//const osg::GraphicsContext::Traits* traits3D = _viewer3D->getCamera()->getGraphicsContext()->getTraits();
				_viewer2D->getCamera()->setViewport(new osg::Viewport(0, 0, traits2D->width, traits2D->height));
				_viewer3D->getCamera()->setViewport(new osg::Viewport(0, 0, 0, 0));

				//视点保持一致
				_earthManipulator2D->setViewpoint(_earthManipulator3D->getViewpoint());

				//开启关闭对应子节点
				_switchMapNode3D->setAllChildrenOff();
				_switchMapNode2D->setAllChildrenOn();

				////新加 从_viewer移除
				//_viewer->removeView(_viewer3D);
				//_viewer->removeView(_viewer2D);
				for (unsigned int i = 0; i < _viewer->getNumViews(); i++)
					_viewer->removeView(_viewer->getView(i));
				_viewer->addView(_viewer2D);
			}
			else
			{//状态相同,跳过

			}
		}
		if (!render2D && render3D)
		{//仅3D
			if (_render3D == false)
			{//状态不同,改变状态
				_render3D = true;
				_render2D = false;

				//相机视图移动
				const osg::GraphicsContext::Traits* traits3D = _viewer3D->getCamera()->getGraphicsContext()->getTraits();
				//const osg::GraphicsContext::Traits* traits2D = _viewer2D->getCamera()->getGraphicsContext()->getTraits();
				_viewer3D->getCamera()->setViewport(new osg::Viewport(0, 0, traits3D->width, traits3D->height));
				_viewer2D->getCamera()->setViewport(new osg::Viewport(0, 0, 0, 0));

				//视点保持一致
				_earthManipulator3D->setViewpoint(_earthManipulator2D->getViewpoint());

				//开启关闭对应子节点
				_switchMapNode2D->setAllChildrenOff();
				_switchMapNode3D->setAllChildrenOn();

				////新加 从_viewer移除
				//_viewer->removeView(_viewer2D);
				//_viewer->removeView(_viewer3D);
				for (unsigned int i = 0; i < _viewer->getNumViews(); i++)
					_viewer->removeView(_viewer->getView(i));
				_viewer->addView(_viewer3D);
			}
			else
			{//状态相同,跳过

			}
		}
		std::cout << "Current render: " << "2D|" << _render2D << ", " << "3D|" << _render3D << std::endl;
	}
}

void DigitalEarth::addImageLayer(std::string name, bool render3D, OEDriverType driver, std::string filepath)
{
	osg::ref_ptr<osgEarth::ImageLayer> imageLayer = NULL;
	if (driver == OEDriverType::TMS)
	{
		osgEarth::Drivers::TMSOptions tms;
		tms.url() = filepath;
		imageLayer = new osgEarth::ImageLayer(name, tms);
	}
	if (driver == OEDriverType::VBP)
	{
		osgEarth::Drivers::VPBOptions vpb;
		vpb.url() = filepath;
		vpb.primarySplitLevel() = 2;
		vpb.secondarySplitLevel() = 5;
		vpb.directoryStructure() = osgEarth::Drivers::VPBOptions::DS_TASK;
		osgEarth::Config conf_Profile("profile", "global-geodetic");
		osgEarth::ProfileOptions profile(conf_Profile);
		vpb.profile() = profile;
		imageLayer = new osgEarth::ImageLayer(name, vpb);
	}
	if (driver == OEDriverType::GDAL)
	{
		osgEarth::Drivers::GDALOptions gdal;
		gdal.url() = filepath;
		imageLayer = new osgEarth::ImageLayer(name, gdal);
	}
	if (imageLayer.valid())
	{
		if (!render3D && _mapNode2D.valid())
			_mapNode2D->getMap()->addLayer(imageLayer);
		if (render3D && _mapNode3D.valid())
			_mapNode3D->getMap()->addLayer(imageLayer);
	}
}

void DigitalEarth::addElevationLayer(std::string name, bool render3D, OEDriverType driver, std::string filepath)
{
	osg::ref_ptr<osgEarth::ElevationLayer> elevationLayer = NULL;
	if (driver == OEDriverType::TMS)
	{
		osgEarth::Drivers::TMSOptions tms;
		tms.url() = filepath;
		elevationLayer = new osgEarth::ElevationLayer(name, tms);
	}
	if (driver == OEDriverType::VBP)
	{
		osgEarth::Drivers::VPBOptions vpb;
		vpb.url() = filepath;
		vpb.primarySplitLevel() = 2;
		vpb.secondarySplitLevel() = 5;
		vpb.directoryStructure() = osgEarth::Drivers::VPBOptions::DS_TASK;
		osgEarth::Config conf_Profile("profile", "global-geodetic");
		osgEarth::ProfileOptions profile(conf_Profile);
		vpb.profile() = profile;
		elevationLayer = new osgEarth::ElevationLayer(name, vpb);
	}
	if (driver == OEDriverType::GDAL)
	{
		osgEarth::Drivers::GDALOptions gdal;
		gdal.url() = filepath;
		elevationLayer = new osgEarth::ElevationLayer(name, gdal);
	}
	if (elevationLayer.valid())
	{
		if (!render3D && _mapNode2D.valid())
			_mapNode2D->getMap()->addLayer(elevationLayer);
		if (render3D && _mapNode3D.valid())
			_mapNode3D->getMap()->addLayer(elevationLayer);
	}
}

void DigitalEarth::addShp(std::string filePath, ShpType shpType)
{
	//std::string filePath = "C:/Users/72482/Desktop/chinashp/停车场_point.shp";
	//std::string filePath = "world.shp";
	osgEarth::Drivers::OGRFeatureOptions featureData;
	featureData.url() = filePath;

	// Make a feature source layer and add it to the Map:
	osgEarth::Features::FeatureSourceLayerOptions ogrLayer;
	ogrLayer.name() = filePath + "_source";
	ogrLayer.featureSource() = featureData;
	osgEarth::Features::FeatureSourceLayer*  featureSourceLayer = new osgEarth::Features::FeatureSourceLayer(ogrLayer);
	_mapNode3D->getMap()->addLayer(featureSourceLayer);
	osgEarth::Features::FeatureSource *features = featureSourceLayer->getFeatureSource();
	if (!features)
	{
		printf(("无法打开该矢量文件！"));
		return;
	}

	//设置样式
	osgEarth::Symbology::Style style;

	//可见性
	osgEarth::Symbology::RenderSymbol* rs = style.getOrCreate<osgEarth::Symbology::RenderSymbol>();
	rs->depthTest() = false;

	//贴地设置
	osgEarth::Symbology::AltitudeSymbol* alt = style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>();
	alt->clamping() = alt->CLAMP_TO_TERRAIN;
	alt->technique() = alt->TECHNIQUE_DRAPE;

	if (shpType == ShpType::shpLine)
	{
		//设置矢量面样式（包括边界线）
		osgEarth::Symbology::LineSymbol* ls = style.getOrCreateSymbol<osgEarth::Symbology::LineSymbol>();
		ls->stroke()->color() = osgEarth::Symbology::Color("#FA8072");
		ls->stroke()->width() = 1.0;
		ls->tessellationSize()->set(100, osgEarth::Units::KILOMETERS);
	}
	else if (shpType == ShpType::shpIcon)
	{
		osgEarth::Symbology::IconSymbol *is = style.getOrCreateSymbol<osgEarth::Symbology::IconSymbol>();
		is->setImage(osgDB::readImageFile("school.png"));
	}
	else if (shpType == ShpType::shpPoint)
	{
		osgEarth::Symbology::PointSymbol *ps = style.getOrCreateSymbol<osgEarth::Symbology::PointSymbol>();
		ps->fill()->color() = osgEarth::Symbology::Color(0.0, 0.0, 152.0f / 255, 0.8f); //238 230 133
		ps->size() = 10.0;
	}
	else if (shpType == ShpType::shpPolygon)
	{
		osgEarth::Symbology::PolygonSymbol *polygonSymbol = style.getOrCreateSymbol<osgEarth::Symbology::PolygonSymbol>();
		polygonSymbol->fill()->color() = osgEarth::Symbology::Color(152.0f / 255, 251.0f / 255, 152.0f / 255, 0.8f); //238 230 133
		polygonSymbol->outline() = true;
	}
	else if (shpType == ShpType::shpName)
	{
		osgEarth::Symbology::TextSymbol* text = style.getOrCreateSymbol<osgEarth::Symbology::TextSymbol>();
		text->encoding() = osgEarth::Symbology::TextSymbol::ENCODING_UTF8;
		text->font() = "msyh.ttf";//指定中文字体路径

		text->content() = osgEarth::Symbology::StringExpression("[Name]");

		text->size() = 8.0f;
		text->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
		text->fill()->color() = osgEarth::Symbology::Color::White;
		text->halo()->color() = osgEarth::Symbology::Color::Red;
		//text->provider() = "annotation";



	}

	//
	osgEarth::Features::FeatureModelLayerOptions fmlOpt;
	fmlOpt.name() = filePath;
	fmlOpt.featureSourceLayer() = filePath + "_source";
	fmlOpt.enableLighting() = false;
	fmlOpt.styles() = new osgEarth::Symbology::StyleSheet();
	fmlOpt.styles()->addStyle(style);

	osg::ref_ptr<osgEarth::Features::FeatureModelLayer> fml = new osgEarth::Features::FeatureModelLayer(fmlOpt);
	fml->setName("123");
	_mapNode3D->getMap()->addLayer(fml);
}

void DigitalEarth::addElevationColor(std::string name, std::string elevationName, std::string filepath)
{
	osg::ref_ptr<osgEarth::ElevationLayer> elevationLayer = NULL;
	elevationLayer = dynamic_cast<osgEarth::ElevationLayer*>(_mapNode3D->getMap()->getLayerByName(elevationName));
	if (elevationLayer)
	{
		osgEarth::Drivers::ColorRampOptions cro;
		cro.ramp() = filepath;
		cro.elevationLayer() = elevationLayer->options();
		osg::ref_ptr<osgEarth::ImageLayer> colorLayer = new osgEarth::ImageLayer(name, cro);
		_mapNode3D->getMap()->addLayer(colorLayer);
	}
}

void DigitalEarth::setElevationColorVisible(std::string name, bool visible)
{
	osg::ref_ptr<osgEarth::ImageLayer> colorLayer = dynamic_cast<osgEarth::ImageLayer*>(_mapNode3D->getMap()->getLayerByName(name));
	if (colorLayer)
	{
		colorLayer->setVisible(visible);
	}
}

void DigitalEarth::createDrawLine()
{
	drawLineTool = new osgEarth::Util::MeasureToolHandler(_mapNode3D);
}

void DigitalEarth::openDrawLine()
{
	_viewer3D->addEventHandler(drawLineTool);
}

void DigitalEarth::closeDrawLine()
{
	_viewer3D->removeEventHandler(drawLineTool);
}

void DigitalEarth::setTetherNode(osg::Node* node)
{
	_earthManipulator3D->getSettings()->setTetherMode(osgEarth::Util::EarthManipulator::TetherMode::TETHER_CENTER_AND_ROTATION);

	osgEarth::Viewpoint vp = _earthManipulator3D->getViewpoint();
	vp.setHeading(0.0);
	vp.setPitch(-10.0);
	vp.setRange(200.0);
	vp.setNode(node);
	_earthManipulator3D->setViewpoint(vp);
}

void DigitalEarth::create3DfollowView()
{
	_viewer3Dfollow = new osgViewer::View;

	///3DFollow
	_viewer3Dfollow->setSceneData(_root3D);
	_viewer3Dfollow->setName("3Dfollow");

	//3Dfollow
	_earthManipulator3Dfollow = new osgEarth::Util::EarthManipulator;
	_viewer3Dfollow->setCameraManipulator(_earthManipulator3Dfollow);
	_earthManipulator3Dfollow->setHomeViewpoint(osgEarth::Viewpoint("home", 106.56, 29.55, 0.0, 1.0, -86.0, 200000));



	vw = new osg::Viewport(0, 0, 480, 270);
	//3Dfollow
	_viewer3Dfollow->getCamera()->setName("3Dfollow");
	_viewer3Dfollow->getCamera()->setGraphicsContext(gc);
	_viewer3Dfollow->getCamera()->setViewport(NULL);
	_viewer3Dfollow->getCamera()->setProjectionMatrixAsPerspective(60.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 100000000.0f);
	_viewer3Dfollow->getCamera()->setClearColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));
}

void DigitalEarth::createManipulator()
{
	//3D多操作器
	osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> keyPtr = new osgGA::KeySwitchMatrixManipulator();
	//osg::ref_ptr<EarthWalkManipulator> ewm = new EarthWalkManipulator;//漫游操作器

	//使用动画漫游器
	osg::ref_ptr <osgGA::AnimationPathManipulator> animationPathMp = new osgGA::AnimationPathManipulator();
	//给动画漫游器添加关键帧
	_animationPath = new osg::AnimationPath;

	_animationPath->setLoopMode(osg::AnimationPath::SWING);//设置路径是回摆的
	animationPathMp->setAnimationPath(_animationPath);

	keyPtr->addMatrixManipulator('1', "earthMan", _earthManipulator3D);
	keyPtr->addMatrixManipulator('2', "animationPathMan", animationPathMp);
	//keyPtr->addMatrixManipulator('3', "EarthWalkManipulator", ewm);
	_viewer3D->setCameraManipulator(keyPtr);
}

void DigitalEarth::addCameraPath(double lon, double lat, double height, double time)
{
	osgEarth::GeoPoint gPoint(_mapNode3D->getMap()->getSRS(), lon, lat, height);
	osg::Matrix gMatrix1;
	gPoint.createLocalToWorld(gMatrix1);//获取当前地球上的正确姿态
	//由于相机，自身向下看，所以在当前姿态基础上抬起60度，注意是前乘！
	gMatrix1.preMultRotate(osg::Quat(osg::DegreesToRadians(60.0), osg::X_AXIS));
	osg::Quat q;
	gMatrix1.get(q);//获取当前矩阵姿态
	osg::Vec3d vPos = gMatrix1.getTrans();//获取当前矩阵位置
	_animationPath->insert(time, osg::AnimationPath::ControlPoint(vPos, q));
}

void DigitalEarth::setTetherNode3Dfollow(osg::Node * node)
{
	_earthManipulator3Dfollow->getSettings()->setMinMaxPitch(-89, 89);
	_earthManipulator3Dfollow->getSettings()->setTetherMode(osgEarth::Util::EarthManipulator::TetherMode::TETHER_CENTER_AND_HEADING);
	osgEarth::Viewpoint vp = _earthManipulator3Dfollow->getViewpoint();
	vp.setNode(node);
	vp.range() = 500.0;
	vp.heading() = 0.0;
	vp.pitch() = 0.0;

	_earthManipulator3Dfollow->setViewpoint(vp, 3.0);
}

void DigitalEarth::add3DfollowView()
{

	_viewer3Dfollow->getCamera()->setViewport(vw);
	_viewer->addView(_viewer3Dfollow);
}

void DigitalEarth::delet3DfollowView()
{
	_viewer3Dfollow->getCamera()->setViewport(NULL);
	_viewer->removeView(_viewer3Dfollow);
	//_viewer3Dfollow->getCamera()->setViewport(0, 0, 0, 0);

}

void DigitalEarth::init()
{//初始化,即便23D同时渲染,也优先渲染3D
	_viewer = new osgViewer::CompositeViewer;
	_viewer3D = new osgViewer::Viewer;
	_root3D = new osg::Group;
	_root3D->setName("root3D");
	_switchMapNode3D = new osg::Switch;
	_switchMapNode3D->setName("switchMapNode3D");

	if (_mapType == MapType::Map3D)
	{//Map3D
		_viewer2D = NULL;
		_root2D = NULL;
		_switchMapNode2D = NULL;
	}
	if (_mapType == MapType::Map2D_3D)
	{//Map2D_3D
		_viewer2D = new osgViewer::Viewer;
		_root2D = new osg::Group;
		_root2D->setName("root2D");
		_switchMapNode2D = new osg::Switch;
		_switchMapNode2D->setName("switchMapNode2D");
		_switchMapNode2D->setAllChildrenOff();	//关闭2D节点
	}

	initEarth();
	initViewer();

	_viewer->addView(_viewer3D);	
	_viewer->addView(_viewer2D);
	_viewer->setRealizeOperation(new SysncOperation());

	_render2D = false;
	_render3D = true;

	setRenderMapType(_render2D, _render3D);

	osgEarth::GLUtils::setGlobalDefaults(_viewer3D->getCamera()->getOrCreateStateSet());//bug

}

void DigitalEarth::initEarth()
{
	//3DMapNode
	osgEarth::Config conf_3D("type", "geocentric");
	osgEarth::MapOptions options_3D;
	options_3D.mergeConfig(conf_3D);
	options_3D.elevationInterpolation() = osgEarth::ElevationInterpolation::INTERP_BILINEAR;
	osg::ref_ptr<osgEarth::Map> map3D = new osgEarth::Map(options_3D);
	map3D->setName("3DMap");

	osgEarth::MapNodeOptions mapNodeOptions3D;
	mapNodeOptions3D.overlayMipMapping() = true;
	mapNodeOptions3D.overlayResolutionRatio() = 3;
	mapNodeOptions3D.overlayTextureSize() = 4096;
	osgEarth::Drivers::MPTerrainEngine::MPTerrainEngineOptions mpoptions;
	mpoptions.normalizeEdges() = true;
	mapNodeOptions3D.setTerrainOptions(mpoptions);
	_mapNode3D = new osgEarth::MapNode(map3D, mapNodeOptions3D);
	_mapNode3D->setName("3DMapNode");

	if (_mapType == MapType::Map2D_3D)
	{//仅在23D都渲染时，才初始化2Dmapnode
		//2DMapNode
		osgEarth::Config conf_2D("type", "projected");
		osgEarth::MapOptions options_2D;
		osgEarth::Config conf_Profile("profile", "global-mercator");
		osgEarth::ProfileOptions profile(conf_Profile);
		options_2D.profile() = profile;
		options_2D.mergeConfig(conf_2D);

		osg::ref_ptr<osgEarth::Map> map2D = new osgEarth::Map(options_2D);
		map2D->setName("2DMap");
		osgEarth::MapNodeOptions mapNodeOptions2D;
		mapNodeOptions2D.enableLighting() = false;
		_mapNode2D = new osgEarth::MapNode(map2D, mapNodeOptions2D);
		_mapNode2D->setName("2DMapNode");

		_switchMapNode2D->addChild(_mapNode2D);
		_root2D->addChild(_switchMapNode2D);
	}
	else
	{
		_mapNode2D = NULL;
	}

	_switchMapNode3D->addChild(_mapNode3D);
	_root3D->addChild(_switchMapNode3D);

}

void DigitalEarth::initViewer()
{
	_viewer3D->setSceneData(_root3D);
	_viewer3D->setName("3DViewer");
	_viewer3D->addEventHandler(new osgViewer::StatsHandler());
	_viewer3D->addEventHandler(new osgViewer::HelpHandler());
	_viewer3D->addEventHandler(new osgViewer::ThreadingHandler);
	_viewer3D->addEventHandler(new osgGA::StateSetManipulator(_viewer3D->getCamera()->getOrCreateStateSet()));

	//初始化地球操作器
	_earthManipulator3D = new osgEarth::Util::EarthManipulator;
	if (_mapNode3D.valid())
	{
		_earthManipulator3D->setNode(_mapNode3D);
	}
	_earthManipulator3D->setHomeViewpoint(osgEarth::Viewpoint("home", 106.56, 29.55, 0.0, 1.0, -86.0, 200000));
	//_earthManipulator3D->getSettings()->setMinMaxPitch(-90, 90);
	_earthManipulator3D->getSettings()->setArcViewpointTransitions(true);
	_earthManipulator3D->getSettings()->setMinMaxDistance(1.0, 40000000);
	_earthManipulator3D->getSettings()->setTerrainAvoidanceEnabled(true);
	_earthManipulator3D->getSettings()->setTerrainAvoidanceMinimumDistance(1000);
	_viewer3D->setCameraManipulator(_earthManipulator3D);

	//初始化上下文(默认为渲染3D地图状态)
	osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();
	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);
	traits = new osg::GraphicsContext::Traits;
	traits->x = 0;
	traits->y = 0;
	traits->width = width;
	traits->height = height;
	traits->windowDecoration = false;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->samples = 8;
	traits->sampleBuffers = 1;
	gc = osg::GraphicsContext::createGraphicsContext(traits);

	osg::ref_ptr<osg::Camera> camera3D = _viewer3D->getCamera();
	camera3D->setName("3DCamera");
	camera3D->setGraphicsContext(gc);
	camera3D->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	camera3D->setProjectionMatrixAsPerspective(60.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 100000000.0f);
	camera3D->setClearColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));

	if (_mapType == MapType::Map2D_3D)
	{
		_viewer2D->setSceneData(_root2D);
		_viewer2D->setName("2DViewer");
		_viewer2D->addEventHandler(new osgViewer::StatsHandler());
		_viewer2D->addEventHandler(new osgViewer::HelpHandler());
		_viewer2D->addEventHandler(new osgViewer::ThreadingHandler);
		_viewer2D->addEventHandler(new osgGA::StateSetManipulator(_viewer3D->getCamera()->getOrCreateStateSet()));

		_earthManipulator2D = new osgEarth::Util::EarthManipulator;
		if (_mapNode2D.valid())
		{
			_earthManipulator2D->setNode(_mapNode2D);
		}
		_earthManipulator2D->setHomeViewpoint(osgEarth::Viewpoint("home", 106.56, 29.55, 0.0, 1.0, -86.0, 200000));
		_earthManipulator2D->getSettings()->setMinMaxDistance(1.0, 40000000);
		_earthManipulator2D->getSettings()->setMinMaxPitch(-90, -90);
		_viewer2D->setCameraManipulator(_earthManipulator2D);

		osg::ref_ptr<osg::Camera> camera2D = _viewer2D->getCamera();
		camera2D->setName("2DCamera");
		camera2D->setGraphicsContext(gc);
		camera2D->setViewport(new osg::Viewport(0, 0, 0, 0));
		camera2D->setProjectionMatrixAsPerspective(60.0f, static_cast<double>(traits->width) / static_cast<double>(traits->height), 1.0f, 100000000.0f);
		camera2D->setClearColor(osg::Vec4(0.0, 0.0, 0.0, 1.0));
	}

	/*

	//camera2D->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);

	//osg::CullStack::CullingMode cullingMode2D = camera2D->getCullingMode();
	//cullingMode2D &= ~(osg::CullStack::SMALL_FEATURE_CULLING);
	//camera2D->setCullingMode(cullingMode2D);

	//camera3D->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);

	//osg::CullStack::CullingMode cullingMode3D = camera3D->getCullingMode();
	//cullingMode3D &= ~(osg::CullStack::SMALL_FEATURE_CULLING);
	//camera3D->setCullingMode(cullingMode3D);
	*/
}