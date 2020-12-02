/*
	Explosion.h
	包含一个class CExplosion
	用于在指定世界坐标下创建给定半径的爆炸
*/
#pragma once
#include <Windows.h>
#include <osg/Node>
#include <osgParticle/ExplosionEffect>

class CExplosion
{
public:
	CExplosion(float radius);
	~CExplosion();

	/*判断爆炸是否结束,用于判断模型消失时机, 需要每帧调用*/
	bool isExplosionEnd();
	/*在vpos处创建爆炸节点*/
	osg::ref_ptr<osg::Node> createExplosionNode(osg::Vec3d vPos);
private:
	osg::ref_ptr<osgParticle::ExplosionEffect> _explosionEffect;
	int _startNum;
	float _radius;
};