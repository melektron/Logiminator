#include <gtkmm.h>

class EntryExample : public Gtk::Window
{
public:
  EntryExample();
  virtual ~EntryExample();

protected:
  //Signal handlers:
  void on_button_close();

  void on_completion_action_activated(int index);

  //See the comment in the implementation:
  //bool on_completion_match(const Glib::ustring& key, const Gtk::TreeModel::const_iterator& iter);


  //Tree model columns, for the EntryCompletion's filter model:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord
  {
  public:

    ModelColumns()
    { add(m_col_id); add(m_col_name); }

    Gtk::TreeModelColumn<unsigned int> m_col_id;
    Gtk::TreeModelColumn<Glib::ustring> m_col_name;
  };

  ModelColumns m_Columns;

  typedef std::map<int, Glib::ustring> type_actions_map;
  type_actions_map m_CompletionActions;

  //Child widgets:
  Gtk::Box m_HBox;
  Gtk::Box m_VBox;
  Gtk::Entry m_Entry;
  Gtk::Label m_Label;
  Gtk::Button m_Button_Close;
};