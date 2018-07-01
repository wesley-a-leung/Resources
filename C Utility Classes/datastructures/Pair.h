#ifndef DATASTRUCTURES_PAIR_H_
#define DATASTRUCTURES_PAIR_H_

#define pair(T1, T2)\
    typedef struct {\
        T1 first;\
        T2 second;\
    } pair_##T1##_##T2;\
    \
    pair_##T1##_##T2 make_pair_##T1##_##T2(T1 first, T2 second) {\
        pair_##T1##_##T2 ret;\
        ret.first = first;\
        ret.second = second;\
        return ret;\
    }

#endif /* DATASTRUCTURES_PAIR_H_ */
