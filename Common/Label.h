/*
	Label.h
	包含一个class CLabel
	用于创建和管理标牌,标牌包含标牌连线,标牌框以及标牌文本
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

	/*更新连线Geom*/
	//void updateConnectLineGeom(osg::ref_ptr<osg::Geometry> pGeom, osg::Matrixd mat);
	void updateConnectLineGeom();
	/*更新方盒Geom*/
	//void updateBox(osg::ref_ptr<osg::Geometry>box, const osg::BoundingBox& boundingBox, const osg::Vec4& color, const float& margin = 0, const bool& bGradient = false);
	void updateTextShowBoxGeom(const bool& bGradient = false);
	void updateTextShowLineGeom(const bool& bGradient = false);
	/*更新标牌文本框位置*/
	void updateLableObj();
	/*更新文本内容*/
	void updateTextInfo(std::string strInfo);

	/*获取标牌根结点*/
	osg::ref_ptr<osg::AutoTransform> getLabelRoot() { return _labelAutoTransform; }


	/*获取文本包围盒*/
	/*osg::BoundingBox getTextBoudingBox() { return _text->getBoundingBox(); }*/
protected:
	/*构建默认方盒Geom*/
	osg::ref_ptr<osg::Geometry> createDefBoxGeom(const bool& bGradient = false);
	/*构建连线Geom*/
	osg::ref_ptr<osg::Geometry> createConnectLineGeom();
	/*构建文本Geom*/
	void createTextGeom(/*LabelParm labelParm, std::string textInfo*/);
	/*构建自动变换节点*/
	void createAutoTransformNode(osg::AutoTransform::AutoRotateMode mode, bool bEnableAutoScale, bool bEnbleActive = false);

private:
	void initLabelGroup();
	void initLabelAutoTransform();

private:
	osg::ref_ptr<osg::AutoTransform> _labelAutoTransform;
	osg::ref_ptr<osg::Group> _labelGroup;

	//连线
	osg::ref_ptr<osg::Geode> _connectLineGeode;
	osg::ref_ptr<osg::Geometry> _connectLineGeom;
	//标牌框+文本
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

	float _connectLineLength;	//初始连线长度（默认7.0）	
	float _connectLineDegreee;	//初始连线角度（默认45.0）
	std::string _labelInfo;		//标牌文本信息
	osg::Vec4f _lineColor;		//连线以及边框颜色
	osg::Vec4f _fillColor;		//文本填充色
	float _margin;				//文本框外边距

	//LabelParm _labelParm;		//标牌结构体变量(暂时没用到)
};
