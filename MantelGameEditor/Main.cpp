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
        cout << "AWAKE PHASE ===============================" << endl;
        app->Awake();

        // Start
        cout << "START PHASE ===============================" << endl;
        app->Start();

        // Update
        cout << "UPDATE PHASE ===============================" << endl;
        while (app->DoUpdate()) { }

        // CleanUp
        cout << "CLEANUP PHASE ===============================" << endl;
        app->CleanUp();


        RELEASE(app);
        cout << "Exiting without errors:  SUCCESS" << endl;
        return EXIT_SUCCESS;
    }
    catch (const exception& ex) {
        cout << "Exiting with errors: FAIL" << endl;
        cerr << ex.what() << endl;
        return EXIT_FAILURE;
    }
}