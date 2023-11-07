#include "Globals.h"
#include "App.h"

App* app = nullptr;

int main(int argc, char* args[])
{
    cout << "Starting " << TITLE << endl;

    try {
        app = new App(argc, args);

        if (app == NULL) { throw exception("app = NULL"); }

        // Awake
        cout << "--Awake of app--" << endl;
        app->Awake();

        // Start
        cout << "--Start of App--" << endl;
        app->Start();

        // Update
        cout << "--Update of App--" << endl;
        while (app->DoUpdate()) { }

        // CleanUp
        cout << "--CleanUp of app--" << endl;
        app->CleanUp();


        RELEASE(app);
        cout << "Exiting with NO errors :)" << endl;
        return EXIT_SUCCESS;
    }
    catch (const exception& ex) {
        cout << "Exiting with errors :(  : " << endl;
        cerr << ex.what() << endl;
        return EXIT_FAILURE;
    }
}