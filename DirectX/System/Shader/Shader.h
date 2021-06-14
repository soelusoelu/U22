#pragma once

#include "../SystemInclude.h"
#include "../../DirectX/InputElementDesc.h"
#include <memory>
#include <string>
#include <vector>

class Buffer;
class InputElement;
class ConstantBufferManager;
class InputElementManager;

class Shader {
    using BufferPtr = std::unique_ptr<Buffer>;
    using BufferPtrArray = std::vector<BufferPtr>;

public:
    Shader(const std::string& fileName, const std::string& directoryPath);
    ~Shader();

    //プログラムの終わりの終了処理
    static void finalize();

    //シェーダーにデータを転送する
    void transferData(const void* data, unsigned size, unsigned constantBufferIndex = 0) const;
    //シェーダーに値を渡すための開始・終了処理
    bool map(D3D11_MAPPED_SUBRESOURCE* mapRes, ID3D11Buffer* buffer, unsigned sub = 0, D3D11_MAP type = D3D11_MAP_WRITE_DISCARD, unsigned flag = 0) const;
    void unmap(ID3D11Buffer* buffer, unsigned sub = 0) const;

    //使用するコンスタントバッファを登録
    void setVSConstantBuffers(unsigned index = 0, unsigned numBuffers = 1) const;
    void setPSConstantBuffers(unsigned index = 0, unsigned numBuffers = 1) const;

    //頂点シェーダーを取得する
    ID3D11VertexShader* getVertexShader() const;
    //ピクセルシェーダーを取得する
    ID3D11PixelShader* getPixelShader() const;
    //頂点レイアウトを取得する
    const InputElement& getVertexLayout() const;
    //コンスタントバッファを取得する
    const Buffer& getConstantBuffer(unsigned index = 0) const;
    //シェーダー名を取得する
    const std::string& getShaderName() const;

private:
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    //シェーダの生成
    void createVertexShader(const std::string& fileName);
    void createPixelShader(const std::string& fileName);
    //シェーダーをコンパイルする
    bool compileShader(Microsoft::WRL::ComPtr<ID3DBlob>* out, const std::string& fileName, const std::string& entryPoint, const std::string& target);
    //インプットレイアウトの生成
    void createInputLayout(const std::vector<InputElementDesc>& layout);

private:
    std::string mShaderName;
    Microsoft::WRL::ComPtr<ID3DBlob> mVSBlob;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> mVertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> mPixelShader;
    BufferPtrArray mConstantBuffers;
    std::unique_ptr<InputElement> mVertexLayout;

    static inline ConstantBufferManager* constantBufferManager = nullptr;
    static inline InputElementManager* inputElementManager = nullptr;
};
