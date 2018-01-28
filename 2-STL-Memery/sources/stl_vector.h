#ifndef _MYSTL_INTERNAL_VECTOR_
#define _MYSTL_INTERNAL_VECTOR_
#include <iostream>
#include "stl_config.h"
#include "stl_construct.h"
#include "stl_alloc.h"
#include "type_traits.h"
__STL_BEGIN_NAMESPACE
template<typename T, typename Alloc = allocator<T>>
class vector {
private:
	T* start_;
	T* finish_;
	T* endOfStroger_;
	typedef Alloc dataAllocator;
public:
	typedef T                   value_type;
	typedef value_type*         pointer;
	typedef const value_type*   const_pointer;
	typedef value_type&         reference;
	typedef const value_type&   const_reference;
	typedef value_type*         iterator;
	typedef const value_type*   const_iterator;
	typedef ptrdiff_t           difference_type;
	typedef size_t              size_type;

public:
	vector() : start_(0), finish_(0), endOfStroger_(0) {}
	explicit vector(const size_type n);
	vector(const size_type n, const value_type& value);
	vector(const vector& v);
	vector& operator = (const vector& v);
	/*	
		vector(vector&& v);
		vector& operator = (vector&& v);
		template<typename InputIterator>
		vector(InputIterator first, InputIterator last);
	*/
	~vector();
public:
	bool operator == (const vector& v) const;
	bool operator != (const vector& v) const;
public:
	//Iterators
	iterator                begin() { return start_; }
	const_iterator          begin() const { return (start_); }
	const_iterator          cbegin() const { return (start_); }
	/*reverse_iterator        rbegin();
	const_reverse_iterator  crbegin() const;*/
	iterator                end() { return (finish_); }
	const_iterator          end() const { return (finish_); }
	const_iterator          cend() const { return (finish_); }
	/*reverse_iterator        rend();
	const_reverse_iterator  crend() const;*/

public:
	//Capacity
	difference_type size() const { return finish_ - start_; }
	//difference_type max_size(); what is this??
	difference_type capacity() const { return endOfStroger_ - start_; }
	bool empty() const { return start_ == finish_; }
	void reserve(size_type n);
	void shrink_to_fit();

public:
	//Element access
	reference operator[] (const difference_type i) { return *(begin() + i); }
	const_reference operator[] (const difference_type i) const { return *(cbegin() + i); }
	reference front() { return *(begin()); }
	reference back() { return *(end() - 1); }
	pointer data() { return start_; }

public:
	//Modifiers
	void clear();
	/*void resize(size_type n, value_type val = value_type());*/
	void swap(vector& v);
	void push_back(const value_type& value);
	void pop_back();
	iterator insert(iterator position, const value_type& val);
	iterator erase(iterator position);

public:
	//Alloc get_allocator();

private:
	inline void destoryAndDeallocateAll();

	template<class InputIterator>
	inline void allocateAndCopy(InputIterator first, InputIterator last);
	inline void allocateAndFillN(const size_type n, const value_type& value);
	/*
		template<class InputIterator>
		void reallocateAndCopy(iterator position, InputIterator first, InputIterator last);
		void reallocateAndFillN(iterator position, const size_type& n, const value_type& val);
	*/
};
//析构彧构造拷贝赋值
template<typename T, typename Alloc>
vector<T, Alloc>::vector(const size_type n){
  allocateAndFillN(n, value_type());
}
template<typename T, typename Alloc>
vector<T, Alloc>::vector(const size_type n, const value_type& value){
  allocateAndFillN(n, value);
}
template<typename T, typename Alloc>
vector<T, Alloc>::vector(const vector<T, Alloc>& v) {
	allocateAndCopy(v.begin(), v.end());
}
template<typename T, typename Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator = (const vector<T, Alloc>& v) {
	if (this != &v) {
		allocateAndCopy(v._State, v.finish_);
	}
	return *this;
}
template<typename T, typename Alloc>
vector<T, Alloc>::~vector() {
	destoryAndDeallocateAll();
}
//重载操作符
template<typename T, typename Alloc>
bool vector<T, Alloc>::operator == (const vector<T, Alloc>& v) const {
	if (size() != v.size()) {
		return false;
	}
	auto ptr_1 = start_;
	auto ptr_2 = v.start_;
	for (; ptr_1 != finish_ && ptr_2 != v.finish_; ++ptr_1, ++ptr_2) {
		if (*ptr_1 != *ptr_2) {
			return false;
		}
	}
	return true;
}
template<typename T, typename Alloc>
bool vector<T, Alloc>::operator != (const vector<T, Alloc> &v) const {
	return !(*this == v);
}

