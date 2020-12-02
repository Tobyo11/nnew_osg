#include "AirborneRadar.h"
#include <osg/Geometry>
AirborneRadar::AirborneRadar(RadarParam * radarParam)
{
	_radarParam = radarParam;

	_airborneRadar = new osg::MatrixTransform;
	_airborneRadar->setName("airborneRadar");
}



AirborneRadar::~AirborneRadar()
{
	if (_radarParam)
		delete _radarParam;
}

void AirborneRadar::addAirborneRadar(bool is3D)
{
	osg::ref_ptr<osg::MatrixTransform> radarSelf = new osg::MatrixTransform;
	
	if (is3D)
	{
		radarSelf->addChild(create3DRadar());
		radarSelf->setName("airborneRadarSelf3D");
	}
	else
	{
		radarSelf->addChild(create2DRadar());
		radarSelf->setName("airborneRadarSelf2D");
	}
	double offsetAngle = abs( (_radarParam->maxHeading - _radarParam->minHeading) / 2 - 90 );
	if (_radarParam->maxHeading - _radarParam->minHeading > 180)
		offsetAngle = -offsetAngle;
	radarSelf->setMatrix(osg::Matrix::rotate(osg::DegreesToRadians(offsetAngle), osg::Z_AXIS));

	_airborneRadar->addChild(radarSelf);
}

osg::ref_ptr<osg::Geode> AirborneRadar::create2DRadar()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->setName("airborneRadarGeode2D");
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry;
	geom->setName("airborneRadarGeom2D");
	osg::ref_ptr<osg::Vec3Array> verArray = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> colArray = new osg::Vec4Array;
	colArray->push_back(_radarParam->quadColor);

	for (double ta = 0; ta < _radarParam->maxHeading - _radarParam->minHeading; ta += _radarParam->intHeading)
	{
		double xMin = _radarParam->minDis * cos(osg::DegreesToRadians(ta));
		double yMin = _radarParam->minDis * sin(osg::DegreesToRadians(ta));
		verArray->push_back(osg::Vec3d(xMin, yMin, 0));
		double xMax = _radarParam->maxDis * cos(osg::DegreesToRadians(ta));
		double yMax = _radarParam->maxDis * sin(osg::DegreesToRadians(ta));
		verArray->push_back(osg::Vec3d(xMax, yMax, 0));
	}

	geom->setVertexArray(verArray);
	geom->setColorArray(colArray);
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);
	osg::ref_ptr<osg::DrawElementsUInt> deu = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
	for (int i = 0; i < verArray->size() - 2; i += 2)
	{
		deu->push_back(i);
		deu->push_back(i + 1);
		deu->push_back(i + 3);
		deu->push_back(i + 2);
	}
	geom->addPrimitiveSet(deu);

	osg::StateSet* pStateSet = geom->getOrCreateStateSet();
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	pStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	geode->addDrawable(geom);

	return geode;
}

osg::ref_ptr<osg::Geode> AirborneRadar::create3DRadar()
{
	osg::ref_ptr<osg::Geode> geode = new osg::Geode;
	geode->setName("airborneRadarGeode3D");
	osg::ref_ptr<osg::Geometry> geomQuad = new osg::Geometry;
	osg::ref_ptr<osg::Geometry> geomLine = new osg::Geometry;
	geomQuad->setName("airborneRadarGeomQuad3D");
	geomLine->setName("airborneRadarGeomLine3D");
	osg::ref_ptr<osg::Vec3Array> verArray = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> colArrayQuad = new osg::Vec4Array;
	osg::ref_ptr<osg::Vec4Array> colArrayLine = new osg::Vec4Array;
	colArrayQuad->push_back(_radarParam->quadColor);
	colArrayLine->push_back(_radarParam->lineColor);

	for (double ta = 0; ta < _radarParam->maxHeading - _radarParam->minHeading; ta += _radarParam->intHeading)
	{
		for (double ba = _radarParam->minPitch; ta < _radarParam->maxPitch; ta += _radarParam->intPitch)
		{
			double xMin = _radarParam->minDis * cos(osg::DegreesToRadians(ba)) * cos(osg::DegreesToRadians(ta));
			double yMin = _radarParam->minDis * cos(osg::DegreesToRadians(ba)) * sin(osg::DegreesToRadians(ta));
			double zMin = _radarParam->minDis * sin(osg::DegreesToRadians(ba));
			verArray->push_back(osg::Vec3d(xMin, yMin, zMin));

			double xMax = _radarParam->maxDis * cos(osg::DegreesToRadians(ba)) * cos(osg::DegreesToRadians(ta));
			double yMax = _radarParam->maxDis * cos(osg::DegreesToRadians(ba)) * sin(osg::DegreesToRadians(ta));
			double zMax = _radarParam->maxDis * sin(osg::DegreesToRadians(ba));
			verArray->push_back(osg::Vec3d(xMax, yMax, zMax));
		}
	}

	return geode;
}
