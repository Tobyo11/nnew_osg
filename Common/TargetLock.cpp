#include "TargetLock.h"
#include <osg/Depth>
#include <osg/LineWidth>
#include <osg/MatrixTransform>

TargetLock::TargetLock()
{
	_lockGeode = new osg::Geode;
	_lockGeode->setName("lockGeode");
	_lockLineGeom = new osg::Geometry;
	_lockLineGeom->setName("lockLineGeom");

	_lockPosUpdate = false;
}

TargetLock::~TargetLock()
{
	//
}

void TargetLock::updateLockPos(osg::Vec3d platPos, std::vector<osg::Vec3d> vecTargetPos)
{
	if (_lockPosUpdate)
	{//��������ʱ�Ž��и���
		_platPos = platPos;
		_vecTargetPos = vecTargetPos;

		osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*>(_lockLineGeom->getVertexArray());
		verArray->clear();
		auto iter = _vecTargetPos.begin();
		while (iter != _vecTargetPos.end())
		{
			verArray->push_back(_platPos);
			verArray->push_back(*iter);

			iter++;
		}

		for (int i = 0; i < _lockLineGeom->getPrimitiveSetList().size(); i++)
			_lockLineGeom->removePrimitiveSet(i);
		_lockLineGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray->size()));

		verArray->dirty();
		_lockLineGeom->dirtyBound();
	}
}

/*2020.11.30 �¼� updateLockPos����*/
void TargetLock::updateLockPos(osg::Vec3d platPos, std::vector<osg::MatrixTransform*> vecModel)
{
	if (_lockPosUpdate)
	{//��������ʱ�Ž��и���
		_platPos = platPos;
		_vecModel = vecModel;

		osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*>(_lockLineGeom->getVertexArray());
		verArray->clear();

		auto iter = _vecModel.begin();
		while (iter != _vecModel.end())
		{
			verArray->push_back(_platPos);
			verArray->push_back((*iter)->getBound().center());

			iter++;
		}

		for (int i = 0; i < _lockLineGeom->getPrimitiveSetList().size(); i++)
			_lockLineGeom->removePrimitiveSet(i);
		_lockLineGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray->size()));

		verArray->dirty();
		_lockLineGeom->dirtyBound();
	}
}

void TargetLock::addLock(osg::Vec4d color)
{
	osg::ref_ptr<osg::Vec3Array> verArray = new osg::Vec3Array(2);
	osg::ref_ptr<osg::Vec4Array> colArray = new osg::Vec4Array;
	verArray->setDataVariance(osg::Object::DYNAMIC);
	colArray->push_back(color);

	_lockLineGeom->setDataVariance(osg::Object::DYNAMIC);
	_lockLineGeom->setUseDisplayList(false);
	_lockLineGeom->setUseVertexBufferObjects(true);
	_lockLineGeom->setVertexArray(verArray);
	_lockLineGeom->setColorArray(colArray);
	_lockLineGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	_lockLineGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray->size()));

	osg::ref_ptr < osg::StateSet> pStateSet = _lockLineGeom->getOrCreateStateSet();
	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	pStateSet->setRenderBinDetails(298, "RenderBin");
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	pStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	pStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::LESS, 0.0, 1.0, true), osg::StateAttribute::ON);
	pStateSet->setAttributeAndModes(new osg::LineWidth(2), osg::StateAttribute::ON);

	_lockGeode->addDrawable(_lockLineGeom);
	_lockPosUpdate = true;
}

void TargetLock::deleteLock()
{
	_lockPosUpdate = false;
	_platPos = osg::Vec3d();
	_vecTargetPos.clear();
	_lockGeode->removeUpdateCallback(_lockGeode->getUpdateCallback());
	for (unsigned int i = 0; i < _lockGeode->getNumChildren(); i++)
	{
		_lockGeode->removeChild(_lockGeode->getChild(i));
	}
}

