#include <gdkmm.h>
#include "widgets/profile.hh"

using better::Profile;


Profile::Profile( const fs::path &p_file_path, size_t p_size_px ) :
    m_size(p_size_px)
{
    if (!p_file_path.empty())
        m_pixbuf = Gdk::Pixbuf::create_from_file(p_file_path.string());

    this->set_content_height(p_size_px);
    this->set_content_width(p_size_px);

    this->set_draw_func(sigc::mem_fun(*this, &Profile::on_draw));
}


Profile::Profile( BaseObjectType                   *p_cobject,
                  const Glib::RefPtr<Gtk::Builder> &,
                  size_t                            p_size_px ) :
    Gtk::DrawingArea(p_cobject),
    m_size(p_size_px)
{}


void
Profile::change_picture( const fs::path &p_file_path )
{
    m_pixbuf = Gdk::Pixbuf::create_from_file(p_file_path.string());

    this->set_content_height(m_pixbuf->get_height());
    this->set_content_width(m_pixbuf->get_width());
}


void
Profile::on_draw( const Cairo::RefPtr<Cairo::Context> &p_cr,
                  const int32_t                       &p_w,
                  const int32_t                       &p_h )
{
    if (!m_pixbuf) return;

    auto scaled = m_pixbuf->scale_simple(m_size, m_size,
                                         Gdk::InterpType::BILINEAR);

    double radius { std::min(p_w, p_h) / 2.0F };
    p_cr->arc(p_w / 2.0F, p_h / 2.0F, radius, 0, 2 * M_PI);
    p_cr->clip();

    Gdk::Cairo::set_source_pixbuf(p_cr, scaled, 0, 0);
    p_cr->paint();
}