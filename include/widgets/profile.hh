#pragma once
#include <filesystem>
#include <gtkmm/drawingarea.h>

namespace fs = std::filesystem;

namespace Gtk { class Builder; }


namespace better
{
    /**
     * A widget for displaying user profile pictures.
     */
    class Profile : public Gtk::DrawingArea
    {
    public:
        Profile( const fs::path &p_file_path, size_t p_size_px = 64 );

        Profile( BaseObjectType                   *p_cobject,
                 const Glib::RefPtr<Gtk::Builder> &p_builder,
                 size_t                            p_size_px );


        void change_picture( const fs::path &p_file_path );


    private:
        Glib::RefPtr<Gdk::Pixbuf> m_pixbuf;
        size_t m_size;


        void on_draw( const Cairo::RefPtr<Cairo::Context> &p_cr,
                    const int32_t                       &p_w,
                    const int32_t                       &p_h );
    };
}