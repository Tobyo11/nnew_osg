///*2020.11.30*/
///*�˴������std::vector<osg::MatrixTransform*> ����ز���*/
//#include <iostream>
//#include <windows.h>
//
//#include <osg/MatrixTransform>
//#include <osg/Array>
//#include <osgViewer/Viewer>
//
//std::vector<osg::MatrixTransform*> vecModel;
//
//int main(int argc, char** argv)
//{
//	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//
//	for (int i = 0; i < 5; i++)
//	{
//		osg::ref_ptr<osg::MatrixTransform> model = new osg::MatrixTransform;
//		model->setName("mt-" + std::to_string(i));
//		vecModel.push_back(model.get());
//	}
//
//	vecModel[0]->getBound()
//
//	viewer->setSceneData(root);
//	return viewer->run();
//}