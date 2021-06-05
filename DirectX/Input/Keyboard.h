﻿#pragma once

#include "IKeyboard.h"
#include "../System/SystemInclude.h"
#include <rapidjson/document.h>
#include <dinput.h>
#include <string>

class Keyboard : public IKeyboard {
public:
    Keyboard();
    ~Keyboard();
    virtual bool getKeyDown(KeyCode key) const override;
    virtual bool getKey(KeyCode key) const override;
    virtual bool getKeyUp(KeyCode key) const override;
    virtual int horizontal() const override;
    virtual int vertical() const override;
    virtual int getNumber() const override;
    virtual bool getEnter() const override;

    bool initialize(const HWND& hWnd, IDirectInput8* directInput);
    void loadProperties(const rapidjson::Value& inObj);
    void saveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const;
    void update();
    //文字列をKeyCodeに変換
    static void stringToKeyCode(const std::string& src, KeyCode* dst);

public:
    //無効な数字
    static constexpr int INVALID_NUMBER = -1;

private:
    IDirectInputDevice8* mKeyDevice;
    byte mCurrentKeys[256];
    byte mPreviousKeys[256];
    KeyCode mEnterKey;
    std::string mEnterKeyStr;
};
