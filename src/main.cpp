/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
19.12.22, 13:10
*/

#include <stdio.h>
#include <gtkmm/application.h>
#include "windows/main_window.h"


int main(int argc, char *argv[])
{
    auto app = Gtk::Application::create(argc, argv, "work.elektron.gtk_testapp");


    MainWindow main_window;


    int ret = app->run(main_window);
    if (ret == 0)
        printf("OK - exiting ...\n");
    else
        printf("ERROR: Application return code %d - exiting ...\n", ret);
    return ret;
}