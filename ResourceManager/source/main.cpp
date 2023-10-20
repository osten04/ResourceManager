#pragma once

#include <string>
#include <iostream>

#include "ResourceManager/cResourceManager.h"

int main()
{
	RM::byteArray* data = RM::cResourceManager::getInstance().getResource("bakedData/testfolder/stringtest.txt");

	std::string test( (char*)data->data() , data->size() );

	std::cout << test;

	return 0;
}