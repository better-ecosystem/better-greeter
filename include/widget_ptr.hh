#pragma once
#include <utility>
#include <gtkmm/widget.h>

template<typename T>
concept WidgetType = std::derived_from<T, Gtk::Widget>;


template<WidgetType T_Widget>
class WidgetPtr
{
public:
    template<typename... T_CtorParams>
    WidgetPtr( T_CtorParams &&...p_params ) :
        m_widget(new T_Widget(std::forward<T_CtorParams>(p_params)...))
    {}

    WidgetPtr( WidgetPtr &&p_other ) noexcept :
        m_widget(p_other.m_widget)
    { m_widget = nullptr; }


    ~WidgetPtr( void )
    { delete m_widget; }

    WidgetPtr( const WidgetPtr & ) = delete;
    auto operator=( const WidgetPtr & ) -> WidgetPtr & = delete;


    auto
    operator=( WidgetPtr &&p_other ) noexcept -> WidgetPtr &
    {
        if (this == &p_other) return *this;
        if (m_widget != nullptr) {
            if (auto *parent = m_widget->get_parent())
                parent->remove(*m_widget);
            delete m_widget;
        }
        m_widget = p_other.m_widget;
        p_other.m_widget = nullptr;
    }


    [[nodiscard]]
    auto
    get( void ) -> T_Widget *
    { return m_widget; }


    [[nodiscard]]
    auto
    operator->() -> T_Widget *
    { return m_widget; }


    [[nodiscard]]
    auto
    operator*() -> T_Widget &
    { return *m_widget; }


private:
    T_Widget *m_widget;
};