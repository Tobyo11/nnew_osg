/*
	Label.h
	����һ��class CLabel
	���ڴ����͹������,���ư�����������,���ƿ��Լ������ı�
*/
#pragma once
#pragma once

#include <iostream>
#include <windows.h>

#include <osg/Array>
#include <osg/Geometry>
#include <osg/AutoTransform>

#include <osgText/Text>

class CLabel
{
public:
	//struct LabelParm
	//{

	//};

	CLabel(const std::string str, osg::Vec4f lineColor, osg::Vec4f fillColor, float margin);
	~CLabel();

	/*��������Geom*/
	//void updateConnectLineGeom(osg::ref_ptr<osg::Geometry> pGeom, osg::Matrixd mat);
	void updateConnectLineGeom();
	/*���·���Geom*/
	//void updateBox(osg::ref_ptr<osg::Geometry>box, const osg::BoundingBox& boundingBox, const osg::Vec4& color, const float& margin = 0, const bool& bGradient = false);
	void updateTextShowBoxGeom(const bool& bGradient = false);
	void updateTextShowLineGeom(const bool& bGradient = false);
	/*���±����ı���λ��*/
	void updateLableObj();
	/*�����ı�����*/
	void updateTextInfo(std::string strInfo);

	/*��ȡ���Ƹ����*/
	osg::ref_ptr<osg::AutoTransform> getLabelRoot() { return _labelAutoTransform; }


	/*��ȡ�ı���Χ��*/
	/*osg::BoundingBox getTextBoudingBox() { return _text->getBoundingBox(); }*/
protected:
	/*����Ĭ�Ϸ���Geom*/
	osg::ref_ptr<osg::Geometry> createDefBoxGeom(const bool& bGradient = false);
	/*��������Geom*/
	osg::ref_ptr<osg::Geometry> createConnectLineGeom();
	/*�����ı�Geom*/
	void createTextGeom(/*LabelParm labelParm, std::string textInfo*/);
	/*�����Զ��任�ڵ�*/
	void createAutoTransformNode(osg::AutoTransform::AutoRotateMode mode, bool bEnableAutoScale, bool bEnbleActive = false);

private:
	void initLabelGroup();
	void initLabelAutoTransform();

private:
	osg::ref_ptr<osg::AutoTransform> _labelAutoTransform;
	osg::ref_ptr<osg::Group> _labelGroup;

	//����
	osg::ref_ptr<osg::Geode> _connectLineGeode;
	osg::ref_ptr<osg::Geometry> _connectLineGeom;
	//���ƿ�+�ı�
	osg::ref_ptr<osg::MatrixTransform> _label;
	osg::ref_ptr<osg::MatrixTransform> _labelSelf;
	osg::ref_ptr<osg::MatrixTransform> _labelObj;		//    |
														//    |
	osg::ref_ptr<osg::Group> _textGroup;				//<---|
	osg::ref_ptr<osgText::Text> _text;					//    |
	osg::ref_ptr<osg::Geode> _textGeode;				//    |
														//    |	
	osg::ref_ptr<osg::Geode> _showBoxGeode;				//<---|
	osg::ref_ptr<osg::Geometry> _textShowLineGeom;
	osg::ref_ptr<osg::Geometry> _textShowBoxGeom;

	float _connectLineLength;	//��ʼ���߳��ȣ�Ĭ��7.0��	
	float _connectLineDegreee;	//��ʼ���߽Ƕȣ�Ĭ��45.0��
	std::string _labelInfo;		//�����ı���Ϣ
	osg::Vec4f _lineColor;		//�����Լ��߿���ɫ
	osg::Vec4f _fillColor;		//�ı����ɫ
	float _margin;				//�ı�����߾�

	//LabelParm _labelParm;		//���ƽṹ�����(��ʱû�õ�)
};
