#include <gdkmm.h>
#include "widgets/profile.hh"


Profile::Profile( const std::string &p_file_path ) :
    m_pixbuf(Gdk::Pixbuf::create_from_file(p_file_path))
{
    this->set_content_height(m_pixbuf->get_height());
    this->set_content_width(m_pixbuf->get_width());

    this->set_draw_func(sigc::mem_fun(*this, &Profile::on_draw));
}


void
Profile::on_draw( const Cairo::RefPtr<Cairo::Context> &p_cr,
                  const int32_t                       &p_w,
                  const int32_t                       &p_h )
{
    double radius { std::min(p_w, p_h) / 2.0F };
    p_cr->arc(p_w / 2.0F, p_h / 2.0F, radius, 0, 2 * M_PI);
    p_cr->clip();

    Gdk::Cairo::set_source_pixbuf(p_cr, m_pixbuf,
                                  (p_w - m_pixbuf->get_width()) / 2.0F,
                                  (p_h - m_pixbuf->get_height()) / 2.0F);
    p_cr->paint();
}