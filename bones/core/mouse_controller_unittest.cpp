﻿#include "env_unittest.h"
#include "mouse_controller.h"
#include "root.h"

namespace bones
{


TEST(MouseControllerUnitTest, CheckConstructor)
{
    Root * root = new Root;
    MouseController mc(root);
    EXPECT_EQ(root, mc.root_);
    root->release();
}

TEST(MouseControllerUnitTest, Shift)
{
    Root * root = new Root;
    View * v1 = new View;
    View * v2 = new View;
    View * v3 = new View;

    View * v11 = new View;
    View * v12 = new View;
    View * v111 = new View;
    View * v31 = new View;

    MouseController mc(root);
    root->attachChildToBack(v1);
    root->attachChildToBack(v2);
    root->attachChildToBack(v3);

    v1->attachChildToBack(v11);
    v1->attachChildToBack(v12);
    v11->attachChildToBack(v111);
    v3->attachChildToBack(v31);
    root->setSize(100, 100);
    v1->setLoc(0, 0);
    v1->setSize(20, 20);
    v2->setLoc(10, 10);
    v2->setSize(20, 20);
    v3->setLoc(40, 40);
    v3->setSize(10, 10);
    v11->setLoc(2, 2);
    v11->setSize(5, 5);
    v12->setLoc(10, 10);
    v12->setSize(5, 5);
    v111->setLoc(1, 1);
    v111->setSize(3, 3);
    v31->setLoc(5, 5);
    v31->setSize(5, 5);

    EXPECT_EQ(nullptr, mc.over());
    MouseEvent move(kET_MOUSE_MOVE, kMB_NONE, root,
        Point::Make(80, 80), Point::Make(80, 80), 0);
    EXPECT_EQ(nullptr, mc.over());
    MouseEvent move1(kET_MOUSE_MOVE, kMB_NONE, root, 
        Point::Make(29, 20), Point::Make(29, 20), 0);
    mc.handleEvent(move1);
    EXPECT_EQ(v2, mc.over());
    MouseEvent move2(kET_MOUSE_MOVE, kMB_NONE, root,
        Point::Make(45, 45), Point::Make(45, 45), 0);
    mc.handleEvent(move2);
    EXPECT_EQ(v31, mc.over());

    EXPECT_EQ(nullptr, mc.capture());
    MouseEvent down1(kET_MOUSE_DOWN, kMB_LEFT, root,
        Point::Make(45, 45), Point::Make(45, 45), 0);
    mc.handleEvent(down1);
    EXPECT_EQ(v31, mc.capture());
    mc.handleEvent(move1);
    EXPECT_EQ(v31, mc.capture());
    EXPECT_EQ(v31, mc.over());
    MouseEvent up1(kET_MOUSE_UP, kMB_LEFT, root,
        Point::Make(29, 20), Point::Make(29, 20), 0);
    mc.handleEvent(up1);
    EXPECT_EQ(nullptr, mc.capture());
    EXPECT_EQ(v2, mc.over());

    //处理意外情况 没有up直接leave
    mc.handleEvent(down1);
    EXPECT_EQ(v31, mc.capture());
    mc.handleEvent(move1);
    EXPECT_EQ(v31, mc.capture());
    MouseEvent leave1(kET_MOUSE_LEAVE, kMB_NONE, root,
        Point::Make(45, 45), Point::Make(45, 45), 0);
    mc.handleEvent(leave1);
    EXPECT_EQ(nullptr, mc.capture());
    EXPECT_EQ(nullptr, mc.over());
    //当前capture 没有mouseable属性
    mc.handleEvent(down1);
    EXPECT_EQ(v31, mc.capture());
    v31->setMouseable(false);
    mc.handleEvent(move1);
    EXPECT_EQ(nullptr, mc.capture());
    EXPECT_EQ(v2, mc.over());
    v31->setMouseable(true);
    //当前capture 被remove
    mc.handleEvent(down1);
    EXPECT_EQ(v31, mc.capture());
    v31->detachFromParent();
    mc.handleEvent(move1);
    EXPECT_EQ(nullptr, mc.capture());
    EXPECT_EQ(v2, mc.over());

    root->recursiveDetachChildren();
    v31->release();

    v11->release();
    v12->release();
    v111->release();

    v1->release();
    v2->release();
    v3->release();

    root->release();
}

}