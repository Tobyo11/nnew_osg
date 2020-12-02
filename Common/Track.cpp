#include "Track.h"

#include <osg/Geode>
#include <osg/Depth>
#include <osg/Point>
#include <osg/LineWidth>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>

CTrack::CTrack(TrackType trackType, osg::Vec4 color)
{
	_trackType = trackType;
	_trackGeode = new osg::Geode;
	_trackGeom = new osg::Geometry;
	//_trackSelf = new osg::MatrixTransform;
	_trackColor = color;
	initTrackGeom();
}

CTrack::~CTrack()
{

}

void CTrack::initTrackGeom()
{
	osg::ref_ptr<osg::Vec3Array> trackVertexArray = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> trackColorArray = new osg::Vec4Array;
	trackVertexArray->setDataVariance(osg::Object::DYNAMIC);
	trackColorArray->setDataVariance(osg::Object::DYNAMIC);
	trackVertexArray->push_back(osg::Vec3());
	trackColorArray->push_back(_trackColor);

	_trackGeom->setDataVariance(osg::Object::DYNAMIC);
	_trackGeom->setUseDisplayList(false);
	_trackGeom->setUseVertexBufferObjects(true);
	_trackGeom->setVertexArray(trackVertexArray);
	_trackGeom->setColorArray(trackColorArray);

	osg::ref_ptr < osg::StateSet> pStateSet = _trackGeom->getOrCreateStateSet();
	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	pStateSet->setRenderBinDetails(299, "RenderBin");
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	pStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	pStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::LESS, 0.0, 1.0, true), osg::StateAttribute::ON);

	switch (_trackType)
	{
	case TrackType::Point:
	{
		_trackGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
		_trackGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, trackVertexArray->size()));
		pStateSet->setAttributeAndModes(new osg::Point(2), osg::StateAttribute::ON);
	}
	break;
	case TrackType::Line:
	{
		_trackGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
		_trackGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, trackVertexArray->size()));
		pStateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
		pStateSet->setAttributeAndModes(new osg::LineWidth(3), osg::StateAttribute::ON);
	}
	break;
	case TrackType::Band:
	{
		_trackGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		_trackGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, trackVertexArray->size()));
	}
	break;
	default:
		break;
	}

	
	//_trackGeode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0, 0.0, 0.0), 10.0, 10.0, 10.0)));
	_trackGeode->addDrawable(_trackGeom);
	//_trackSelf->addChild(_trackGeode);

	//_trackSelf->setMatrix(osg::Matrix::scale(10, 10, 10));
}

NTrack::NTrack(TrackType trackType, osg::Vec4 color)
{
	_trackType = trackType;
	_trackGeode = new osg::Geode;
	_trackGeom = new osg::Geometry;
	_trackSelf = new osg::MatrixTransform;
	_trackAutoTransform = new osg::AutoTransform;

	_trackAutoTransform->setAutoRotateMode(osg::AutoTransform::AutoRotateMode::NO_ROTATION);
	_trackAutoTransform->setAutoScaleToScreen(false);
	_trackAutoTransform->setMinimumScale(20.0);
	_trackAutoTransform->setMaximumScale(50.0);
	_trackAutoTransform->setScale(osg::Vec3d(10, 10, 10));
	_trackColor = color;
	initTrackGeom();
}

NTrack::~NTrack()
{

}

void NTrack::initTrackGeom()
{
	osg::ref_ptr<osg::Vec3Array> trackVertexArray = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> trackColorArray = new osg::Vec4Array;
	trackVertexArray->setDataVariance(osg::Object::DYNAMIC);
	trackColorArray->setDataVariance(osg::Object::DYNAMIC);
	trackVertexArray->push_back(osg::Vec3());
	trackColorArray->push_back(_trackColor);

	_trackGeom->setDataVariance(osg::Object::DYNAMIC);
	_trackGeom->setUseDisplayList(false);
	_trackGeom->setUseVertexBufferObjects(true);
	_trackGeom->setVertexArray(trackVertexArray);
	_trackGeom->setColorArray(trackColorArray);

	osg::ref_ptr < osg::StateSet> pStateSet = _trackGeom->getOrCreateStateSet();
	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	pStateSet->setRenderBinDetails(299, "RenderBin");
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	pStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	pStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::LESS, 0.0, 1.0, true), osg::StateAttribute::ON);

	switch (_trackType)
	{
	case TrackType::Point:
	{
		_trackGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
		_trackGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS, 0, trackVertexArray->size()));
		pStateSet->setAttributeAndModes(new osg::Point(2), osg::StateAttribute::ON);
	}
	break;
	case TrackType::Line:
	{
		_trackGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
		_trackGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, trackVertexArray->size()));
		pStateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
		pStateSet->setAttributeAndModes(new osg::LineWidth(3), osg::StateAttribute::ON);
	}
	break;
	case TrackType::Band:
	{
		//_trackGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		//_trackGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP, 0, trackVertexArray->size()));
		_trackGeom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		_trackGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP, 0, trackVertexArray->size()));
		pStateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
		pStateSet->setAttributeAndModes(new osg::LineWidth(3), osg::StateAttribute::ON);
	}
	break;
	default:
		break;
	}


	//_trackGeode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.0, 0.0, 0.0), 10.0, 10.0, 10.0)));
	_trackGeode->addDrawable(_trackGeom);
	//_trackSelf->addChild(_trackGeode);
	//_trackAutoTransform->addChild(_trackSelf);

	//_trackSelf->setMatrix(osg::Matrix::scale(10, 10, 10));
}