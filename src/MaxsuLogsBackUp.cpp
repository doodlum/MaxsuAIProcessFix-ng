#include "MaxsuLogsBackUp.h"


namespace MaxsuLogsBackUpNamespace
{

	bool CreateBackUpLogs(const std::string log_path, unsigned int logs_num)
	{
		if (!std::filesystem::exists(log_path)) //判断路劲下的log文件是否存在
			return false;

		//-------------循环生成并覆盖现有的log文件备份----------------------------

		for (unsigned int i = logs_num - 1; i >= 0; i--)
		{
			auto const thisbackfile = GetBackUpLogNameByIndex(log_path, i);

			auto const lastbackfile = GetBackUpLogNameByIndex(log_path, i - 1);

			if (i == 0)
			{
				if (std::filesystem::exists(log_path))
					std::filesystem::rename(log_path, thisbackfile);

				return true;
			}


			if (std::filesystem::exists(lastbackfile))
				std::filesystem::rename(lastbackfile, thisbackfile);

		}

		return false;

		//--------------------------------------------------------------

	}



}