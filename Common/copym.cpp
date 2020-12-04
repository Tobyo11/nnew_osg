//南水之源  20180101
#include "copym.h"
#include <osgViewer\Viewer>

#include <osgDB\ReadFile>
#include <osg\MatrixTransform>


using namespace osgEarth;

EarthWalkManipulator::EarthWalkManipulator()
{
	_eye = osg::Vec3d(0, 0, 0);
	_speed = 1.0;
	_transversal = false;
}

EarthWalkManipulator::~EarthWalkManipulator()
{
}

//获取相机的姿态矩阵 
osg::Matrixd EarthWalkManipulator::getMatrix() const
{
	osg::Matrix mat;
	mat.setRotate(_rotate);//先旋转
	mat.postMultTranslate(_eye);//再平移
	return mat;
}

osg::Matrixd EarthWalkManipulator::getInverseMatrix() const
{
	osg::Matrix mat;
	mat.setRotate(-_rotate);
	mat.preMultTranslate(-_eye);
	return mat;
	//return osg::Matrixd::inverse(getMatrix());
}


void EarthWalkManipulator::computeHomePosition()
{
}

void
EarthWalkManipulator::home(double unused)
{
	//经纬度0,0点姿态
	//_eye = osg::Vec3d(20078236, 0, 0);
	//_speed = 10.0;
	//osg::Matrix mHomeAttitude = osg::Matrix::lookAt(osg::Vec3d(0,0,0),osg::Vec3d(-1,0,0), osg::Vec3d(0,0,1));

	//使用中国南海上空的姿态
	_eye = osg::Vec3d(-4814692, 9236103, 5911710);
	_speed = 1000.0;
	osg::Matrix mHomeAttitude = osg::Matrix::lookAt(osg::Vec3d(0, 0, 0), osg::Vec3d(0.4, -0.77, -0.495), osg::Vec3d(0.228, -0.439, 0.869));

	_rotate = mHomeAttitude.getRotate();
	flushMouseEventStack();
}

void
EarthWalkManipulator::home(const osgGA::GUIEventAdapter&, osgGA::GUIActionAdapter& us)
{
	home(0.0);
	us.requestRedraw();
}

void
EarthWalkManipulator::setNode(osg::Node* node)
{
	// you can only set the node if it has not already been set, OR if you are setting
	// it to NULL. (So to change it, you must first set it to NULL.) This is to prevent
	// OSG from overwriting the node after you have already set on manually.
	if (node == 0L || !_node.valid())
	{
		_root = node;
		_node = node;
		_mapNode = 0L;
		_srs = 0L;

		established();

		osg::Matrix matrixGood1;
		GeoPoint point1(_srs, 0, 0, 10000.0);
		point1.createLocalToWorld(matrixGood1);

		_eye = matrixGood1.getTrans();

		osg::Vec3d worldup;
		point1.createWorldUpVector(worldup);

		osg::Matrix mat;
		matrixGood1.getRotate().get(mat);
		osg::Vec3d eye, center, up;
		mat.getLookAt(eye, center, up);
		mat.makeLookAt(eye, -worldup, up);

		_rotate = mat.getRotate();

	}
}

osg::Node*
EarthWalkManipulator::getNode()
{
	return _node.get();
}