/*2020.11.30 Ŀ���������� �ı����߱��ַ�ʽ*/
/****************2020.11.30 Ŀ�����������ߵ�����ص�,����TargetLock.cpp����Ͷ���******************************/
class RayLineCallback :public osg::NodeCallback
{
public:
	RayLineCallback(osg::Vec3d startPos, osg::Vec3d endPos, float partLength/*�ܳ���ռ��*/, float blank/*�������(�ܳ���ռ��)*/, float step/*ÿ���ƶ�����*/)
	{
		_startPos = startPos;
		_endPos = endPos;
		_length = (endPos - startPos).length() * partLength;
		_norVec = (endPos - startPos) / (endPos - startPos).length();
		_blankLength = (endPos - startPos).length() * blank;
		_step = step;
		
		_first = true;
	}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
	{
		osg::ref_ptr<osg::Geometry> geom = dynamic_cast<osg::Geometry*> (node);
		if (geom)
		{
			osg::ref_ptr<osg::Vec3Array> verArray = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());
			if (_first)
			{
				verArray->clear();
				osg::Vec3d point = _startPos;
				int i = 0;
				while ((point - _startPos).length() < (_endPos - _startPos).length())
				{
					verArray->push_back(point);
					if (i % 2 == 0)
					{
						point = point + _norVec * _length;
					}
					else
					{
						point = point + _norVec * _blankLength;
					}
					i++;
				}
				if (i % 2 == 1)
					verArray->push_back(_endPos);
				_first = false;
			}
			else
			{
				//�ȶ�ÿ����������ƶ�
				auto iter = verArray->begin();
				while (iter != verArray->end())
				{
					(*iter) += _norVec * _step;
					iter++;
				}
				//�ٶԶ�������ж�(�Ƿ񳬹��߶��������յ�)
				//��ǰ�����������
				osg::Vec3d firstPos = verArray->at(0);
				osg::Vec3d secondPos = verArray->at(1);
				if ((firstPos - secondPos).length() < _length)
				{
					verArray->erase(verArray->begin());
					verArray->insert(verArray->begin(), _startPos);
				}
				else
				{
					if ((firstPos - _endPos).length() < (_startPos + _norVec * _blankLength - _endPos).length())
					{
						verArray->insert(verArray->begin(), _startPos);
						verArray->insert(verArray->begin(), _startPos + _norVec * _blankLength);
					}
				}
				//������һ������
				osg::Vec3d rfirstPos = verArray->at(verArray->size() - 1);
				osg::Vec3d rsecondPos = verArray->at(verArray->size() - 2);
				if ((rsecondPos - _startPos).length() >= (_endPos - _startPos).length())
				{
					verArray->pop_back();
					verArray->pop_back();
				}
				rfirstPos = verArray->at(verArray->size() - 1);
				rsecondPos = verArray->at(verArray->size() - 2);
				if ((rfirstPos - _startPos).length() > (_endPos - _startPos).length())
				{
					verArray->pop_back();
					verArray->push_back(_endPos);
				}
			}

			for (int i = 0; i < geom->getPrimitiveSetList().size(); i++)
				geom->removePrimitiveSet(i);
			geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray->size()));

			verArray->dirty();
		}
		traverse(node, nv);
	}
protected:
	osg::Vec3d _startPos;
	osg::Vec3d _endPos;
	float _length;
	float _blankLength;
	osg::Vec3d _norVec;
	osg::Vec3d _lastPos;
	float _step;	//	�ƶ�����

	bool _first;
};
/**************************************************************************************************************/

NTargetLock::NTargetLock()
{
	_lockPosUpdate = false;
	_goalNum = 0;

	_lock = new osg::MatrixTransform;
	_lock->setName("lock");
}

NTargetLock::~NTargetLock()
{
	//
}

void NTargetLock::addLock(osg::Vec4d color)
{
	_goalNum++;
	if (_goalNum == 0)
		return;

	_lock->addChild(createLock(color));
	_lockPosUpdate = true;
}

void NTargetLock::clearLock()
{
	_lockPosUpdate = false;
	_goalNum = 0;

	for (unsigned int i = 0; i < _lock->getNumChildren(); i++)
	{
		_lock->removeChild(_lock->getChild(i));
	}
}

void NTargetLock::deleteLock(unsigned int n)
{
	if (n < _lock->getNumChildren())
	{
		_lock->removeChild(_lock->getChild(n));
		_goalNum--;
		if (_goalNum == 0)
			_lockPosUpdate = false;
	}
}

osg::ref_ptr<osg::MatrixTransform> NTargetLock::createLock(osg::Vec4d color)
{
	osg::ref_ptr<osg::MatrixTransform> lockSelf = new osg::MatrixTransform;
	lockSelf->setName("lockSelf");
	osg::ref_ptr<osg::Geode> lockGeode = new osg::Geode;
	lockGeode->setName("lockGeode");
	osg::ref_ptr<osg::Geometry> lockGeom = new osg::Geometry;
	lockGeom->setName("lockGeom");

	osg::ref_ptr<osg::Vec3Array> verArray = new osg::Vec3Array(2);
	osg::ref_ptr<osg::Vec4Array> colArray = new osg::Vec4Array;
	verArray->setDataVariance(osg::Object::DYNAMIC);
	colArray->push_back(color);

	lockGeom->setDataVariance(osg::Object::DYNAMIC);
	lockGeom->setUseDisplayList(false);
	lockGeom->setUseVertexBufferObjects(true);
	lockGeom->setVertexArray(verArray);
	lockGeom->setColorArray(colArray);
	lockGeom->setColorBinding(osg::Geometry::BIND_OVERALL);
	lockGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, verArray->size()));

	osg::ref_ptr < osg::StateSet> pStateSet = lockGeom->getOrCreateStateSet();
	pStateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
	pStateSet->setRenderBinDetails(298, "RenderBin");
	pStateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
	pStateSet->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF);
	pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	pStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::LESS, 0.0, 1.0, true), osg::StateAttribute::ON);
	pStateSet->setAttributeAndModes(new osg::LineWidth(2), osg::StateAttribute::ON);

	lockGeom->setUpdateCallback(new RayLineCallback(osg::Vec3d(), osg::Vec3d(0.0, 1.0, 0.0), 0.03, 0.02, 0.001));

	lockGeode->addDrawable(lockGeom);
	lockSelf->addChild(lockGeode);

	return lockSelf;
}
