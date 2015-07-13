﻿
#include "ref.h"
#include "logging.h"

namespace bones
{

Ref::Ref() :count_(1)
{    
    LOG_VERBOSE << "create:" << this->getClassName() << this;
}

Ref::~Ref()
{
    assert(count_ == 0);
}

void Ref::retain()
{
    assert(count_ > 0);
    ++count_;
}

void Ref::release()
{
    assert(count_ > 0);
    --count_;
    if (0 == count_)
    {
        LOG_VERBOSE << "destroy:" << this->getClassName() << this;
        delete this;
    }
        

}

int Ref::getRefCount() const
{
    return count_;
}

const char * Ref::getClassName() const
{
    return kClassRef;
}

NoncopyableObject::NoncopyableObject()
{

}

}