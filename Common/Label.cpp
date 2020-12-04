#include "Label.h"

#include <osg/MatrixTransform>
#include <osg/LineWidth>

CLabel::CLabel(const std::string str, osg::Vec4f lineColor, osg::Vec4f fillColor, float margin)
{
	_labelAutoTransform = new osg::AutoTransform;
	_labelAutoTransform->setName("labelAutoTransform");
	_labelGroup = new osg::Group;
	_labelGroup->setName("labelGroup");

	_connectLineGeode = new osg::Geode;
	_connectLineGeode->setName("connectLineGeode");
	_connectLineGeom = new osg::Geometry;
	_connectLineGeom->setName("connectLineGeom");

	_label = new osg::MatrixTransform;
	_label->setName("label");
	_labelSelf = new osg::MatrixTransform;
	_labelSelf->setName("labelSelf");
	_labelObj = new osg::MatrixTransform;
	_labelObj->setName("labelObj");

	_textGroup = new osg::Group;
	_textGroup->setName("_textGroup");
	_text = new osgText::Text;
	_text->setName("text");
	_textGeode = new osg::Geode;
	_textGeode->setName("textGeode");

	_showBoxGeode = new osg::Geode;
	_showBoxGeode->setName("showBoxGeode");
	_textShowLineGeom = new osg::Geometry;
	_textShowLineGeom->setName("textShowLineGeom");
	_textShowBoxGeom = new osg::Geometry;
	_textShowBoxGeom->setName("textShowBoxGeom");

	_connectLineDegreee = 45.0;
	_connectLineLength = 700.0;

	_labelInfo = str;
	_lineColor = lineColor;
	_fillColor = fillColor;
	_margin = margin;

	initLabelGroup();
	initLabelAutoTransform();
}

CLabel::~CLabel()
{
	//
}


void CLabel::updateConnectLineGeom()
{
	if (_connectLineGeom)
	{
		osg::Vec3 vPos = _label->getMatrix().getTrans();
		if (vPos.length() > 0.1)
		{
			_connectLineGeom->removePrimitiveSet(0, _connectLineGeom->getNumPrimitiveSets());
			osg::Vec3Array* pVertex = dynamic_cast<osg::Vec3Array*>(_connectLineGeom->getVertexArray());
			pVertex->clear();
			pVertex->push_back(osg::Vec3(0.0, 0.0, 0.0));
			pVertex->push_back(vPos);

			osg::Vec4Array* pColor = dynamic_cast<osg::Vec4Array*>(_connectLineGeom->getColorArray());
			pColor->clear();
			pColor->push_back(_lineColor);

			_connectLineGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
		}
	}
}

//void CLabel::updateBox(osg::ref_ptr<osg::Geometry>box, const osg::BoundingBox& boundingBox, const osg::Vec4& color, const float& margin /*= 0*/, const bool& bGradient /*= false*/)
//{
//	auto v = dynamic_cast<osg::Vec3Array*>(box->getVertexArray());
//	auto c = dynamic_cast<osg::Vec4Array*>(box->getColorArray());
//
//	(*v)[3] = osg::Vec3(boundingBox.xMax() + margin, boundingBox.yMax() + margin, 0);
//	(*v)[2] = osg::Vec3(boundingBox.xMin() - margin, boundingBox.yMax() + margin, 0);
//	(*v)[1] = osg::Vec3(boundingBox.xMin() - margin, boundingBox.yMin() - margin, 0);
//	(*v)[0] = osg::Vec3(boundingBox.xMax() + margin, boundingBox.yMin() - margin, 0);
//
//	if (!bGradient)
//		(*c)[0] = color;
//	else
//	{
//		osg::Vec4 color_end = color;
//		color_end.a() /= 5;
//		(*c)[0] = color_end;
//		(*c)[1] = color_end;
//		(*c)[2] = color;
//		(*c)[3] = color;
//	}
//
//	v->dirty();
//	c->dirty();
//	box->dirtyBound();
//}

