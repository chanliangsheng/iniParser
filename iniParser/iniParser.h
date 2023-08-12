#pragma once
#include <string>
#include <variant>
#include <unordered_map>
#include <fstream>
#include <iostream>
#include <sstream>
namespace iniParser{
	struct StringVisitor
	{
		std::string operator()(int value) {
			return std::to_string(value);
		};

		std::string operator()(double value) {
			return std::to_string(value);
		};

		std::string operator()(std::string value) {
			std::string res;
			res += '"';
			res += move(value);
			res += '"';
			return res;
		};
	};

	typedef std::variant<int, double, std::string> value;
	typedef std::unordered_map<std::string, value> key_value;
	
	class section
	{
	public:
		value GetValue(std::string& key);
		// 重载a["test"] = 1这样的使用方法
		value& operator[](const std::string& key);
		std::string str();
	public:
		section();
	private:
		key_value m_key_value;
	};

	typedef std::unordered_map<std::string, section> obj;
	class ini {
	public:
		ini();
		section& operator[](const std::string& key);
		std::string str();
	private:
		obj m_obj;
	};

	ini Parseini(const std::string& path);
	// 修剪字符串的左右空格
	std::string_view trim(std::string_view str);
	// 把字符串转换成value
	value ParseStringToValue(std::string_view str);
}

