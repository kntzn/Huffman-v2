#pragma once
#include <assert.h>

template <typename T, size_t len> class sarray
    {
    private:
        T container [len];

        size_t count;

    public:
        sarray ();

        ~sarray ();

        bool push_back (T elem);
        bool pop_back ();

        T & operator[](const size_t idx) const;

        //T &operator[] (const size_t idx) const;

        T* data ();

        T back ();

        size_t size ();
        size_t max_size ();

    };

template<typename T, size_t len>
inline sarray<T, len>::sarray ():
    count (0),
    container ()
    {
    }

template<typename T, size_t len>
inline sarray<T, len>::~sarray ()
    {
    }

template<typename T, size_t len>
inline bool sarray<T, len>::push_back (T elem)
    {
    // array is full
    assert (count < len);

    container [count++] = elem;

    return true;
    }

template<typename T, size_t len>
inline bool sarray<T, len>::pop_back ()
    {
    count--;
    
    return true;
    }

template<typename T, size_t len>
inline T & sarray<T, len>::operator[] (const size_t idx) const
    {
    /*index out of range*/
    if (!(0 <= idx && idx < count))
        std::cout << idx << "/" << count << std::endl;
    assert (0 <= idx && idx < count);

    return T (container [idx]);
    }

template<typename T, size_t len>
inline T * sarray<T, len>::data ()
    {
    return container;
    }

template<typename T, size_t len>
inline T sarray<T, len>::back ()
    {
    return container [count - 1];
    }

template<typename T, size_t len>
inline size_t sarray<T, len>::size ()
    {
    return size_t (count);
    }

template<typename T, size_t len>
inline size_t sarray<T, len>::max_size ()
    {
    return size_t (len);
    }
