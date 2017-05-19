#pragma once
#include <unordered_map>
#include <string>

namespace LibLogic
{
	class DataLoader
	{
	public:
		DataLoader() = default;
		~DataLoader() = default;
		void LoadData(const char* fileDir);

	private:
		bool ReadFromFile(const char* fileDir);

	private:
		enum { BUFFER_SIZE = 4096 };
		char _readBuffer[BUFFER_SIZE];
	};
}