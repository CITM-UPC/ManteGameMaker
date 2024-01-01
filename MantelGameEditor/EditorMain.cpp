#include <iostream>
#include "EditorApp.h"

EditorApp* editor = NULL;

int main(int argc, char* args[]) {

	editor = new EditorApp();

	// start
	bool toContinue = true;
	toContinue = editor->Start();
	if (toContinue) {

		while (toContinue) {
			toContinue = editor->Update();
		}

		editor->Cleanup();

		return 1;
	}
	else {
		std::cout << "ERROR" << std::endl;
		return -1;
	}

	return 0;
}