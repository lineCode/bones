﻿
#include "mouse_controller.h"
#include "root_view.h"
#include "event_dispatcher.h"
#include "view.h"
#include "rect.h"



namespace bones
{

MouseController::MouseController(RootView * root)
:root_(root)
{
    ;
}


void MouseController::removed(View * n)
{
    if (n->contains(capture_.get()))
        shiftCapture(nullptr);

    if (n->contains(over_.get()))
        shiftOver(nullptr);
}

void MouseController::shiftIfNecessary()
{
    if (capture_ && !capture_->isVisible())
        shiftCapture(nullptr);

    auto target = getTargetByPos(last_mouse_point_);
    if ((target != over_.get()))
    {
        shiftOver(target);
        if (!target)
            return;
        MouseEvent e(kET_MOUSE_MOVE, kMB_NONE, target, target->mapToLocal(last_mouse_point_), last_mouse_point_, 0);
        pushEvent(e);
    }
}

void MouseController::handleEvent(MouseEvent & e)
{
    if (e.type() != kET_MOUSE_WHEEL && 
        ( e.type() < kET_MOUSE_ENTER || e.type() > kET_MOUSE_LEAVE) )
        return;

    if (kET_MOUSE_LEAVE == e.type())
    {
        shiftCapture(nullptr);
        shiftOver(nullptr);
        last_mouse_point_.set(-1, -1);
        return;
    }
    else if (kET_MOUSE_ENTER == e.type())
    {
        shiftOver(root_);
        last_mouse_point_.set(0, 0);
        return;
    }

    View * target = e.target();
    if (target == root_)
        target = getTargetByPos(e.loc());
    if (!target)
        return;
    last_mouse_point_ = e.loc();

    MouseEvent me(e.type(), e.button(), target, target->mapToLocal(e.loc()), e.loc(), e.flags());
    if (kET_MOUSE_DOWN == me.type() && me.isLeftMouse())
    {//如果是左键按下 自动capture
        shiftCapture(me.target());
    }
    shiftOver(target);
    pushEvent(me);
    if (kET_MOUSE_UP == me.type() && me.isLeftMouse())
    {//左键弹起 取消capture
        shiftCapture(nullptr);
        //由于capture的原因 弹起时鼠标已经进入其它控件 其它控件需要有个明确的mouse move消息
        shiftIfNecessary();
    }
}

void MouseController::shiftOver(View * n)
{
    RefPtr<View> target;
    target.reset(n);
    if (over_ != target)
    {
        EventDispatcher dispatcher;
        EventDispatcher::Path old_path;
        EventDispatcher::getPath(over_.get(), old_path);
        EventDispatcher::Path new_path;
        EventDispatcher::getPath(n, new_path);
        if (over_)
        {
            Point empty;
            MouseEvent me(kET_MOUSE_LEAVE, kMB_NONE, over_.get(), empty, empty, 0);
            dispatcher.run(me, old_path);
        }
        over_ = target;
        if (over_)
        {
            Point empty;
            MouseEvent me(kET_MOUSE_ENTER, kMB_NONE, over_.get(), empty, empty, 0);
            dispatcher.run(me, new_path);
        }
    }
}

View * MouseController::getTargetByPos(const Point & pt)
{
    View * target = capture_.get();
    if (!target)
        target = root_->hitTest(pt);

    return target;
}

void MouseController::shiftCapture(View * n)
{
    capture_.reset(n);
}

View * MouseController::capture() const
{
    return capture_.get();
}

View * MouseController::over() const
{
    return over_.get();
}

void MouseController::pushEvent(MouseEvent & e)
{
    EventDispatcher dispatcher;
    EventDispatcher::Path path;
    EventDispatcher::getPath(e.target(), path);
    dispatcher.run(e, path);
}

}