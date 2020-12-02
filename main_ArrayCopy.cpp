//#include <windows.h>
//#include <iostream>
//
//#include <osg/Array>
//
//int main()
//{
//	osg::ref_ptr<osg::Vec3Array> v1 = new osg::Vec3Array;
//	for (int i = 0; i < 10; i++)
//		v1->push_back(osg::Vec3d(i, i, i));
//	osg::ref_ptr<osg::Vec3Array> v2 = dynamic_cast<osg::Vec3Array*>(v1->clone(osg::CopyOp::DEEP_COPY_ARRAYS));
//	v2->push_back(osg::Vec3d(50, 50, 50));
//	v1->clear();
//	return 0;
//}