void CLabel::updateTextShowBoxGeom(const bool& bGradient /*= false*/)
{
	auto v = dynamic_cast<osg::Vec3Array*>(_textShowBoxGeom->getVertexArray());
	auto c = dynamic_cast<osg::Vec4Array*>(_textShowBoxGeom->getColorArray());

	(*v)[3] = osg::Vec3(_text->getBoundingBox().xMax() + _margin, _text->getBoundingBox().yMax() + _margin, 0);
	(*v)[2] = osg::Vec3(_text->getBoundingBox().xMin() - _margin, _text->getBoundingBox().yMax() + _margin, 0);
	(*v)[1] = osg::Vec3(_text->getBoundingBox().xMin() - _margin, _text->getBoundingBox().yMin() - _margin, 0);
	(*v)[0] = osg::Vec3(_text->getBoundingBox().xMax() + _margin, _text->getBoundingBox().yMin() - _margin, 0);

	if (!bGradient)
		(*c)[0] = _fillColor;
	else
	{
		osg::Vec4 color_end = _fillColor;
		color_end.a() /= 5;
		(*c)[0] = color_end;
		(*c)[1] = color_end;
		(*c)[2] = _fillColor;
		(*c)[3] = _fillColor;
	}

	v->dirty();
	c->dirty();
	_textShowBoxGeom->dirtyBound();
}
void CLabel::updateTextShowLineGeom(const bool& bGradient /*= false*/)
{
	auto v = dynamic_cast<osg::Vec3Array*>(_textShowLineGeom->getVertexArray());
	auto c = dynamic_cast<osg::Vec4Array*>(_textShowLineGeom->getColorArray());

	(*v)[3] = osg::Vec3(_text->getBoundingBox().xMax() + _margin, _text->getBoundingBox().yMax() + _margin, 0);
	(*v)[2] = osg::Vec3(_text->getBoundingBox().xMin() - _margin, _text->getBoundingBox().yMax() + _margin, 0);
	(*v)[1] = osg::Vec3(_text->getBoundingBox().xMin() - _margin, _text->getBoundingBox().yMin() - _margin, 0);
	(*v)[0] = osg::Vec3(_text->getBoundingBox().xMax() + _margin, _text->getBoundingBox().yMin() - _margin, 0);

	if (!bGradient)
		(*c)[0] = _lineColor;
	else
	{
		osg::Vec4 color_end = _lineColor;
		color_end.a() /= 5;
		(*c)[0] = color_end;
		(*c)[1] = color_end;
		(*c)[2] = _lineColor;
		(*c)[3] = _lineColor;
	}

	v->dirty();
	c->dirty();
	_textShowLineGeom->dirtyBound();
}

void CLabel::updateLableObj()
{
	auto boundingBox = _showBoxGeode->getBoundingBox();
	osg::Vec3d vTrans(0, 0, 0);
	osg::Vec2 _boxSize = osg::Vec2(boundingBox.xMax() - boundingBox.xMin(), boundingBox.yMax() - boundingBox.yMin());

	vTrans.x() -= boundingBox.xMin();
	vTrans.y() -= boundingBox.yMin();
	_labelObj->setMatrix(osg::Matrixd::translate(vTrans));

	auto bounding = _textShowBoxGeom->getBoundingBox();
	auto bounding1 = _textShowLineGeom->getBoundingBox();
	auto bounding2 = _text->getBoundingBox();
}

void CLabel::updateTextInfo(std::string strInfo)
{
	_text->setText(strInfo);
	_text->dirtyBound();
}

osg::ref_ptr<osg::Geometry> CLabel::createDefBoxGeom(const bool& bGradient /*= false*/)
{
	osg::ref_ptr<osg::Geometry> box = new osg::Geometry;
	osg::Vec3Array* v = new osg::Vec3Array(4);
	v->setDataVariance(osg::Object::DYNAMIC);
	box->setVertexArray(v);

	if (!bGradient)
	{
		osg::Vec4Array* c = new osg::Vec4Array(1);
		c->setDataVariance(osg::Object::DYNAMIC);
		box->setColorArray(c);
		box->setColorBinding(osg::Geometry::BIND_OVERALL);
	}
	else
	{
		osg::Vec4Array* c = new osg::Vec4Array(4);
		c->setDataVariance(osg::Object::DYNAMIC);
		box->setColorArray(c);
		box->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	}

	box->addPrimitiveSet(new osg::DrawArrays(GL_POLYGON, 0, v->getNumElements()));
	box->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	box->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	box->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

	return box;
}

