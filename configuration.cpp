#include "configuration.h"
Configuration::Configuration() {}
Configuration& Configuration::getInstance() {
	static Configuration instance;
	return instance;
}