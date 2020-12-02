///*2020.11.30 以下仅作锁定目标的测试*/
//#include "Common/DataStruct.h"
//#include "Common/DigitalEarth.h"
//#include "Common/Model2D.h"
//#include "Common//Model3D.h"
//#include "Common/Utility.h"
//#include "Common/Explosion.h"
//#include "Common/PickHandle.h"
//#include "Common//PickDragHandler.h"
//
//#include <iostream>
//
////待锁定的目标飞机
//std::vector<CModel2D*> vecModel2D;
//std::vector<CModel3D*> vecModel3D;
//std::vector<osg::MatrixTransform*> vecModelMT2D;
//std::vector<osg::MatrixTransform*> vecModelMT3D;
//
//class CKeyboardHandler2D :public osgGA::GUIEventHandler
//{
//public:
//
//	CKeyboardHandler2D(DigitalEarth* earth) :
//		_earth(earth)
//	{
//	}
//
//	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
//	{
//		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
//		if (!viewer)return false;
//
//		switch (ea.getEventType())//判断事件，做出响应
//		{
//		case osgGA::GUIEventAdapter::KEYDOWN:
//			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_F3)
//			{//切换为3Dmapnode
//				_earth->setRenderMapType(false, true);
//			}
//			break;
//		default:break;
//		}
//		return false;
//	}
//private:
//	DigitalEarth* _earth;
//};
//
//class CKeyboardHandler3D :public osgGA::GUIEventHandler
//{
//public:
//
//	CKeyboardHandler3D(DigitalEarth* earth) :
//		_earth(earth)
//	{
//	}
//
//	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
//	{
//		osg::ref_ptr<osgViewer::Viewer> viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
//		if (!viewer)return false;
//
//		switch (ea.getEventType())//判断事件，做出响应
//		{
//		case osgGA::GUIEventAdapter::KEYDOWN://ea.getEventType()获取到的如果是键盘事件
//			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_F2)
//			{//切换为2Dmapnode
//				_earth->setRenderMapType(true, false);
//			}
//			//if (ea.getKey() == osgGA::GUIEventAdapter::KEY_U)
//			//{//移除目标锁定绘制
//			//	for (int i = 0; i < vecModel3D.size(); i++)
//			//	{
//			//		vecModel3D[i]->removeTargetLockAll();
//			//	}
//			//}
//			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_B)
//			{
//				osg::Matrix matTarget3D = getWorldMatrixfromLonLatHeight(true, 106.56, 29.55, 1000);
//				CExplosion* exp = new CExplosion(1000);
//				removeChildrenbyNodeName(_earth->getSwitchMapNode3D(), "explosionEffect");
//				_earth->addChild3D(exp->createExplosionNode(osg::Vec3d() * matTarget3D));
//			}
//			break;
//		default:break;
//		}
//		return false;
//	}
//private:
//	DigitalEarth* _earth;
//};
//
//int main(int argc, char** argv)
//{
//	DigitalEarth* earth = new DigitalEarth();
//	earth->addImageLayer("GoogleImage2D", false, OEDriverType::TMS, "D:/EarthMap/Openstreet/tms.xml");
//	earth->addImageLayer("GoogleImage3D", true, OEDriverType::TMS, "D:/EarthMap/yingxiang/tms.xml");
//	earth->addElevationLayer("3DElevation", true, OEDriverType::VBP, "D:/EarthMap/yingxiang/output/earth.ive");
//
//	earth->getViewer2D()->addEventHandler(new CKeyboardHandler2D(earth));
//	earth->getViewer3D()->addEventHandler(new CKeyboardHandler3D(earth));
//
//	ModelConfig mc = ModelConfig();
//	CModel3D* m3 = new CModel3D(mc);
//	m3->setModelScale(osg::Vec3(5, 5, 5));
//	osg::Matrix mat3 = getWorldMatrixfromLonLatHeight(true, 106.44, 29.54, 6000);
//	osg::ref_ptr<osg::Vec3Array> verArray3 = createCirclePath(7000, mat3);
//	m3->setModelAnimationPathCallback(verArray3, 1000, mat3);
//	m3->setModelMatrix(mat3);
//	earth->addChild3D(m3->getModelGroup());
//
//	ModelConfig mc2 = mc;
//	mc2.modelFilePath = "./PanoISRResource/texture/TYPE_AIRCRAFT.png";
//	CModel2D* m2 = new CModel2D(mc2);
//	m2->setModelScale(osg::Vec3(5, 5, 5));
//	osg::Matrix mat2 = getWorldMatrixfromLonLatHeight(false, 106.44, 29.54, 6000);
//	osg::ref_ptr<osg::Vec3Array> verArray2 = createCirclePath(7000, mat2);
//	m2->setModelAnimationPathCallback(verArray2, 1000, mat2);
//	m2->setModelMatrix(mat2);
//	earth->addChild2D(m2->getModelGroup());
//
//	for (int i = 0; i < 1; i++)
//		for (int j = 0; j < 2; j++)
//		{
//			CModel3D* modelCopy = /*new CModel3D(mc)*/ m3->clone(osg::CopyOp::SHALLOW_COPY);
//			modelCopy->setModelName(modelCopy->getModelName() + "-" + (std::to_string(i) + std::to_string(j)));
//			modelCopy->setModelScale(osg::Vec3(5, 5, 5));
//
//			osg::Matrix mat3D = getWorldMatrixfromLonLatHeight(true/*earth->getMapNode3D()*/, 106.56 + i * 0.1, 29.55 + j * 0.1, 2000/* + j * 2000*/);
//			osg::ref_ptr<osg::Vec3Array> vecArray3D = createCirclePath(7000, mat3D);
//			modelCopy->setModelAnimationPathCallback(vecArray3D, 1000, mat3D);
//			modelCopy->setModelMatrix(mat3D);
//			earth->addChild3D(modelCopy->getModelGroup());
//			vecModel3D.push_back(modelCopy);
//			vecModelMT3D.push_back(modelCopy->getModel());
//
//
//			CModel2D* modelCopy2D = m2->clone(osg::CopyOp::SHALLOW_COPY);
//			modelCopy2D->setModelName(modelCopy2D->getModelName() + "-" + (std::to_string(i) + std::to_string(j)));
//			modelCopy2D->setModelScale(osg::Vec3(5, 5, 5));
//
//			osg::Matrix mat2D = getWorldMatrixfromLonLatHeight(false/*earth->getMapNode2D()*/, 106.56 + i * 0.1, 29.55 + j * 0.1, 2000 /*+ j * 2000*/);
//			osg::ref_ptr<osg::Vec3Array> vecArray2D = createCirclePath(7000, mat2D);
//			modelCopy2D->setModelAnimationPathCallback(vecArray2D, 1000, mat2D);
//			modelCopy2D->setModelMatrix(mat2D);
//			earth->addChild2D(modelCopy2D->getModelGroup());
//			vecModel2D.push_back(modelCopy2D);
//			vecModelMT2D.push_back(modelCopy2D->getModel());
//		}
//
//	m3->addTargetLock(vecModelMT3D);
//	m2->addTargetLock(vecModelMT2D);
//
//	//m3->removeTargetLockAll();
//
//	return earth->start();
//}