#include "ConfigMgr.h"

ConfigMgr::ConfigMgr() {
	boost::filesystem::path current_path = boost::filesystem::current_path();
	boost::filesystem::path config_path = current_path / "config.ini";

	boost::property_tree::ptree pt;
	boost::property_tree::read_ini(config_path.string(), pt);

	for (const auto section_pair : pt) {
		std::string config_name = section_pair.first;
		boost::property_tree::ptree section_tree = section_pair.second;
		std::map<std::string, std::string> section_config;
		for (auto key_value_tree : section_tree) {
			std::string key = key_value_tree.first;
			std::string value = key_value_tree.second.get_value<std::string>();
			section_config[key] = value;
		}
		SectionInfo sectionInfo;
		sectionInfo._section_datas = section_config;
		_config_map[config_name] = sectionInfo;

	}
}

ConfigMgr::~ConfigMgr() {
	_config_map.clear();
}

ConfigMgr::ConfigMgr(const ConfigMgr& other) {
	_config_map = other._config_map;
}

ConfigMgr& ConfigMgr::operator=(const ConfigMgr& other) {
	if (&other == this) {
		return *this;
	}
	_config_map = other._config_map;
	return *this;
}

SectionInfo ConfigMgr::operator[](const std::string& key) {
	if (_config_map.find(key) == _config_map.end()) {
		return SectionInfo();
	}
	return _config_map[key];
}

ConfigMgr& ConfigMgr::GetInstance() {
	static ConfigMgr cfg_mgr;
	return cfg_mgr;
}
