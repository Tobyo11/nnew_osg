/*
	DigitalEarth.h
	����һ��class DigitalEarth
	��Ҫ���ܰ���
		��ʼ������,
		��ʼ��view,
		��ʼ��������,
		��ʼ�����������,
		��ʼ������ڵ�
		ά��23ά��ͼ����ת���ӿ�,
		��Ⱦѭ����ʼ���ƽӿڵ�
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
	/*������Ⱦ��ͼ����*/
	void setRenderMapType(bool render2D, bool render3D);
	/*���ͼ���*/
	void addImageLayer(std::string name, bool render3D, OEDriverType driver, std::string filepath);
	/*��Ӹ̲߳�*/
	void addElevationLayer(std::string name, bool render3D, OEDriverType driver, std::string filepath);
	/*���3D�߲���ɫ(�����ļ��ĸ�ʽΪ��1�е�ֵΪ�̸߳߶ȣ���2-5�е�ֵΪRGBA��ʽ����ɫֵ��0-255��ȡֵ��Χ��)*/
	void addElevationColor(std::string name, std::string evevationName, std::string filepath);
	/*�����ض����ֵĸ߲���ɫ����*/
	void setElevationColorVisible(std::string name, bool visible);
	/*���shp��*/
	void addShp(std::string filePath, ShpType shpType);
	/*��ʼ������*/
	void createDrawLine();
	/*�򿪻���*/
	void openDrawLine();
	/*�رջ���*/
	void closeDrawLine();

	/*��Ӻ��ӽ��*/
	template<class T>
	bool addChild2D(const osg::ref_ptr<T>& child) { return _switchMapNode2D ? _switchMapNode2D->addChild(child) : false; }
	template<class T>
	bool addChild3D(const osg::ref_ptr<T>& child) { return _switchMapNode3D ? _switchMapNode3D->addChild(child) : false; }

	/*(3D)��Ӹ��ٽ��*/
	void setTetherNode(osg::Node* node);

	/*��ȡview*/
	osg::ref_ptr<osgViewer::CompositeViewer> getViewer() { return _viewer; }
	osg::ref_ptr<osgViewer::Viewer> getViewer2D() { return _viewer2D; }
	osg::ref_ptr<osgViewer::Viewer> getViewer3D() { return _viewer3D; }
	osg::ref_ptr<osgViewer::View> getViewer3Dfollow() { return _viewer3Dfollow; }

	/*(3Dfollow)����*/
	void create3DfollowView();

	/*(·������)����*/
	void createManipulator();

	/*������·���ڵ�*/
	void addCameraPath(double lon, double lat, double height, double time);

	/*(3Dfollow)��Ӹ��ٽ��*/
	void setTetherNode3Dfollow(osg::Node* node);

	/*����followview*/
	void add3DfollowView();

	/*ɾ��followview*/
	void delet3DfollowView();

	/*��ȡmapnode*/
	osg::ref_ptr<osgEarth::MapNode> getMapNode2D() { return _mapNode2D; }
	osg::ref_ptr<osgEarth::MapNode> getMapNode3D() { return _mapNode3D; }

	/*��ȡswitchMapnode*/
	osg::ref_ptr<osg::Switch> getSwitchMapNode2D() { return _switchMapNode2D; }
	osg::ref_ptr<osg::Switch> getSwitchMapNode3D() { return _switchMapNode3D; }

	/*��ȡskyNode*/
	osg::ref_ptr<osgEarth::Util::SkyNode> getskyNode() { return _skyNode; }

	/*��ȡskyNode*/
	void setdateTime(int _year, int _month, int _day, float _hours);

	/*��ȡMapType*/
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
	osg::ref_ptr<osg::GraphicsContext> gc;//ͼ�νӿ�

	osg::ref_ptr <osg::AnimationPath> _animationPath;//·�����
	osg::ref_ptr<osgEarth::Util::MeasureToolHandler> drawLineTool;//����ָ��

	osg::ref_ptr<osg::Viewport> vw;

	MapType _mapType;

	bool _render2D;
	bool _render3D;
};