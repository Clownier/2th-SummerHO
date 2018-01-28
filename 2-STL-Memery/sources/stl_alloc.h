#ifndef _MySTL_INTRENAL_ALLOC_H
#define _MySTL_INTRENAL_ALLOC_H
#include <stddef.h>
#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <iomanip>
/*
空间配置器：
    功能：
        完成内存的配置和释放
    设计思想：
        1. 向system heap索要空间
        2. 考虑多线程(multi-threads)状态
        3. 考虑内存不足的应对措施(应该由用户提供)
        4. 考虑过多小型区块造成的内存破碎问题
    基本架构：
        1. 采用malloc与free管理内存的配置与释放
        2. 内存的配置采用二级空间适配器进行管理
         2.1 当要申请的内存大于128bytes，采用第一级适配器，直接进行malloc向system heap寻求空间
         2.2 当要申请的内存小于128bytes，采用第二级适配器，用memory pool策略进行管理
*/
__STL_BEGIN_NAMESPACE
/*
第一级空间适配器
	1. 使用malloc分配堆空间，使用free释放空间
	2. 当内存不足时触发处理函数
	3. 用户可以设置自己的内存不足处理函数
	4. 以void*为形参可以接收任意类型的指针
*/
template<int inst>
class malloc_alloc{
	public:
		static void* allocate(size_t n);//分配内存
		static void deallocate(void* p, size_t n); //释放空间
		static void* reallocate(void* p, size_t old_size, size_t new_size);//重新分配内存
		static void (*set_malloc_handler(void (*f)()))();//修改内存不足异常处理函数
	private:
		static void* oom_alloc(size_t t);				// malloc内存不足处理
		static void* oom_realloc(void* p, size_t t);			// realloc内存不足处理
		static void (*malloc_alloc_handler)(); // 内存不足异常处理函数
};

template<int inst>
void* malloc_alloc<inst>::allocate(size_t n){
	void* result = malloc(n);
	if(0 == result)
		result = oom_alloc(n);
	return result;
}

template<int inst>
void malloc_alloc<inst>::deallocate(void* p, size_t n){
	free(p);
}

template<int inst>
void* malloc_alloc<inst>::reallocate(void* p, size_t old_size, size_t new_size){
	void* result = realloc(p, new_size);
	if(0 == result)
		result = oom_realloc(result);
	return result;
}

template<int inst>
void (*malloc_alloc<inst>::set_malloc_handler(void(*f)()))(){
	void (*old_fun)() =  malloc_alloc_handler;
	malloc_alloc_handler = f;
	return (old_fun);
}

template<int inst>
void* malloc_alloc<inst>::oom_alloc(size_t n){
	void* result;
	for(;;){
		if(0 == malloc_alloc_handler){
			throw std::bad_alloc();
		}
		(*malloc_alloc_handler)();
		result = malloc(n);
		if(result)
			return (result);
	}
}

template<int inst>
void* malloc_alloc<inst>::oom_realloc(void* p, size_t t){
	void* result;
	for(;;){
		if(0 == malloc_alloc_handler){
			throw std::bad_alloc();
		}
		(*malloc_alloc_handler)();
		result = realloc(p, t);
		if(result)
			return (result);
	}
}
template<int inst>
void (*malloc_alloc<inst>::malloc_alloc_handler)() = 0;

typedef malloc_alloc<0> first_alloc;
/*第一级适配器结束*/


/*
第二级空间适配器
	1. 处理多次请求小内存的问题
	2. 基本思想：一次从堆中申请较大内存，以后每次有内存申请时从这一大块内存中取出
		合适的一小块。
	3. 维护已经malloc申请但未使用内存块，每次有新内存申请来到，从这些块中取出大小
		合适的（这给我们一个提示，我们可以将不同大小的内存分别在不同的地方维护，这样
		有新的申请直接去那个地方取就好，这就有了自由链表）
	4. 释放时，将其加入到相应大小的位置即可
*/

