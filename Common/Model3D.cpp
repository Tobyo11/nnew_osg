#include "Model3D.h"

#include <osg/ComputeBoundsVisitor>

#include <osgDB/ReadFile>

#include <osgEarth/VirtualProgram>

CModel3D::CModel3D(ModelConfig &mc):BaseModel(mc)

{
	initModel();

	addTrackUpdateCallback(true);
	addLabelUpdateCallback(true);
}

CModel3D::CModel3D(const CModel3D& model, const osg::CopyOp& copyop)
{
	_modelColor = model._modelColor;
	_modelFilePath = model._modelFilePath;
	_modelName = model._modelName;
	_trackType = model._trackType;
	_adjustVar = model._adjustVar;

	initTrackLabel();
	
	_modelNode = (osg::Node*)(model._modelNode->clone(copyop));
	addModelSelfChild(_modelNode);

	getModel()->setName(_modelName);

	addTrackUpdateCallback(true);
	addLabelUpdateCallback(true);
}

CModel3D::~CModel3D()
{
	//
}

void CModel3D::initModel()
{
	//model3D
	removeAllModelSelfChildren();
	_modelNode = osgDB::readNodeFile(_modelFilePath);
	if (_modelNode)
	{
		_modelNode->getOrCreateStateSet()->setMode(GL_RESCALE_NORMAL, osg::StateAttribute::ON);
		addModelSelfChild(_modelNode);
		setModelTextureShow(false, _modelColor);
		osg::ComputeBoundsVisitor boundVisitor;
		_modelNode->accept(boundVisitor);
		_adjustVar = boundVisitor.getBoundingBox().yMin();
		_adjustVar = _adjustVar + abs(_adjustVar / 3);

		_modelNode->setName("modelNode");
		//for (int i = 0; i < _modelNode->asGroup()->getNumChildren(); i++)
		//{
		//	_modelNode->asGroup()->getChild(i)->setName("_modelNodelChild");
		//}
	}
	else
	{
		std::cout << "initModel() Error" << std::endl;
		std::cout << "	load 3D model fail" << std::endl;
	}
}

void CModel3D::setModelTextureShow(bool show, osg::Vec4 color /*= osg::Vec4(0, 0, 0, 0)*/)
{
	if (show)
	{
		setModelShaderUniform(_modelNode, 0, color, 1);
	}
	else
	{
		setModelShaderUniform(_modelNode, 1, color, 1);
	}
}

void CModel3D::setModelAnimationPathCallback(osg::ref_ptr<osg::Vec3Array> pointArray, double speed, osg::Matrix matToWorld)
{
	osg::ref_ptr<osg::AnimationPath> ap = getAnimationPathfromArray(pointArray, true, speed, matToWorld);
	getModel()->addUpdateCallback(new osg::AnimationPathCallback(ap));
}

void CModel3D::setModelShaderUniform(osg::ref_ptr<osg::Node> node, bool enableColor, osg::Vec4 color, bool enableLight)
{
	osg::StateSet* pStateSet = node->getOrCreateStateSet();
	static osg::ref_ptr<osgEarth::VirtualProgram> vp;
	if (!vp.get())
	{
		vp = new osgEarth::VirtualProgram;

		std::string s_vert_clip =
			"#version 330 compatibility\n"
			"varying mat4 osg_ViewMatrixInverse;\n"
			"varying vec4 texcoord;\n"
			"varying vec4 vMaterialColor;\n"
			"varying float NdotL;\n"
			"varying vec3  ReflectVec;\n"
			"varying vec3  ViewVec;\n"
			"void ra_vert_clip(inout vec4 vertex) {\n"
			"	vec3 ecPos     = vec3 (gl_ModelViewMatrix * gl_Vertex);\n"
			"	vec3 tnorm     = normalize(gl_NormalMatrix * gl_Normal);\n"
			"	texcoord	   = gl_MultiTexCoord0;\n"
			"	vMaterialColor = gl_FrontMaterial.ambient;\n"
			"	vec3 lightVec  = normalize(vec3(1,3,0));\n"
			"	ReflectVec     = normalize(reflect(-lightVec, tnorm));"
			"	ViewVec        = normalize(-ecPos);"
			"	NdotL          = dot(lightVec, tnorm);"
			"}\n";
		std::string s_frag_color =
			"#version 330 compatibility\n"
			"uniform sampler2D uColorBuf;\n"
			"uniform vec4 uColor;\n"
			"uniform bool enable;\n"
			"uniform bool enablelight = true;\n"
			"varying vec4 texcoord;\n"
			"varying vec4 vMaterialColor;\n"
			"varying float NdotL;\n"
			"varying vec3  ReflectVec;\n"
			"varying vec3  ViewVec;\n"
			"void ra_frag_coloring(inout vec4 color){\n"
			"	vec3 nreflect = normalize(ReflectVec);\n"
			"	vec3 nview    = normalize(ViewVec);\n"
			"	float spec    = max(dot(nreflect, nview), 0.0);\n"
			"	spec          = pow(spec, 32.0);\n"
			"	float LightIntensity = 0.4 * ( 1.0 + NdotL + spec);\n"
			"	LightIntensity = clamp(pow(LightIntensity,1.8) + 0.2,0.1,1.0);\n"
			"	vec4 texColor = texture2D(uColorBuf, texcoord.xy);\n"
			"	\n"
			"	if(texColor.r<0.01 && texColor.g< 0.01 && texColor.b<0.01)\n"
			"		texColor.rgb = vMaterialColor.rgb;\n"
			"	\n"
			"	if(!enablelight)\n"
			"		LightIntensity = 1.0;\n"
			"	\n"
			"	if(enable)\n"
			"		color = vec4(uColor.xyz * LightIntensity, uColor.a * vMaterialColor.a);\n"
			"	else\n"
			"		color = vec4(texColor.xyz * LightIntensity, vMaterialColor.a);\n"
			"	\n"
			"}\n";
		vp->setFunction("ra_vert_clip", s_vert_clip, osgEarth::ShaderComp::LOCATION_VERTEX_CLIP);
		vp->setFunction("ra_frag_coloring", s_frag_color, osgEarth::ShaderComp::LOCATION_FRAGMENT_COLORING);
	}
	pStateSet->addUniform(new osg::Uniform("uColor", color));
	pStateSet->addUniform(new osg::Uniform("enable", enableColor));
	pStateSet->addUniform(new osg::Uniform("enablelight", enableLight));
	pStateSet->setAttributeAndModes(vp, osg::StateAttribute::ON);
}
