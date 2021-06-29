#pragma once

#include <cassert>

template <typename T>
inline void safeDelete(T*& p) {
    delete p;
    p = nullptr;
}

template <typename T>
inline void safeDeleteArray(T*& p) {
    delete[] p;
    p = nullptr;
}

template <typename T>
inline void safeRelease(T*& p) {
    if (p) {
        p->Release();
        p = nullptr;
    }
}

template<typename TO, typename FROM>
inline TO checkedCast(FROM obj) {
#if _DEBUG
    if (obj) {
        TO ret = dynamic_cast<TO>(obj);
        assert(ret);
        return ret;
    }
#endif // _DEBUG
    return static_cast<TO>(obj);
}
