//#include <windows.h>
//#include <osg/Geode>
//#include <osg/LineWidth>
//#include <osg/AutoTransform>
//#include <osgViewer/Viewer>
//#include <osgViewer/ViewerEventHandlers>
//#include <osgDB/ReadFile>
//#include <osgFX/OutLine>
//#include <osgGA/GUIEventHandler>
//
//class OutLineHandler : public osgGA::GUIEventHandler
//{
//public:
//	OutLineHandler(osg::ref_ptr<osg::Group>& root):
//	_root(root)
//	{
//
//	}
//
//	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa)
//	{
//		osg::ref_ptr<osgViewer::View> view = dynamic_cast<osgViewer::View*> (&aa);
//		if (NULL == view)
//		{
//			return false;
//		}
//
//		switch (ea.getEventType())
//		{
//		case osgGA::GUIEventAdapter::KEYDOWN:
//		{
//			if (ea.getKey() == osgGA::GUIEventAdapter::KEY_O)
//			{
//				osg::ref_ptr<osg::Node> model = _root->getChild(0);
//				if (model)
//				{
//					osg::ref_ptr<osgFX::Outline> outline = new osgFX::Outline();
//					outline->setColor(osg::Vec4(1, 1, 0, 1));
//					outline->setWidth(5);
//					outline->addChild(model);
//					outline->setName("outline");
//					_root->replaceChild(model, outline);
//				}
//				//osg::ref_ptr<osgFX::Outline> outline = new osgFX::Outline();
//				//outline->setColor(osg::Vec4(1, 1, 0, 1));
//				//outline->setWidth(5);
//				//outline->addChild(node);
//				//parent->replaceChild(node, outline);
//			}
//		}
//			break;
//		default:
//			break;
//		}
//
//		return false;
//	}
//private:
//	osg::ref_ptr<osg::Group> _root;
//};
//int main(int argc, char** argv)
//{
//	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
//	osg::ref_ptr<osg::Group> root = new osg::Group;
//
//	osg::ref_ptr<osg::Node> model = osgDB::readNodeFile("./PanoISRResource/model/f35.fbx");
//	if (model)
//		root->addChild(model);
//
//	viewer->setSceneData(root);
//
//	//osg::DisplaySettings::instance()->setMinimumNumStencilBits(1);
//	//unsigned int clearMask = viewer->getCamera()->getClearMask();
//	//viewer->getCamera()->setClearMask(clearMask | GL_STENCIL_BUFFER_BIT);
//	//viewer->getCamera()->setClearStencil(0);
//
//	viewer->addEventHandler(new OutLineHandler(root));
//
//	return viewer->run();
//}