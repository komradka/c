#include <iostream>

#include "argsparser.h"

#include "nd_manager.h"
#include "manager.hpp"

using namespace std;

int main(int argc, char *argv[]) 
{
    argsparser parser (argc, argv);

    error err = parser.parse ();
    if (!err.is_ok ())
      {
        printf ("%s\n", err.description_cstr ());
        return -1;
      }

    nd_manager project;
    project.create_kernel_threads (parser.total_threads ());

    QApplication app(argc, argv);
    manager window_manager;
    project.set_manager (&window_manager);

    // if (argc != 2)
    // {
    //     cout << "Usage: " << argv[0] << " input_param" << endl;
    //     app.exec();
    //     return 0;
    // }
    
    // char *file = argv[1];

    window_manager.make_window();

    app.exec();
    return 0;
}
