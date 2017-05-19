#include "DataLoader.h"
#include "UnitTemplate.h"
#include "YaloTable.h"
#include "Logger.h"
#include "json/json.h"

namespace LibLogic
{
	void DataLoader::LoadData(const char* fileDir)
	{
		if (!ReadFromFile(fileDir))
		{
			LibLogger::cWarning << "UnitSpect.json load fail" << std::endl;
			LibLogger::cWarning << "File is in common folder" << std::endl;
			return;
		}

		std::string doc = _readBuffer;
		Json::Value root;
		Json::Reader reader;

		if (!reader.parse(doc, root))
		{
			LibLogger::cWarning << "Json parse fail" << std::endl;
			return;
		}

		Json::Value jArray = root["UnitSpecs"];
		auto& unitSpecs = UnitTemplate::GetInstance()->GetUnitSpecs();
		auto& yaloTable = YaloTable::GetInstance()->GetYaloCosts();
		for (int i = 0; i < jArray.size(); i++)
		{
			Json::Value jEle = jArray[i];
			UnitSpec us;
			us.attackDelay	= jEle["attackDelay"].asFloat();
			us.attackFreq	= jEle["attackFreq"].asFloat();
			us.attackPower	= jEle["attackPower"].asInt();
			us.attackRange	= jEle["attackRange"].asFloat();
			us.hp			= jEle["hp"].asInt();
			us.isMagic		= jEle["isMagic"].asBool();
			us.moveSpeed	= jEle["moveSpeed"].asFloat();
			us.sightRange	= jEle["sightRange"].asFloat();
			us.summonTime	= jEle["summonTime"].asFloat();
			us.unitName		= jEle["unitName"].asString();
			us.yaloCost		= jEle["yaloCost"].asInt();
			unitSpecs.emplace(us.unitName, us);
			yaloTable.emplace(us.unitName, us.yaloCost);
		}

		UnitTemplate::GetInstance()->SetCtorAndState();

		LibLogger::cInfo << "Load data success" << std::endl;
	}

	bool DataLoader::ReadFromFile(const char* fileDir)
	{
		FILE* fp = nullptr;
		fopen_s(&fp, fileDir, "rb");
		if (fp == nullptr) return false;

		size_t fileSize = fread(_readBuffer, 1, BUFFER_SIZE, fp);

		fclose(fp);

		return true;
	}
}