//
//#include <Windows.h>
//#include <iostream>
//#include <osgDB/ReadFile>
//#include <osgViewer/ViewerEventHandlers>
//#include <osgAnimation/BasicAnimationManager>
//
//#include <iostream>
//
//struct AnimationManagerFinder : public osg::NodeVisitor
//{
//	osg::ref_ptr<osgAnimation::BasicAnimationManager> _am;
//	AnimationManagerFinder() : osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN) {}
//	void apply(osg::Node* node) {
//		if (_am.valid())
//			return;
//		if (node->getUpdateCallback()) {
//			osgAnimation::AnimationManagerBase* b = dynamic_cast<osgAnimation::AnimationManagerBase*>(node->getUpdateCallback());
//			if (b) {
//				_am = new osgAnimation::BasicAnimationManager(*b);
//				return;
//			}
//		}
//		traverse(*node);
//	}
//};
//osg::ref_ptr< osg::Node> animationnode(std::string filepath, int num)
//{
//	//读取带动画的节点
//	osg::ref_ptr<osg::Node> animationNode = osgDB::readNodeFile(filepath);
//	//获得节点的动画列表
//	osg::ref_ptr <AnimationManagerFinder> m_cFinder = new AnimationManagerFinder();
//	m_cFinder->apply(animationNode);
//	animationNode->accept(*m_cFinder);
//	if (m_cFinder->_am.valid())
//	{
//		animationNode->setUpdateCallback(m_cFinder->_am.get());
//	}
//	else
//	{
//		std::cout << "No animotion" << std::endl;
//		return animationNode;
//	}
//	std::cout << m_cFinder->_am->getAnimationList().size() << std::endl;
//	if (num > m_cFinder->_am->getAnimationList().size() || num < 1)
//	{
//		std::cout << "Out of animotion list" << std::endl;
//		return animationNode;
//	}
//	osgAnimation::AnimationList::const_iterator it = m_cFinder->_am->getAnimationList().begin();
//	it = it + num - 1;
//	std::string animationName = (*it)->getName();
//	osgAnimation::Animation::PlayMode playMode = osgAnimation::Animation::LOOP;
//	(*it)->setPlayMode(playMode);//设置播放模式
//	//(*it)->setDuration(5.0);//设置播放时间
//	//从动画列表中选择一个动画，播放
//	m_cFinder->_am->playAnimation(*it);
//
//	return animationNode;
//}
//int main(int argc, char ** argv)
//{
//	osgViewer::Viewer viewer;
//
//
//
//	osg::Group* root = new osg::Group;
//
//	root->addChild(animationnode("./PanoISRResource/model/ddc.fbx", 1).get());
//
//	viewer.setSceneData(root);
//
//
//	return viewer.run();
//}