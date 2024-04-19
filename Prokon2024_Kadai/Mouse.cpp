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
    if (GetMousePoint(&this->x, &this->y) == -1) { //�}�E�X�̈ʒu�擾
        return false;
    }
    int MouseInput = GetMouseInput();    //�}�E�X�̉�������Ԏ擾
    for (int i = 0; i < BUTTON_NUM; i++) {            //�}�E�X�̃L�[�͍ő�8�܂Ŋm�F�o����
        if ((MouseInput & 1 << i) != 0)
        {
            this->_pressingCount[i]++;   //������Ă�����J�E���g�A�b�v
            this->_releasingCount[i] = 0; //������Ă�����0
        }
        
        else
        {
            this->_pressingCount[i] = 0; //������ĂȂ�������0
            this->_releasingCount[i]++;   //������ĂȂ�������J�E���g�A�b�v
        }
    }
    this->WheelRotVol = GetMouseWheelRotVol();    //�z�C�[����]�ʎ擾
    return true;
}

/*!
@brief keyCode�̃L�[��������Ă���t���[������Ԃ�
*/
int Mouse::getPressingCount(int keyCode) {
    keyCode = std::log2(keyCode);
    if (!isAvailableCode(keyCode)) {
        return -1;
    }
    return _pressingCount[keyCode];
}

/*!
@brief keyCode�̃L�[��������Ă���t���[������Ԃ�
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
