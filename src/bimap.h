#ifndef BIMAP_H
#define BIMAP_H

#include <map>

template<typename T, typename R>
class bimap
{
public:
    explicit bimap() {}
    virtual ~bimap() {}

    void put(T t, R r) {
        erase(t);
        erase(r);
        tKeyMap[t] = r;
        rKeyMap[r] = t;
    }

    void put(R r, T t) {
        erase(t);
        erase(r);
        rKeyMap[r] = t;
        tKeyMap[t] = r;
    }

    void erase(T t) {
        R value = tKeyMap[t];
        tKeyMap.erase(t);
        rKeyMap.erase(value);
    }

    void erase(R r) {
        T value = rKeyMap[r];
        rKeyMap.erase(r);
        tKeyMap.erase(value);
    }

    typename std::map<T, R>::size_type count(T t) const {
        return tKeyMap.count(t);
    }

    typename std::map<R, T>::size_type count(R r) const {
        return rKeyMap.count(r);
    }

    const std::map<T, R>& getMap() const {
        return tKeyMap;
    }

    const std::map<R, T>& getInverted() const {
        return rKeyMap;
    }

private:
    std::map<T, R> tKeyMap;
    std::map<R, T> rKeyMap;
};

#endif // BIMAP_H
