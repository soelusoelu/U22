#include "SaveModelViewerCollider.h"
#include "../../Utility/FileUtil.h"
#include "../../Utility/LevelLoader.h"
#include <string>

void SaveModelViewerCollider::save(const GameObject& target) {
    //保存するディレクトリとファイル名を取得する
    std::string filePath, filename;
    if (FileUtil::saveFileDialog(filePath, filename)) {
        auto savePath = FileUtil::getAssetsFromAbsolutePath(filePath);
        auto saveDirectory = FileUtil::getDirectryFromFilePath(savePath);

        //保存
        LevelLoader::saveOnlyComponents(target, filename, saveDirectory);
    }
}
