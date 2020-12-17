/*
	Utility.h
	功能性函数头文件,无class

	可能存在的问题:getLonLatHeightfromWorld()函数得到的经纬高可能存在误差,尤其是高度
*/
#pragma once

#include <windows.h>
#include <iostream>

#include <osg/Texture2D>
#include <osgViewer/Viewer>
#include <osgEarth/MapNode>

//创建2D纹理
osg::Texture2D* createTexture2D(std::string sTextureName, osg::Texture::WrapMode sWrapMode, osg::Texture::WrapMode tWrapMode);
//创建带纹理的图元
osg::Node* createNode(osg::Texture2D* tex, int iRenderNum, float fWidth, float fHeight, float fPosX, float fPosY, osg::Vec4 vColor, bool bDepth, bool bAddShader = true);
//创建圆形动画路径(世界坐标)
osg::ref_ptr<osg::Vec3Array> createCirclePath(float radius, osg::Matrix matToWorld);
//创建圆形动画路径(世界坐标) 1204 新加
osg::Vec4Array* createCirclePath2(double _lon, double _lat, double _height);

//根据mapnode的SRS由经纬高得到世界变换矩阵
osg::Matrix getWorldMatrixfromLonLatHeight(osg::ref_ptr<osgEarth::MapNode> mapNode, double lon, double lat, double height);
//根据mapnode的SRS由世界坐标点得到经纬高度
void getLonLatHeightfromWorld(osg::ref_ptr<osgEarth::MapNode> mapNode, osg::Vec3d pos, double& lon, double &lat, double& height);
//由世界坐标点得到经纬高度(默认SRS,3DSRS = wgs84, 2DSRS = spherical-mercator)
osg::Matrix getWorldMatrixfromLonLatHeight(bool is3D, double lon, double lat, double height);
//世界坐标点得到经纬高度(默认SRS,3DSRS = wgs84, 2DSRS = spherical-mercator)
void getLonLatHeightfromWorld(bool is3D, osg::Vec3d pos, double& lon, double &lat, double& height);
////世界坐标矩阵得到经纬高度(默认SRS,3DSRS = wgs84, 2DSRS = spherical-mercator)
//void getLonLatHeightfromWorldMatrix(bool is3D, osg::Matrix mat, double& lon, double &lat, double& height);
//获取两点之间的距离
double getDistance(osg::Vec3 from, osg::Vec3 to);
//分割字符串(s:要分割的字符串, v分割后的字符串向量, c:分割字符)
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
//截取小数点精度,并转化为字符串（num:要转换的数字 precision:精度，表示小数点后几位）
std::string convertDoubletoString(double num, int precision);
//在节点路径中寻找与给定字符串名相同的首个结点并返回,未找到则返回NULL
osg::ref_ptr<osg::Node> findNodebyName(osg::NodePath& nodePath, const std::string& name);
//屏幕坐标转换为世界坐标
osg::Vec3f convertScreentoWorld(osg::ref_ptr<osgViewer::View> view, float x, float y);
//清除某个Group结点下所有名字为str的子节点
void removeChildrenbyNodeName(osg::ref_ptr<osg::Group> root, const std::string& str);
//计算由空间向量v1旋转至v2所需的四元数(v1,v2的起始点一致)
osg::Quat computeMatrix2Vector(osg::Vec3d v1, osg::Vec3d v2);
////使模型透明化，trans用于调节透明度，范围为0.0-1.0，其中1.0代表不透明。
//void setModelTransparent(osg::Node *node, float trans);


