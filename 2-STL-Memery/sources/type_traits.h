#ifndef _MYSTL_INTERNAL_TRAITS
#define _MYSTL_INTERNAL_TRAITS

#ifndef _MySTL_INTERNAL_CONFIG_H
#include "stl_config.h"
#endif // !_MySTL_INTERNAL_CONFIG_H

__STL_BEGIN_NAMESPACE
/*
	traits负责得到型别的特性，决定这个类型是否：
		1. 需要默认构造
		2. 需要默认析构
	is_integer(T) 将返回T是否为int的扩展类型
*/
struct _true_type{};
struct _false_type {};
template<typename T>
struct type_traits {
	typedef _false_type	has_trivial_default_constructor;
	typedef _false_type	has_trivial_destructor;
};

template<>
struct type_traits<bool>{
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<char> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<signed char> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<unsigned char> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<wchar_t> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<short> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<unsigned short> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<int> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<unsigned int> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<long> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<unsigned long> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<long long> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<unsigned long long> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<float> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<double> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template<>
struct type_traits<long double> {
	typedef _true_type	has_trivial_default_constructor;
	typedef _true_type	has_trivial_destructor;
};
template <typename T>
struct type_traits<T*> {
	typedef _true_type    has_trivial_default_constructor;
	typedef _true_type    has_trivial_destructor;
};

template<typename T>
struct is_integer{
	typedef _false_type	_Integer;
};
template<>
struct is_integer<bool> {
	typedef _true_type	_Integer;
};
template<>
struct is_integer<char> {
	typedef _true_type	_Integer;
};
template<>
struct is_integer<signed char> {
	typedef _true_type	_Integer;
};
template<>
struct is_integer<unsigned char> {
	typedef _true_type	_Integer;
};
template<>
struct is_integer<wchar_t> {
	typedef _true_type	_Integer;
};
template<>
struct is_integer<short> {
	typedef _true_type	_Integer;
};
template<>
struct is_integer<unsigned short> {
	typedef _true_type	_Integer;
};
template<>
struct is_integer<int> {
	typedef _true_type	_Integer;
};
template<>
struct is_integer<unsigned int> {
	typedef _true_type	_Integer;
};
template<>
struct is_integer<long> {
	typedef _true_type	_Integer;
};
template<>
struct is_integer<unsigned long> {
	typedef _true_type	_Integer;
};
template<>
struct is_integer<long long> {
	typedef _true_type	_Integer;
};
template<>
struct is_integer<unsigned long long> {
	typedef _true_type	_Integer;
};
__STL_END_NAMESPACE
#endif // _MYSTL_INTERNAL_TRAITS
