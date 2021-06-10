#include "AssetsManager.h"
#include "GlobalFunction.h"
#include "Shader/Shader.h"
#include "Texture/Texture.h"
#include "Texture/TextureFromFile.h"
#include "../Mesh/Mesh.h"
#include "../Utility/FileUtil.h"

AssetsManager::AssetsManager() = default;

AssetsManager::~AssetsManager() = default;

AssetsManager& AssetsManager::instance() {
    if (!mInstance) {
        mInstance = new AssetsManager();
    }
    return *mInstance;
}

void AssetsManager::finalize() {
    safeDelete(mInstance);
}

void AssetsManager::loadTexture(const std::string& fileName, const std::string& directoryPath) {
    //テクスチャを読み込む
    loadTextureGetID(directoryPath + fileName);
}

int AssetsManager::createTexture(const std::string& filename, const std::string& directoryPath) {
    //テクスチャを読み込む
    int id = loadTextureGetID(directoryPath + filename);

    return id;
}

int AssetsManager::addTexture(const std::shared_ptr<Texture>& texture) {
    int id = mTextures.size();
    mTextures.emplace_back(TextureParam{ texture, "", id });
    return id;
}

const std::shared_ptr<Texture>& AssetsManager::getTextureFromID(int id) const {
    return mTextures[id].texture;
}

void AssetsManager::loadMesh(const std::string& fileName, const std::string& directoryPath) {
    loadMeshFromFilePath(directoryPath + fileName);
}

void AssetsManager::loadMeshFromFilePath(const std::string& filePath) {
    //読み込み済みなら何もしない
    if (loadedMesh(filePath)) {
        return;
    }

    //メッシュを生成し格納
    auto mesh = std::make_shared<Mesh>();
    mesh->loadMesh(filePath);
    mMeshes.emplace(filePath, mesh);
}

std::shared_ptr<Mesh> AssetsManager::createMesh(const std::string& fileName, const std::string& directoryPath) {
    //メッシュを読み込む
    loadMesh(fileName, directoryPath);

    //読み込んだメッシュを返す
    return mMeshes[directoryPath + fileName];
}

std::shared_ptr<Mesh> AssetsManager::createMeshFromFilePath(const std::string& filePath) {
    //メッシュを読み込む
    loadMeshFromFilePath(filePath);

    //読み込んだメッシュを返す
    return mMeshes[filePath];
}

std::shared_ptr<Shader> AssetsManager::createShader(const std::string& fileName, const std::string& directoryPath) {
    std::shared_ptr<Shader> shader = nullptr;

    //ディレクトパスとファイル名を結合する
    auto filePath = directoryPath + fileName;

    //読み込み済みなら
    if (mShaders.find(filePath) != mShaders.end()) {
        shader = mShaders[filePath];
    } else {
        //シェーダーを生成し格納
        shader = std::make_shared<Shader>(fileName, directoryPath);
        mShaders.emplace(filePath, shader);
    }

    return shader;
}

int AssetsManager::loadTextureGetID(const std::string& filePath) {
    //読み込み済みなら何もしない
    int id = -1;
    if (loadedTexture(filePath, &id)) {
        return id;
    }

    //テクスチャを生成し格納
    auto texture = std::make_shared<TextureFromFile>(filePath);
    id = mTextures.size();
    mTextures.emplace_back(TextureParam{ texture, filePath, id });

    return id;
}

bool AssetsManager::loadedTexture(const std::string& filePath, int* outID) const {
    for (const auto& tex : mTextures) {
        if (tex.filePath == filePath) {
            if (outID) {
                *outID = tex.id;
            }
            return true;
        }
    }

    return false;
}

bool AssetsManager::loadedMesh(const std::string& filePath) const {
    auto itr = mMeshes.find(filePath);
    return (itr != mMeshes.end());
}
