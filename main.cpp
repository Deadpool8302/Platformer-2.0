#include<chrono>
#include<SFML/Graphics.hpp>
#include"data/src/App.hpp"

int main() {
	App app;
	while (app.isRunning())
	{
		app.update();
		app.render();
	}
}


