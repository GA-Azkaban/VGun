#pragma once
#include <vector>
#include <string>

#include "Singleton.h"



namespace HDData
{
	class AnimationController;
	class GameObject;
}

namespace HDEngine
{
	class AnimationLoader : public Singleton<AnimationLoader>
	{
		friend Singleton;

	private:
		AnimationLoader();

		struct Node
		{
			std::string nodeName;
			std::string motion;
		};

		struct Param
		{
			std::string paramName;
			int type;

			float f_value;
			int i_value;
			bool b_value;
			bool t_value;

			std::string condition;
		};

		struct Link
		{
			std::string start;
			std::string end;
			Param param;

			bool hasEngageMotion = false;
			std::string engageMotion;
		};

	public:
		HDData::AnimationController& LoadAnimationData(std::string fileName);

	private:
		void LoadFromJson(std::string filePath);
		void CreateState(HDData::AnimationController* con);
		void CreateParam(HDData::AnimationController* con);
		void SetLink(HDData::AnimationController* con);
		void SetEntryState(HDData::AnimationController* con);
		std::string CutAnimationName(std::string ani);

	private:
		std::vector<Node> _nodes;
		std::vector<Param> _params;
		std::vector<Link> _links;
		std::string entryNode;
	};
}