bool EarthWalkManipulator::handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us)
{
	switch (ea.getEventType())
	{
	case(osgGA::GUIEventAdapter::FRAME):
	{
		osg::Vec3d  v3Eye, v3Center, v3Up;
		v3Eye = _eye;//使用相机实际位置
		osg::Vec3d v3EyeLonLat;
		_srs->transformFromWorld(v3Eye, v3EyeLonLat);

		if (!_transversal && v3EyeLonLat.z() < 10000000)//距离地面1千万米以内需要矫正
		{
			//先获取当前位置的经纬度，再获取当前正上，正北
			osg::Matrix mRealAttitude;

			if (v3EyeLonLat.z() < 0)//v3EyeLonLat.z()是眼点实际海拔
				v3EyeLonLat.z() = 100;//将海拔0以下的物体拉到海拔100米

			GeoPoint gEyeGeo(_srs, v3EyeLonLat.x(), v3EyeLonLat.y(), v3EyeLonLat.z());
			gEyeGeo.createLocalToWorld(mRealAttitude);

			osg::Vec3d v3HorizonUp;//指天向量
			gEyeGeo.createWorldUpVector(v3HorizonUp);

			_rotate.get(mRealAttitude);//要使用当前相机的姿态
			mRealAttitude.getLookAt(v3Eye, v3Center, v3Up);//获取新的位置和姿态

			osg::Vec3 v3Direction = v3Center - v3Eye;
			mRealAttitude.makeLookAt(osg::Vec3d(0, 0, 0), v3Direction, v3HorizonUp);
			float fCosAngle = (v3Direction*v3HorizonUp / v3Direction.length()) / v3HorizonUp.length();
			if (abs(fCosAngle) < 0.9)//在cos@小于0.9的时候矫正
				_rotate = mRealAttitude.getRotate();
		}

	}break;
	case(osgGA::GUIEventAdapter::PUSH):
	{
	}break;
	case(osgGA::GUIEventAdapter::RELEASE):
	{
		flushMouseEventStack();
	}break;
	case(osgGA::GUIEventAdapter::DRAG):
	{
		if (calcMovement(ea))//根据鼠标在屏幕中的位置调整相机转向
		{
			us.requestRedraw();
			return true;
		}
	};
	case(osgGA::GUIEventAdapter::SCROLL)://由于已经每帧都调整姿态，所以手动滚动不需要了
	{
		osg::Vec3   v3Direction;         //视点方向  
		osg::Matrix mCameraQuat;
		osg::Vec3d  v3Eye, v3Center, v3Up;
		_rotate.get(mCameraQuat);
		mCameraQuat.getLookAt(v3Eye, v3Center, v3Up);//这里的v3Eye不是实际相机的位置，而是0，0，0
		v3Direction = v3Center - v3Eye;
		v3Direction.normalize();
		osg::Vec3d v3CrossVector = v3Up ^ v3Direction;
		v3CrossVector.normalize();
		switch (ea.getScrollingMotion())
		{
		case osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_UP://逆时针旋转相机
		{
			_eye += v3Direction * _speed;
		}break;
		case osgGA::GUIEventAdapter::ScrollingMotion::SCROLL_DOWN://顺时针旋转相机
		{
			_eye -= v3Direction * _speed;
		}break;
		}
		return true;
	}break;
	case (osgGA::GUIEventAdapter::KEYDOWN):
	{
		osg::Vec3   v3Direction;         //视点方向  
		osg::Matrix mCameraQuat;
		osg::Vec3d  v3Eye, v3Center, v3Up;
		_rotate.get(mCameraQuat);
		mCameraQuat.getLookAt(v3Eye, v3Center, v3Up);//这里的v3Eye不是实际相机的位置，而是0，0，0
		v3Direction = v3Center - v3Eye;
		v3Direction.normalize();
		osg::Vec3d v3CrossVector = v3Up ^ v3Direction;
		v3CrossVector.normalize();
		if (ea.getKey() == 'q' || ea.getKey() == 'Q' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Page_Up)//往头部前进
		{
			_eye += v3Up * _speed;
		}
		if (ea.getKey() == 'e' || ea.getKey() == 'E' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Page_Down)//往尾部后退
		{
			_eye -= v3Up * _speed;
		}
		if (ea.getKey() == 'w' || ea.getKey() == 'W' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Up)//前进
		{
			_eye += v3Direction * _speed;
		}
		if (ea.getKey() == 's' || ea.getKey() == 'S' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Down)//后退
		{
			_eye -= v3Direction * _speed;
		}
		if (ea.getKey() == 'a' || ea.getKey() == 'A' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Left)//左移
		{
			_transversal = true;
			_eye += v3CrossVector * _speed;
		}
		if (ea.getKey() == 'd' || ea.getKey() == 'D' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Right)//右移
		{
			_transversal = true;
			_eye -= v3CrossVector * _speed;
		}
		if (ea.getKey() == '-' || ea.getKey() == '_')//减10倍移动速度
		{
			_speed /= 10.0;
			if (_speed < 1.0)
			{
				_speed = 1.0;
			}
		}
		if (ea.getKey() == '=' || ea.getKey() == '+')//加10倍移动速度
		{
			_speed *= 10.0;
			if (_speed > 100000.0)
			{
				_speed = 100000.0;
			}
		}

		if (ea.getKey() == 'h' || ea.getKey() == 'H')//在当前经纬度，姿态回正：1.视点向地面 2.头部向正北
		{
			v3Eye = _eye;//使用相机实际位置
			osg::Vec3d v3EyeLonLat;
			_srs->transformFromWorld(v3Eye, v3EyeLonLat);
			//先获取当前位置的经纬度，再获取当前正上，正北
			osg::Matrix mRealAttitude;

			if (v3EyeLonLat.z() < 0)//v3EyeLonLat.z()是眼点实际海拔
				v3EyeLonLat.z() = 100;//将海拔0以下的物体拉到海拔100米

			GeoPoint gEyeGeo(_srs, v3EyeLonLat.x(), v3EyeLonLat.y(), v3EyeLonLat.z());
			gEyeGeo.createLocalToWorld(mRealAttitude);

			osg::Vec3d v3HorizonUp;//指天向量
			gEyeGeo.createWorldUpVector(v3HorizonUp);

			_eye = mRealAttitude.getTrans();

			mRealAttitude.getLookAt(v3Eye, v3Center, v3Up);//获取新的位置和姿态

			osg::Matrix mDeviationAttitude;//向北位置偏移0.00001纬度，为了计算正北方向
			GeoPoint gDeviationEyeGeo(_srs, v3EyeLonLat.x(), v3EyeLonLat.y() + 0.00001, v3EyeLonLat.z());
			gDeviationEyeGeo.createLocalToWorld(mDeviationAttitude);
			osg::Vec3d v3DeviationNorthPoint = mDeviationAttitude.getTrans();
			osg::Vec3d v3NorthHeadUp = v3DeviationNorthPoint - v3Eye;
			v3NorthHeadUp.normalize();//指北向量

			if (v3EyeLonLat.y() < 89.99999  && v3EyeLonLat.y() > -90.0)
			{
				mRealAttitude.makeLookAt(osg::Vec3d(0, 0, 0), -v3HorizonUp, v3NorthHeadUp);
			}
			_rotate = mRealAttitude.getRotate();
		}
		if (ea.getKey() == 'g' || ea.getKey() == 'G')//在当前经纬度，头部回正：1.视点中心不变 2.头部向天
		{
			v3Eye = _eye;//使用相机实际位置
			osg::Vec3d v3EyeLonLat;
			_srs->transformFromWorld(v3Eye, v3EyeLonLat);
			//先获取当前位置的经纬度，再获取当前正上，正北
			osg::Matrix mRealAttitude;

			if (v3EyeLonLat.z() < 0)//v3EyeLonLat.z()是眼点实际海拔
				v3EyeLonLat.z() = 100;//将海拔0以下的物体拉到海拔100米

			GeoPoint gEyeGeo(_srs, v3EyeLonLat.x(), v3EyeLonLat.y(), v3EyeLonLat.z());
			gEyeGeo.createLocalToWorld(mRealAttitude);

			osg::Vec3d v3HorizonUp;//指天向量
			gEyeGeo.createWorldUpVector(v3HorizonUp);


			_rotate.get(mRealAttitude);//要使用当前相机的姿态
			mRealAttitude.getLookAt(v3Eye, v3Center, v3Up);//获取新的位置和姿态

			osg::Vec3 v3Direction = v3Center - v3Eye;
			mRealAttitude.makeLookAt(osg::Vec3d(0, 0, 0), v3Direction, v3HorizonUp);
			_rotate = mRealAttitude.getRotate();
		}
		if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Shift_L)
		{
			_speed = 1000000.0;
		}
		if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Control_L)
		{
			_speed = 100000.0;
		}
	}break;
	case (osgGA::GUIEventAdapter::KEYUP):
	{
		if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Shift_L)
		{
			_speed = 1000.0;
		}
		if (ea.getKey() == osgGA::GUIEventAdapter::KEY_Control_L)
		{
			_speed = 100.0;
		}

		if (ea.getKey() == 'a' || ea.getKey() == 'A' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Left)//左移
		{
			_transversal = false;
		}
		if (ea.getKey() == 'd' || ea.getKey() == 'D' || ea.getKey() == osgGA::GUIEventAdapter::KEY_Right)//右移
		{
			_transversal = false;
		}
	}break;
	default:
		return false;
	}
}

