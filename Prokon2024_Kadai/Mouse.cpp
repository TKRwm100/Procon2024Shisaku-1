#include "Mouse.h"
#include "DxLib.h"
#include <cmath>

bool Mouse::isAvailableCode(int keyCode)
{
    if (!(0 <= keyCode && keyCode < BUTTON_NUM)) {
        return false;
    }
    return true;
}

bool Mouse::update() {
    if (GetMousePoint(&this->x, &this->y) == -1) { //マウスの位置取得
        return false;
    }
    int MouseInput = GetMouseInput();    //マウスの押した状態取得
    for (int i = 0; i < BUTTON_NUM; i++) {            //マウスのキーは最大8個まで確認出来る
        if ((MouseInput & 1 << i) != 0)
        {
            this->_pressingCount[i]++;   //押されていたらカウントアップ
            this->_releasingCount[i] = 0; //押されていたら0
        }
        
        else
        {
            this->_pressingCount[i] = 0; //押されてなかったら0
            this->_releasingCount[i]++;   //押されてなかったらカウントアップ
        }
    }
    this->WheelRotVol = GetMouseWheelRotVol();    //ホイール回転量取得
    return true;
}

/*!
@brief keyCodeのキーが押されているフレーム数を返す
*/
int Mouse::getPressingCount(int keyCode) {
    keyCode = std::log2(keyCode);
    if (!isAvailableCode(keyCode)) {
        return -1;
    }
    return _pressingCount[keyCode];
}

/*!
@brief keyCodeのキーが離されているフレーム数を返す
*/
int Mouse::getReleasingCount(int keyCode) {
    keyCode = std::log2(keyCode);
    if (!isAvailableCode(keyCode)) {
        return -1;
    }
    return _releasingCount[keyCode];
}

int Mouse::getX()
{
    return x;
}

int Mouse::getY()
{
    return y;
}

int Mouse::getWheelRotVol()
{
    return WheelRotVol;
}
