#include "Utility.h"

#include <osg/ImageStream>
#include <osg/PolygonOffset>
#include <osg/PrimitiveSet>
#include <osg/Geometry>
#include <osg/BlendColor>
#include <osg/BlendFunc>

#include <osgEarth/Terrain>
#include <osgEarth/VirtualProgram>

osg::Texture2D* createTexture2D(std::string sTextureName, osg::Texture::WrapMode sWrapMode, osg::Texture::WrapMode tWrapMode)
{
	osg::Image *pImage = NULL;
	osg::Texture2D *pTex = NULL;
	unsigned char *pImageDataAllocated = NULL;

	pImage = osgDB::readImageFile(sTextureName);
	osg::ImageStream* imageStream = dynamic_cast<osg::ImageStream*>(pImage);
	if (imageStream) imageStream->play();

	if (pImage == NULL)
	{
		std::cout << "Fail to read Texture file [" << sTextureName << "] due to unexpected error.";
		return NULL;
	}

	pImage->setInternalTextureFormat(GL_RGBA32F_ARB);
	pTex = new osg::Texture2D(pImage);
	pTex->setResizeNonPowerOfTwoHint(false);
	pTex->setUseHardwareMipMapGeneration(true);
	pTex->setMaxAnisotropy(16.0f);
	pTex->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR_MIPMAP_LINEAR);
	pTex->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);
	pTex->setWrap(osg::Texture2D::WRAP_S, sWrapMode);
	pTex->setWrap(osg::Texture2D::WRAP_T, tWrapMode);
	pTex->setName(sTextureName);
	return pTex;
}

osg::Node* createNode(osg::Texture2D* tex, int iRenderNum, float fWidth, float fHeight, float fPosX, float fPosY, osg::Vec4 vColor, bool bDepth, bool bAddShader/* = true*/)
{
	osg::Geometry* g = new osg::Geometry();

	g->getOrCreateStateSet()->setTextureAttributeAndModes(0, tex);
	g->getOrCreateStateSet()->setMode(GL_BLEND, osg::StateAttribute::ON);
	if (bDepth) {
		g->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::ON);
		osg::PolygonOffset* polyoffset = new osg::PolygonOffset;
		polyoffset->setFactor(-1.0f);
		polyoffset->setUnits(-1.0f);
		g->getOrCreateStateSet()->setAttributeAndModes(polyoffset, osg::StateAttribute::ON);
	}
	else g->getOrCreateStateSet()->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);

	g->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

	float fSize = 2.0;
	osg::Vec3Array* v = new osg::Vec3Array();
	v->push_back(osg::Vec3(-fWidth * 0.5, -fHeight * 0.5, 0) + osg::Vec3(fPosX, fPosY, 0.0));
	v->push_back(osg::Vec3(fWidth * 0.5, -fHeight * 0.5, 0) + osg::Vec3(fPosX, fPosY, 0.0));
	v->push_back(osg::Vec3(fWidth * 0.5, fHeight * 0.5, 0) + osg::Vec3(fPosX, fPosY, 0.0));
	v->push_back(osg::Vec3(-fWidth * 0.5, fHeight * 0.5, 0) + osg::Vec3(fPosX, fPosY, 0.0));
	g->setVertexArray(v);

	osg::Vec2Array* t = new osg::Vec2Array();
	t->push_back(osg::Vec2(0, 0));
	t->push_back(osg::Vec2(1, 0));
	t->push_back(osg::Vec2(1, 1));
	t->push_back(osg::Vec2(0, 1));
	g->setTexCoordArray(0, t);

	osg::Vec4Array* c = new osg::Vec4Array();
	c->push_back(osg::Vec4(1, 1, 1, 1));
	g->setColorArray(c);
	g->setColorBinding(osg::Geometry::BIND_OVERALL);

	g->addPrimitiveSet(new osg::DrawArrays(GL_QUADS, 0, 4));

	osg::Geode* geode = new osg::Geode();
	geode->addDrawable(g);

	osg::StateSet* pStateSet = geode->getOrCreateStateSet();
	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

	if (bAddShader)
	{
		//_setNodeShaderUniform(geode, vColor);
		static const char* vertClipMaskSource = {
			"varying vec2 texCoord;\n"
			"void ra_mask_vert_clip(inout vec4 vertexCLIP)\n"
			"{\n"
			"	texCoord = gl_MultiTexCoord0.xy;\n"
			"}\n"
		};

		static const char* fragColorMaskSource = {
			"uniform sampler2D uColorBuf;\n"
			"uniform vec4 uColor;\n"
			"varying vec2 texCoord;"
			"void ra_mask_frag_coloring(inout vec4 color)\n"
			"{\n"
			"  color =uColor * texture2D( uColorBuf, texCoord);\n"
			"}\n"
		};
		osg::ref_ptr<osgEarth::VirtualProgram> program = new osgEarth::VirtualProgram;
		program->setFunction("ra_mask_vert_clip", vertClipMaskSource, osgEarth::ShaderComp::LOCATION_VERTEX_CLIP);
		program->setFunction("ra_mask_frag_coloring", fragColorMaskSource, osgEarth::ShaderComp::LOCATION_FRAGMENT_COLORING);
		pStateSet->setAttributeAndModes(program);
		pStateSet->addUniform(new osg::Uniform("uColor", vColor));
	}

	return geode;
}

