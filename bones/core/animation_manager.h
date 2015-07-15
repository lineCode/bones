﻿#ifndef BONES_CORE_ANIMATION_MANAGER_H_
#define BONES_CORE_ANIMATION_MANAGER_H_


#include "ref.h"
#include <map>
#include <list>

namespace bones
{
class Animation;

class AnimationManager
{
public:
    static AnimationManager * Get();

    void add(Ref * ref, Animation * ani);

    void remove(Ref * ref, Animation * ani);

    void remove(Ref * ref);

    void removeAll();

    void run(uint64_t delta);
protected:
    AnimationManager();
private:
    std::map<RefPtr<Ref>, std::list<RefPtr<Animation>>> animations_;
};

}

#endif