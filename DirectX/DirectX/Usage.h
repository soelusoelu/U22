#pragma once

#include "../System/SystemInclude.h"

enum class Usage {
    DEFAULT, //GPU�ɂ��ǂݏ���
    IMMUTABLE, //GPU�̓ǂݍ��݂̂݉\
    DYNAMIC, //GPU�̓ǂݍ��݂�CPU�̏������݂��\
    STAGING //GPU����CPU�ւ̃f�[�^�]�����T�|�[�g
};

D3D11_USAGE toUsage(Usage usage);
