#include "iniParser.h"

using namespace std;
using namespace iniParser;

value section::GetValue(std::string& key)
{
	auto find_itr = this->m_key_value.find(key);
	if (find_itr == m_key_value.end()) {
		return string("");
	}
	else
	{
		return m_key_value[key];
	}
}

value& section::operator[](const std::string& key)
{
	// 返回一个引用，然后自动使用了variant的'='方法，自动多态，将对应的值放在对应的类型上
	return this->m_key_value[key];
}

std::string section::str()
{
	string res;
	for (auto itr = m_key_value.begin(); itr != m_key_value.end(); itr++) {
		if (itr != m_key_value.begin()) {
			res += ',';
		}
		res += '"';
		res += itr->first;
		res += '"';
		res += ":";
		res += visit(StringVisitor(), itr->second);
	}
	return move(res);
}

section::section()
{
	// 空的key_value
	this->m_key_value = key_value();
}

ini::ini()
{
	this->m_obj = obj();
}

section& iniParser::ini::operator[](const std::string& key)
{
	return m_obj[key];
}

std::string iniParser::ini::str()
{
	string res;
	for (auto itr = m_obj.begin(); itr != m_obj.end(); itr++) {
		res += "section name:";
		res += itr->first;
		res += '\n';
		res += "key-value:";
		res += '\n';
		res += itr->second.str();
		res += '\n';
		res += '\n';
	}
	return res;
}


ini iniParser::Parseini(const std::string& path)
{
	// 打开文件
	fstream file(path);

	// 如果成功打开了此文件
	if (file.is_open()) {
		string line;
		string temp;
		ini res;
		// 将file的每一行读取到line中国
		while (getline(file, line))
		{
			string_view str(line);
			// 如果是'['开头，说明是个section
			if (line[0] == '[') {
				size_t end = str.find_last_of(']');
				str = trim(str.substr(1, end - 1));
				temp = string(str);
				section sec;
				// 加入到键值对中
				res[temp] = sec;
			}
			// 如果不是，说明是section内的键值对
			else {
				size_t key_end = str.find_first_of('=');
				// 如果这行没有'='，说明不是键值对
				if (key_end == str.npos) {
					continue;
				}
				// 解析key和value
				string key = string(trim(str.substr(0, key_end - 1)));
				value val = ParseStringToValue(trim(str.substr(key_end + 1)));
				// 加入res中
				res[temp][key] = val;
			}
		}
		return move(res);
	}
	// 如果无法打开此文件
	else {
		cout << "无法打开此文件" << endl;
	}

	return ini();
}

std::string_view iniParser::trim(std::string_view str)
{

	size_t begin = str.find_first_not_of(" \"");
	size_t end = str.find_last_not_of(" \"");

	return move(str.substr(begin, end - begin + 1));
}

value iniParser::ParseStringToValue(std::string_view str)
{
	int int_value;
	double double_value;
	istringstream iss(str.data());

	// 通过三种可能返回不同的类型
	if (iss >> int_value && iss.eof()) {
		return int_value;
	}
	else if (iss >> double_value && iss.eof()) {
		return double_value;
	}
	else {
		return string(str);
	}
}

