# STL标准库和泛型编程

所谓Generic Programming(GP，泛型编程)，就是使用template(模板)为主要工具来编写程序。

> 使用一个东西，
>
> 却不明白它的道理，
>
> 不高明！



## 认识headers、版本、重要资源

> C++ Standardf Library (C++ 标准库)
>
> **S**tandard **T**emplate **L**ibrary (STL，标准模板库)



### 标准库以header files形式呈现

* C++标准库的header files不带扩展名(.h)，例如 `#include <vector>`
* 新式C header files不带扩展名(.h)，例如 `#include <cstdio>`
* 旧式C header files(带有扩展名.h)仍然可用，例如 `#include <stdio.h>`
* 新式headers内的组件封装于namespace "std"
  * using namespace std;
  * using std::cout; 
* 旧式header内的组件不封装于namespace "std"



### C++标准库参考网站

* [www.cplusplus.com](www.cplusplus.com)
* [www.cppreference.com](www.cppreference.com)
* [gcc.gnu.org](gcc.gnu.org)



## STL体系机构基础介绍

### STL六大部件(Components):

* 容器(Containers)
* 分配器(Allocators)
* 算法(Algorithms)
* 迭代器(Iterators)
* 适配器(Adapters)
* 仿函式(Functors)



### 六大部件之间的关系

<img src=".\picture\六大部件之间的关系.png" style="zoom:50%; float:left;" />

**容器**解决了管理内存的问题，所以它需要**分配器**来做支撑，对数据所做的操作被写成了模板函数放在了**算法**中，而**迭代器**作为容器和算法之间的桥梁，是一种泛化的指针，供算法操作容器中的数据。**仿函式**向算法解释容器中数据与操作之间的关系&规则，供算法去调用。**适配**器能够对容器、算法、仿函数进行转换，从而使其于其他组件适配。



### 代码示例及解释

```cpp
#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>

using namespace std;

int main() {
    int ia[6] = {27, 210, 12, 47, 109, 83};
    vector<int,allocator<int>> vi(ia,ia+6);
    cout << count_if(vi.begin(), vi.end(),
            not1(bind2nd(less<int>(), 40)));
    return 0;
}
```

上面的代码中`vector`就是一个**容器**，vector后的尖括号中，第一个参数`int`指明了容器中元素的数据类型，第二个参数`allocator<int>`为容器配置了一个**分配器**，用于分配内存，并且这个分配器也是一个模板，也要写明分配的内存的数据类型。

`count_if`则是一个**算法**，参数列表的前两项就是**迭代器**，即`vi.begin()`和`vi.end()`。

`not1`和`bind2nd`均为仿函式**适配器**，前者对运行结果进行取反操作，后者会将仿函式的第二个参数进行了绑定，像上面的`less<int>()`有两个参数A、B，该函数会返回A小于B的结果，而`bind2nd`就将40绑定到了第二个参数B上，再通过`not1`将结果取反。

所以算法`count_if`会通过给定的容器的迭代器，获取容器中的数据，并结合适配器修饰后的仿函式执行算法，上述代码的结果就是求出容器中大于等于40的元素个数。



### "前闭后开" 区间

上面介绍到的迭代器`vi.begin()`和`vi.end()`，第一个begin()指向容器的第一个元素，而end()指向的是最后一个元素的下一个位置，所以对于一个容器来说，可以用数学中的“前闭后开”区间，生动形象地形容它迭代器`begin()`和`end()`框出的容器范围。

**用迭代器遍历一个容器中的所有元素**

```cpp
Container<T> c;
Container<T>::iterator ite = c.begin();
for (; ite != c.end(); ++ite)
	...
```

这是C++原始的写法，显示地用`Container<T>::iterator`去声明一个迭代器`ite`，然后再利用`++`运算符不断将迭代器移向下一个位置，直到遇见了迭代器`end()`，而在C++11中，提供了基于范围的for循环遍历，代码如下：

