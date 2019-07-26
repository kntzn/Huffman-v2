#pragma once

template <typename T> void QuickSort (T* arr, size_t array_size, bool (*compare) (T left, T right), unsigned left = 0, unsigned right = -1)
    {
    if (right == -1)
        right = array_size;

    unsigned CurL = left;
    unsigned CurR = right;
    T CurMid = arr [(left + right) / 2];

    while (CurL <= CurR)
        {
        while (arr [CurL] < CurMid) CurL++;
        while (arr [CurR] > CurMid) CurR--;

        if (CurL <= CurR)
            {
            if (!compare (arr [CurL], arr [CurR]))
                {
                T temp_left = arr [CurL];
                arr [CurL]  = arr [CurR];
                arr [CurR]  = temp_left;
                }

            CurL++;
            CurR--;
            }

        if (CurL > CurR) break;
        }

    if (CurL < right) QuickSort (arr, array_size, compare, CurL, right);
    if (left < CurR)  QuickSort (arr, array_size, compare, left, CurR);

    }

