#pragma once
#include "Singleton.h"
class Mouse :public Singleton<Mouse>
{
    friend Singleton<Mouse>;
    int x;
    int y;          //座標
    int WheelRotVol;//ホイールの回転量
    static const  int BUTTON_NUM = 8;
    int _pressingCount[BUTTON_NUM];  //ボタンの押した状態
    int _releasingCount[BUTTON_NUM];  //ボタンの押した状態

    bool isAvailableCode(int keyCode);//keyCodeが有効なキー番号か問う
public:
    bool update();	//更新
    int getPressingCount(int keyCode);//keyCodeのキーが押されているフレーム数を取得
    int getReleasingCount(int keyCode);//keyCodeのキーが離されているフレーム数を取得
    int getX();
    int getY();
    int getWheelRotVol();
};
