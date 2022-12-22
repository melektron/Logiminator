/*
ELEKTRON © 2022
Written by melektron
www.elektron.work
19.12.22, 23:25

impl for hello world window
*/

#include <iostream>
#include "button_examples.h"

ButtonExamples::ButtonExamples()
    : m_container(Gtk::ORIENTATION_VERTICAL),
      m_infolabel("Info"),
      m_getinfo_button("Update Info"),
      m_toggle("toggle button"),
      m_check("check button"),
      m_rb1(m_rbgroup, "radio button 1"),
      m_rb2(m_rbgroup, "radio button 2"),
      m_rb3(m_rbgroup, "radio button 3")
{
    set_title("Button Types");

    set_border_width(10);

    add(m_container);
    m_container.pack_start(m_infolabel);
    m_container.pack_start(m_getinfo_button);
    m_container.pack_start(m_toggle);
    m_container.pack_start(m_check);
    m_container.pack_start(m_rb1);
    m_container.pack_start(m_rb2);
    m_container.pack_start(m_rb3);

    // attach signal handler to the "clicked" signal
    // of the button
    m_getinfo_button.signal_clicked().connect(sigc::mem_fun(*this, &ButtonExamples::on_button_change));
    m_toggle.signal_clicked().connect(sigc::mem_fun(*this, &ButtonExamples::on_button_change));
    m_check.signal_clicked().connect(sigc::mem_fun(*this, &ButtonExamples::on_button_change));
    m_rb1.signal_clicked().connect(sigc::mem_fun(*this, &ButtonExamples::on_button_change));
    m_rb2.signal_clicked().connect(sigc::mem_fun(*this, &ButtonExamples::on_button_change));
    m_rb3.signal_clicked().connect(sigc::mem_fun(*this, &ButtonExamples::on_button_change));


    // display the button
    show_all_children();
}

ButtonExamples::~ButtonExamples()
{
}

void ButtonExamples::on_button_change()
{
    int btnnr = 0;
    if (m_rb1.get_active()) btnnr = 1;
    else if (m_rb2.get_active()) btnnr = 2;
    else if (m_rb3.get_active()) btnnr = 3;
    m_infolabel.set_label(
        "Togglebutton: " + std::to_string(m_toggle.get_active())
        + "\nCheckbutton: " + std::to_string(m_check.get_active())
        + "\nRadiobuttons: " + std::to_string(btnnr)
    );
}