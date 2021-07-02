#pragma once

#include "FileMode.h"
#include "../Math/Math.h"
#include <rapidjson/document.h>
#include <string>
#include <vector>

//Jsonファイル解析ヘルパークラス
class JsonHelper {
public:
    //FileModeに応じてgetter/setter処理を分ける
    static void getSetInt(
        int& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc,
        FileMode mode
    );
    static void getSetFloat(
        float& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc,
        FileMode mode
    );
    static void getSetString(
        std::string& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc,
        FileMode mode
    );
    static void getSetBool(
        bool& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc,
        FileMode mode
    );
    static void getSetVector2(
        Vector2& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc,
        FileMode mode
    );
    static void getSetVector3(
        Vector3& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc,
        FileMode mode
    );
    static void getSetVector4(
        Vector4& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc,
        FileMode mode
    );
    static void getSetQuaternion(
        Quaternion& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc,
        FileMode mode
    );
    static void getSetStringArray(
        std::vector<std::string>& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc,
        FileMode mode
    );
    static void getSetVector3Array(
        std::vector<Vector3>& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc,
        FileMode mode
    );

    //解析に成功したらtrue, 失敗したらfalse
    //true: nameから値をセット
    //false: 値を変えずにreturn
    static bool getInt(
        int& out,
        const char* name,
        const rapidjson::Value& inObject
    );
    static bool getFloat(
        float& out,
        const char* name,
        const rapidjson::Value& inObject
    );
    static bool getString(
        std::string& out,
        const char* name,
        const rapidjson::Value& inObject
    );
    static bool getBool(
        bool& out,
        const char* name,
        const rapidjson::Value& inObject
    );
    static bool getVector2(
        Vector2& out,
        const char* name,
        const rapidjson::Value& inObject
    );
    static bool getVector3(
        Vector3& out,
        const char* name,
        const rapidjson::Value& inObject
    );
    static bool getVector4(
        Vector4& out,
        const char* name,
        const rapidjson::Value& inObject
    );
    static bool getQuaternion(
        Quaternion& out,
        const char* name,
        const rapidjson::Value& inObject
    );
    static bool getStringArray(
        std::vector<std::string>& out,
        const char* name,
        const rapidjson::Value& inObject
    );
    static bool getVector3Array(
        std::vector<Vector3>& out,
        const char* name,
        const rapidjson::Value& inObject
    );

    //setter
    static void setInt(
        int value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc
    );
    static void setFloat(
        float value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc
    );
    static void setString(
        const std::string& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc
    );
    static void setBool(
        bool value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc
    );
    static void setVector2(
        const Vector2& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc
    );
    static void setVector3(
        const Vector3& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc
    );
    static void setVector4(
        const Vector4& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc
    );
    static void setQuaternion(
        const Quaternion& value,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc
    );
    static void setStringArray(
        const std::vector<std::string>& values,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc
    );
    static void setVector3Array(
        const std::vector<Vector3>& values,
        const char* name,
        rapidjson::Value& inObject,
        rapidjson::Document::AllocatorType& alloc
    );
};
