#include "BulletShooter.h"
#include "PlayerMotions.h"
#include "PlayerSounds.h"
#include "../PlayerEnemyCommon/HitPoint.h"
#include "../PlayerEnemyCommon/PlayerEnemyConnection.h"
#include "../../Engine/Camera/Camera.h"
#include "../../Engine/Collider/OBBCollider.h"
#include "../../Engine/Mesh/SkinMeshComponent.h"
#include "../../Engine/Sound/SoundComponent.h"
#include "../../../Collision/Collision.h"
#include "../../../Device/Time.h"
#include "../../../Engine/DebugManager/DebugUtility/Debug.h"
#include "../../../GameObject/GameObject.h"
#include "../../../GameObject/GameObjectManager.h"
#include "../../../Input/Input.h"
#include "../../../Sound/Player/Frequency.h"
#include "../../../Sound/Player/SoundPlayer.h"
#include "../../../Sound/Volume/SoundVolume.h"
#include "../../../System/Window.h"
#include "../../../Transform/Transform3D.h"

BulletShooter::BulletShooter()
    : Component()
    , mCamera(nullptr)
    , mAnimation(nullptr)
    , mSound(nullptr)
    , mShotCoolTime(std::make_unique<Time>())
    , mShotRaySetter(nullptr)
    , mIsADS(false)
{
}

BulletShooter::~BulletShooter() = default;

void BulletShooter::start() {
    const auto& cam = gameObject().getGameObjectManager().find("Camera");
    mCamera = cam->componentManager().getComponent<Camera>();

    mAnimation = getComponent<SkinMeshComponent>();
    mSound = getComponents<SoundComponent>()[PlayerSounds::SHOT];
    mSound->getSoundVolume().setVolume(0.5f);
    mSound->getSoundPlayer().getFrequency().setFrequencyRatio(2.f);

    getComponent<HitPoint>()->callbackChangeHp([&](const HitPoint& hp) { onChangeHp(hp); });

    mShotCoolTime->forceOverlimit();
}

void BulletShooter::update() {
    if (!mShotCoolTime->isTime()) {
        mShotCoolTime->update();
    }
}

void BulletShooter::saveAndLoad(rapidjson::Value& inObj, rapidjson::Document::AllocatorType& alloc, FileMode mode) {
    mShotCoolTime->saveAndLoad(inObj, alloc, mode);
}

void BulletShooter::originalUpdate() {
    ads();
    if (!mIsADS) {
        return;
    }
    if (!mShotCoolTime->isTime()) {
        return;
    }
    if (!Input::joyPad().getJoyDown(JoyCode::RightButton)) {
        return;
    }

    //カメラの中心から弾となるレイを飛ばす
    const auto center = Window::size() / 2.f;
    mShotRay = mCamera->screenToRay(center);
    mShotRaySetter->setShotRay(&mShotRay);

    //アニメーション変更
    mAnimation->changeMotion(PlayerMotions::SHOT);
    mAnimation->setLoop(false);

    mSound->getSoundPlayer().playStreaming();
    mShotCoolTime->reset();
}

void BulletShooter::setConnector(const GameObject& connector) {
    auto pec = connector.componentManager().getComponent<PlayerEnemyConnection>();
    mShotRaySetter = pec.get();
}

void BulletShooter::ads() {
    const auto& pad = Input::joyPad();
    static constexpr auto code = JoyCode::LeftButton;
    if (pad.getJoyDown(code)) {
        mIsADS = true;
        mAnimation->changeMotion(PlayerMotions::START_ADS);
        mAnimation->setLoop(false);
        mOnStartAds();
    }
    if (pad.getJoyUp(code)) {
        mIsADS = false;
        mAnimation->changeMotion(PlayerMotions::STOP_ADS);
        mAnimation->setLoop(false);
        mOnStopAds();
    }
    if (pad.getJoy(code)) {
        mOnAds();
    }
}

bool BulletShooter::isAds() const {
    return mIsADS;
}

void BulletShooter::onAds(const std::function<void()>& f) {
    mOnAds += f;
}

void BulletShooter::onStartAds(const std::function<void()>& f) {
    mOnStartAds += f;
}

void BulletShooter::onStopAds(const std::function<void()>& f) {
    mOnStopAds += f;
}

void BulletShooter::onChangeHp(const HitPoint& hp) {
    if (mIsADS) {
        mIsADS = false;
        mOnStopAds();
    }
}
