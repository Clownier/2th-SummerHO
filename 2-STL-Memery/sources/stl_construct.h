#ifndef _MySTL_INTERNAL_CONSTRUCT_H_
#define _MySTL_INTERNAL_CONSTRUCT_H_

#include <new>
/* 
    功能: 
        1. 内存分配后的建构功能：construct
        2. 内存释放前的解构功能：destory    
    全局函数：
        1. void construct(T1* p, T2& V) 
        2. void construct(T* p)
        2. void destory(T* p)
        3. void destory(ForwardIterator f, ForwardIterator l)
        4. void destory(...*, ...*)
    详细说明：
        1. construct() 函数调用 placement new 将从alloc分配好的内存给V并进行构造。
        2. destory() 第一个版本调用析构函数
        3. destory() 第二个版本对每一个迭代器对应的元素调用第一个版本的destory()
*/
__STL_BEGIN_NAMESPACE
template<typename T1, typename T2> 
inline void construct(T1* p, T2& value) {
	new ((void*)p) T1(value);
}

template<typename T> 
inline void construct(T* p) {
	new ((void*)p) T();
}

template<typename T>
inline void destory(T* p) {
	p->~T();
}

// template<typename ForwardIterator>
// inline void _destory(ForwardIterator f, ForWardIterator l, _ture_type){}

template<typename ForwardIterator>
inline void _destory(ForwardIterator f, ForwardIterator l) {
	for (; f != l; ++f) {
		destory(&*f);
	}
}

template<typename ForwardIterator>
inline void destory(ForwardIterator f, ForwardIterator l) {
	_destory(f, l);
}

inline void destory(char*, char*){}
inline void destory(int*, int*) {}
inline void destory(float*, float*) {}
inline void destory(double*, double*) {}
#ifdef _MySTL_HAS_WCHAR_T
inline void destory(wchar_t*, wchar_t*) {}
#endif/*_MySTL_HAS_WCHAR_T*/

__STL_END_NAMESPACE

#endif/*_MySTL_INTERNAL_CONSTRUCT_H_*/
