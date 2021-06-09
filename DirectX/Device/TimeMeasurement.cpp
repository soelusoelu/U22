#include "TimeMeasurement.h"
#include "../Engine/DebugManager/DebugUtility/Debug.h"
#include "../Utility/StringUtil.h"

TimeMeasurement::TimeMeasurement(int numMeasurements)
    : mStart()
    , mCount(0)
    , mNumMeasurements(numMeasurements)
{
    mMeasurements.resize(numMeasurements);
}

TimeMeasurement::~TimeMeasurement() = default;

void TimeMeasurement::start() {
    mStart = std::chrono::system_clock::now();
}

double TimeMeasurement::end() {
    auto end = std::chrono::system_clock::now();
    auto dur = end - mStart;
    //auto result = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
    auto result = static_cast<double>(dur.count() / 1000.0);

    if (mCount < mNumMeasurements) {
        mMeasurements[mCount] = result;
        ++mCount;

        if (mCount == mNumMeasurements) {
            auto avg = static_cast<float>(calcAverage());
            Debug::log(StringUtil::floatToString(avg, 6));

            mCount = 0;
        }
    }

    return result;
}

double TimeMeasurement::calcAverage() const {
    //配列の合計値を求める
    double sum = 0.0;
    for (const auto& x : mMeasurements) {
        sum += x;
    }

    //配列の平均を求める
    auto avg = sum / mMeasurements.size();

    return avg;
}
