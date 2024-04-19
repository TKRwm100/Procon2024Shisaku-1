#pragma once
#include "Singleton.h"
class Mouse :public Singleton<Mouse>
{
    friend Singleton<Mouse>;
    int x;
    int y;          //���W
    int WheelRotVol;//�z�C�[���̉�]��
    static const  int BUTTON_NUM = 8;
    int _pressingCount[BUTTON_NUM];  //�{�^���̉��������
    int _releasingCount[BUTTON_NUM];  //�{�^���̉��������

    bool isAvailableCode(int keyCode);//keyCode���L���ȃL�[�ԍ����₤
public:
    bool update();	//�X�V
    int getPressingCount(int keyCode);//keyCode�̃L�[��������Ă���t���[�������擾
    int getReleasingCount(int keyCode);//keyCode�̃L�[��������Ă���t���[�������擾
    int getX();
    int getY();
    int getWheelRotVol();
};
