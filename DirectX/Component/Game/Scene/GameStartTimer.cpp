#include "GameStartTimer.h"
#include "../../Engine/Text/Text.h"
#include "../../../Device/Time.h"
#include "../../../GameObject/GameObject.h"
#include "../../../Math/Math.h"
#include "../../../Utility/Easing.h"
#include "../../../Utility/JsonHelper.h"
#include "../../../Utility/StringUtil.h"

GameStartTimer::GameStartTimer()
    : Component()
    , mNumber(nullptr)
    , mCountDownTimer(std::make_unique<Time>())
    , mBeginScale(0.f)
    , mEndScale(0.f)
{
}

GameStartTimer::~GameStartTimer() = default;

void GameStartTimer::start() {
    mNumber = getComponent<Text>();
    setTextScale(mBeginScale);
}

void GameStartTimer::update() {
    mCountDownTimer->update();
    if (mCountDownTimer->isTime()) {
        //カウントダウンが終了したことを通知する
        mOnEndTimer();

        //もう不要なのでゲームオブジェクトごと削除する
        gameObject().destroy();

        return;
    }

    //カウントダウンタイムをintに変換し設定する
    float curTime = mCountDownTimer->getCountDownTime();
    int timer = static_cast<int>(curTime) + 1;
    //mNumber->setNumber(timer);
    mNumber->setText(StringUtil::intToString(timer));

    //残り秒数の小数点以下からスケールを求める
    float integral = 0.f;
    auto f = modff(curTime, &integral);
    auto nextScale = Math::lerp(mBeginScale, mEndScale, 1.f - Easing::easeInCubic(f));
    setTextScale(nextScale);
}

void GameStartTimer::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    JsonHelper::getSet(mBeginScale, "beginScale", inObj, alloc, mode);
    JsonHelper::getSet(mEndScale, "endScale", inObj, alloc, mode);

    mCountDownTimer->saveAndLoad(inObj, alloc, mode);
}

bool GameStartTimer::isEndTimer() const {
    return mCountDownTimer->isTime();
}

void GameStartTimer::onEndTimer(const std::function<void()>& f) {
    mOnEndTimer += f;
}

void GameStartTimer::setTextScale(float scale) {
    mNumber->setScale(Vector2::one * scale);
}
