/*
	Explosion.h
	����һ��class CExplosion
	������ָ�����������´��������뾶�ı�ը
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

	/*�жϱ�ը�Ƿ����,�����ж�ģ����ʧʱ��, ��Ҫÿ֡����*/
	bool isExplosionEnd();
	/*��vpos��������ը�ڵ�*/
	osg::ref_ptr<osg::Node> createExplosionNode(osg::Vec3d vPos);
private:
	osg::ref_ptr<osgParticle::ExplosionEffect> _explosionEffect;
	int _startNum;
	float _radius;
};