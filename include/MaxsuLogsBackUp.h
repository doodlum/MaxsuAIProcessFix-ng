#pragma once

namespace MaxsuLogsBackUpNamespace
{

	bool CreateBackUpLogs(const std::string log_path, unsigned int logs_num);



	static inline const std::string GetBackUpLogNameByIndex(std::string log_path, unsigned int log_Index)
	{
		auto backfile = log_path;

		backfile += std::to_string(log_Index);

		return backfile;

	}


	static inline const std::string GetLogAbsolutetPath(const std::filesystem::path a_fileName)
	{
		wchar_t* pathBuffer;

		auto result = SHGetKnownFolderPath(FOLDERID_Documents, KNOWN_FOLDER_FLAG::KF_FLAG_DEFAULT, nullptr, &pathBuffer);

		if (result != S_OK)
		{
			return 0;
		}

		std::unique_ptr<wchar_t[], decltype(&CoTaskMemFree)> path(pathBuffer, CoTaskMemFree);

		auto fileName = a_fileName.native();

		fileName.insert(0, path.get());

		std::filesystem::path filePath(std::move(fileName));

		return filePath.string();
	}









}