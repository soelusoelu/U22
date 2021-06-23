﻿#include "FbxMeshParser.h"
#include "FbxUtility.h"

FbxMeshParser::FbxMeshParser() = default;

FbxMeshParser::~FbxMeshParser() = default;

//長いけど無駄にfor文を回さなくて済む
void FbxMeshParser::parse(
    MeshVertices& meshVertices,
    MeshVerticesPosition& meshVerticesPosition,
    Indices& indices,
    FbxMesh* fbxMesh
) const {
    //頂点数
    int polygonVertexCount = fbxMesh->GetPolygonVertexCount();
    //インデックスバッファの取得
    int* polygonVertices = fbxMesh->GetPolygonVertices();
    //頂点座標配列
    FbxVector4* src = fbxMesh->GetControlPoints();

    //法線配列を取得する
    FbxArray<FbxVector4> normalArray;
    fbxMesh->GetPolygonVertexNormals(normalArray);

    //UV配列を取得する
    FbxArray<FbxVector2> uvArray;
    getUVs(uvArray, fbxMesh);

    //事前に拡張しとく
    meshVertices.resize(polygonVertexCount);
    meshVerticesPosition.resize(polygonVertexCount);

    //メッシュごとのトランスフォームを計算
    FbxNode* node = fbxMesh->GetNode();
    auto t = FbxUtility::fbxDouble3ToVector3(node->LclTranslation.Get());
    auto r = FbxUtility::fbxDouble3ToVector3(node->LclRotation.Get());
    Quaternion q(r);
    auto s = FbxUtility::fbxDouble3ToVector3(node->LclScaling.Get());
    auto mat = Matrix4::createScale(s) * Matrix4::createFromQuaternion(q) * Matrix4::createTranslation(t);

    for (int i = 0; i < polygonVertexCount; ++i) {
        MeshVertex vertex;

        int index = polygonVertices[i];
        vertex.pos = FbxUtility::fbxVector4ToVector3(src[index]);
        vertex.pos = Vector3::transform(vertex.pos, mat);

        vertex.normal = FbxUtility::fbxVector4ToVector3(normalArray[i]);
        vertex.normal = Vector3::transform(vertex.normal, q);

        //UVは使用している場合のみ
        if (uvArray.Size() > 0) {
            vertex.uv.x = static_cast<float>(uvArray[i][0]);
            vertex.uv.y = 1.f - static_cast<float>(uvArray[i][1]);
        }

        //頂点情報を格納
        meshVertices[i] = vertex;
        meshVerticesPosition[i] = vertex.pos;
    }

    //indicesはポリゴン頂点数
    indices.resize(polygonVertexCount);

    for (int i = 0, polyCount = fbxMesh->GetPolygonCount(); i < polyCount; ++i) {
        auto idx = i * 3;
        indices[idx] = idx;
        indices[idx + 1] = idx + 1;
        indices[idx + 2] = idx + 2;
    }
}

void FbxMeshParser::loadNormal(FbxMesh* mesh) {
    //FbxGeometryElementNormal* normalElement = mesh->GetElementNormal();
    //if (!normalElement) {
    //    return;
    //}

    //if (normalElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
    //    for (size_t i = 0; i < mesh->GetControlPointsCount(); i++) {
    //        int normalIndex = 0;
    //        if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
    //            normalIndex = i;
    //        }
    //        if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
    //            normalIndex = normalElement->GetIndexArray().GetAt(i);
    //        }

    //        FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);
    //        mNormals[normalIndex].x = static_cast<float>(-normal[0]);
    //        mNormals[normalIndex].y = static_cast<float>(normal[1]);
    //        mNormals[normalIndex].z = static_cast<float>(normal[2]);
    //    }
    //} else if (normalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
    //    int indexByPolygonVertex = 0;
    //    for (size_t i = 0; i < mesh->GetPolygonCount(); i++) {
    //        int polygonSize = mesh->GetPolygonSize(i);
    //        for (size_t j = 0; j < polygonSize; j++) {
    //            int normalIndex = 0;
    //            if (normalElement->GetReferenceMode() == FbxGeometryElement::eDirect) {
    //                normalIndex = indexByPolygonVertex;
    //            }
    //            if (normalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect) {
    //                normalIndex = normalElement->GetIndexArray().GetAt(indexByPolygonVertex);
    //            }

    //            FbxVector4 normal = normalElement->GetDirectArray().GetAt(normalIndex);
    //            mNormals[normalIndex].x = static_cast<float>(-normal[0]);
    //            mNormals[normalIndex].y = static_cast<float>(normal[1]);
    //            mNormals[normalIndex].z = static_cast<float>(normal[2]);

    //            indexByPolygonVertex++;
    //        }
    //    }
    //}
}