bool
EarthWalkManipulator::established()
{
	if (_srs.valid() && _mapNode.valid() && _node.valid())
		return true;

	// lock down the observed node:
	osg::ref_ptr<osg::Node> safeNode;
	if (!_node.lock(safeNode))
		return false;

	// find a map node or fail:
	_mapNode = osgEarth::MapNode::findMapNode(safeNode.get());
	if (!_mapNode.valid())
		return false;

	// Cache the SRS.
	_srs = _mapNode->getMapSRS();
	return true;
}

void EarthWalkManipulator::addMouseEvent(const osgGA::GUIEventAdapter& ea)
{
	_ga_t1 = _ga_t0;
	_ga_t0 = &ea;
}

void EarthWalkManipulator::flushMouseEventStack()
{
	_ga_t1 = NULL;
	_ga_t0 = NULL;
}

bool EarthWalkManipulator::calcMovement(const osgGA::GUIEventAdapter& ea)
{
	osg::Quat qat;
	osg::Matrix mat;
	_rotate.get(mat);
	osg::Vec3d eye, center, up;
	mat.getLookAt(eye, center, up);

	osg::Vec3d dirction = center - eye;
	dirction.normalize();
	up.normalize();
	osg::Vec3d cross = dirction ^ up;
	cross.normalize();

	_ga_t1 = _ga_t0;
	_ga_t0 = &ea;

	if (_ga_t0.get() == NULL || _ga_t1.get() == NULL) return false;

	double x1 = _ga_t0->getXnormalized() - _ga_t1->getXnormalized();
	double y1 = _ga_t0->getYnormalized() - _ga_t1->getYnormalized();

	osg::Vec3d deviation(0, 0, 0);
	deviation += cross * x1;
	deviation += up * y1;

	mat = osg::Matrix::lookAt(eye, deviation + center, up);
	_rotate = mat.getRotate();

	return true;
}