//容量相关操作
template<typename T, typename Alloc>
void vector<T, Alloc>::reserve(size_type n) {
	if (n > size()) {
		T* newstart_ = (T*)Alloc::allocate(n);
		T* newfinish_ = end() - begin() + newstart_;
		int i = 0;
		for (auto ptr = start_; ptr != finish_; ++ptr, ++i) {
			construct(newstart_ + i, *ptr);
		}
		destoryAndDeallocateAll();
		start_ = newstart_;
		finish_ = newfinish_;
		endOfStroger_ = start_ + n;
	}
	else {
		return;
	}
}
template<typename T, typename Alloc>
void vector<T, Alloc>::shrink_to_fit() {
	size_t s = capacity() - size();
	Alloc::deallocate(finish_, s);
	endOfStroger_ = finish_;
}
//修改元素操作
template<typename T, typename Alloc>
void vector<T, Alloc>::clear() {//clear 并不会影响vector的容量
	destory(start_, finish_);
	finish_ = start_;
}
template<typename T, typename Alloc>
void vector<T, Alloc>::swap(vector& v) {
	if (this != &v) {
		T* temp = start_;
		start_ = v.start_;
		v.start_ = temp;
		temp = finish_;
		finish_ = v.finish_;
		v.finish_ = temp;
		temp = endOfStroger_;
		endOfStroger_ = v.endOfStroger_;
		v.endOfStroger_ = temp;
	}
}

template<typename T, typename Alloc>
void vector<T, Alloc>::push_back(const value_type& value) {
	insert(end(), value);
}
template<typename T, typename Alloc>
void vector<T, Alloc>::pop_back() {
	--finish_;
	destory(finish_);
}

template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type& val) {
	const auto index = position - begin();
	difference_type leftStorage_ = endOfStroger_ - finish_;
	if (leftStorage_ >= 1) {
		auto ptr = end();
		for (; ptr != position; --ptr) {
			construct(ptr, *(ptr - 1));
		}
		construct(ptr, val);
		++finish_;
	}
	else {
		size_t cap = empty() ? 1 : 2 * capacity();
		reserve(cap);
		insert(begin() + index, val);
	}
	return begin() + index;
}
template<typename T, typename Alloc>
typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator position) {
	const auto index = position - begin();
	auto ptr = position;
	for (; ptr + 1 != end(); ++ptr) {
		destory(ptr);
		construct(ptr, *(ptr + 1));
	}
	--finish_;
	if (size() <= capacity() / 4) {
		reserve(2 * size());
	}
	return begin() + index;
}
template<typename T, typename Alloc>
void vector<T, Alloc>::destoryAndDeallocateAll(){
	if (capacity() != 0){
		destory(start_, finish_);
		Alloc::deallocate(start_, capacity());
	}
}
template<typename T, typename Alloc>
void vector<T, Alloc>::allocateAndFillN(const size_type n, const value_type& value) {
	start_ = Alloc::allocate(n);
	for(size_t i = 0; i < n; ++i){
		construct(start_ + i, value);
	}
	finish_ = endOfStroger_ = start_ + n;
}

template<typename T, typename Alloc>
template<typename InputIterator>
void vector<T, Alloc>::allocateAndCopy(InputIterator first, InputIterator last) {
	auto s = last - first;
	start_ = Alloc::allocate(s);
	for (size_t i = 0; i != s; ++i) {
		construct(start_ + i, *(first + i));
	}
	endOfStroger_ = finish_ = start_ + s;
}
__STL_END_NAMESPACE
#endif