void FbxMeshParser::loadUV(FbxMesh* mesh) {
    //すべてのUVセットを反復処理する
    //for (int uvSetIndex = 0; uvSetIndex < uvSetNameList.GetCount(); uvSetIndex++) {
    //    //uvSetIndex-番目のUVセットを取得
    //    const char* uvSetName = uvSetNameList.GetStringAt(uvSetIndex);
    //    const FbxGeometryElementUV* uvElement = mesh->GetElementUV(uvSetName);
    //    if (!uvElement) {
    //        continue;
    //    }

    //    //マッピングモードeByPolygonVertexおよびeByControlPointのみをサポート
    //    if (uvElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex && uvElement->GetMappingMode() != FbxGeometryElement::eByControlPoint) {
    //        return;
    //    }

    //    //インデックス配列。uvデータを参照するインデックスを保持します
    //    const bool useIndex = uvElement->GetReferenceMode() != FbxGeometryElement::eDirect;
    //    const int indexCount = (useIndex) ? uvElement->GetIndexArray().GetCount() : 0;

    //    //ポリゴンごとにデータを反復処理する
    //    const int polyCount = mesh->GetPolygonCount();

    //    if (uvElement->GetMappingMode() == FbxGeometryElement::eByControlPoint) {
    //        for (int polyIndex = 0; polyIndex < polyCount; polyIndex++) {
    //            //MakePolyに渡す必要がある最大インデックス配列を作成します
    //            const int polySize = mesh->GetPolygonSize(polyIndex);
    //            for (int vertIndex = 0; vertIndex < polySize; vertIndex++) {
    //                //頂点配列の現在の頂点のインデックスを取得します
    //                int polyVertIndex = mesh->GetPolygonVertex(polyIndex, vertIndex);

    //                //UVインデックスは参照モードに依存します
    //                int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyVertIndex) : polyVertIndex;

    //                FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
    //                mUVs[uvIndex].x = static_cast<float>(uv[0]);
    //                mUVs[uvIndex].y = 1.f - static_cast<float>(uv[1]);
    //            }
    //        }
    //    } else if (uvElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex) {
    //        int polyIndexCounter = 0;
    //        for (int polyIndex = 0; polyIndex < polyCount; polyIndex++) {
    //            //MakePolyに渡す必要がある最大インデックス配列を作成します
    //            const int polySize = mesh->GetPolygonSize(polyIndex);
    //            for (int vertIndex = 0; vertIndex < polySize; vertIndex++) {
    //                if (polyIndexCounter < indexCount) {
    //                    //UVインデックスは参照モードに依存します
    //                    int uvIndex = useIndex ? uvElement->GetIndexArray().GetAt(polyIndexCounter) : polyIndexCounter;

    //                    FbxVector2 uv = uvElement->GetDirectArray().GetAt(uvIndex);
    //                    mUVs[uvIndex].x = static_cast<float>(uv[0]);
    //                    mUVs[uvIndex].y = 1.f - static_cast<float>(uv[1]);

    //                    polyIndexCounter++;
    //                }
    //            }
    //        }
    //    }
    //}
}

void FbxMeshParser::getUVs(
    FbxArray<FbxVector2>& uvs,
    const FbxMesh* fbxMesh
) const {
    FbxStringList uvNameList;
    getUVsFromUVSetNameList(uvs, uvNameList, fbxMesh);
}

void FbxMeshParser::getUVsFromUVSetNameList(
    FbxArray<FbxVector2>& uvs,
    FbxStringList& uvNameList,
    const FbxMesh* fbxMesh
) const {
    //UVの名前リストを取得
    fbxMesh->GetUVSetNames(uvNameList);
    //UVリストの名前からUVを取得する
    fbxMesh->GetPolygonVertexUVs(uvNameList.GetStringAt(0), uvs);
}