```cpp
std::vector<double> vec;
for ( auto elem : vec) {
	std::cout << elem << std::endl;
}
for ( auto& elem : vec) {
	elem *= 3;
}
```

 上面的`auto`关键字能够自动地识别变量的数据类型，从而精简代码，而第二个for循环中，只有通过`auto&`，加上引用才能够做到修改容器中的数据，否则`elem`就只是容器中一个元素的拷贝，for循环中的语句将不会对容器中的数据产生任何影响。



## 容器-结构与分类

容器的分类大致上分为两种，一种是**序列式容器(Sequence Containers)**，另一种是**关联式容器(Associative Containers)**。

关联式容器，是通过键值对来存储数据，因为用key来找东西很快，所以很适合用来查找数据。

除了上面的两种容器，还有第三种容器叫**不定序容器(Unordered Containers)**，是C++11所新增加的，其底层是用哈希表实现的。



###  序列式容器

#### array

array是C++11新增加的一个封装数组，在生成实例对象时就申请了固定的一块内存，后续是无法发生改变的。

在使用array时需要传入两个参数，一个是存储元素的数据类型，另一个是个常量，固定容器的大小。

然后生成随机数塞入array容器中，并用clock()函数计算存储数据所用的时间，和快速排序及二分查找的时间。

```cpp
#include <iostream>
#include <array>
#include <ctime>

using std::cin;
using std::cout;
using std::endl;
using std::string;

const int ASIZE = 500000;

long get_a_target_long() {
    cout << "please input a target number: ";
    long target;
    cin >> target;
    return target; 
}

int compareLongs(const void* a, const void* b) {
    return (*(long*)a - *(long*)b);
}

namespace jj01 {

void test_array() {
    cout << "test_array()..............\n";
    
    std::array<long,ASIZE> c;
    
    clock_t timeStart = clock();
    for(int i=0; i < ASIZE; ++i) {
        c[i] = rand();
    }
    cout << "milli-seconds: " << (clock() - timeStart) << endl;
    cout << "array.size()= " << c.size() << endl;
    cout << "array.front()= " << c.front() << endl;
    cout << "array.back()= " << c.back() << endl;
    cout << "array.data()= " << c.data() << endl;

    long target = get_a_target_long();
    
    timeStart = clock();
    std::qsort(c.data(), ASIZE, sizeof(long), compareLongs);
    long* pItem = (long*) std::bsearch(&target, (c.data()), ASIZE, sizeof(long), compareLongs);
    cout << "qsort() + bsearch(), milli-seconds: " << (clock() - timeStart) << endl;
    if(pItem != NULL)
        cout << "found, " << *pItem << endl;
    else
        cout << "not found!" << endl;
}

}

int main() {
    jj01::test_array();
    return 0;
}
/* 运行结果
test_array()..............
milli-seconds: 23
array.size()= 500000
array.front()= 41
array.back()= 29794
array.data()= 0x437a84
please input a target number: 20000
qsort() + bsearch(), milli-seconds: 57
found, 20000
*/
```

#### vector

vector容器支持动态内存扩充，在声明一个容器后，通过调用vector的`capacity()`函数可知，容器当前分配的内存空间为0，在`push_back()`一个元素后，它分配的内存空间就变为了1，通过不断的插入元素、输出已分配的内存空间数，可以发现，vector在没有空间可以容纳新元素时，会申请自身两倍的内存空间，即如果当前vector中有16个元素，再插入一个新的元素后，vector会申请32个内存空间， 然后把原先的16个元素全部转移过去，再插入第17个元素，随后直到容器插入第33个元素时，会再申请64个内存空间用于容纳新的元素。

以下代码演示了vector的测试过程，同样是测试插入元素和查找元素的速度：

