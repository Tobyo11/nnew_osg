/*
	DigitalEarth.h
	包含一个class DigitalEarth
	主要功能包括
		初始化地球,
		初始化view,
		初始化操作器,
		初始化场景根结点,
		初始化地球节点
		维护23维地图场景转换接口,
		渲染循环开始控制接口等
*/
#pragma once

#include "DataStruct.h"

#include <osgViewer/Viewer>
#include <osgViewer/CompositeViewer>
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthUtil/MeasureTool>
#include <osgEarth/MapNode>
#include <osg/AnimationPath>
#include <osgEarthUtil/ExampleResources>
class DigitalEarth
{
public:
	DigitalEarth();
	DigitalEarth(MapType mapType);
	~DigitalEarth();

	int start();
	/*设置渲染地图类型*/
	void setRenderMapType(bool render2D, bool render3D);
	/*添加图像层*/
	void addImageLayer(std::string name, bool render3D, OEDriverType driver, std::string filepath);
	/*添加高程层*/
	void addElevationLayer(std::string name, bool render3D, OEDriverType driver, std::string filepath);
	/*添加3D高层着色(配置文件的格式为第1列的值为高程高度，第2-5列的值为RGBA格式的颜色值（0-255的取值范围）)*/
	void addElevationColor(std::string name, std::string evevationName, std::string filepath);
	/*设置特定名字的高层着色显隐*/
	void setElevationColorVisible(std::string name, bool visible);
	/*添加shp层*/
	void addShp(std::string filePath, ShpType shpType);
	/*初始化画线*/
	void createDrawLine();
	/*打开画线*/
	void openDrawLine();
	/*关闭画线*/
	void closeDrawLine();

	/*添加孩子结点*/
	template<class T>
	bool addChild2D(const osg::ref_ptr<T>& child) { return _switchMapNode2D ? _switchMapNode2D->addChild(child) : false; }
	template<class T>
	bool addChild3D(const osg::ref_ptr<T>& child) { return _switchMapNode3D ? _switchMapNode3D->addChild(child) : false; }

	/*(3D)添加跟踪结点*/
	void setTetherNode(osg::Node* node);

	/*获取view*/
	osg::ref_ptr<osgViewer::CompositeViewer> getViewer() { return _viewer; }
	osg::ref_ptr<osgViewer::Viewer> getViewer2D() { return _viewer2D; }
	osg::ref_ptr<osgViewer::Viewer> getViewer3D() { return _viewer3D; }
	osg::ref_ptr<osgViewer::View> getViewer3Dfollow() { return _viewer3Dfollow; }

	/*(3Dfollow)创建*/
	void create3DfollowView();

	/*(路径漫游)创建*/
	void createManipulator();

	/*添加相机路径节点*/
	void addCameraPath(double lon, double lat, double height, double time);

	/*(3Dfollow)添加跟踪结点*/
	void setTetherNode3Dfollow(osg::Node* node);

	/*呼出followview*/
	void add3DfollowView();

	/*删除followview*/
	void delet3DfollowView();

	/*获取mapnode*/
	osg::ref_ptr<osgEarth::MapNode> getMapNode2D() { return _mapNode2D; }
	osg::ref_ptr<osgEarth::MapNode> getMapNode3D() { return _mapNode3D; }

	/*获取switchMapnode*/
	osg::ref_ptr<osg::Switch> getSwitchMapNode2D() { return _switchMapNode2D; }
	osg::ref_ptr<osg::Switch> getSwitchMapNode3D() { return _switchMapNode3D; }

	/*获取skyNode*/
	osg::ref_ptr<osgEarth::Util::SkyNode> getskyNode() { return _skyNode; }

	/*获取skyNode*/
	void setdateTime(int _year, int _month, int _day, float _hours);

	/*获取MapType*/
	MapType getMapType() { return _mapType; }

protected:
	void init();
	void initEarth();
	void initViewer();

private:
	osg::ref_ptr<osgViewer::CompositeViewer> _viewer;

	osg::ref_ptr<osgViewer::Viewer> _viewer2D;

	osg::ref_ptr<osgViewer::Viewer> _viewer3D;

	osg::ref_ptr<osgViewer::View> _viewer3Dfollow;

	osg::ref_ptr<osg::Group> _root2D;
	osg::ref_ptr<osg::Group> _root3D;

	osg::ref_ptr<osg::Switch> _switchMapNode2D;
	osg::ref_ptr<osg::Switch> _switchMapNode3D;

	osg::ref_ptr<osgEarth::MapNode> _mapNode2D;
	osg::ref_ptr<osgEarth::MapNode> _mapNode3D;

	osg::ref_ptr<osgEarth::Util::EarthManipulator> _earthManipulator2D;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> _earthManipulator3D;
	osg::ref_ptr<osgEarth::Util::EarthManipulator> _earthManipulator3Dfollow;

	osg::ref_ptr<osgEarth::Util::SkyNode> _skyNode;

	osg::ref_ptr<osg::GraphicsContext::Traits> traits;
	osg::ref_ptr<osg::GraphicsContext> gc;//图形接口

	osg::ref_ptr <osg::AnimationPath> _animationPath;//路径相机
	osg::ref_ptr<osgEarth::Util::MeasureToolHandler> drawLineTool;//画线指针

	osg::ref_ptr<osg::Viewport> vw;

	MapType _mapType;

	bool _render2D;
	bool _render3D;
};