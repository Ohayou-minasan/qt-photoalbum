#ifndef CONST_H
#define CONST_H

#endif // CONST_H


enum TreeItemType{
    TreeItemPro = 1, //表示项目的条目
    TreeItemDir = 2, //表示项目的文件夹
    TreeItemPic = 3, //表示项目的图片
};

enum PicBtnState{
    //播放图标的三种状态
    PicBtnStateNormal = 1,
    PicBtnStateHover = 2,
    PicBtnStatePressed = 3,

    //停止图标的三种状态
    PicBtnState2Normal = 4,
    PicBtnState2Hover = 5,
    PicBtnState2Pressed = 6,
};


const int PROGRESS_WIDTH = 300;
const int PROGRESS_MAX = 300;
//preListWidget的大小
const int PREITEM_SIZE = 100;
//图片预览的大小
const int PREICON_SIZE = 90;
