/*
	PickDragHandler.h
	包含class PickDragHandler

*/
#pragma once

#include <windows.h>

#include <osgGA/GUIEventHandler>
#include <osgManipulator/TranslateAxisDragger>
#include <osgManipulator/TrackballDragger>
#include <osg/MatrixTransform>

class PickDragHandler : public osgGA::GUIEventHandler
{
public:
	PickDragHandler(/*bool is3D*/);
	~PickDragHandler();

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

protected:
	//模型选中后显示拖拽坐标轴
	void pick(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	//拖拽器切换
	void changeDragger();
	//清空拖拽器
	void clearDragger();

private:
	bool _is3D;
	float _mouseX;
	float _mouseY;	//记录鼠标xy值
	bool _leftCtrlKeyDown;	//左ctrl按下true, 释放false
	bool _mouseLeftPush;	//鼠标左键按下true,释放false

	osg::ref_ptr<osgManipulator::TranslateAxisDragger> _modelTranslateDragger;	//三维平移拖拽器
	osg::ref_ptr<osgManipulator::TrackballDragger> _modelRotateDragger;		//三维旋转拖拽器
	osg::ref_ptr<osg::MatrixTransform> _modelSelf;	//模型自身结点

};