```cpp
string get_a_target_string() {
    cout << "please input a target number: ";
    long target;
    cin >> target;
    char buf[10];
    snprintf(buf, 10, "%d", target);
    return string(buf); 
}

int compareStrings(const void* a, const void* b) {
    if( *(string*)a > *(string*)b) return 1;
    else if( *(string*)a < *(string*)b) return -1;
    else return 0;
}

namespace jj02 {

void test_vector() {
    cout << "test_vector()........\n";
    cout << "How many numbers? ";
    long value;
    cin >> value;

    std::vector<string> c;
    char buf[10];
    
    clock_t timeStart = clock();
    for(long i=0; i < value; ++i) {
        try {
            snprintf(buf, 10, "%d", rand());
            c.push_back(string(buf));
        }
        catch(exception& p) {
            cout << "i= " << i << " " << p.what() << endl;
            abort();
        }
    }
    cout << "milli-seconds: " << (clock() - timeStart) << endl;
    cout << "vector.size()= " << c.size() << endl;
    cout << "vector.front()= " << c.front() << endl;
    cout << "vector.back()= " << c.back() << endl;
    cout << "vector.data()= " << c.data() << endl;
    cout << "vector.capacity()= " << c.capacity() << endl;
    
    string target = get_a_target_string();
    {
    timeStart = clock();
    auto pItem = std::find(c.begin(), c.end(), target);
    cout << "::find(), milli-seconds= " << (clock() - timeStart) << endl;
    if(pItem != c.end())
        cout << "found, " << *pItem << endl;
    else
        cout << "not found!" << endl;
    }
    {
    timeStart = clock();
    std::sort(c.begin(), c.end());
    string* pItem = (string*)bsearch(&target, (c.data()), c.size(), sizeof(string), compareStrings);
    cout << "sort() + bsearch(), milli-seconds= " << (clock() - timeStart) << endl;  
    if(pItem != NULL)
        cout << "found, " << *pItem << endl;
    else
        cout << "not found!" << endl;
    }
}

}
/*
test_vector()........
How many numbers? 100000
milli-seconds: 346
vector.size()= 100000
vector.front()= 41
vector.back()= 1629
vector.data()= 0x14e0020
vector.capacity()= 131072
please input a target number: 23456
::find(), milli-seconds= 7
found, 23456
sort() + bsearch(), milli-seconds= 115
found, 23456
*/
```

#### list

list容器是一个双向链表， 在插入一个元素后，它就会申请一块相应大小的内存空间，所以内存的扩充是很缓慢的，并不像vector会造成空间的浪费，在插入元素操作上是非常快的，但相应的也就无法实现随机存取。

#### forward_list

forward_list是C++11新增加的一个单向链表容器，其设计目标是达到与最好的手写单向链表数据结构相当的性能，仅提供`push_front()`函数用于将新元素插入链表的头部，因为将元素插入链表的尾部这个操作是极其的缓慢的，它不能够实现随机存取。需要注意的是它没有`size()`函数用于查看链表中元素的个数，因为保存或计算其大小会比手写链表多出额外的开销

#### deque

deque是一个双端队列，能够从前后插入新元素，其实现是由一连串一连串的内存拼接而成，在往前或往后无法插入新元素后，deque会申请一连串内存用于容纳新的元素。而deque对外“宣称”它是一整块连续的内存，并能够实现随机存取，但其实这仅仅是一种假象。

#### queue & stack

queue和stack，一个能够做到“一端进另一端出”，一个能够做到“一端进同一端出”，其功能是deque的子集，所以在其实现上是对deque双端队列的再一次封装，所以有人认为queue和stack不应该被认定为容器，而是一个容器的适配器(Container Adapter)。



### 关联式容器

#### set & map

set和map底层都提供了排序功能，红黑树形式存储的键值是有序的。

#### multiset & multimap

相比于上面的set和map，multiset和multimap允许插入相同的key，即对于multiset来说，不会对相同的元素去重，相同的key都会被储存下来，而对于multimap来说，同一个key值，可以映射到多个value上，这样的话，multimap就不能做到像map那样用`[ ]`运算符传入key来访问value。



### 不定序容器

#### unordered_set & unordered_map & unordered_multiset & unordered_multimap

带有unordered前缀的容器都是采用哈希表来实现数据的存储，要注意的是对于这些特殊的容器，包括之前提到的list和forward_list，它们自身会提供一些成员函数，如sort()或find()，相比于STL提供的统一的std::find()和std::sort()函数要更加的快速，因为其根据存储的不同特点重写了更优的算法，所以对于重复功能的函数，优先选择容器自带的函数。



## 分配器及其使用

