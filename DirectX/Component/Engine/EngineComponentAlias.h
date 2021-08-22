#pragma once

//エイリアス宣言用ヘッダ

#include <memory>
#include <vector>

class OBBCollider;

using OBBColliderPtr = std::shared_ptr<OBBCollider>;
using OBBColliderPtrArray = std::vector<OBBColliderPtr>;

class SpriteComponent;

using SpritePtr = std::shared_ptr<SpriteComponent>;
using SpritePtrArray = std::vector<SpritePtr>;
