#ifndef _MySTL_INTERNAL_ITERATOR_
#define _MySTL_INTERNAL_ITERATOR_
__STL_BEGIN_NAMESPACE
/*
STL迭代器设计原因
	1. 迭代器是将容器与算法相联系的一层粘合剂，是STL最重要的设计思想之一
	2. 容器应该具有方法返回自己的迭代器供算法使用，算法的设计与迭代器的实现应该没有联系
	3. 迭代器最重要的功能是解引用，取成员，即操作*与->，但是这显然需要知道具体实现细节，我们将他交给具体的容器去做
	4. 但是显然不可以每一个容器都定义自己的一套互不相关的迭代器，那样算法也需要对每套迭代器重新编写，这显然是不符合便准的
	5. 所以需要一个具体的标准设计迭代器，这就是这个文件的意义
STL迭代器设计思想
	1. 算法不知道容器的细节，唯一知道的就是迭代器，所以，算法需要干什么，我的迭代器就要支持它干什么
	2. 算法需要定义新的同类型变量，所以我们要可以从迭代器中得到原变量的类型
	3. 算法要对迭代器进行加减操作，所以容器要对迭代器进行运算符的重载
	4. 有些算法只需要读取迭代器内容，有些只需要写，有些只需要每次加一，而有些却需要每次加好多，所以我们有了迭代器的分类
STL迭代器设计技法
	1. 既然说了有这么多种类的迭代器，那么我们如何取用？每一种迭代器设计一种方法，根据迭代器的不同选择不同的方法。
	2. 如何根据迭代器推断它的类型，这和上一个问题其实相似，所以我们又需要定义一个可以得到迭代器类型的类
*/

//迭代器类型，因为我们仅仅需要类型，所以定义成空结构体就好，继承关系是因为我们需要对迭代器类型分层次，即哪一个可以被哪一个替代
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag{};
struct random_access_iterator_tag : public bidirectional_iterator_tag{};

template<typename Catagory, typename T, typename Difference = ptrdiff_t, typename Reference = T&, typename Pointer = T*>
struct iterator{
	typedef  Pointer 							pointer;
	typedef  Reference 						reference;
	typedef  T 										value_type;
	typedef  Difference 					difference_type;
	typedef  Catagory 						iterator_catagory;
};
/*例如：若有一算法想得到与迭代器类型完全相同的变量，则：
	template<Iterator>
	void foo(Iterator Itr){
		iterator_traits<Iterator>::value_type V; // 定义与该迭代器类型完全一样的变量
	}
	为何不直接写Iterator::value_type V呢？
	因为内置变量（如int）你无法定义value_type成员，所以只好加一层封装
*/
template<typename Iterator>
struct iterator_traits{
	typedef typename Iterator::pointer 							pointer;
	typedef typename Iterator::reference 						reference;
	typedef typename Iterator::value_type 					value_type;
	typedef typename Iterator::difference_type 			difference_type;
	typedef typename Iterator::iterator_catagory 		iterator_catagory;
};

template<typename T>
struct iterator_traits<T*>{
	typedef typename T*															pointer;
	typedef typename T& 														reference;
	typedef typename T 															value_type;
	typedef typename ptrdiff_t											difference_type;
	typedef typename random_access_iterator_tag 		iterator_catagory;
};

template<typename T>
struct iterator_traits<const T*>{
	typedef typename T*															pointer;
	typedef typename T& 														reference;
	typedef typename T 															value_type;
	typedef typename ptrdiff_t											difference_type;
	typedef typename random_access_iterator_tag 		iterator_catagory;
};

//一个advance函数
template<typename InputIterator, typename Distance>
inline void advance(InputIterator& i, Distance n){
	_advance(i, n, iterator_traits<InputIterator>::iterator_catagory);
}

template<typename InputIterator, typename Distance>
inline void _advance(InputIterator& i, Distance n, input_iterator_tag){
	while(n--) ++i;
}

template<typename BidirectionalIterator, typename Distance>
inline void _advance(BidirectionalIterator& i, Distance n, bidirectional_iterator_tag){
	if(n > 0)
		while(n--) ++i;
	else
		while(n++) --i;
}

template<typename RandomIterator, typename Distance>
inline void _advance(RandomIterator& i, Distance n){
	i += n;
}
__STL_END_NAMESPACE
#endif