对于一个分配器，它最重要的作用就是申请和释放内存，对应的方法就是`allocator`和`deallocator`，从下面的VC6所附标准库的源码看来，它的`allocator`和`deallocator`调用了`operator new`和`operator delete`方法，而这两个方法最终也是调用了C的`malloc`和`free`函数，所以说分配器是对`malloc`和`free`的封装再封装。

``` cpp
template <class _Ty>
class allocator {
public:
    typedef _SIZT size_type;
    typedef _PDFT difference_type;
    typedef _Ty _FARQ *pointer;
    typedef _Ty value_type;
    pointer allocate(size_type _N, const void *) {
        return (_Allocator((difference_type)_N, (pointer)0));
    }
    void deallocator(void _FARQ *_P, size_type) {
		operator delete(_P);
    }
}
=============================================================
//上面用到的_Allocator定义
template <class _Ty> inline
_Ty _FARQ *_Allocator(_PDFT _N, _Ty _FARQ *) {
    if(_N < 0) _N = 0;
    return ((_Ty _FARQ *)operator new((_SIZT)_N * sizeof(_Ty)));
}
=============================================================
// operator new对malloc的封装
// ...\vc98\crt\src\newop2.cpp
void *operator new(size_t size, const std::nothrow_t&) _THROW0() {
    void *p;
    while((p=malloc(size))==0) {
        _TRY_BEGIN
		if(_callnewh(size)==0) break;
        _CATCH(std::bad_alloc) return(0);
        _CATCH_END
    }
    return (p);
}
// <new.h> of CB5
inline void* _RTLENTRY operator new(size_t size, const std::nothrow_t&) {
    size = size ? size : 1;
    return malloc(size);
}
```

对于每个容器，我们是看不到其背后的内存分配过程的，因为容器作为一个工具，使用者不用去关注到其内存是如何分配的，这些工作全部转交给分配器去完成，甚至在声明一个容器时，都不用选择分配器，而是采用默认的分配器去执行。

如下是容器vector的模板头代码

```cpp
template < class T, class Alloc = allocator<T> > class vector;
```

后面的`allocator<T>`会根据用户输入的数据类型自动生成分配器

如果想要使用`std::allocator`以外的`allocator`，得自行`#include <ext\...>`， 如下(GNU环境下)：

```cpp
#include <ext\array_allocator.h>
#include <ext\mt_allocator.h>
#include <ext\debug_allocator.h>
#include <ext\pool_allocator.h>
#include <ext\bitmap_allocator.h>
#include <ext\malloc_allocator.h>
#include <ext\new_allocator.h>
```

分配器在使用时，申请要传入申请的内存数量，释放内存时也要传入内存数量。这对于编程来说是不合理的，一般我们不会去直接使用分配器。使用的例子如下。

```cpp
int* p;
std::allocator<int> alloc1;
p = alloc1.allocate(1);
*p = 233;
cout << *p << endl;
alloc1.deallocate(p,1);
```

## OOP vs GP

> OOP(Object-Oriented Programming，面向对象编程)
>
> GP(Generic Programming，泛型编程) 

OOP企图将datas和methods联系在一起，形成一个类，而GP则是想让它们结合起来。在C++标准模板库中，OOP的思想不是很明显，没有很多的继承和虚函数。

采用GP有两个优点：

* Containers和Algorithms团队可以各自闭门造车，其中以Iterator沟通即可。
* Algorithms通过Iterator确定操作范围，并通过Iterator却用Container元素。

举个例子，下面两份代码，一份是两个容器(vector和deque)，另一份是排序算法的函数头代码。

```cpp
template < class T, class Alloc = allocator<T> >
class vector {...};
template < class T, class Alloc = allocator<T> >
class deque {...};
```

这两个容器都提供 RandomAccessIterator(随机访问存储器)。

```cpp
template <class RandomAccessIterator>
void sort (RandomAccessIterator first, RandomAccessIterator last) {...}
template <class RandomAccessIterator, class Compare>
void sort (RandomAccessIterator first, RandomAccessIterator last, Compare comp) {...}
```

从sort的参数列表来看，我们只需要传入两个随机访问存储器，就可以完成对容器中数据的排序，如`::sort(c.begin(),c.end())`。另外可以注意到存在第三个参数`Compare`，它的作用是传入一个仿函式(Functor)，用于对一些数据的大小关系进行定义。

