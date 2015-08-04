﻿#ifndef BONES_CORE_IMAGE_H_
#define BONES_CORE_IMAGE_H_

#include "view.h"
#include "color.h"
#include "pixmap.h"
#include "rect.h"

class SkShader;

namespace bones
{

class Image : public Skin
{
private:
    enum Style
    {
        kImage,
        kImageNine,
    };
public:
    Image();

    ~Image();

    void set(Pixmap & pm);//直接绘制 不拉伸

    void set(Pixmap & pm, Rect & nine_center);//九宫格拉伸

    const char * getClassName() const override;
protected:
    void onDraw(SkCanvas & canvas) override;
private:
    void drawImage(SkCanvas & canvas, const Rect & bounds);

    void drawImageNine(SkCanvas & canvas, const Rect & bounds);
private:
    Style style_;
    Pixmap pixmap_;
    Rect nine_center_;
};

}


#endif