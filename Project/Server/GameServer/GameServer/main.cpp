#include "App.h"

int main(void)
{
	auto app = new LibLogic::App();
	app->Run();
	delete app;

	return 0;
}