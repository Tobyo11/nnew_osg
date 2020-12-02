/*
	!!!暂时未使用,拖拽类目前使用的是PickDragHandler.h

	PickHandler.h
	包含class PickHandler
	move()未完善
	3D情况下，对模型的移动是错误的,需要重新考虑3维状况下的模型移动操作
*/
#pragma once

#include <windows.h>

#include <osgGA/GUIEventHandler>
//#include <osgManipulator/TranslateAxisDragger>
//#include <osgManipulator/Translate2DDragger>

class PickHandler : public osgGA::GUIEventHandler
{
public:
	PickHandler(bool is3D);
	~PickHandler();

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

protected:
	//模型结点记录
	void appendNodePath(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	//清空模型结点记录
	void clearNodePath() { _sceneNodePath.clear(); }
	//改变模型状态
	void changeModelScale();
	////模型点选效果
	//void pick(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	//模型移动
	void move(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
	bool _is3D;
	float _mouseX;	
	float _mouseY;	//记录鼠标xy值
	int _count;	//计数
	float _enlargeVar;	//放大状态的放大系数
	int _enlargeNodeNum;	//当前放大状态结点所在_sceneNodePath的序号,-1表示没有被放大的结点或_sceneNodePath为空
	int _enlargeNodeLast;	//上一个放大状态的结点
	bool _leftCtrlKeyDown;	//左ctrl按下true, 释放false
	bool _mouseLeftPush;	//鼠标左键按下true,释放false
	bool _mouseMove;		//鼠标左键按下移动后true,左键释放false

	std::vector<std::pair<bool/*放大状态*/, osg::Node*>> _sceneNodePath;	//记录左键点击时的场景中的结点（由近至远存放,初始化时全为false）	

	//osg::ref_ptr<osgManipulator::Selection> _modelSelection;
	//osg::ref_ptr<osgManipulator::Dragger> _modelDragger;
};