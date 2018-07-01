#ifndef ALGORITHMS_COMPARISON_H_
#define ALGORITHMS_COMPARISON_H_

#define less(T)\
    int less_##T(const void *a, const void *b) {\
        T aa = *(T*) a;\
        T bb = *(T*) b;\
        if (aa < bb) return -1;\
        else if (aa > bb) return 1;\
        else return 0;\
    }

#define greater(T)\
    int greater_##T(const void *a, const void *b) {\
        T aa = *(T*) a;\
        T bb = *(T*) b;\
        if (aa > bb) return -1;\
        else if (aa < bb) return 1;\
        else return 0;\
    }

#endif /* ALGORITHMS_COMPARISON_H_ */
