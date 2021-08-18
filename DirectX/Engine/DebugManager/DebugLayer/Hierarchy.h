﻿#pragma once

#include "Inspector/IInspector.h"
#include "../../../Device/FileOperator.h"
#include "../../../GameObject/IGameObjectsGetter.h"
#include "../../../Math/Math.h"
#include <list>
#include <memory>
#include <utility>

class DrawString;
class GameObject;
class Button;

class Hierarchy
    : public FileOperator
{
    using GameObjectPtr = std::shared_ptr<GameObject>;
    using ButtonGameObjectPair = std::pair<std::unique_ptr<Button>, GameObjectPtr>;
    using ButtonGameObjectPairList = std::list<ButtonGameObjectPair>;

public:
    Hierarchy();
    ~Hierarchy();
    void initialize(const IGameObjectsGetter* getter, IInspector* inspector);
    void update();
    //マネージャーに登録されてる全ゲームオブジェクトを表示
    void drawGameObjects(DrawString& drawString) const;

private:
    Hierarchy(const Hierarchy&) = delete;
    Hierarchy& operator=(const Hierarchy&) = delete;

    virtual void saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) override;

    //全ゲームオブジェクトをボタン登録する
    void setGameObjectToButton();
    //ボタンにゲームオブジェクトを登録する
    void entryToButton(ButtonGameObjectPairList::iterator& itr, const GameObjectPtr& target);
    //すべての子をボタン登録する
    void setGameObjectToButtonChildren(ButtonGameObjectPairList::iterator& itr, const GameObjectPtr& parent);
    //文字列を描画する
    void draw(DrawString& drawString, const GameObject& target, const Vector2& position, int childHierarchy = 0) const;
    //すべての子を描画する
    void drawChildren(DrawString& drawString, const GameObject& parent, Vector2& position, int childHierarchy = 1) const;
    //描画位置を1行分下げる
    void downDrawPositionOneLine(Vector2& position) const;
    //1行の高さを取得する
    float getOneLineHeight() const;
    //ゲームオブジェクトに親がいるか
    bool haveParent(const GameObject& gameObject) const;

private:
    const IGameObjectsGetter* mGameObjectsGetter;
    IInspector* mInspector;
    ButtonGameObjectPairList mButtons;
    //画面に表示する行数
    int mNumRowsToDisplay;
    //行間
    float mLineSpace;
    //表示する位置
    Vector2 mPosition;
    //文字のスケール
    Vector2 mScale;
    //表示位置をずらす文字数
    int mOffsetCharCountX;
    int mOffsetCharCountY;
    //1文字の大きさ
    float mCharWidth;
    float mCharHeight;
    //非アクティブ時の文字の透過度
    float mNonActiveAlpha;
};