template<int inst> 
class default_alloc_template {
public:
	static void* allocate(size_t n);
	static void* reallocate(void* p, size_t old_size, size_t new_size);
	static void deallocate(void* p, size_t n);
private:
	enum { ALIGN = 8 };
	enum { MAX_BYTES = 128 };
	enum { NUM_LISTS = MAX_BYTES / ALIGN };
private:
	union Obj {
		union Obj* free_list_next;
		char client_data[1];
	};
private:
	static Obj* free_list[NUM_LISTS];
	static char* start_free;// 使用char*因为要进行算数运算，void*无法进行算术运算
	static char* end_free;
	static size_t heap_size;
private:
	static size_t round_up(size_t bytes);
	static size_t free_list_index(size_t bytes);
	static void* refill(size_t bytes);
	static char* chunk_alloc(size_t n, int& nobjs);
};
//管理 8, 16, 24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128
template<int inst>
typename default_alloc_template<inst>::Obj* 
	default_alloc_template<inst>::free_list[default_alloc_template<inst>::NUM_LISTS] = 
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,};

template<int inst>
char* default_alloc_template<inst>::start_free = 0;

template<int inst>
char* default_alloc_template<inst>::end_free = 0;

template<int inst>
size_t default_alloc_template<inst>::heap_size = 0;

template<int inst>
size_t default_alloc_template<inst>::round_up(size_t bytes){
	// bytes + bytes % ALIGN, CSAPP lab-1 !!!
	return ((bytes + (size_t)default_alloc_template<inst>::ALIGN-1) &
		 (~(default_alloc_template<inst>::ALIGN-1)));
}

template<int inst>
size_t default_alloc_template<inst>::free_list_index(size_t bytes){
	return ((bytes + (size_t)default_alloc_template<inst>::ALIGN-1)
		/(size_t)default_alloc_template<inst>::ALIGN - 1);
}

//当freelist中的空间不够时，调用这一函数，返回一个大小为n的对象，并且适当得增加freelist的节点
template<int inst>
void* default_alloc_template<inst>::refill(size_t n){
	int nobjs = 20;
	char* chunk = chunk_alloc(n, nobjs);
	if(1 == nobjs) return (chunk);
	Obj** my_free_list = free_list + free_list_index(n);
	*my_free_list = (Obj*)(chunk + n);
	Obj* result = (Obj*) chunk;
	Obj* current_ptr;
	Obj* next_ptr	= (Obj*)(chunk + n);
	for(int i = 1; ; ++i){
		current_ptr = next_ptr;
		next_ptr = (Obj*) ((char*)next_ptr + n);
		if(nobjs - 1 == i){
			current_ptr -> free_list_next = 0;
			break;
		}
		current_ptr	-> free_list_next = next_ptr;
	}
	return (result);
}
template<int inst>
char* default_alloc_template<inst>::chunk_alloc(size_t bytes, int& nobjs) {
	char* result;
	size_t total_size = bytes * nobjs;
	size_t bytes_left = end_free - start_free;
	if (bytes_left >= total_size) {
		result = start_free;
		start_free += total_size;
		return (result);
	}
	else if (bytes_left >= bytes) {
		nobjs = bytes_left / bytes;
		total_size = nobjs * bytes;
		result = start_free;
		start_free += total_size;
		return (result);
	}
	else {
		size_t bytes_to_get = 2 * total_size + round_up(heap_size >> 4);
		if (bytes_left > 0) {
			Obj** my_free_list = free_list + free_list_index(bytes_left);
			((Obj*)start_free)->free_list_next = *my_free_list;
			*my_free_list = (Obj*)start_free;
		}
		start_free = (char*)malloc(bytes_to_get);
		if (0 == start_free) {
			size_t i;
			Obj** my_free_list;
			Obj* p;
			for (i = bytes; i <= (size_t)MAX_BYTES; i += (size_t)ALIGN) {
				my_free_list = free_list + free_list_index(i);
				p = *my_free_list;
				if (0 != p) {
					*my_free_list = p->free_list_next;
					start_free = (char*)p;
					end_free = start_free + i;
					return(chunk_alloc(bytes, nobjs));
				}
			}
			end_free = 0;
			start_free = (char*)first_alloc::allocate(bytes_to_get);
		}
		heap_size += bytes_to_get;
		end_free = start_free + bytes_to_get;
		return (chunk_alloc(bytes, nobjs));
	}
}
//当需要配置的空间大于128bytes，调用第一级配置器
//否则，从自由链表中找到可用的区块，直接拿来用，若没有可用的区块，则调用refill，重新填充自由链表指示的区块
template<int inst>	
void* default_alloc_template<inst>::allocate(size_t n){
	if(n > (size_t)MAX_BYTES){
		return (first_alloc::allocate(n));
	}
	Obj** my_free_list = free_list + free_list_index(n);
	Obj* result = *my_free_list;
	if(0 == result){
		void* r = refill(round_up(n));
		/*std::cout << "after refill " << std::dec << n << " : " << std::endl;
		for (int i = 1; i <= 16; ++i) {
			std::cout << "  " << std::dec <<  8 * i << " " << std::endl;
			Obj* my = free_list[i - 1];
			while (my) {
				std::cout << "    " << std::hex << my << std::endl;
				my = my->free_list_next;
			}
		}*/
		return r;
	}
	*my_free_list = result->free_list_next;
	/*std::cout << "after allocate " << std::dec << n << " : " << std::endl;
	for (int i = 1; i <= 16; ++i) {
		std::cout << "  " << std::dec << 8 * i << " " << std::endl;
		Obj* my = free_list[i - 1];
		while (my) {
			std::cout << "    " << std::hex << my << std::endl;
			my = my->free_list_next;
		}
	}*/
	return (result);
}

