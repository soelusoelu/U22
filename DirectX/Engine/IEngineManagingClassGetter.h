#pragma once

#include "Pause/IPause.h"

class DebugManager;

//エンジン機能統括クラスが管理しているクラスにアクセスする
class IEngineManagingClassGetter {
public:
    virtual ~IEngineManagingClassGetter() = default;
    //デバッグ機能へのアクセス
    virtual DebugManager& debug() const = 0;
    //ポーズ機能へのアクセス
    virtual IPause& pause() const = 0;
};