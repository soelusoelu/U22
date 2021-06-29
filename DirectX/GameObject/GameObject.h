﻿#pragma once

#include "IThisGetter.h"
#include "Object.h"
#include <rapidjson/document.h>
#include <memory>
#include <string>

class ComponentManager;
class GameObjectManager;
class Transform3D;

class GameObject final
    : public Object
    , public IThisGetter<GameObject>
{
public:
    GameObject();
    ~GameObject();

    virtual GameObject& getThis() override;
    virtual const GameObject& getThis() const override;

    //更新
    void update();
    //遅延更新
    void lateUpdate();

    //ロード/セーブ
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;

    //アクティブ指定
    void setActive(bool value);
    //アクティブ状態の取得
    bool getActive() const;

    //名前を設定する
    void setName(const std::string& name);
    //名前の取得
    const std::string& name() const;
    //タグを設定する
    void setTag(const std::string& tag);
    //タグの取得
    const std::string& tag() const;

    //トランスフォームの取得
    Transform3D& transform() const;
    //コンポーネント管理者の取得
    ComponentManager& componentManager() const;

    //GameObjectManagerの取得
    GameObjectManager& getGameObjectManager();

    //GameObjectManagerの登録
    static void setGameObjectManager(GameObjectManager* manager);
    //ゲームオブジェクトを生成
    static std::shared_ptr<GameObject> create(const std::string& name, const std::string& tag);

private:
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

private:
    std::unique_ptr<Transform3D> mTransform;
    std::unique_ptr<ComponentManager> mComponentManager;
    std::string mName;
    std::string mTag;
    bool mIsActive;

    static inline GameObjectManager* mGameObjectManager = nullptr;
};
