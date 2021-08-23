#include "YouDied.h"
#include "../Player/PlayerColliderController.h"
#include "../../Engine/Sprite/SpriteComponent.h"
#include "../../../Device/Time.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Math/Math.h"
#include "../../../Utility/JsonHelper.h"

YouDied::YouDied()
    : Component()
    , mYouDied(nullptr)
    , mBackGround(nullptr)
    , mStartingUpToTime(std::make_unique<Time>())
    , mYouDiedTimer(std::make_unique<Time>())
    , mBackGroundTimer(std::make_unique<Time>()) {
}

YouDied::~YouDied() = default;

void YouDied::start() {
    auto sprites = getComponents<SpriteComponent>();
    mYouDied = sprites[0];
    mBackGround = sprites[1];

    const auto& player = gameObject().getGameObjectManager().find("Player");
    player->componentManager().getComponent<PlayerColliderController>()->onDead([&] { activate(); });
}

void YouDied::update() {
    if (!mYouDied->getActive()) {
        return;
    }

    //死亡画像表示までの猶予時間を進める
    if (!mStartingUpToTime->isTime()) {
        mStartingUpToTime->update();
        return;
    }

    if (!mBackGroundTimer->isTime()) {
        mBackGroundTimer->update();
        setAlpha(*mBackGround, *mBackGroundTimer);
    } else {
        if (!mYouDiedTimer->isTime()) {
            mYouDiedTimer->update();
            setAlpha(*mYouDied, *mYouDiedTimer);

            if (hasEndedDrawing()) {
                mOnEndedDrawing();
            }
        }
    }
}

void YouDied::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    float timer = 0.f;
    JsonHelper::getSet(timer, "startingUpToTime", inObj, alloc, mode);
    mStartingUpToTime->setLimitTime(timer);
    JsonHelper::getSet(timer, "youDiedTimer", inObj, alloc, mode);
    mYouDiedTimer->setLimitTime(timer);
    JsonHelper::getSet(timer, "backGroundTimer", inObj, alloc, mode);
    mBackGroundTimer->setLimitTime(timer);
}

void YouDied::activate() {
    mYouDied->setActive(true);
    mBackGround->setActive(true);
}

bool YouDied::hasEndedDrawing() const {
    return Math::equal(mYouDied->getAlpha(), 1.f);
}

void YouDied::onEndedDrawing(const std::function<void()>& f) {
    mOnEndedDrawing += f;
}

void YouDied::setAlpha(SpriteComponent& target, const Time& timer) {
    float alpha = Math::lerp(0.f, 1.f, timer.rate());
    if (alpha > 1.f) {
        alpha = 1.f;
    }
    target.setAlpha(alpha);
}
