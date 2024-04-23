#include <fstream>
#include <sstream>
#include <string>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>

#include "AnimationLoader.h"
#include "Component.h"
#include "AnimationController.h"

using rapidjson::Document;
using rapidjson::SizeType;
using rapidjson::Value;

const std::string ANIMATIONDATA_PATH = "Resources/AnimationData/";

namespace HDEngine
{

	HDData::AnimationController& HDEngine::AnimationLoader::LoadAnimationData(std::string fileName)
	{
		HDData::AnimationController* aniCon = new HDData::AnimationController();
		LoadFromJson(ANIMATIONDATA_PATH + fileName);
		CreateState(aniCon);
		CreateParam(aniCon);
		SetLink(aniCon);
		SetEntryState(aniCon);

		_nodes.clear();
		_params.clear();
		_links.clear();

		return *aniCon;
	}

	AnimationLoader::AnimationLoader()
	{

	}

	void AnimationLoader::LoadFromJson(std::string filePath)
	{
		// 모든 값을 string으로 묶어서 '한 번' 가져온다
		std::string serializeDataOnce;
		std::ifstream inputFile(filePath);

		if (inputFile.is_open())
		{
			std::stringstream buffer;
			buffer << inputFile.rdbuf();
			serializeDataOnce = buffer.str();
			inputFile.close();
		}

		Document doc;
		doc.Parse(serializeDataOnce.c_str());

		// nodes
		if (doc.HasMember("Nodes") && doc["Nodes"].IsObject())
		{
			const Value& nodes = doc["Nodes"];
			for (Value::ConstMemberIterator itr = nodes.MemberBegin(); itr != nodes.MemberEnd(); ++itr)
			{
				Node* node = new Node;
				const Value& nodeInfo = itr->value;

				if (nodeInfo.HasMember("name") && nodeInfo["name"].IsString())
				{
					node->nodeName = nodeInfo["name"].GetString();
				}
				if (nodeInfo.HasMember("upperFBX") && nodeInfo["upperFBX"].IsString())
				{
					auto p = nodeInfo["upperFBX"].GetString();
					node->upperFBX = CutAnimationName(p);
				}
				if (nodeInfo.HasMember("lowerFBX") && nodeInfo["lowerFBX"].IsString())
				{
					auto p = nodeInfo["lowerFBX"].GetString();
					node->lowerFBX = CutAnimationName(p);
				}

				_nodes.push_back(*node);
			}
		}

		// params
		if (doc.HasMember("Params") && doc["Params"].IsObject())
		{
			const Value& params = doc["Params"];
			for (Value::ConstMemberIterator itr = params.MemberBegin(); itr != params.MemberEnd(); ++itr)
			{
				Param* param = new Param;
				const Value& paramInfo = itr->value;

				if (paramInfo.HasMember("paramName") && paramInfo["paramName"].IsString())
				{
					param->paramName = paramInfo["paramName"].GetString();
				}
				if (paramInfo.HasMember("type") && paramInfo["type"].IsInt())
				{
					int type = paramInfo["type"].GetInt();
					switch (type)
					{
						case 0: { param->type = type; param->f_value = paramInfo["value"].GetFloat(); } break;
						case 1: { param->type = type; param->i_value = paramInfo["value"].GetInt(); } break;
						case 2: { param->type = type; param->b_value = paramInfo["value"].GetBool(); } break;
						case 3: { param->type = type; param->t_value = paramInfo["value"].GetBool(); } break;
						default: {} break;
					}
				}
				_params.push_back(*param);
			}
		}

		// links
		if (doc.HasMember("Links") && doc["Links"].IsObject())
		{
			const Value& links = doc["Links"];
			for (Value::ConstMemberIterator itr = links.MemberBegin(); itr != links.MemberEnd(); ++itr)
			{
				Link* link = new Link;
				const Value& linkInfo = itr->value;

				if (linkInfo.HasMember("start") && linkInfo["start"].IsString())
				{
					link->start = linkInfo["start"].GetString();
				}
				if (linkInfo.HasMember("end") && linkInfo["end"].IsString())
				{
					link->end = linkInfo["end"].GetString();
				}
				if (linkInfo.HasMember("condition") && linkInfo["condition"].IsString())
				{
					link->param.paramName = linkInfo["condition"].GetString();
				}
				if (linkInfo.HasMember("type") && linkInfo["type"].IsInt())
				{
					link->param.type = linkInfo["type"].GetInt();

					switch (link->param.type)
					{
						case 0: { link->param.f_value = linkInfo["value"].GetFloat(); } break;
						case 1: { link->param.i_value = linkInfo["value"].GetInt(); } break;
						case 2:
						{
							std::string b = linkInfo["value"].GetString();
							if (b == "True") 
							{ 
								link->param.b_value = true; 
							}
							else if(b == "False") 
							{ 
								link->param.b_value = false; 
							}
						} break;
						case 3: { link->param.t_value = linkInfo["value"].GetString(); } break;
						default: {} break;
					}
				}

				if (linkInfo.HasMember("transitionAnimation") && linkInfo["transitionAnimation"].GetString() != "")
				{
					link->hasEngageMotion = true;
					link->engageMotion = linkInfo["transitionAnimation"].GetString();
				}

				_links.push_back(*link);
			}
		}

		if (doc.HasMember("EntryNode"))
		{
			const Value& node = doc["EntryNode"];
			entryNode = doc["EntryNode"].GetString();
		}

	}

