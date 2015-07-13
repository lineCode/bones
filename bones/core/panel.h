﻿#ifndef BONES_CORE_PANEL_H_
#define BONES_CORE_PANEL_H_

#include "widget.h"
#include "root_view.h"


namespace bones
{
class Size;

class Panel : public Widget,
              public RootView::Delegate
{
public:
    enum NCArea
    {
        kCaption = 0,
        kBottom,
        kBottomLeft,
        kBottomRight,
        kLeft,
        kRight,
        kTop,
        kTopLeft,
        kTopRight,

        kNCAreaCount,
    };
public:
    static bool Initialize();

    static bool Uninitialize();
public:
    Panel();

    bool create(const Panel * parent, const wchar_t * window_name);

    bool destroy();

    void setNCArea(NCArea area, const Rect & rect);

    void setCursor(Cursor cursor);

    const char * getClassName() const override;

    RootView * getRootView() const;
protected:
    virtual void onGeometryChanged(WINDOWPOS & pos);

    virtual void onGeometryChanging(WINDOWPOS & pos);

    virtual void onPaint(HDC hdc, const Rect & rect);

    virtual void invalidateRect(const Rect & rect) override;

    virtual void changeCursor(Cursor cursor) override;
private:
    LRESULT processEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT defProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT handleNCCreate(UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT handleNCCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT handleNCHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT handlePositionChanges(UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT handleFocus(UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT handlePaint(UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT handleMouse(UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT handleNCDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT handleSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
    RefPtr<RootView> root_;
    Rect nc_area_[kNCAreaCount];
    bool track_mouse_;
    Cursor cursor_;
private:
    friend LRESULT CALLBACK PanelProc(HWND hWnd,
                                         UINT uMsg,
                                         WPARAM wParam,
                                         LPARAM lParam);
};

}

#endif