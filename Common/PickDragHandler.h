/*
	PickDragHandler.h
	����class PickDragHandler

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
	//ģ��ѡ�к���ʾ��ק������
	void pick(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	//��ק���л�
	void changeDragger();
	//�����ק��
	void clearDragger();

private:
	bool _is3D;
	float _mouseX;
	float _mouseY;	//��¼���xyֵ
	bool _leftCtrlKeyDown;	//��ctrl����true, �ͷ�false
	bool _mouseLeftPush;	//����������true,�ͷ�false

	osg::ref_ptr<osgManipulator::TranslateAxisDragger> _modelTranslateDragger;	//��άƽ����ק��
	osg::ref_ptr<osgManipulator::TrackballDragger> _modelRotateDragger;		//��ά��ת��ק��
	osg::ref_ptr<osg::MatrixTransform> _modelSelf;	//ģ��������

};