下面我们就来利用仿函式定义一下字符串之间的大小关系。

```cpp
template <class T> 
const T& max (const T& a, const T& b) {
    return a < b? b : a;
}
template <class T, class Compare>
const T& max (const T& a, const T& b, Compare comp) {
    return comp(a, b) ? b : a;
}
bool strLonger(const string& s1, const string& s2) {
    return s1.size() < s2.size();
}
cout << "max of zoo and hello: " << max(string("zoo"), string("hello")) << endl; // zoo
cout << "max of zoo and hello: " << max(string("zoo"), string("hello"), strLonger) << endl; // hello
```

上面的代码第一个输出是`zoo`，因为max会使用string类中的运算符`operator <`来比较两个字符串的大小，默认是按字典序排，所以zoo比hello要大。第二个输出是`hello`，因为我们传入了一个仿函式`strLonger`，算法会根据仿函式的逻辑执行，在strLonger中我们认为字符串大小由其长度决定，故输出的是较长的hello。



## 运算符重载 & 模板

> Operator Overloading 操作符重载
>
> Template 模板



### 运算符重载

运算符重载可以让我们更好的去设计一个工具的各种行为，例如迭代器本身是一个泛化的指针，那么我们就需要去重载它的`*`、`->`、`++`运算符等等，下面是一部分源码：

```cpp
template <class T, class Ref, class Ptr>
struct __list_Iterator {
    typedef __list_Iterator<T, Ref, Ptr> self;
    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef __list_node<T>* link_type;
    typedef ptrdiff_t difference_type;
    
    link_type node;
    
    reference operator*() const { return (*node).data; }
    pointer operator->() const  { return &(operator*()); }
    self& operator++() 			{ node = (link_type)((*node).next); return *this; }
    self operator++(int) 		{ self tmp = *this; ++*this; return tmp; }
    ...
};
```

在重载运算符时，可以是成员函数，也可以是非成员函数，规则如下：

| Expression | As member function | As non-member function     |
| ---------- | ------------------ | -------------------------- |
| @a         | (a).operator@( )   | operator@(a)               |
| a@b        | (a).operator@(b)   | operator@(a,b)             |
| a=b        | (a).operator=(b)   | :fire:cannot be non-member |
| a[b]       | (a).operator[](b)  | :fire:cannot be non-member |
| a->        | (a).operator->( )  | :fire:cannot be non-member |
| a@         | (a).operator@(0)   | operator@(a,0)             |



### 类模板

阅读C++标准库需要的另一个基础就是模板，下面是一个描述复数的模板类，对于的一个复数拥有实部和虚部，它可以是浮点数，也可以是个整数。我们在设计的时候可以不用去考虑它实虚部的数据类型，而是在类前面用`template <class T>`来生成一个模板，实部和虚部的数据类型用`T`去表示。这样的话，我们在实际使用时，用类名加尖括号`< >`，尖括号中写入数据类型，那么程序就会根据模板去生成对应的类。

```cpp
template <class T>
class complex {
public:
    complex(T r = 0, T i = 0) : re(r), im(i) {}
    complex& operator += (const complex&);
    T real() const { return re; }
    T imag() const { return im; }
private:
    T re, im;
    friend complex& __doapl (complex*, const complex&);
};
========================================================
{
    complex<double> c1(2.5, 1.5);
    complex<int> c2(2, 6);
    ...
}
```



### 函数模板

函数模板和类模板相似，就是在函数头前通过`template <class T>`泛指一个数据类型。下面是一个求最小的stone的部分程序，通过调用min函数获取结果，min函数实际上是一个函数模板，在传入参数r1、r2后，编译器对函数模板进行实参的推导。

```cpp
stone r1(2,5), r2(3,3), r3;
r3 = min(r1,r2);
```

函数模板如下，实参推导的结果是`T`为`stone`，于是调用了实参`b`的`stone::operator<()`。

```cpp
template <class T>
inline
const T& min(const T& a, const T& b) {
	return b < a ? b : a;
}

class stone {
public:
    stone(int w, int h, int we): _w(w), _h(h), _weight(we) {}
    bool operator< (const stone& rhs) const {
        return _weight < rhs._weight;
    }
private:
    int _w, _h, _weight;
};
```



