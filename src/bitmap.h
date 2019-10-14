//
// Created by bodguy on 2019-10-14.
//

#ifndef GLYPH_LOADING_BITMAP_H
#define GLYPH_LOADING_BITMAP_H

#include <vector>

template<typename T>
class bitmap {
public:
    bitmap();
    bitmap(int w, int h, T val);

    void clear(int w, int h, T val);
    int get_idx(const int x, const int y) const;
    T get(const int x, const int y) const;
    void set(const int x, const int y, const T val);
    bool replace_part(bitmap<T> const& other, int x_left, int y_bottom);

    int width, height;
    std::vector<T> data;
};

template <typename T>
bitmap<T>::bitmap() :width(0), height(0), data() {

}

template <typename T>
bitmap<T>::bitmap(int w, int h, T val) :width(w), height(h), data() {
    clear(w, h, val);
}

template <typename T>
void bitmap<T>::clear(int w, int h, T val) {
    data.resize(w*h);
    std::fill(data.begin(), data.end(), val);
}

template <typename T>
int bitmap<T>::get_idx(const int x, const int y) const {
    if (x < 0 || y < 0 || x > width || y > height) {
        return -1;
    }
    int rows_from_bottom = (height - 1) - y;
    int arr_pos = width * rows_from_bottom;
    arr_pos += x;
    return arr_pos;
}

template <typename T>
T bitmap<T>::get(const int x, const int y) const {
    int idx = get_idx(x, y);
    assert(idx >= 0);
    return data[idx];
}

template <typename T>
void bitmap<T>::set(const int x, const int y, const T val) {
    int idx = get_idx(x, y);
    if (idx < 0) {
        return;
    }
    data[idx] = val;
}

template <typename T>
bool bitmap<T>::replace_part(bitmap<T> const& other, int x_left, int y_bottom) {
    if ((x_left + other.width) > width || (y_bottom + other.height) > height) {
        return false;
    }

    for (int row = 0; row < other.height; row ++) {
        for (int col = 0; col < other.width; col++) {
            set(col + x_left, row + y_bottom, other.get(col, row));
        }
    }
    return true;
}

#endif //GLYPH_LOADING_BITMAP_H
