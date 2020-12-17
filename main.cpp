/*
	1.使用addview和removeView方式可以提升大约20帧,但是在23维切换时不同步(标牌未更新,模型在23维下的位置不一致)
		考虑实际使用情况,每次发送位置信息,原则上能保持23维的同步(如果不拖拽)
	2.标牌回调中应可以根据视点远近进行显隐和缩放（未实现）
		-3.尾迹添加自动缩放后不显示，需要解决[飘带现使用线进行绘制]
	4.尾迹需要后移至模型尾部(3D)
	5.动态改变模型未实现
	6.二维标牌避让未实现
	7.通过配置文件改变操作方式未实现
	8.浅拷贝可能存在的问题:动态改变颜色时,其余浅拷贝对象同样会修改颜色
		-9.PickHandler中move()函数需要改善,因为还没有场景管理类,对模型的移动操作有点复杂[使用osg自带的拖拽器]
	10.模型拖拽方面23维没有进行同步,需要场景管理
*/

#include "Common/DataStruct.h"
#include "Common/DigitalEarth.h"
#include "Common/Model2D.h"
#include "Common//Model3D.h"
#include "Common/Utility.h"
#include "Common/Explosion.h"
#include "Common/PickHandle.h"
#include "Common//PickDragHandler.h"

#include <iostream>
#include <osgEarthUtil/Controls>
CModel3D* modelCopy1;//test
CModel3D* modelCopy2;//test
CModel3D* modelCopy3;//test
CModel3D* modelCopy4;//test
CModel3D* modelCopy5;//test
CModel2D* modelCopy12D;
std::vector<CModel2D*> vecModel2D;
std::vector<CModel3D*> vecModel3D;
std::vector<osg::MatrixTransform*> vecModelMT2D;
std::vector<osg::MatrixTransform*> vecModelMT3D;

class CKeyboardHandler2D :public osgGA::GUIEventHandler
{
public:

	CKeyboardHandler2D(DigitalEarth* earth) :
		_earth(earth)
	{
	}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (!viewer)return false;

		switch (ea.getEventType())//判断事件，做出响应
		{
		case osgGA::GUIEventAdapter::KEYDOWN:
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_F3)
			{//切换为3Dmapnode
				_earth->setRenderMapType(false, true);
			}
			break;
		default:break;
		}
		return false;
	}
private:
	DigitalEarth* _earth;
};

class CKeyboardHandler3D :public osgGA::GUIEventHandler
{
public:

	CKeyboardHandler3D(DigitalEarth* earth) :
		_earth(earth)
	{
	}

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
	{
		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
		if (!viewer)return false;

		switch (ea.getEventType())//判断事件，做出响应
		{
		case osgGA::GUIEventAdapter::KEYDOWN://ea.getEventType()获取到的如果是键盘事件
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_F2)
			{//切换为2Dmapnode
				_earth->setRenderMapType(true, false);
			}
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_U)
			{//移除目标锁定绘制
				for (int i = 0; i < vecModel3D.size(); i++)
				{
					vecModel3D[i]->removeTargetLockAll();
				}
			}
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_B)
			{
				osg::Matrix matTarget3D = getWorldMatrixfromLonLatHeight(true, 120.047, 24.507, 1000);
				CExplosion* exp = new CExplosion(1000);
				removeChildrenbyNodeName(_earth->getSwitchMapNode3D(), "explosionEffect");
				_earth->addChild3D(exp->createExplosionNode(osg::Vec3d() * matTarget3D));
			}
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_F4)//3d follow
			{

				if (_earth->getViewer()->getNumViews() == 3)
				{
					//_earth->getViewer3Dfollow()->getCamera()->setViewport(0, 0, 100, 100);
					_earth->delet3DfollowView();
				}
				else if (_earth->getViewer()->getNumViews() == 2)
				{
					_earth->setTetherNode3Dfollow(modelCopy1->getModelNode());
					//_earth->setTetherNode3Dfollow(model->getModel3D());
					//_earth->getViewer3Dfollow()->getCamera()->setViewport(0, 0, 480, 270);
					_earth->add3DfollowView();
				}
			}
			if (ea.getKey() == 'B')//add shp
			{
				_earth->addShp("D:/data/chinashp/world.shp", shpLine);
			}
			if (ea.getKey() == 'M')//add shp
			{
				_earth->getMapNode3D()->getMap()->removeLayer(_earth->getMapNode3D()->getMap()->getLayerByName("123"));
			}
			if (ea.getKey() == 'N')//delet shp
			{
				_earth->addShp("D:/data/chinashp/pp.shp", shpName);
				//_earth->addShp("D:/data/chinashp/铁路_polyline.shp", shpLine);
			}
			if (ea.getKey() == 'O')//open draw line
			{
				_earth->openDrawLine();
			}
			if (ea.getKey() == 'P')////close draw line
			{
				_earth->closeDrawLine();
			}
		
			//if (ea.getKey() == osgGA::GUIEventAdapter::KEY_V)
			//{//隐藏高程着色
			//	_earth->setElevationColorVisible("3DElevationColor", false);
			//}
			break;
		default:break;
		}
		return false;
	}