osg::ref_ptr<osg::Geometry> CLabel::createConnectLineGeom()
{
	osg::ref_ptr<osg::Geometry> pGeom = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> pVertexArray = new osg::Vec3Array;
	osg::ref_ptr<osg::Vec4Array> pColorArray = new osg::Vec4Array;
	pColorArray->push_back(_lineColor);

	pGeom->setDataVariance(osg::Object::DYNAMIC);
	pGeom->setUseDisplayList(false);
	pGeom->setVertexArray(pVertexArray);
	pGeom->setColorArray(pColorArray);
	pGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	pGeom->dirtyDisplayList();

	osg::ref_ptr < osg::StateSet> pStateSet = pGeom->getOrCreateStateSet();
	pStateSet->setMode(GL_LINE_SMOOTH, osg::StateAttribute::ON);
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	pStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	pStateSet->setAttributeAndModes(new osg::LineWidth(1), osg::StateAttribute::ON);

	return pGeom;
}

void CLabel::createTextGeom(/*LabelParm labelParm, std::string textInfo*/)
{
	_text->setAlignment(osgText::TextBase::AlignmentType::LEFT_TOP);
	_text->setAxisAlignment(osgText::TextBase::XY_PLANE);
	_text->setDataVariance(osg::Object::DYNAMIC);
	static osg::ref_ptr<osgText::Font> s_font = osgText::readFontFile("msyh.ttf");
	_text->setFont(s_font.get());
	_text->setFontResolution(64, 64);
	_text->setText(_labelInfo);
	_text->setCharacterSize(3);
	_text->setPosition(osg::Vec3());		//初始位置（0 0 0）
	_text->setColor(osg::Vec4(1, 1, 1, 1));
	_text->setLineSpacing(0.2);
	//_text->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	//_text->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	//_text->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	//_text->getOrCreateStateSet()->setRenderBinDetails(680, "RenderBin");
}

void CLabel::createAutoTransformNode(osg::AutoTransform::AutoRotateMode mode, bool bEnableAutoScale, bool bEnbleActive /*= false*/)
{
	_labelAutoTransform->setAutoScaleToScreen(bEnableAutoScale);
	_labelAutoTransform->setAutoRotateMode(mode);
	_labelAutoTransform->setMinimumScale(20.0);	
	_labelAutoTransform->setMaximumScale(2500.0);
	_labelAutoTransform->setAutoScaleTransitionWidthRatio(0.1);
	//_labelAutoTransform->setScale(osg::Vec3d(50.0, 50.0, 50.0));
}

void CLabel::initLabelGroup()
{
	_textShowBoxGeom = createDefBoxGeom();
	_textShowLineGeom = createDefBoxGeom();
	_textShowLineGeom->getPrimitiveSet(0)->setMode(GL_LINE_LOOP);

	_showBoxGeode->addDrawable(_textShowBoxGeom);
	_showBoxGeode->addDrawable(_textShowLineGeom);
	//_showBoxGeode->setName("showBoxGeode");


	//文本方框
	_labelObj->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	_labelObj->getOrCreateStateSet()->setRenderBinDetails(680, "RenderBin");	//渲染排序,否则出现不显示的情况
	_labelObj->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	_labelObj->addChild(_showBoxGeode);
	_labelObj->addChild(_textGroup);

	_labelSelf->addChild(_labelObj);
	_label->addChild(_labelSelf);
	_labelGroup->addChild(_label);
	_labelGroup->addChild(_connectLineGeode);

	createTextGeom();

	//_textGeode->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, 0);
	_textGeode->addDrawable(_text);

	_textGroup->addChild(_textGeode);

	updateTextShowBoxGeom();
	updateTextShowLineGeom();
	updateLableObj();

	//连线
	_connectLineGeom = createConnectLineGeom();
	_connectLineGeode->addDrawable(_connectLineGeom);
	_connectLineGeode->getOrCreateStateSet()->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	_connectLineGeode->getOrCreateStateSet()->setRenderBinDetails(679, "RenderBin");


	//相对位置变换
	osg::Vec3d _selectionPos = osg::Vec3d(_connectLineLength * cosf(osg::DegreesToRadians(_connectLineDegreee)), _connectLineLength * sinf(osg::DegreesToRadians(_connectLineDegreee)), 0.0);
	_label->setMatrix(_label->getMatrix() * osg::Matrix::translate(_selectionPos));
	updateConnectLineGeom();
}

void CLabel::initLabelAutoTransform()
{
	createAutoTransformNode(osg::AutoTransform::ROTATE_TO_SCREEN, false);
	_labelAutoTransform->addChild(_labelGroup);
}