#include "stdafx.h"
#include "DynamicModel.h"
#include "StringUtils.h"

#include "OgreSceneManager.h"
#include "OgreEntity.h"
#include "OgreAnimation.h"
#include "OgreSkeletonManager.h"

#include "tinyxml.h"

DynamicModel::DynamicModel(Ogre::SceneManager *sceneManager)
	:SceneObject(sceneManager, SceneObject::DYNAMIC_MODEL)
{
}

void DynamicModel::create(std::string name, std::string path)
{
	SceneObject::create(name, path);
	// ��Modelת��Ϊ������Xml����֮
	Ogre::DataStreamPtr DS = 
		Ogre::ResourceGroupManager::getSingleton().openResource(path);
	Ogre::MemoryDataStreamPtr MemDS =
		Ogre::MemoryDataStreamPtr(new Ogre::MemoryDataStream(path, DS));
	TiXmlDocument Doc;
	TiXmlElement *Root = NULL;
	Doc.Parse((const char*)MemDS->getPtr());
	if(!Doc.Error())
		Root = Doc.RootElement();
	if(Root->ValueTStr() != "model")
	{
		AfxMessageBox("��Ч��Model�ļ�");
		return;
	}
	Ogre::String ModelPath;
	size_t Pos = path.find_last_of('/');
	if(Pos != std::string::npos)
	{
		// substrȡ[0, Pos+1)�ڵĴ�
		ModelPath = path.substr(0, Pos+1);
	}

	std::map<std::string, Ogre::SceneNode*> BoneMap;
	// һ����ܣ�Frame���������������Bone�������������Anim����һ���������������켣��Track��
	TiXmlElement *FrameElmt = Root->FirstChildElement("frame");
	if(FrameElmt)
	{
		Ogre::String FrameName = ModelPath+
			StringUtils::utf8ToGbk(FrameElmt->Attribute("name"));
		Ogre::SkeletonPtr Frame = Ogre::SkeletonManager::getSingleton().load(
			FrameName, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME).staticCast<Ogre::Skeleton>();
		// ��������
		Ogre::Skeleton::BoneIterator BoneIter = Frame->getBoneIterator();
		while(BoneIter.hasMoreElements())
		{
			Ogre::Bone *Bone = BoneIter.getNext();
			cloneBone(Bone, &BoneMap);
		}
		// ��������[auto]
		Ogre::Animation *FrameAnim = Frame->getAnimation("[auto]");
		Ogre::Animation *SceneAnim = sceneManager->createAnimation(sceneNode->getName(), FrameAnim->getLength());
		SceneAnim->setInterpolationMode(FrameAnim->getInterpolationMode());
		SceneAnim->setRotationInterpolationMode(FrameAnim->getRotationInterpolationMode());
		// 1) ���������еĹ켣�����俽�������������У������ù������
		Ogre::Animation::NodeTrackIterator &NodeTrackIter = FrameAnim->getNodeTrackIterator();
		while(NodeTrackIter.hasMoreElements())
		{
			Ogre::NodeAnimationTrack *NodeAnimTrack = NodeTrackIter.getNext()->_clone(SceneAnim);
			Ogre::Node *Bone = NodeAnimTrack->getAssociatedNode();
			NodeAnimTrack->setAssociatedNode(BoneMap[Bone->getName()]);
		}
		// 2) ...
		SceneAnim->_keyFrameListChanged();
		Ogre::AnimationState *AnimState = sceneManager->createAnimationState(sceneNode->getName());
		AnimState->setEnabled(true);
		mAnimStates.push_back(AnimState);
	}

	size_t Cnt = 0;TiXmlElement *EntityElmt = Root->FirstChildElement("entity");
	for(;EntityElmt!=NULL; EntityElmt=EntityElmt->NextSiblingElement(), Cnt++)
	{
		Ogre::String Mesh = ModelPath + StringUtils::utf8ToGbk(EntityElmt->Attribute("mesh"));
		Ogre::String Node = StringUtils::utf8ToGbk(EntityElmt->Attribute("node"));
		Ogre::String EntityName = 
			sceneNode->getName()+"<"+Ogre::StringConverter::toString(Cnt)+">";
		Ogre::Entity *Entity = sceneManager->createEntity(EntityName, Mesh);
		Entity->setUserAny(Ogre::Any((SceneObject*)this));
		mEntities.push_back(Entity);
		Ogre::SceneNode *SceneNode = NULL;
		if(!Node.empty())
		{
			SceneNode = BoneMap[Node];
		}
		TiXmlElement *OffsetElmt = EntityElmt->FirstChildElement("offset");
		if(OffsetElmt)
		{
			Ogre::Vector3 translate(0.0, 0.0, 0.0);	Ogre::Quaternion rotate;
			OffsetElmt = OffsetElmt->FirstChildElement();
			for(;OffsetElmt!=NULL ;OffsetElmt=OffsetElmt->NextSiblingElement())
			{
 				if ("translate" == OffsetElmt->ValueTStr())
 				{
 					OffsetElmt->QueryFloatAttribute("x", &translate.x);
 					OffsetElmt->QueryFloatAttribute("y", &translate.y);
 					OffsetElmt->QueryFloatAttribute("z", &translate.z);
 				}
 				else if ("rotate" == OffsetElmt->ValueTStr())
 				{
 					OffsetElmt->QueryFloatAttribute("w", &rotate.w);
 					OffsetElmt->QueryFloatAttribute("x", &rotate.x);
 					OffsetElmt->QueryFloatAttribute("y", &rotate.y);
 					OffsetElmt->QueryFloatAttribute("z", &rotate.z);
 				}
			}
			if (SceneNode == NULL)
				SceneNode = sceneNode->createChildSceneNode(translate, rotate);
			else
				SceneNode = sceneNode->createChildSceneNode(translate, rotate);
		}
		if(SceneNode == NULL)
			SceneNode = sceneNode;
		SceneNode->attachObject(Entity);
		try
		{
			Ogre::AnimationState *AnimState = Entity->getAnimationState("[auto]");
			AnimState->setEnabled(true);
			mAnimStates.push_back(AnimState);
		}catch(...) {}
	}
}