private:
	DigitalEarth* _earth;
};
/////35path
const osgEarth::Util::Controls::SpatialReference* mapSRS;
///
double get_dis(osg::Vec3 from, osg::Vec3 to)
{
	return sqrt((to.x() - from.x())*(to.x() - from.x()) + (to.y() - from.y())*(to.y() - from.y()) + (to.z() - from.z())*(to.z() - from.z()));
}
///
double get_runtime(osg::Vec3 from, osg::Vec3 to, double speed)
{
	double dist = get_dis(from, to);
	if (speed == 0)
		return 10000000000;
	return dist / speed;
}
osg::ref_ptr<osg::AnimationPath> creatAitPath(osg::Vec4Array* ctrl)
{
	osg::ref_ptr<osg::AnimationPath> animationPath = new osg::AnimationPath;
	animationPath->setLoopMode(osg::AnimationPath::LOOP);

	double sp_angle;
	double cz_angle;

	osg::Vec3d point_cur;
	osg::Vec3d point_next;
	osg::Matrix matrix;
	osg::Quat _rotation;
	//_rotation.makeRotate(osg::DegreesToRadians(270.0), 0.0, 0.0, 1.0);
	double time = 0;


	for (osg::Vec4Array::iterator iter = ctrl->begin(); iter != ctrl->end(); iter++)
	{
		osg::Vec4Array::iterator iter2 = iter;
		iter2++;

		if (iter2 == ctrl->end())
		{
			break;
		}






		double x, y, z;
		//std::cout << osg::DegreesToRadians(iter->y()) << "-" << osg::DegreesToRadians(iter->x()) << std::endl;
		mapSRS->getEllipsoid()->convertLatLongHeightToXYZ(osg::DegreesToRadians(iter->y()), osg::DegreesToRadians(iter->x()), iter->z(), x, y, z);

		point_cur = osg::Vec3d(x, y, z);
		mapSRS->getEllipsoid()->convertLatLongHeightToXYZ(osg::DegreesToRadians(iter2->y()), osg::DegreesToRadians(iter2->x()), iter2->z(), x, y, z);
		point_next = osg::Vec3d(x, y, z);

		//水平角
		if (iter->x() == iter2->x())
		{
			sp_angle = osg::PI_2;
		}
		else
		{
			sp_angle = atan((iter2->y() - iter->y()) / (iter2->x() - iter->x()));
			if (iter2->x() > iter->x())
			{
				sp_angle += (osg::PI_2 * 2);
			}
		}
		//俯仰角

		if (iter->z() == iter2->z())
		{

			cz_angle = 0;
		}
		else
		{
			if (0 == sqrt(pow(get_dis(point_cur, point_next), 2) - pow((iter2->z() - iter->z()), 2)))
			{
				cz_angle = osg::PI_2;
			}
			else
			{
				cz_angle = atan((iter2->z() - iter->z()) / sqrt(pow(get_dis(point_cur, point_next), 2) - pow((iter2->z() - iter->z()), 2)));

			}
			if (cz_angle > osg::PI_2)
			{
				cz_angle = osg::PI_2;
			}
			if (cz_angle < -osg::PI_2)
			{
				cz_angle = -osg::PI_2;
			}
		}
		//求变换矩阵
		mapSRS->getEllipsoid()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(iter->y()), osg::DegreesToRadians(iter->x()), iter->z(), matrix);
		_rotation.makeRotate(0, osg::Vec3(1.0, 0.0, 0.0), cz_angle, osg::Vec3(0.0, 1.0, 0.0), sp_angle + osg::PI_2, osg::Vec3(0.0, 0.0, 1.0));
		matrix.preMultRotate(_rotation);

		//matrix.preMult(osg::Matrix::scale(osg::Vec3(100, 100, 100)));

		//matrix.preMultScale(osg::Matrix::scale(osg::Vec3(100, 100, 100)));

		animationPath->insert(time, osg::AnimationPath::ControlPoint(point_cur, matrix.getRotate()));

		//求下一点time
		time += get_runtime(point_cur, point_next, iter2->w());

	}
	animationPath->insert(time, osg::AnimationPath::ControlPoint(point_next, matrix.getRotate()));

	return animationPath.release();
}
////
int main(int argc, char** argv)
{
	DigitalEarth* earth = new DigitalEarth();

	mapSRS = earth->getMapNode3D()->getMapSRS();

	earth->create3DfollowView();//初始化3d follow
	earth->createDrawLine();//初始化画线
	earth->createManipulator();//初始化多操作器

	for (int i = 0; i < 100; i++)//添加路径相机路径
		earth->addCameraPath(106.56, 29.55 + 0.01*i, 2500, i);
	
	earth->addImageLayer("GoogleImage2D", false, OEDriverType::TMS, "D:/Openstreet/tms.xml");
	earth->addImageLayer("GoogleImage3D", true, OEDriverType::TMS, "D:/data/google/image/tms.xml");
	earth->addElevationLayer("3DElevation", true, OEDriverType::VBP, "D:/data/Earth/output/earth.ive");
	//earth->addElevationColor("3DElevationColor", "3DElevation", "D:/data/elevation.clr");

	earth->getViewer2D()->addEventHandler(new CKeyboardHandler2D(earth));
	earth->getViewer3D()->addEventHandler(new CKeyboardHandler3D(earth));

	earth->getViewer3D()->addEventHandler(new PickDragHandler());

	ModelConfig mc = ModelConfig();

	CModel3D* m3 = new CModel3D(mc);
	ModelConfig mc2 = mc;
	mc2.modelFilePath = "PanoISRResource/texture/TYPE_AIRCRAFT.png";
	CModel2D* m2 = new CModel2D(mc2);



	for (int i = 0; i < 5; i++)
		for (int j = 0; j < 5; j++)
		{
			CModel3D* modelCopy = /*new CModel3D(mc)*/ m3->clone(osg::CopyOp::SHALLOW_COPY);
			modelCopy->setModelName(modelCopy->getModelName() + "-" + (std::to_string(i) + std::to_string(j)));
			modelCopy->setModelScale(osg::Vec3(3, 3, 3));

			osg::Matrix mat3D = getWorldMatrixfromLonLatHeight(true/*earth->getMapNode3D()*/, 106.56 + i * 0.05, 29.55 + j * 0.05, 2000/* + j * 2000*/);
			osg::ref_ptr<osg::Vec3Array> vecArray3D = createCirclePath(7000, mat3D);
			modelCopy->setModelAnimationPathCallback(vecArray3D, 1000, mat3D);
			modelCopy->setModelMatrix(mat3D);
			earth->addChild3D(modelCopy->getModelGroup());
			vecModel3D.push_back(modelCopy);
			//vecModelMT3D.push_back(modelCopy->getModel());

			CModel2D* modelCopy2D = m2->clone(osg::CopyOp::SHALLOW_COPY);
			modelCopy2D->setModelName(modelCopy2D->getModelName() + "-" + (std::to_string(i) + std::to_string(j)));
			

			osg::Matrix mat2D = getWorldMatrixfromLonLatHeight(false/*earth->getMapNode2D()*/, 106.56 + i * 0.05, 29.55 + j * 0.05, 2000 /*+ j * 2000*/);
			osg::ref_ptr<osg::Vec3Array> vecArray2D = createCirclePath(7000, mat2D);
			modelCopy2D->setModelAnimationPathCallback(vecArray2D, 1000, mat2D);
			modelCopy2D->setModelMatrix(mat2D);
			earth->addChild2D(modelCopy2D->getModelGroup());
			vecModel2D.push_back(modelCopy2D);
			//vecModelMT2D.push_back(modelCopy2D->getModel());
		}
	//test 3d follow
	{
		modelCopy1 = m3->clone(osg::CopyOp::SHALLOW_COPY);
		modelCopy1->setModelScale(osg::Vec3(3, 3, 3));

		//osg::Matrix mat3D = getWorldMatrixfromLonLatHeight(earth->getMapNode3D(), 106.56 + 1.8, 29.55 + 2.8, 10200);
		//
		osg::Vec4Array* vecArray3D = createCirclePath2(0.02, -0.005, 2000);
		//modelCopy1->setModelAnimationPathCallback(vecArray3D, 1000, mat3D);
		osg::ref_ptr<osg::AnimationPath> ap= creatAitPath(vecArray3D);//这个是另外一种移动方式
		modelCopy1->setModelAnimationPathCallback1(ap);
		//modelCopy1->setModelMatrix(mat3D);
		earth->addChild3D(modelCopy1->getModelGroup());
		vecModel3D.push_back(modelCopy1);
		
		modelCopy12D = m2->clone(osg::CopyOp::SHALLOW_COPY);
		//modelCopy12D->setModelScale(osg::Vec3(3, 3, 3));
		
		osg::Matrix mat2D = getWorldMatrixfromLonLatHeight(earth->getMapNode2D(), 106.56 + 1.8, 29.55 + 1.8,10200);
		osg::ref_ptr<osg::Vec3Array> vecArray2D = createCirclePath(7000, mat2D);
		modelCopy12D->setModelAnimationPathCallback(vecArray2D, 1000, mat2D);
		modelCopy12D->setModelMatrix(mat2D);
		earth->addChild2D(modelCopy12D->getModelGroup());
		vecModel2D.push_back(modelCopy12D);

		//osg::AutoTransform* at = new osg::AutoTransform;

		//at->setAutoScaleToScreen(TRUE);
		//at->setAutoScaleTransitionWidthRatio(0.0001);
		//at->setMinimumScale(0.0);
		//at->setMaximumScale(FLT_MAX);

		//at->addChild(modelCopy1->gettrack());

		//earth->getmroot()->addChild(at);


	}
	{
		modelCopy2 = m3->clone(osg::CopyOp::SHALLOW_COPY);
		modelCopy2->setModelScale(osg::Vec3(3, 3, 3));
		osg::Vec4Array* vecArray3D = createCirclePath2(0.02, 0.02,0);
		osg::ref_ptr<osg::AnimationPath> ap = creatAitPath(vecArray3D);//这个是另外一种移动方式
		modelCopy2->setModelAnimationPathCallback1(ap);
		earth->addChild3D(modelCopy2->getModelGroup());
		vecModel3D.push_back(modelCopy2);
		vecModelMT3D.push_back(modelCopy2->getModel());

	}
	{
		modelCopy3 = m3->clone(osg::CopyOp::SHALLOW_COPY);
		modelCopy3->setModelScale(osg::Vec3(3, 3, 3));
		osg::Vec4Array* vecArray3D = createCirclePath2(-0.02, -0.02, 0);
		osg::ref_ptr<osg::AnimationPath> ap = creatAitPath(vecArray3D);//这个是另外一种移动方式
		modelCopy3->setModelAnimationPathCallback1(ap);
		earth->addChild3D(modelCopy3->getModelGroup());
		vecModel3D.push_back(modelCopy3);
		vecModelMT3D.push_back(modelCopy3->getModel());

	}
	{
		modelCopy4 = m3->clone(osg::CopyOp::SHALLOW_COPY);
		modelCopy4->setModelScale(osg::Vec3(3, 3, 3));
		osg::Vec4Array* vecArray3D = createCirclePath2(0, 0, 0);
		osg::ref_ptr<osg::AnimationPath> ap = creatAitPath(vecArray3D);//这个是另外一种移动方式
		modelCopy4->setModelAnimationPathCallback1(ap);
		earth->addChild3D(modelCopy4->getModelGroup());
		vecModel3D.push_back(modelCopy4);
		vecModelMT3D.push_back(modelCopy4->getModel());

	}
	{
		CModel3D* modelCopy = /*new CModel3D(mc)*/ m3->clone(osg::CopyOp::SHALLOW_COPY);
		modelCopy->setModelName(modelCopy->getModelName() + "-" + (std::to_string(0) + std::to_string(0)));
		modelCopy->setModelScale(osg::Vec3(3, 3, 3));

		osg::Matrix mat3D = getWorldMatrixfromLonLatHeight(true/*earth->getMapNode3D()*/, 106.56, 29.55, 20000/* + j * 2000*/);
		osg::ref_ptr<osg::Vec3Array> vecArray3D = createCirclePath(7000, mat3D);
		//modelCopy->setModelAnimationPathCallback(vecArray3D, 1000, mat3D);
		modelCopy->setModelMatrix(mat3D);
		earth->addChild3D(modelCopy->getModelGroup());
		vecModel3D.push_back(modelCopy);
		//vecModelMT3D.push_back(modelCopy->getModel());

		CModel2D* modelCopy2D = m2->clone(osg::CopyOp::SHALLOW_COPY);
		modelCopy2D->setModelName(modelCopy2D->getModelName() + "-" + (std::to_string(0) + std::to_string(0)));


		osg::Matrix mat2D = getWorldMatrixfromLonLatHeight(false/*earth->getMapNode2D()*/, 106.56, 29.55, 200 /*+ j * 2000*/);
		osg::ref_ptr<osg::Vec3Array> vecArray2D = createCirclePath(7000, mat2D);
		//modelCopy2D->setModelAnimationPathCallback(vecArray2D, 1000, mat2D);
		modelCopy2D->setModelMatrix(mat2D);
		earth->addChild2D(modelCopy2D->getModelGroup());
		vecModel2D.push_back(modelCopy2D);
		//vecModelMT2D.push_back(modelCopy2D->getModel());
	
		modelCopy->setTrackVisible(false);
		modelCopy->removeLabelCallback();
		modelCopy->setLabelVisible(false);
		modelCopy->removeTrackCallback();

		modelCopy2D->setTrackVisible(false);
		modelCopy2D->removeLabelCallback();
		modelCopy2D->setLabelVisible(false);
		modelCopy2D->removeTrackCallback();
	}
	
	//CExplosion* exp = new CExplosion(3000000);
	//earth->addChild3D(exp->createExplosionNode(osg::Vec3d(0,0,0)*getWorldMatrixfromLonLatHeight(true, 120.047 - 1.6*0.7, 24.507 + 0.6*0.7,1000)));
	//for (int i = 0; i < vecModel3D.size(); i++)
	//{
	//	vecModel3D[i]->closeModelAutoTransform();
	//}
	//for (int i = 0; i < vecModel2D.size(); i++)
	//{
	//	vecModel2D[i]->closeModelAutoTransform();
	//}

	//modelCopy1->addTargetLock(vecModelMT3D);
	//modelCopy12D->addTargetLock(vecModelMT2D);

	//display
	osg::ref_ptr<osg::Node> airport = osgDB::readNodeFile("airport.ive");
	//airport->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF |osg:: StateAttribute::OVERRIDE);
	osg::MatrixTransform* mt_airport = new osg::MatrixTransform;
	mt_airport->addChild(airport);
	osg::Matrixd mt_temp;
	osg::ref_ptr<osg::CoordinateSystemNode> csn = new osg::CoordinateSystemNode;
	csn->setEllipsoidModel(new osg::EllipsoidModel); 
	csn->getEllipsoidModel()->computeLocalToWorldTransformFromLatLongHeight(osg::DegreesToRadians(24.507), osg::DegreesToRadians(120.047), 1000, mt_temp);
	mt_airport->setMatrix(mt_temp);
	earth->getSwitchMapNode3D()->addChild(mt_airport);
	vecModelMT3D.push_back(mt_airport);

	modelCopy1->addTargetLock(vecModelMT3D);
	//modelCopy2->addTargetLock(vecModelMT3D);
	//modelCopy3->addTargetLock(vecModelMT3D);
	//modelCopy4->addTargetLock(vecModelMT3D);


	return earth->start();
}