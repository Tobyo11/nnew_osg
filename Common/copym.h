#pragma once
//��ˮ֮Դ  20180101
#include <Windows.h>
#include <osgGA/CameraManipulator>
#include <osgEarth/MapNode>
#include <osgEarth/Viewpoint>
#include <osgEarth/GeoData>

class EarthWalkManipulator :public osgGA::CameraManipulator
{
public:
	EarthWalkManipulator();
	~EarthWalkManipulator();

	//����������������ʵ�ֵ�4�����麯��  
	virtual void setByMatrix(const osg::Matrixd& matrix) {}  //���������λ����̬����  
	virtual void setByInverseMatrix(const osg::Matrixd& matrix) {}  //�����������ͼ����  
	virtual osg::Matrixd getMatrix() const;  //��ȡ�������̬���� 
	virtual osg::Matrixd getInverseMatrix() const;   //��ȡ�������ͼ���� 

	//���в�����������Ӧ
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);

	// Attach a node to the manipulator.
	virtual void setNode(osg::Node*);
	virtual osg::Node* getNode();
	bool established();

	virtual void computeHomePosition();

	/**
	* Sets the camera position, optionally moving it there over time.
	*/
	//virtual void setViewpoint(const osgEarth::Viewpoint& vp, double duration_s = 0.0);
	virtual void home(double /*unused*/);
	virtual void home(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us);


	void addMouseEvent(const osgGA::GUIEventAdapter& ea);
	bool calcMovement(const osgGA::GUIEventAdapter& ea);

	void flushMouseEventStack();
protected:
	osg::Vec3   _eye;               //�ӵ�λ��  
	osg::Quat    _rotate;            //��ת��̬ 
	osg::ref_ptr<osg::Node>  _root;

	osg::observer_ptr<osg::Node> _node;
	osg::observer_ptr<osgEarth::MapNode>   _mapNode;

	osg::ref_ptr<const osgEarth::SpatialReference> _srs;

	float        _speed;                //�ٶ�
	bool        _transversal;        //����
	// Internal event stack comprising last two mouse events.
	osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t1;
	osg::ref_ptr<const osgGA::GUIEventAdapter> _ga_t0;
};