#include <iostream>

// #include "manager.hpp"
#include "project_window/project_main_window.hpp"

using namespace std;

int main(int argc, char *argv[])
{
  // nd_manager project;
  // project.create_kernel_threads (parser.total_threads ());

  project_manager pm;

  QApplication app(argc, argv);

  Project_Window w(&pm);
  w.show();
  // manager window_manager;
  // project.set_manager (&window_manager);

  // window_manager.make_window();

  app.exec();
  return 0;
}
