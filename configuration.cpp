#include "configuration.hpp"

Configuration::Configuration() {}
Configuration& Configuration::getInstance() {
	static Configuration instance;
	return instance;
}