osg::ref_ptr<osg::Vec3Array> createCirclePath(float radius, osg::Matrix matToWorld)
{
	osg::ref_ptr<osg::Vec3Array> plVertexArray = new osg::Vec3Array;
	for (int ta = 0; ta < 360; ta++)
	{
		double x = radius * cos(osg::DegreesToRadians((double)ta));
		double y = radius * sin(osg::DegreesToRadians((double)ta));
		plVertexArray->push_back(osg::Vec3f(x, y, 0) * matToWorld);
	}

	return plVertexArray;
}

osg::Matrix getWorldMatrixfromLonLatHeight(osg::ref_ptr<osgEarth::MapNode> mapNode, double lon, double lat, double height)
{
	const osgEarth::SpatialReference* wgs84 = osgEarth::SpatialReference::get("wgs84");
	osgEarth::GeoPoint gp(wgs84, lon, lat, height, osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
	osgEarth::GeoPoint p;
	if (!mapNode->getTerrain()->getSRS()->isEquivalentTo(gp.getSRS()))
		p = gp.transform(mapNode->getTerrain()->getSRS());
	else
		p = gp;
	osg::Matrix mat;
	p.createLocalToWorld(mat);
	return mat;
}

double getDistance(osg::Vec3 from, osg::Vec3 to)
{
	return sqrt((to.x() - from.x())*(to.x() - from.x()) + (to.y() - from.y())*(to.y() - from.y()) + (to.z() - from.z())*(to.z() - from.z()));
}

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

void getLonLatHeightfromWorld(osg::ref_ptr<osgEarth::MapNode> mapNode, osg::Vec3d pos, double& lon, double &lat, double& height)
{
	const osgEarth::SpatialReference* wgs84 = osgEarth::SpatialReference::get("wgs84");
	osgEarth::GeoPoint gp;
	gp.fromWorld(mapNode->getTerrain()->getSRS(), pos);
	if (!wgs84->isEquivalentTo(gp.getSRS()))
		gp = gp.transform(wgs84);
	lon = gp.x();
	lat = gp.y();
	height = gp.z();
}

std::string convertDoubletoString(double num, int precision)
{
	std::string strNum = std::to_string(num);
	std::vector<std::string> vecStrNum;
	SplitString(strNum, vecStrNum, ".");
	if (vecStrNum.size() <= 1)
		return "";

	if (precision >= 6)
		return strNum;

	std::string str = vecStrNum[1];
	str.erase(str.begin() + precision, str.end());
	return vecStrNum[0] + "." + str;
}

osg::ref_ptr<osg::Node> findNodebyName(osg::NodePath& nodePath, const std::string& name)
{
	int nNodeSize = static_cast<int> (nodePath.size());
	for (int i = nNodeSize - 1; i >= 0; i--)
	{
		if (nodePath[i]->getName() == name)
			return nodePath[i];
	}

	return NULL;
}

osg::Matrix getWorldMatrixfromLonLatHeight(bool is3D, double lon, double lat, double height)
{
	const osgEarth::SpatialReference* wgs84 = osgEarth::SpatialReference::get("wgs84");
	const osgEarth::SpatialReference* mercator = osgEarth::SpatialReference::get("spherical-mercator");
	osgEarth::GeoPoint gp(wgs84, lon, lat, height, osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
	osgEarth::GeoPoint p;
	if (!is3D)
		p = gp.transform(mercator);
	else
		p = gp;
	osg::Matrix mat;
	p.createLocalToWorld(mat);
	return mat;
}

void getLonLatHeightfromWorld(bool is3D, osg::Vec3d pos, double& lon, double &lat, double& height)
{
	const osgEarth::SpatialReference* wgs84 = osgEarth::SpatialReference::get("wgs84");
	const osgEarth::SpatialReference* mercator = osgEarth::SpatialReference::get("spherical-mercator");
	osgEarth::GeoPoint gp;
	if (is3D)
	{
		gp.fromWorld(wgs84, pos);
	}
	else
	{
		gp.fromWorld(mercator, pos);
		gp = gp.transform(wgs84);
	}
	lon = gp.x();
	lat = gp.y();
	height = gp.z();
}

//void getLonLatHeightfromWorldMatrix(bool is3D, osg::Matrix mat, double& lon, double &lat, double& height)
//{
//	const osgEarth::SpatialReference* wgs84 = osgEarth::SpatialReference::get("wgs84");
//	const osgEarth::SpatialReference* mercator = osgEarth::SpatialReference::get("spherical-mercator");
//	osgEarth::GeoPoint gp;
//	gp.createWorldToLocal(mat);
//}


osg::Vec3f convertScreentoWorld(osg::ref_ptr<osgViewer::View> view, float x, float y)
{//模型视图变换->投影变换->视口变换
	osg::Vec3f vec3;
	osg::ref_ptr<osg::Camera> camera = view->getCamera();
	osg::Vec3 vScreen(x, y, 0);
	osg::Matrix mVPW = camera->getViewMatrix() * camera->getProjectionMatrix() * camera->getViewport()->computeWindowMatrix();
	osg::Matrix invertVPW;
	invertVPW.invert(mVPW);
	vec3 = vScreen * invertVPW;
	return vec3;
}
void removeChildrenbyNodeName(osg::ref_ptr<osg::Group> root, const std::string& str)
{
	for (unsigned int i = root->getNumChildren() - 1; i < root->getNumChildren(); i++)
	{
		if (root->getChild(i)->getName() == str)
		{
			root->removeChild(i);
		}
	}
}
osg::Quat computeMatrix2Vector(osg::Vec3d v1, osg::Vec3d v2)
{
	double sin = v1.x() * v2.y() - v2.x() * v1.y();
	double cos = v1.x() * v2.x() + v1.y() * v2.y();
	double head = atan2(sin, cos);

	double pitch = asin(v2.z() / v2.length());

	return osg::Quat(pitch, osg::X_AXIS, 0, osg::Y_AXIS, head, osg::Z_AXIS);
}
//void setModelTransparent(osg::Node *node, float trans)
//{
//	osg::StateSet * stateset = node->getOrCreateStateSet();
//
//	stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
//	stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
//	stateset->setMode(GL_DEPTH, osg::StateAttribute::OFF);
//	stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
//
//	osg::BlendColor *bc = new osg::BlendColor(osg::Vec4(1.0, 1.0, 1.0, 1.0));
//	osg::ref_ptr<osg::BlendFunc>blendFunc = new osg::BlendFunc();
//	blendFunc->setSource(osg::BlendFunc::CONSTANT_ALPHA);
//	blendFunc->setDestination(osg::BlendFunc::ONE_MINUS_CONSTANT_ALPHA);
//	stateset->setAttributeAndModes(bc, osg::StateAttribute::ON);
//	stateset->setAttributeAndModes(blendFunc, osg::StateAttribute::ON);
//	bc->setConstantColor(osg::Vec4(1, 1, 1, trans));//第四个参数用于调节透明度
//}