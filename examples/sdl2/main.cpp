#include "SDL.h"
#include "DemoApplication.h"

int main(int, [[maybe_unused]] char* argv[]) {
	DemoApplication application{};

	application.initialise(1024, 768);
	application.execute();
	application.shutdown();

	return 0;
}