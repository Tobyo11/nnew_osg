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
CModel3D* modelCopy1;//test
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
				osg::Matrix matTarget3D = getWorldMatrixfromLonLatHeight(true, 106.56, 29.55, 1000);
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

int main(int argc, char** argv)
{
	DigitalEarth* earth = new DigitalEarth();

	earth->create3DfollowView();//初始化3d follow
	earth->createDrawLine();//初始化画线
	earth->createManipulator();//初始化多操作器

	for (int i = 0; i < 100; i++)//添加路径相机路径
		earth->addCameraPath(106.56, 29.55 + 0.01*i, 2500, i);
	
	earth->addImageLayer("GoogleImage2D", false, OEDriverType::TMS, "D:/Openstreet/tms.xml");
	earth->addImageLayer("GoogleImage3D", true, OEDriverType::TMS, "D:/data/google/image/tms.xml");
	earth->addElevationLayer("3DElevation", true, OEDriverType::VBP, "D:/data/Earth/output/earth.ive");
	//earth->addElevationColor("3DElevationColor", "3DElevation", "D:/EarthMap/elevation.clr");

	earth->getViewer2D()->addEventHandler(new CKeyboardHandler2D(earth));
	earth->getViewer3D()->addEventHandler(new CKeyboardHandler3D(earth));

	earth->getViewer3D()->addEventHandler(new PickDragHandler());

	ModelConfig mc = ModelConfig();

	CModel3D* m3 = new CModel3D(mc);
	ModelConfig mc2 = mc;
	mc2.modelFilePath = "PanoISRResource/texture/TYPE_AIRCRAFT.png";
	CModel2D* m2 = new CModel2D(mc2);

	

	for (int i = 0; i < 33; i++)
		for (int j = 0; j < 33; j++)
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

		osg::Matrix mat3D = getWorldMatrixfromLonLatHeight(earth->getMapNode3D(), 106.56 + 1.8, 29.55 + 2.8, 10200);
		osg::ref_ptr<osg::Vec3Array> vecArray3D = createCirclePath(7000, mat3D);
		modelCopy1->setModelAnimationPathCallback(vecArray3D, 1000, mat3D);
		modelCopy1->setModelMatrix(mat3D);
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
	
	//CExplosion* exp = new CExplosion(3000000);
	//earth->addChild3D(exp->createExplosionNode(osg::Vec3d()));
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

	return earth->start();
}