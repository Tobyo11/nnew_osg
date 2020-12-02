/*
	!!!��ʱδʹ��,��ק��Ŀǰʹ�õ���PickDragHandler.h

	PickHandler.h
	����class PickHandler
	move()δ����
	3D����£���ģ�͵��ƶ��Ǵ����,��Ҫ���¿���3ά״���µ�ģ���ƶ�����
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
	//ģ�ͽ���¼
	void appendNodePath(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	//���ģ�ͽ���¼
	void clearNodePath() { _sceneNodePath.clear(); }
	//�ı�ģ��״̬
	void changeModelScale();
	////ģ�͵�ѡЧ��
	//void pick(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
	//ģ���ƶ�
	void move(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

private:
	bool _is3D;
	float _mouseX;	
	float _mouseY;	//��¼���xyֵ
	int _count;	//����
	float _enlargeVar;	//�Ŵ�״̬�ķŴ�ϵ��
	int _enlargeNodeNum;	//��ǰ�Ŵ�״̬�������_sceneNodePath�����,-1��ʾû�б��Ŵ�Ľ���_sceneNodePathΪ��
	int _enlargeNodeLast;	//��һ���Ŵ�״̬�Ľ��
	bool _leftCtrlKeyDown;	//��ctrl����true, �ͷ�false
	bool _mouseLeftPush;	//����������true,�ͷ�false
	bool _mouseMove;		//�����������ƶ���true,����ͷ�false

	std::vector<std::pair<bool/*�Ŵ�״̬*/, osg::Node*>> _sceneNodePath;	//��¼������ʱ�ĳ����еĽ�㣨�ɽ���Զ���,��ʼ��ʱȫΪfalse��	

	//osg::ref_ptr<osgManipulator::Selection> _modelSelection;
	//osg::ref_ptr<osgManipulator::Dragger> _modelDragger;
};