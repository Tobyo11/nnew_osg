#include "Explosion.h"
#include <osgEarth/Registry>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ParticleEffect>

CExplosion::CExplosion(float radius)
{
	_startNum = 0;
	_radius = radius;
}

CExplosion::~CExplosion()
{


}

osg::ref_ptr<osg::Node> CExplosion::createExplosionNode(osg::Vec3d vPos)
{
	_explosionEffect = new osgParticle::ExplosionEffect(vPos, _radius, 1.0);
	std::string sTextureName = "./PanoISRResource/texture/smoke.rgb";
	_explosionEffect->getParticleSystem()->setDefaultAttributes(sTextureName, true, false);
	osgEarth::Registry::shaderGenerator().run(_explosionEffect);
	_explosionEffect->setName("explosionEffect");
	return _explosionEffect;
}


bool CExplosion::isExplosionEnd()
{
	_startNum++;

	if (_startNum > 120)
	{
		return true;
	}
	else return false;
}