void DynamicModel::destroy()
{
	SceneObject::destroy();

	std::vector<Ogre::AnimationState*>::iterator AnimStateIter = mAnimStates.begin();
	for(;AnimStateIter!=mAnimStates.end(); AnimStateIter++)
	{
		if ((*AnimStateIter)->getAnimationName() == "[auto]")
			continue;
		Ogre::String AnimName = (*AnimStateIter)->getAnimationName();	// ��Ϊ����Ҫ���٣����������ֵ�����
		sceneNode->getCreator()->destroyAnimation(AnimName);
		sceneNode->getCreator()->destroyAnimationState(AnimName);
	}

	std::vector<Ogre::Entity*>::iterator EntityIter = mEntities.begin();
	for(;EntityIter!=mEntities.end(); EntityIter++)
	{
		sceneManager->destroyEntity(*EntityIter);
	}
}

void DynamicModel::update(float elapsed)
{
	std::vector<Ogre::AnimationState*>::iterator Iter = mAnimStates.begin();
	for(;Iter!=mAnimStates.end(); Iter++)
		(*Iter)->addTime(elapsed);
}

float DynamicModel::getBoundingRadius()
{
	return _getBoundingRadius(sceneNode);
}

void DynamicModel::cloneBone(Ogre::Bone *Bone, 
	std::map<std::string, Ogre::SceneNode*> *BoneMap)
{
	_cloneBone(Bone, sceneNode, BoneMap);
}

void DynamicModel::_cloneBone(Ogre::Bone *Bone, Ogre::SceneNode *SceneNode, 
	std::map<std::string, Ogre::SceneNode*> *BoneMap)
{
	Ogre::SceneNode *NewSceneNode = SceneNode->createChildSceneNode();
	if(!Bone->getName().empty())
		BoneMap->insert(std::make_pair(Bone->getName(), NewSceneNode));
	NewSceneNode->setOrientation(Bone->getOrientation());
	NewSceneNode->setPosition(Bone->getPosition());
	NewSceneNode->setScale(Bone->getScale());
	NewSceneNode->setInitialState();

	Ogre::Node::ChildNodeIterator ChildIter = Bone->getChildIterator();
	while(ChildIter.hasMoreElements())
	{
		_cloneBone((Ogre::Bone*)ChildIter.getNext(), NewSceneNode, BoneMap);
	}
}

Ogre::Real DynamicModel::_getBoundingRadius(Ogre::SceneNode *SceneNode)
{
	Ogre::SceneNode::ChildNodeIterator ChildIter = SceneNode->getChildIterator();
 	float ChildRadius(0);
 	while (ChildIter.hasMoreElements())
 	{
 		Ogre::SceneNode* Child = (Ogre::SceneNode*)ChildIter.getNext();
 		ChildRadius = _getBoundingRadius(Child);
		ChildRadius += Child->getPosition().length();
 	}
 	Ogre::SceneNode::ObjectIterator ObjIter = SceneNode->getAttachedObjectIterator();
	float ObjRadius(0);
 	while(ObjIter.hasMoreElements())
	{
		Ogre::MovableObject* Obj = ObjIter.getNext();
		ObjRadius = Obj->getBoundingRadius() + Obj->getBoundingBox().getCenter().length();
	}
	return ChildRadius > ObjRadius ? ChildRadius : ObjRadius;
}
