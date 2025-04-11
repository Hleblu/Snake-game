#include "configuration.h"
Configuration* Configuration::instance = nullptr;
Configuration::Configuration() {}
Configuration* Configuration::getInstance() {
	if (instance == nullptr) {
		instance = new Configuration();
	}
	return instance;
}