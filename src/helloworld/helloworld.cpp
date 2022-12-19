/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
19.12.22, 23:25

impl for hello world window
*/

#include <iostream>
#include "helloworld.h"


HelloWorld::HelloWorld()
: m_hibutton("Hello, World!")
{
    set_border_width(10);

    // attach signal handler to the "clicked" signal 
    // of the button
    m_hibutton.signal_clicked().connect(sigc::mem_fun(*this, &HelloWorld::on_button_clicked));

    // place the button on the window
    add(m_hibutton);

    // display the button
    m_hibutton.show();
}

HelloWorld::~HelloWorld()
{
}

void HelloWorld::on_button_clicked()
{
    std::cout << "Hello, World! Clicked!" << std::endl;
}