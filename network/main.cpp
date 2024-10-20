#include <iostream>
#include "reader/reader.hpp"
#include "manager.hpp"

using namespace std;

int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);
    manager window_manager;

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