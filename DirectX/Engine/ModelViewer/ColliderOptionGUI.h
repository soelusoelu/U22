#pragma once

#include "../../Device/FileOperator.h"
#include "../../Device/Function.h"
#include <functional>
#include <memory>
#include <vector>

class OBBCollider;

class ColliderOptionGUI
    : public FileOperator
{
    using OBBColliderPtr = std::shared_ptr<OBBCollider>;

public:
    ColliderOptionGUI(std::vector<OBBColliderPtr>& colliders);
    ~ColliderOptionGUI();
    //Gui��`�悷��
    void drawGui();
    //Gui���j���[���J��
    void open(const std::shared_ptr<OBBCollider>& target);
    //Gui���j���[���J���Ă��邩
    bool isOpened() const;
    //Gui���j���[�����Ă��邩
    bool isClosed() const;
    //OK�{�^���������ꂽ�ۂ̃R�[���o�b�N
    void onOkButton(const std::function<void()>& f);

private:
    ColliderOptionGUI(const ColliderOptionGUI&) = delete;
    ColliderOptionGUI& operator=(const ColliderOptionGUI&) = delete;

    void division();

private:
    std::shared_ptr<OBBCollider> mTarget;
    std::vector<OBBColliderPtr>& mColliders;
    int mDivisionCount;
    int mAxisButton;
    bool mIsOpened;
    Function<void()> mOnOkButton;

    static inline constexpr int AXIS_COUNT = 3;
    static inline constexpr int MIN_DIVISION_COUNT = 1;
    static inline constexpr int MAX_DIVISION_COUNT = 10;
};