template<int inst>
void* default_alloc_template<inst>::reallocate(void* p, size_t old_size, size_t new_size){
	if(old_size > (size_t)MAX_BYTES && new_size > (size_t)MAX_BYTES){
		return (first_alloc::reallocate(p, old_size, new_size));
	}
	if(round_up(old_size) == round_up(new_size)) return (p);
	void* result = allocate(new_size);
	size_t copy_size = old_size > new_size? new_size : old_size;
	std::memcpy(result, p, copy_size);
	deallocate(p, old_size);
	return (result); 
}

template<int inst>
void default_alloc_template<inst>::deallocate(void* p, size_t n) {
	if (n > (size_t)MAX_BYTES) {
		(first_alloc::deallocate(p, n));
		return;
	}
	Obj** my_free_list = free_list + free_list_index(n);
	Obj* q = (Obj*)p;
	q->free_list_next = *my_free_list;
	*my_free_list = q;
	//std::cout << "after deallocate " << std::dec << n << " : " << std::endl;
	/*for (int i = 1; i <= 16; ++i) {
		std::cout << "  " << std::dec << 8 * i << " " << std::endl;
		Obj* my = free_list[i - 1];
		while (my) {
			std::cout << "    " << std::hex << my << std::endl;
			my = my->free_list_next;
		}
	}*/
}
typedef default_alloc_template<0> second_alloc;

template<class T, class Alloc = second_alloc>
class allocator {
public:
	static T* allocate(size_t n) {
		return 0 == n ? 0 : (T*)Alloc::allocate(n * sizeof(T));
	}
	static T* allocate(void) {
		return (T*)Alloc::allcoate(sizeof(T));
	}
	static void deallocate(T* p, size_t n) {
		if (n != 0) Alloc::deallocate(p, n * sizeof(T));
	}
	static void deallocate(T* p) {
		Alloc::deallocate(p, sizeof(T));
	}
};
__STL_END_NAMESPACE
#endif/*_MySTL_INTRENAL_ALLOC_H*/
