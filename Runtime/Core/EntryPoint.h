#pragma once

#include <iostream>

//extern MarsEngine::Application* MarsEngine::createApplication();

int main(int argc, char** argv)
{
	//MarsEngine::Log::init();

#ifdef WIN32
	std::cout << "Main() success!\n";
#endif

	return 0;
}