	void AnimationLoader::CreateState(HDData::AnimationController* con)
	{
		for (auto& node : _nodes)
		{
			con->CreateState(node.nodeName, node.upperFBX, node.lowerFBX);
		}
	}

	void AnimationLoader::CreateParam(HDData::AnimationController* con)
	{
		for (auto& param : _params)
		{
			switch (param.type)
			{
				case 0: { con->CreateFloatParam(param.paramName, param.f_value); } break;
				case 1: { con->CreateIntParam(param.paramName, param.i_value); } break;
				case 2: { con->CreateBoolParam(param.paramName, param.b_value); } break;
				case 3: { con->CreateTriggerParam(param.paramName); } break;
				default: {} break;
			}
		}
	}

	void AnimationLoader::SetLink(HDData::AnimationController* con)
	{
		for (auto& link : _links)
		{
			switch (link.param.type)
			{
				case 0:
				{
					if (link.hasEngageMotion) con->GetState(link.start).MakeTransition(link.end).AddCondition(link.end, link.param.paramName, link.param.condition, link.param.f_value);
					else con->GetState(link.start).MakeTransition(link.end).AddCondition(link.end, link.param.paramName, link.param.condition, link.param.f_value);
					
				}
				break;
				case 1:
				{
					con->GetState(link.start).MakeTransition(link.end).AddCondition(link.end, link.param.paramName, link.param.condition, link.param.i_value);
				}
				break;
				case 2:
				{
					if(link.hasEngageMotion) con->GetState(link.start).MakeTransition(link.end).AddCondition(link.end, link.param.paramName, link.param.b_value, link.engageMotion);
					else con->GetState(link.start).MakeTransition(link.end).AddCondition(link.end, link.param.paramName, link.param.b_value);
				}
				break;
				case 3:
				{
					con->GetState(link.start).MakeTransition(link.end).AddTrigger(link.end, link.param.paramName, link.param.t_value);
				}
				break;
				default:
					break;
			}
		}
	}

	void AnimationLoader::SetEntryState(HDData::AnimationController* con)
	{
		con->SetEntryState(entryNode);
	}

	std::string AnimationLoader::CutAnimationName(std::string ani)
	{
		// 뒤에서 두 번째 언더바의 위치 찾기
		size_t lastSecondUnderscoreIndex = std::string::npos;
		size_t underscoreCount = 0;
		for (size_t i = ani.length() - 1; i != std::string::npos; --i)
		{
			if (ani[i] == '_') 
			{
				underscoreCount++;
				if (underscoreCount == 2) 
				{
					lastSecondUnderscoreIndex = i;
					break;
				}
			}
		}

		size_t fbxIndex = ani.find(".fbx");

		// 뒤에서 두 번째 언더바 이후의 문자열 가져오기
		std::string result = ani.substr(lastSecondUnderscoreIndex + 1, fbxIndex - lastSecondUnderscoreIndex - 1);

		return result;
	}

}

