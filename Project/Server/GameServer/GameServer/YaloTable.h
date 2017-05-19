#pragma once
#include <unordered_map>
#include <string>

namespace LibLogic
{
	class YaloTable
	{
		friend class DataLoader;
		friend class YaloManager;

	public:
		static YaloTable* GetInstance()
		{
			if (instance == nullptr)
			{
				instance = new YaloTable();
			}

			return instance;
		}

		int GetYaloCost(const std::string& unitName)
		{
			if (yaloCosts.find(unitName) == yaloCosts.end())
			{
				return 0;
			}

			return yaloCosts[unitName];
		}

	private:
		YaloTable() = default;
		~YaloTable() = default;
		auto& GetYaloCosts() { return yaloCosts; }

	private:
		static YaloTable* instance;
		std::unordered_map<std::string, int> yaloCosts;
		const int maxYaloNumber = 10;
		const int startYaloNumber = 4;
		const float yaloChargeTime = 2.0f;
	};
}
