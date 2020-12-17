/*
	Utility.h
	�����Ժ���ͷ�ļ�,��class

	���ܴ��ڵ�����:getLonLatHeightfromWorld()�����õ��ľ�γ�߿��ܴ������,�����Ǹ߶�
*/
#pragma once

#include <windows.h>
#include <iostream>

#include <osg/Texture2D>
#include <osgViewer/Viewer>
#include <osgEarth/MapNode>

//����2D����
osg::Texture2D* createTexture2D(std::string sTextureName, osg::Texture::WrapMode sWrapMode, osg::Texture::WrapMode tWrapMode);
//�����������ͼԪ
osg::Node* createNode(osg::Texture2D* tex, int iRenderNum, float fWidth, float fHeight, float fPosX, float fPosY, osg::Vec4 vColor, bool bDepth, bool bAddShader = true);
//����Բ�ζ���·��(��������)
osg::ref_ptr<osg::Vec3Array> createCirclePath(float radius, osg::Matrix matToWorld);
//����Բ�ζ���·��(��������) 1204 �¼�
osg::Vec4Array* createCirclePath2(double _lon, double _lat, double _height);

//����mapnode��SRS�ɾ�γ�ߵõ�����任����
osg::Matrix getWorldMatrixfromLonLatHeight(osg::ref_ptr<osgEarth::MapNode> mapNode, double lon, double lat, double height);
//����mapnode��SRS�����������õ���γ�߶�
void getLonLatHeightfromWorld(osg::ref_ptr<osgEarth::MapNode> mapNode, osg::Vec3d pos, double& lon, double &lat, double& height);
//�����������õ���γ�߶�(Ĭ��SRS,3DSRS = wgs84, 2DSRS = spherical-mercator)
osg::Matrix getWorldMatrixfromLonLatHeight(bool is3D, double lon, double lat, double height);
//���������õ���γ�߶�(Ĭ��SRS,3DSRS = wgs84, 2DSRS = spherical-mercator)
void getLonLatHeightfromWorld(bool is3D, osg::Vec3d pos, double& lon, double &lat, double& height);
////�����������õ���γ�߶�(Ĭ��SRS,3DSRS = wgs84, 2DSRS = spherical-mercator)
//void getLonLatHeightfromWorldMatrix(bool is3D, osg::Matrix mat, double& lon, double &lat, double& height);
//��ȡ����֮��ľ���
double getDistance(osg::Vec3 from, osg::Vec3 to);
//�ָ��ַ���(s:Ҫ�ָ���ַ���, v�ָ����ַ�������, c:�ָ��ַ�)
void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
//��ȡС���㾫��,��ת��Ϊ�ַ�����num:Ҫת�������� precision:���ȣ���ʾС�����λ��
std::string convertDoubletoString(double num, int precision);
//�ڽڵ�·����Ѱ��������ַ�������ͬ���׸���㲢����,δ�ҵ��򷵻�NULL
osg::ref_ptr<osg::Node> findNodebyName(osg::NodePath& nodePath, const std::string& name);
//��Ļ����ת��Ϊ��������
osg::Vec3f convertScreentoWorld(osg::ref_ptr<osgViewer::View> view, float x, float y);
//���ĳ��Group�������������Ϊstr���ӽڵ�
void removeChildrenbyNodeName(osg::ref_ptr<osg::Group> root, const std::string& str);
//�����ɿռ�����v1��ת��v2�������Ԫ��(v1,v2����ʼ��һ��)
osg::Quat computeMatrix2Vector(osg::Vec3d v1, osg::Vec3d v2);
////ʹģ��͸������trans���ڵ���͸���ȣ���ΧΪ0.0-1.0������1.0����͸����
//void setModelTransparent(osg::Node *node, float trans);


