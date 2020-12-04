/*
	1.ʹ��addview��removeView��ʽ����������Լ20֡,������23ά�л�ʱ��ͬ��(����δ����,ģ����23ά�µ�λ�ò�һ��)
		����ʵ��ʹ�����,ÿ�η���λ����Ϣ,ԭ�����ܱ���23ά��ͬ��(�������ק)
	2.���ƻص���Ӧ���Ը����ӵ�Զ���������������ţ�δʵ�֣�
		-3.β������Զ����ź���ʾ����Ҫ���[Ʈ����ʹ���߽��л���]
	4.β����Ҫ������ģ��β��(3D)
	5.��̬�ı�ģ��δʵ��
	6.��ά���Ʊ���δʵ��
	7.ͨ�������ļ��ı������ʽδʵ��
	8.ǳ�������ܴ��ڵ�����:��̬�ı���ɫʱ,����ǳ��������ͬ�����޸���ɫ
		-9.PickHandler��move()������Ҫ����,��Ϊ��û�г���������,��ģ�͵��ƶ������е㸴��[ʹ��osg�Դ�����ק��]
	10.ģ����ק����23άû�н���ͬ��,��Ҫ��������
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

		switch (ea.getEventType())//�ж��¼���������Ӧ
		{
		case osgGA::GUIEventAdapter::KEYDOWN:
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_F3)
			{//�л�Ϊ3Dmapnode
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

		switch (ea.getEventType())//�ж��¼���������Ӧ
		{
		case osgGA::GUIEventAdapter::KEYDOWN://ea.getEventType()��ȡ��������Ǽ����¼�
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_F2)
			{//�л�Ϊ2Dmapnode
				_earth->setRenderMapType(true, false);
			}
			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_U)
			{//�Ƴ�Ŀ����������
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
				//_earth->addShp("D:/data/chinashp/��·_polyline.shp", shpLine);
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
			//{//���ظ߳���ɫ
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

	earth->create3DfollowView();//��ʼ��3d follow
	earth->createDrawLine();//��ʼ������
	earth->createManipulator();//��ʼ���������

	for (int i = 0; i < 100; i++)//���·�����·��
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