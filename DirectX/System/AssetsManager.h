#pragma once

#include "AssetsDirectoryPath.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class Mesh;
class Texture;
class Shader;

class AssetsManager {
private:
    //シングルトンだからprivate
    AssetsManager();
public:
    ~AssetsManager();
    //アセット管理クラスのインスタンスを返す
    static AssetsManager& instance();
    //終了処理
    void finalize();

    //テクスチャを読み込む
    void loadTexture(const std::string& fileName, const std::string& directoryPath = AssetsDirectoryPath::TEXTURE_PATH);
    //ファイルパスからテクスチャIDを取得する
    int createTexture(const std::string& filename, const std::string& directoryPath = AssetsDirectoryPath::TEXTURE_PATH);
    //テクスチャを追加する
    int addTexture(const std::shared_ptr<Texture>& texture);
    //IDからテクスチャを取得する
    const std::shared_ptr<Texture>& getTextureFromID(int id) const;

    //メッシュを読み込む
    void loadMesh(const std::string& fileName, const std::string& directoryPath = AssetsDirectoryPath::MODEL_PATH);
    //メッシュを読み込む
    void loadMeshFromFilePath(const std::string& filePath);
    //ファイルパスからメッシュを取得する
    std::shared_ptr<Mesh> createMesh(const std::string& fileName, const std::string& directoryPath = AssetsDirectoryPath::MODEL_PATH);
    //ファイルパスからメッシュを取得する
    std::shared_ptr<Mesh> createMeshFromFilePath(const std::string& filePath);

    //ファイルパスからシェーダーを取得する
    std::shared_ptr<Shader> createShader(const std::string& fileName, const std::string& directoryPath = AssetsDirectoryPath::SHADER_PATH);

private:
    AssetsManager(const AssetsManager&) = delete;
    AssetsManager& operator=(const AssetsManager&) = delete;

    //テクスチャを読み込みIDを取得する
    int loadTextureGetID(const std::string& filePath);
    //読み込み済みのテクスチャか
    bool loadedTexture(const std::string& filePath, int* outID = nullptr) const;
    //読み込み済みのメッシュか
    bool loadedMesh(const std::string& filePath) const;

private:
    struct TextureParam {
        std::shared_ptr<Texture> texture;
        std::string filePath;
        int id;
    };

    static inline AssetsManager* mInstance = nullptr;

    std::vector<TextureParam> mTextures;
    std::unordered_map<std::string, std::shared_ptr<Mesh>> mMeshes;
    std::unordered_map<std::string, std::shared_ptr<Shader>> mShaders;
};
