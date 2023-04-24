这个是一个简单的，微小的C++STL标准库。写这个小项目的动机是我平时使用了较多的STL的的容器和算法，恰好我正在阅读侯捷老师的<<STL源码刨析>>这本书，于是我萌生了想要写一份自己的STL的想法。写这个项目锻炼了我的泛型编程能力，加强了我对数据结构和算法的理解，体验良好。
这个项目只是一个简单的练手小项目，我的编码能力可能不是很好（我尽力的写好），可能写得不是很好。侯捷老师的书里有很多实用的代码和例子，我也借鉴了许多。
我使用了C++20的标准来编写这个项目，使用了if constexpr表达式来替代了传统的标签分发形式，使得代码更加简洁。我接下来会介绍我是如何写的。


STL 六大组件：

（1）my_stl::allocator
allocator是空间适配器,用于管理泛型容器的内存申请,内存释放,对象构造,对象析构等功能。SGI STL的专属空间适配器是一个双层结构，第一层是对free，malloc等函数的封装，效率不佳；第二层是使用了内存池加16个自由链表的组合，效率较高但也较难实现。侯捷老师的书里实现了两层的空间适配器，我也照着去实现了一个，但是我在使用过程中发现有问题，由于我对内存池的了解较少所以我也不太能够去很好的解决，所有我就没有使用双层的空间适配器，我仅仅实现了一个第一层的空间适配器，仅此而已。

