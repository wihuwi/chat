#pragma once

#include "const.h"

struct SectionInfo {
	SectionInfo() = default;
	~SectionInfo() { _section_datas.clear(); }
	SectionInfo(const SectionInfo& other) {
		this->_section_datas = other._section_datas;
	}
	SectionInfo& operator=(const SectionInfo& other) {
		if (&other == this) {
			return *this;
		}
		this->_section_datas = other._section_datas;
	}
	std::string operator[](const std::string& key) {
		if (_section_datas.find(key) == _section_datas.end()) {
			return "";
		}
		return _section_datas[key];
	}

	std::map<std::string, std::string> _section_datas;
};

class ConfigMgr
{
public:
	ConfigMgr();
	~ConfigMgr();
	ConfigMgr(const ConfigMgr& other);
	ConfigMgr& operator=(const ConfigMgr& other);
	SectionInfo operator[](const std::string& key);
private:
	std::map<std::string, SectionInfo> _config_map;
};

