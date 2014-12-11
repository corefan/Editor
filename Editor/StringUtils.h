#pragma once

#include <string>

class StringUtils
{
public:
	static int compare(std::string l, std::string r);

	/** �ݲ��ṩ��CStringΪ�βε����
	*/

	static std::string utf8ToGbk(std::string src);
	static std::string gbkToUtf8(std::string src);

	static std::string externalPath(std::string prefix, std::string path);
	static std::string internalPath(std::string Prefix, std::string Path);

	static std::string filename(std::string path);
	static std::string extension(std::string path);
};
