#pragma once
#include <gtkmm/drawingarea.h>


/**
 * A widget for displaying user profile pictures.
 */
class Profile : public Gtk::DrawingArea
{
public:
    Profile( const std::string &p_file_path );


private:
    Glib::RefPtr<Gdk::Pixbuf> m_pixbuf;


    void on_draw( const Cairo::RefPtr<Cairo::Context> &p_cr,
                  const int32_t                       &p_w,
                  const int32_t                       &p_h );
};