### 模板的泛化 & 特化 & 偏特化

上面的类模板和函数模板的代码示例都属于是模板的泛化，而有的时候，并不是对于所有的数据类型，都可以用同一份代码去实现。所以引入了模板的特化，我们能够通过对某一特定类型编写特定的代码去执行，有时候是迫不得已，有时候则是为了优化性能。

下面代码展示了模板的特化，其写法是把前面的`template<class type>`尖括号中的类型去掉，并在模板头后加入`<type>`，type是特定的数据类型。

```cpp
// 泛化
template <class type>
struct __type_traits {
    typedef __true_type this_dummy_member_must_be_first;
    typedef __flase_type has_trivial_default_constructor;
    ...
};
// 特化
template<> struct __type_traits<int> {
    typedef __true_type has_trivial_default_constructor;
    ...
};
template<> struct __type_traits<double> {
    typedef __true_type has_trivial_default_constructor;
    ...
};
```

偏特化和特化类似，有人也称其为局部特化，也就是说，特化的是模板类型中的一部分，如下代码中vector后偏特化了bool类型，编译器在收到bool参数时会优先选择偏特化的模板，其中我们能够去依据bool类型的特性去优化代码性能。

```cpp
// 泛化
template <class T, class Alloc = alloc>
class vector {...};
// 偏特化
template <class Alloc>
class vector<bool,Alloc> {...};
```

上面的偏特化是对**数据类型**的偏特化，我们还可以对**结构**进行偏特化。

如下代码指针类型和常量指针类型：

```cpp
// 泛化
template <class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category iterator_category;
    ...
}
// 偏特化
template <class T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    ...
}
// 偏特化
template <class T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag iterator_category;
    ...
}
```



## 深度探索List

<img src=".\picture\深入探索List.png"/>

上图的左边展示的是List双向链表的结构，每个节点都有两个指针，分别指向了它的上一个节点和下一个节点，但于一般的链表不同的是，它刻意地添加了一个空白节点，将链表的头尾连接起来形成一个环状的双向链表，并让迭代器end()指向这个空白节点，用以符合之前讲到的“前闭后开”区间的设计。

同时从右边的代码可以发现，在设计容器的时候，设计者采用了大量的typedef来对各种数据类型进行重命名，这有利于代码的编写。

下面是关于`__list_iterator`迭代器的部分源码：

```cpp
template <class T>
struct __list_node {
    typedef void* void_pointer;
    void_pointer prev;
    void_pointer next;
    T data;
};

template <class T, class Ref, class Ptr>
struct __list_Iterator {
    typedef __list_Iterator<T, Ref, Ptr> self;
    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ptr pointer;
    typedef Ref reference;
    typedef __list_node<T>* link_type;
    typedef ptrdiff_t difference_type;
    
    link_type node;
    
    reference operator*() const { return (*node).data; }
    pointer operator->() const  { return &(operator*()); }
    self& operator++() 			{ node = (link_type)((*node).next); return *this; }
    self operator++(int) 		{ self tmp = *this; ++*this; return tmp; }
    ...
};
```

迭代器是一个泛化的指针，使用者为了能够从一个节点移动到下一个节点或其他操作，会使用到`++`等运算符，所以就需要进行大量的运算符重载。需要注意的是上面代码中，如最后部分的`self tmp = *this`中的this是一个指向自身的指针，所以在使用`*`运算符时，不会唤醒`operator *()`函数，而是由一个指针变成了`self`类型。

上面的源码是G2.9版本的，其中我们可以看到在构建`__list_iterator`模板时是传入了三个参数类型，分别是`T`、`T&`和`T*`。实际上只需要传入一个`T`即可，其余的指针类型和引用类型都可以在类中通过`typedef`解决。事实上，C++团队也确实这么做了，在G4.9版本进行了修改，如下图。

<img src=".\picture\深入探索List2.png"/>

除了上面说的一点不同，G4.9版本还将指向前一个节点和后一个节点的指针类型由`void*`改为`_List_node_base*`，并通过继承改善节点的结构。