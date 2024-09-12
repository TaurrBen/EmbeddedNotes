# C++基础知识

## 面向对象

区别于传统的面向流程，需要抽象出一个类来封装各类方法

- **封装**

1. 将对象的属性（成员变量）和方法（成员函数）封装到一个类里面，便于管理的同时也提高了代码的复用性。

- **继承**

1. 最大程度保留类和类之间的关系，提高代码复用性，降低代码维护成本。

- **多态**

1. 静态多态：编译时确定，函数重载
2. 动态多态：运行时确定调用成员函数的时候，会更具调用方法的对象的类型来执行不同的函数。父类指针调用子类对象



## 继承

public protected peivate

**类实例**（即类对象）不能直接访问类的 **private成员**和**protected成员**，但是能直接访问类的**public成员**。

**无论哪种继承方式**，**子类**都不能直接访问**父类**的 **private成员**；但是能直接访问**父类**的 **protected成员**和**public成员**（注意：是**子类**，而不是**类实例**），并且能通过**父类**的**protected成员函数**和**public成员函数**间接访问**父类**的**private成员**。

对于这三种方式继承的 派生类 来说: 都能访问基类的public, protected 成员;

- public 的方式继承到派生类，这些成员的权限和在基类里的权限保持一致;

- protected方式继承到派生类，成员的权限都变为protected;

- private 方式继承到派生类，成员的权限都变为private;

3.子类通过public方式继承父类，则父类中的public、protected和private属性的成员在 子类 中 依次 是 public、protected和private属性，即通过public继承并不会改变父类原来的数据属性。

4.子类通过protected方式继承父类，则父类中的public、protected和private属性的成员在 子类 中 依次 是 protected、protected和private属性，即通过protected继承原来父类中public属性降级为子类中的protected属性，其余父类属性在子类中不变。

5.子类通过private方式继承父类，则父类中的public、protected和private属性的成员在 子类 中 依次 是 private、private和private属性，即通过private继承原来父类中public属性降级为子类中的private属性，protected属性降级为子类中的private属性，其余父类属性在子类中不变。

注意: 其实父类的原属性并未改变，只是通过 继承关系被继承到子类中的父类成员的个别属性有所变化 ，即只是在子类中父类的个别成员属性降级了，原来父类的成员属性并未变。



友元函数 friend

类的友元函数是定义在类外部，但有权访问类的所有私有（private）成员和保护（protected）成员。尽管友元函数的原型有在类的定义中出现过，但是友元函数并不是成员函数。

```c++
class Box
{
private:
   double width;
public:
   friend void printWidth( Box box );
   void setWidth( double wid );
};
 
// 成员函数定义
void Box::setWidth( double wid ) {
    width = wid;
}
 
// 请注意：printWidth() 不是任何类的成员函数
void printWidth( Box box ) {
   /* 因为 printWidth() 是 Box 的友元，它可以直接访问该类的任何成员 */
   cout << "Width of box : " << box.width <<endl;
}
 
// 程序的主函数
int main( ) {
   Box box; 
   // 使用成员函数设置宽度
   box.setWidth(10.0);
   
   // 使用友元函数输出宽度
   printWidth( box ); 
   return 0;
}
```



## static作用，与c的区别

static 作用主要影响着变量或函数的**生命周期**，**作用域**，以及**存储位置**。

一、修饰局部变量：（函数内部、{}内部）

- 变量的存储区域由**栈**变为**静态区**。
- 变量的生命周期由**局部**变为**全局**。
- 变量的作用域不变。

二、修饰模块内的全局变量：（静态全局变量）

- 变量的存储区域在全局数据区的**静态区**。
- 变量的作用域由**整个程序**变为**当前文件**。（extern声明也不行）（全局变量不暴露）
- 变量的生命周期不变。

三、修饰函数：（当前文件中的函数）

-  函数的作用域由**整个程序**变为**当前文件**。（extern声明也不行）（接口不暴露）

四、修饰C++ 成员变量

- 在类外定义与初始化`int A::_count = 0;`，类内申明`static int _count;`
- 为该类所有对象所共享
- 访问：类名::变量名

五、修饰C++ 成员函数

- 没有隐藏的 `this` 指针，不能访问非静态成员（变量、 函数）
- 不能调用非静态成员函数
- 非静态成员函数可以调用静态成员函数



## 指针与引用的区别

- 指针：指向一个对象后，对它所指向的变量，间接操作

- 引用：目标变量的别名，直接操作

```c++
int a = 996;
int *p = &a; // p是指针, &在此是求地址运算
int &r = a; // r是引用, &在此起标识作用
```

1. 引用必须初始化，指针不用

2. 引用初始化后不能修改，指针可以改变所指对象

3. 指针++为地址，引用++为值

4. sizeof 指针为指针大小，sizeof 引用为数据大小



- 指针转换为引用：*p，随后当参数传入即可

- 引用转换为指针：引用对象&取地址即可



## 左值引用、右值引用

- 左值是指表达式后可以获取地址的对象。换句话说，左值代表一个可以放在等号左边的值，也可以被修改例如，变量、数组元素和通过引用或指针访问的对象都是左值。 int a = 10; // 其中 a 就是左值

- 右值是指表达式后不可以获取地址的临时对象或字面量。右值代表一个临时值，它只能放在等号右边，不能被修改。例如，数字常量、字符串常量、临时变量、返回的临时对象都属于右值。 int a = 10; // 其中 10 就是右值右值


C++11引入了右值引用（rvalue reference）的概念，允许程序员更方便地对右值进行操作和移动语义，例如移动语义的实现和完美转发。右值引用通过`&&`表示。

```c++
int&& r = 42; // 创建一个右引用
```



## 移动语义与完美转发 moce fowrard

- std::move是一个函数模板，用于将给定的对象表示为右值（或将其转换为右值用它执行的操作是对传入的对象进行强制转换，使其能够被移动而不是复制。通过使用std::move，我们可以显式地表达出我们要对对象进行移动操作，以便在适当的情况下利用移动语义，提高程序的性能。
- std::forward也是一个函数模板，用于在函数转发（forwarding）时保持参数类型。它与stdmove类似，但是它能够根据传递给它的类型自动进行转发，既可以用于左值引用，也可以用于右值引用。它的主要用途是在实泛型代码时，将函数参数以原始的转发方式传递给其他函数，以保持参数的类型和值的完整性。

总结起来，std::move用于在移动语义中转移对象的所有权，而std::forward则用于完美转发函数参数，保持参数的类型。这两个函数都是为了高效和灵活地处理C++中的对象转移和函数转发而引入的，能够使代码更加简洁和高效。



当使用std::move时，我们可以将一个对象的所有权从一个对象转移到另一个对象。在下面的例子中，通过使用std::move，我们将source的所有权转移到了destination，这样我们就可以高效地移动source的内容而不是逐个复制每个元素。例如：

```c++
int main() {
    std::vector<int> source = {1, 2, 3, 4, 5};

    // 使用std::move将source的所有权转移到destination
    std::vector<int> destination = std::move(source);

    // source现在为空，已经移动到destination
    std::cout << "Size of source: " << source.size() << std::endl; // 输出 0    // destination包含原来source元素
    std::coutSize of destination: " << destination.size() << std::endl; // 输出 

    return 0;
}
```



当使用std::forward时，我们可以在函数转发中保持参数的类型。在这个例子中，我们定义了一个 `processValue` 函数，它接受一个右值引用参数。然后我们使用 `forwardFunction` 函数来转发参数，使用 `std::forward` 将参数完美转发给 `processValue` 函数。在 `main` 函数中，我们展示了如何使用 `forwardFunction` 函数来传递左值和右值，而调用 `processValue` 函数。通过 `std::forward`，我们可以在函数转发中保持参数类型的完整性。

```c++
// 接受参数的函数
void processValue(int&& x) {
    std::cout << "Processing rvalue: " << x << std::endl;
}

// 使用std::forward转template<typename T>
void forwardFunction(T&& arg) {
    processValue(std::forward<T>(arg));
}

int main() {
    int value = 42;

    // 传递左值，调用processValue函数
    forwardFunction(value);

    // 传递右值，调用processValue函数
    forwardFunction(std::move(value));

    return 0;
}
```



std::forward相比于简单地将参数传递给另一个函数而言，可以提高代码的效率，主要体现在以下几个方面：

1. 避免多余的拷贝：当参数是左值（lvalue）时，使用std::forward**可以将参数作为左值引用传递给下一层函数，避免产生额外的拷贝操作。如果直接传递参数，会导致参数被当作右值（rvalue）来处理，从而触发拷贝构造函数。**
2. 精确匹配重载函数：有时我们在一个函数中需要对传递的参数进行重载函数的调用，而这些重载函数可能接受不同的参数类型（比如一个接受左值引用，一个接受右值引用）。使用std::forward可以精确匹配原始传入参数的类型，从而调用正确的重载函数。
3. 消除重载冗余：std::forward使用引用折叠规则，从而避免引入额外的重载函数，以减少代码的冗余。通过std::forward，可以将参数的左值引用和右值引用统一起来，消除了传递参数时的冗余重载处理。

总而言之，std::forward提供了一种高效的方式来将参数按照原始的值类别和修饰符转发给下一层函数，避免了多余的拷贝操作，精确匹配重载函数，并消除了重载冗余，从而提高了代码的效率。



## 模板类

```c++
// XX.h
template <typename T>
class MyTemplateClass {
private;
	T data;
public:
    MyTemplateClass(T value) : data(value) {}  // 构造函数

    void printData() {
        std::cout << "Data: " << data << std::endl;  // 模板类方法
    }
};


// XX.cpp
MyTemplateClass<int> obj1(10);   // 实例化为处理int类型的对象
MyTemplateClass<double> obj2(3.14);   // 实例化为处理double类型的对象

obj1.printData();    //: Data: 10
obj2.printData();    // 输出: Data: 3.14
```



## 为什么模板类写在.h中，不在.cpp中

模版是在编译的时候实例化的，实例化需要知道模版参数的具体类型，如果把模版的声明和定义分离编译的话，那么cpp文件中的模版实现不知道T的类型，无法实例化。都写到头文件中就解决了   

在C++中，模板类通常需要在头文件（.h）中进行定义和实现，而不是分离到.cpp文件中。这是由C++的**编译模型和模板实例化的特性决定的**。

模板类是在使用时根据实际的模板参数进行实例化的，**编译器需要在编译阶段生成模板类的实例化代码**。因此，编译器需要在编译阶段能够访问模板类的完整定义和现，以便为每个模板参数生成对应的实例化代码。

**如果将模板和实现分离，那编译阶段只能看到模板类，无法生成实例化的代码。这将导致链接阶段找不到所需的实例化代码，进而导致链接错误。**




## new和malloc的区别

|              | new                                      | malloc                               |
| ------------ | ---------------------------------------- | ------------------------------------ |
| 语法         | `int *p = new int(0)`或int *p = new int  | `int *p = (int*)malloc(sizeof(int))` |
| 初始化       | 可以初始化                               | 无                                   |
| 函数与运算法 | 操作符，返回指定类型的地址，不需类型转换 | 函数，返回void *                     |
| 失败返回值   | 抛出异常`bad_alloc`                      | 返回NULL                             |
| 构造析构调用 | 创建对象时自动调用                       | 无                                   |



## 可以用malloc给一个类对象分配内存吗

malloc分配内存不会调用构造函数



## new与delete实现

实际调用malloc 与 free，但区别如下：

- 申请失败后，new返回值为异常，bad_malloc，malloc返回NULL
- 对于内置数据类型一致，对于类，执行构造函数与析构函数

new 实际调用brk()与mmap()系统调用



## 深浅拷贝

- 浅拷贝就是增加了一个指向相同堆区的指针，这将导致在析构的时候会重复释放。默认的拷贝构造和运算符重载都是浅拷贝。
- 深拷贝是在拷贝的时候将内容申请内存，重新拷贝一份，放到内存中，指针指向这个新拷贝的部分，这样就不会出现析构的时候重复释放的问题了。



## 重载和重写

重载：在同一个类中，方法相同，参数数量与类型不同（静态多态性），例：构造函数，函数名相同，参数同（返回值无法判读）

重写：在父类与子类中，方法与参数都相同（动态多态性），子类对象调用该方法时，父类方法被屏蔽



## 虚函数作用及底层实现原理

1. 实现多态性
2. 公有继承（基类定义虚函数，派生类可以重写）
3. 动态联编（父类指针指向子类对象时，调用子类方法）（类似函数重载（静态），重写为动态的）

```c++
//Base Class
class Student {
    private:
        int m_id;

    // protected:
        string m_name;
        int m_gender;

    public:
        Student();
        Student(string name, int gender, int id);
        virtual ~Student();  //申明virtual方法的基类中的析构函数必须为虚函数，否则在释放指针指向的派生类对象时，将调用基类的析构函数造成错误
        virtual void Show_Info();
};

//Derived Class
class Student_Zju : public Student{
    private:
        int m_ser_num;
    public:
        Student_Zju();
        Student_Zju(string name, int gender, int id, int ser_num);
        virtual ~Student_Zju();
        virtual void Show_Info();
};
```

实现机制：为每个类对象添加一个隐藏成员，保存了一个指向函数（虚函数）地址数组的指针，称为虚表指针（虚函数表）

- 如果派生类重写了基类的虚方法，该派生类虚函数表将保存重写的虚函数的地址，而不是基类的虚函数地址。

- 如果基类中的虚方法没有在派生类中重写，那么派生类将继承基类中的虚方法，而且派生类中虚函数表将保存基类中未被重写的虚函数的地址。注意，如果派生类中定义了新的虚方法，则该虚函数的地址也将被添加到派生类虚函数表中。



## 含有纯虚函数的类是否可以实例化

不可以，需要被派生类继承后才行

在基类中不能对虚函数给出具体的有意义的实现，就可以把它声明为纯虚函数，它的实现留给该基类的派生类去做。

```c++
class VirtualClass{
	public:
		virtual void fun1() = 0;  // 纯虚函数
		virtual ~VirtualClass();
};

class ClassA : public VirtualClass{
	public:
		virtual void fun1() {  // 虚函数
			printf("VirtualClass\n");
		};
		virtual ~VirtualClass();
};

int main(){
	//编译报错，这个非法的
	VirtualClass * virtualClass = new VirtualClass();//error: cannot allocate an object of abstract type 'VirtualClass'
	
	VirtualClass * classA = new ClassA();
	classA->fun1();
	return 0;
}
```



## 构造函数是否可以是虚函数，析构函数为什么建议是虚函数

构造函数不可以是虚函数，如果构造函数时虚函数，那么调用构造函数就需要去找vptr，而此时vptr还没有初始化

析构函数需要是虚函数，当父类指针指向子类对象时，释放子类对象时，若父类析构非虚，会调用父类析构，子类相较于父类多出的方法不会被析构

```c++
BaseClass* pObj = new SubClass();
delete pObj;
```

- 若析构函数是虚函数(即加上virtual关键词)，delete时基类和子类都会被释放
- 若析构函数不是虚函数(即不加virtual关键词)，delete时只释放基类，不释放子类，会**造成内存泄漏**问题



## 虚函数表与内存模型

假如一个类有虚函数，当我们构建这个类的实例时，将会额外分配一个指向该类虚函数表的指针，当我们用父类的指针来操作一个子类的时候，这个指向虚函数表的指针就派上用场了，它指明了此时应该使用哪个虚函数表

[C++虚函数表的位置——从内存的角度 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/563418849)

1. 每个类,只要含有虚函数,new出来的对象就包含一个虚函数指针,指向这个类的虚函数表(这个虚函数表一个类用一张)
2. 子类继承父类,会形成一个新的虚函数表,但是虚函数的实际地址还是用的父类的,如果子类重写了某个虚函数,那么子类的虚函数表中存放的就是重写的虚函数的地址
3. 不同类之间可以通过强制转型调用其他类的虚函数



## 如何判断一个方法来自父类还是子类

方法一：可以在父类或子类的相应方法print()一个标记
方法二：dynamic_cast

```c++
class Tfather {
public:
	virtual void f() { cout << "father's f()" << endl; }
};

class Tson : public Tfather {
public:
	void f() { cout << "son's f()" << endl; }
 
	int data; // 我是子类独有成员
};
 
int main() {
	Tfather father;
	Tson son;
	son.data = 123;
 
	Tfather *pf;
	Tson *ps;
	
	/* 上行转换：没有问题，多态有效 */
	ps = &son;
	pf = dynamic_cast<Tfather *>(ps);
	pf->f();
 
	/* 下行转换（pf实际指向子类对象）：没有问题 */
	pf = &son;
	ps = dynamic_cast<Tson *>(pf);
	ps->f();
	cout << ps->data << endl;		// 访问子类独有成员有效
 
	/* 下行转换（pf实际指向父类对象）：含有不安全操作，dynamic_cast发挥作用返回NULL */
	pf = &father;
	ps = dynamic_cast<Tson *>(pf);
	assert(ps != NULL);			 	// 违背断言，阻止以下不安全操作
	ps->f();
	cout << ps->data << endl;		// 不安全操作，对象实例根本没有data成员
 
	/* 下行转换（pf实际指向父类对象）：含有不安全操作，static_cast无视 */
	pf = &father;
	ps = static_cast<Tson *>(pf);
	assert(ps != NULL);
	ps->f();
	cout << ps->data << endl;		// 不安全操作，对象实例根本没有data成员
 
	system("pause");
}
```



## 菱形继承

D类的对象不确定调用哪个父类的方法

![img](F:/本专业/计算机/笔记/嵌入式/嵌入式/嵌入式笔记.assets/1365470-20200417192844851-2073365337.png)

```c++
class Animal{
private:
    int weight;
public:
    virtual int getWeight() {
        return this->weight;  // 共用虚函数
    }
};

class Tiger :  public Animal{};

class Lion : public Animal{};

class Liger : public Tiger, public Lion{};  // 如此定义存在问题，不确定调用哪个getWeight()

int main()
{
    Liger lg;
    lg.getWeight();  // 非法
    lg.Lion::getWeight();  // 合法
}
```

```c++
// 解决方案
class Animal{
private:
    int weight;
public:
    virtual int getWeight() {
        return this->weight;
    }
};

class Tiger : virtual public Animal{};  // 加入virtual虚继承

class Lion : virtual public Animal{};

class Liger : public Tiger, public Lion{};

int main()
{
    Liger lg;
    lg.getWeight();
}
```





## 拷贝构造函数与赋值构造函数

区别：**赋值时对象是否已经存在**

C++每一个类提供默认的拷贝构造函数，但成员变量涉及指针时，浅拷贝带来问题，需要自定义深拷贝

拷贝构造函数调用的情况：

```c++
// 拷贝构造
Complex c2(c1);  //拷贝构造函数初始化
Complex c2 = c1;  //首次创建对象是初始化，不是赋值语句
void Func(Class a) {balabala}  //调用函数时，Class将实参拷贝构造为形参

// 对象作为函数参数
void func(Class class);
// 函数返回值为一个非引用型对象
return A_class;
// 使用一个对象初始化另一个对象
Clsss a = b;  // （a不存在，需要构造）
// 有参构造函数
Class c(a);  //调用拷贝构造函数
```

重载的赋值运算符调用情况：

```c++
// 赋值构造
Complex c1, c2;  //默认构造函数
c1 = c2 ;  //重载的赋值运算符，已经存在对象，不是拷贝构造

// 运算符重载
 A& operator = (const A& other) {}
// 赋值
Class a;
a = b;  // 对象存在，调用赋值
```

```c++
#include <iostream>

class MyClass {
private:
    int privateMember;

public:
    // 默认构造函数
    MyClass() : privateMember(0) {
        std::cout << "Default constructor called." << std::endl;
    }

    // 拷贝构造函数
    MyClass(const MyClass& other) : privateMember(other.privateMember) {
        std::cout << "Copy constructor called." << std::endl;
    }

    // 赋值构造函数
    MyClass& operator=(const MyClass& other) {
        std::cout << "Assignment operator called." << std::endl;
        if (this == &other) {
            return *this;
        }

        privateMember = other.privateMember;
        return *this;
    }

    // 获取私有成员的值
    int getPrivateMember() const {
        return privateMember;
    }

    // 设置私有成员的值
    void setPrivateMember(int value) {
        privateMember = value;
    }
};

int main() {
    MyClass obj1;  // 调用默认构造函数
    obj1.setPrivateMember(42);

    MyClass obj2 = obj1;  // 调用拷贝构造函数
    MyClass obj3;
    obj3 = obj1;  // 调用赋值构造函数

    std::cout << "Value of obj1's private member: " << obj1.getPrivateMember() << std::endl;
    std::cout << "Value of obj2's private member: " << obj2.getPrivateMember() << std::endl;
    std::cout << "Value of obj3's private member: " << obj3.getPrivateMember() << std::endl;

    return 0;
}
```





## C++如何实现只在栈上实例化对象

[(24条消息) 如何限制对象只能建立在堆上或者栈上_舒夜无痕的博客-CSDN博客](https://blog.csdn.net/szchtx/article/details/12000867?spm=1001.2101.3001.6661.1&utm_medium=distribute.pc_relevant_t0.none-task-blog-2~default~CTRLIST~Rate-1-12000867-blog-61196943.pc_relevant_multi_platform_whitelistv4&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-2~default~CTRLIST~Rate-1-12000867-blog-61196943.pc_relevant_multi_platform_whitelistv4&utm_relevant_index=1)

```c++
class A {  // 只在堆heap上建立对象，调用create()函数在堆上创建类A对象，调用destory()函数释放内存
protected:
    A(){}
    ~A(){}
public:
    static A* create() {
        return new A();
    }
    void destory() {
        delete this;
    }
};
```

只有使用new运算符，对象才会建立在堆上，因此，只要禁用new运算符就可以实现类对象只能建立在栈上。将operator new()设为私有即可。

```c++
class A {  // 只在栈stack上建立对象
private:
    void* operator new(size_t t){}     // 注意函数的第一个参数和返回值都是固定的
    void operator delete(void* ptr){} // 重载了new就需要重载delete
public:
    A(){}
    ~A(){}
};
```



## 如何避免内存泄漏，用过什么智能指针，智能指针的实现原理

C++没有内存回收机制，每次程序员new出来的对象需要手动delete，流程复杂时可能会漏掉delete，导致内存泄漏

智能指针：

- shared_ptr（引用计数，新增++，过期--，0释放）
- unique_ptr（独占式，不允许复制拷贝）（线程安全）
- weak_ptr（解决循环引用计数问题）（**循环引用计数**：两个智能指针互相指向对方，造成内存泄漏。需要weak_ptr，将其中的一个指针设置为weak_ptr。）（因为weak_ptr没有共享资源，它的构造函数不会引起智能指针引用计数的变化）

```c++
#include <iostream>
#include <memory>  // 头文件
using namespace std;

class A {
public:
    A(int count) {  // 构造
        _nCount = count;
    }
    ~A(){}  // 析构
    void Print() {
        cout<<"count:"<<_nCount<<endl;  // 公有方法
    }
private:
    int _nCount;  // 私有成员变量
};

int main() {   
    shared_ptr<A> p(new A(10));  // 初始化，堆上新建一个类，p为智能指针
    p->Print();  // 调用
    return 0;
}
```



```c++
#include <memory>
shared_ptr<int> p = make_shared<int> (100);  // 指针指向一块存放100的地址，推荐使用
shared_ptr<int> p {new int(100)};  // 第二种创建方式
```



```c++
#include <memory>
unique_ptr<int> p = make_unique<int>(100);  // 独占指针
unique_ptr<int> p1(p.release());  // 将p的指向及所有权转移到p1
unique_ptr<int> p1 = std::move(p);  // 同样的
```



## shared_ptr多线程安全问题

- 同一个shared_ptr被多个线程“读”是安全的；
- 同一个shared_ptr被多个线程“写”是不安全的；
- 共享引用计数的不同的shared_ptr被多个线程”写“ 是安全的；

引用计数是线程安全的，但在多个线程中对其进行修改不安全

[shared_ptr是线程安全的吗？-腾讯云开发者社区-腾讯云 (tencent.com)](https://cloud.tencent.com/developer/article/1654442)

[当我们谈论shared_ptr的线程安全性时，我们在谈论什么？ - 掘金 (juejin.cn)](https://juejin.cn/post/7038581008945872927)



## 内存泄漏检测

内存泄漏（Memory Leak）是指程序中已动态分配的堆内存由于某种原因程序未释放或无法释放，造成系统内存的浪费，导致程序运行速度减慢甚至系统崩溃等严重后果

避免内存泄露的方法

- 有良好的编码习惯，动态开辟内存空间，及时释放内存
- 采用智能指针来避免内存泄露
- 采用静态分析技术、源代码插装技术等进行检测



## 同步I/O与异步I/O

同步I/O是指程序在进行输入/输出操作时会阻塞当前线程，直到操作完成才继续执行后续代码（死等）

异步I/O是指程序在进行输入/输出操作时不会阻塞当前线程，而是继续执行后续代码，并通过回调或者轮询等机制来获取I/O操作的结果（让出权限等待唤醒）

- 同步I/O简单直观，代码编写相对容易，但会阻塞线程造成资源浪费。
- 异步I/O能够充分利用系统资源，提高并发性能，但需要处理回调和事件驱动等复杂性。



## STL常见容器及其内部实现的数据结构

| 名称           | 描述                      | 存储结构                                       | 方法                                                         |
| -------------- | ------------------------- | ---------------------------------------------- | ------------------------------------------------------------ |
| vector         | 动态分配的数组            | 顺序，array                                    | v.capacity(); //容器容量--v.size(); //容器大小--v.at(int idx); //用法和[]运算符相同--v.push_back(); //尾部插入--v.pop_back(); //尾部删除--v.front(); //获取头部元素--v.back(); //获取尾部元素--v.begin(); //头元素的迭代器--v.end(); //尾部元素的迭代器--v.insert(pos,elem); //pos是vector的插入元素的位置--v.insert(pos, n, elem) //在位置pos上插入n个元素elem--v.insert(pos, begin, end);--v.erase(pos); //移除pos位置上的元素，返回下一个数据的位置--v.erase(begin, end); //移除[begin, end)区间的数据，返回下一个元素的位置--reverse(pos1, pos2); //将vector中的pos1~pos2的元素逆序存储 |
| list           | 双向链表                  | 离散                                           | (1) 元素访问：lt.front();--lt.back();--lt.begin();--lt.end();--(2) 添加元素：--lt.push_back();--lt.push_front();--lt.insert(pos, elem);--lt.insert(pos, n , elem);--lt.insert(pos, begin, end);--lt.pop_back();--lt.pop_front();--lt.erase(begin, end);--lt.erase(elem);--(3)sort()函数、merge()函数、splice()函数：--sort()函数就是对list中的元素进行排序;--merge()函数的功能是：将两个容器合并，合并成功后会按从小到大的顺序排列;--比如：lt1.merge(lt2); lt1容器中的元素全都合并到容器lt2中。--splice()函数的功能是：可以指定合并位置，但是不能自动排序！ |
| stack          | 栈                        | 用list或deque实现                              |                                                              |
| quque          | 队列                      | 用list或deque实现                              |                                                              |
| deque          | 双端队列                  | 分段连续（多个vector连续）                     | (1) 元素访问：d[i];--d.at[i];--d.front();--d.back();--d.begin();--d.end();--添加元素：d.push_back();--d.push_front();--d.insert(pos,elem); //pos是vector的插入元素的位置--d.insert(pos, n, elem) //在位置pos上插入n个元素elem--d.insert(pos, begin, end);--删除元素：d.pop_back();--d.pop_front();--d.erase(pos); //移除pos位置上的元素，返回下一个数据的位置--d.erase(begin, end); //移除[begin, end)区间的数据，返回下一个元素的位置 |
| priority_queue | 优先级队列                | vector                                         |                                                              |
| set            | 集合（有序不重复）        | 红黑树（弱平衡二叉搜索树，二分查找法搜索高效） | s.size(); //元素的数目--s.max_size(); //可容纳的最大元素的数量--s.empty(); //判断容器是否为空--s.find(elem); //返回值是迭代器类型--s.count(elem); //elem的个数，要么是1，要么是0，multiset可以大于一begin 返回一个指向集合中第一个元素的迭代器。--cbegin 返回指向集合中第一个元素的const迭代器。--end 返回指向末尾的迭代器。--cend 返回指向末尾的常量迭代器。--rbegin 返回指向末尾的反向迭代器。--rend 返回指向起点的反向迭代器。--crbegin 返回指向末尾的常量反向迭代器。--crend 返回指向起点的常量反向迭代器。--s.insert(elem);--s.insert(pos, elem);--s.insert(begin, end);--s.erase(pos);--s.erase(begin,end);--s.erase(elem);--s.clear();//清除a中所有元素； |
| multiset       | 集合（有序可重复）        | 红黑树                                         |                                                              |
| unordered_set  | 集合（无序不重复）        | hash                                           |                                                              |
| map            | 键值对（有序不重复）      | 红黑树                                         |                                                              |
| multimap       | 键值对（有序可重复）      | 红黑树                                         |                                                              |
| unordered_map  | 键值对（无序不重复）      | hash                                           |                                                              |
| hash_map       | 哈希表，类似map，速度更快 | hash                                           |                                                              |



## deque底层数据结构

![在这里插入图片描述](F:/本专业/计算机/笔记/嵌入式/嵌入式/嵌入式笔记.assets/20201210223715833.png)



## 红黑树

非严格的平衡搜索二叉树，有自动排序的功能



## sort()

仅支持随机访问的数据结构进行快速排序，如vector、deque、array

```c++
bool func(int a, int b) {
    return a > b;
}
sort(vec.begin(), vec.end(), func);  // 升序排列
```



## partial_sort()

对部分元素进行升序/降序排列，利用大顶堆/小顶堆实现，堆空间为n

```c++
bool func(int a, int b) {
    return a > b;
}
int n = 4;  // 需要排序的数量
partial_sort(vec.begin(), vec.begin() + n, vec.end(), func);  // 仅排序其中的n个元素
```



## is_sorted()

```c++
bool func(int a, int b) {
    return a > b;
}
bool result = is_sorted(vec.begin(), vec.end(), func())  // 返回值为bool，是否按照func定义的顺序排序
```



## is_sorted_until()

```c++
bool func(int a, int b) {
    return a > b;
}
auto it = is_sorted(vec.begin(), vec.end(), func())  // 返回值：指向序列中第一个破坏 comp 排序规则的元素迭代器
```



## find()

```c++
vector<int> vec{ 10,20,30,40,50 };
auto it = find(vec.begin(), vec.end(), 30);  // 起始、终止迭代器、查找的值
if (it != myvector.end())
    cout << "查找成功：" << *it;
else
    cout << "查找失败";
return 0;
```



## find_if()

按照自定义谓词查找

```c++
bool mycomp(int i) {
    return ((i % 2) == 1);
}

vector<int> myvector{ 4,2,3,1,5 };
auto it = find_if(myvector.begin(), myvector.end(), mycomp());
```



## 使用vector如何避免频繁的内存重新分配

内存分配的过程：

1. 分配新的内存块，在大部分实现中，vector和string的容量每次以2为因数增长。也就是说，当容器必须扩展时，它们的容量每次翻倍。
2. 把所有元素从容器的旧内存拷贝到它的新内存。
3. 销毁旧内存中的对象。
4. 回收旧内存。

解决方案：

1. 预分配内存：在创建 vector 对象时，可以使用 reserve() 方法来预分配内存空间，以避免频繁扩容。
2. 合理选择初始容量：在创建 vector 对象时，可以根据数据量的大小估算出合理的初始容量，这样可以尽可能减少扩容的次数。
3. 优化算法：尽可能使用时间复杂度低的算法，避免数据量过大时的性能问题。



## vector  resize()与reserve()

- resize(Container::size_type n)强制把容器改为容纳n个元素。调用resize之后，size将会返回n。如果n小于当前大小，容器尾部的元素会被销毁。如果n大于当前大小，新默认构造的元素会添加到容器尾部。如果n大于当前容量，在元素加入之前会发生重新分配。
- reserve(Container::size_type n)强制容器把它的容量改为至少n，提供的n不小于当前大小。这一般强迫进行一次重新分配，因为容量需要增加。（如果n小于当前容量，vector忽略它，这个调用什么都不做，string可能把它的容量减少为size()和n中大的数，但string的大小没有改变。）



## vector的扩容系数为什么是1.5或2

[面试题：C++vector的动态扩容，为何是1.5倍或者是2倍_vector扩容_森明帮大于黑虎帮的博客-CSDN博客](https://blog.csdn.net/qq_44918090/article/details/120583540)

扩容原理为：申请新空间，拷贝元素，释放旧空间，理想的分配方案是在第N次扩容时如果能复用之前N-1次释放的空间就太好了，如果按照2倍方式扩容，第i次扩容空间大小如下：![img](F:/本专业/计算机/笔记/嵌入式/嵌入式/嵌入式笔记.assets/db4b6319537147d4817bdb43db2709bf.png)

可以看到，每次扩容时，前面释放的空间都不能使用。比如：第4次扩容时，前2次空间已经释放，第3次空间还没有释放(开辟新空间、拷贝元素、释放旧空间)，即前面释放的空间只有1 + 2 = 3，假设第3次空间已经释放才只有1+2+4=7，而第四次需要8个空间，因此无法使用之前已释放的空间，但是按照小于2倍方式扩容，多次扩容之后就可以复用之前释放的空间了。

Linux中：内存heap区域被事先分配为2^n大小，以2的倍数扩容可以方便地进行分配

Win中：内存被free的区域会被系统立即合并，以1.5被分配可以使用被释放的内存



## 迭代器

作用：对于不同的数据结构，通过迭代器均可实现遍历，多态

 **[注意]：迭代器只能前进不能后退**



## 迭代器失效的情况

迭代器失效分三种情况考虑，也是分三种数据结构考虑，分别为数组型，链表型，树型数据结构。

**数组型数据结构：**该数据结构的元素是分配在连续的内存中，insert和erase操作，都会使得删除点和插入点之后的元素挪位置，所以，插入点和删除掉之后的迭代器全部失效，也就是说insert(*iter)(或erase(*iter))，然后在iter++，是没有意义的。解决方法：erase(*iter)的返回值是下一个有效迭代器的值。 iter =cont.erase(iter);

**链表型数据结构：**对于list型的数据结构，使用了不连续分配的内存，删除运算使指向删除位置的迭代器失效，但是不会失效其他迭代器.解决办法两种，erase(*iter)会返回下一个有效迭代器的值，或者erase(iter++).

**树形数据结构：** 使用红黑树来存储数据，插入不会使得任何迭代器失效；删除运算使指向删除位置的迭代器失效，但是不会失效其他迭代器.erase迭代器只是被删元素的迭代器失效，但是返回值为void，所以要采用erase(iter++)的方式删除迭代器。

注意：经过erase(iter)之后的迭代器完全失效，该迭代器iter不能参与任何运算，包括iter++,*ite



## 内联函数inline

作用：将函数入栈出栈的调用开销减少，（将函数展开为代码）

优势：

- 对于简单函数而言加快运行速度，省去了参数压栈、栈帧开辟与回收，结果返回等
- 相当于有类型检查的宏定义，且可以调试

缺陷：

- 以膨胀代码为代价
- 不能包含循环、递归等复杂操作
- 是否内联，程序员不可控。内联函数只是对编译器的建议，是否对函数内联，决定权在于编译器。
- 可能影响代码的执行效率，因为内联函数的本质是把函数体直接嵌入到调用处，这样会导致代码的大小增加，从而可能导致缓存命中率下降，影响执行效率。



## 哈希表

查表，要枚举的话时间复杂度是O(n)，但如果使用哈希表的话， 只需要O(1)就可以做到。我们只需要初始化把这所学校里学生的名字都存在哈希表里，在查询的时候通过索引直接就可以知道这位同学在不在这所学校里了。

哈希碰撞：

<img src="F:/本专业/计算机/笔记/嵌入式/嵌入式/LeetCode.assets/2021010423494884.png" alt="哈希表3" style="zoom: 50%;" />

解决方法：

链表法（tableSize=dataSize）

<img src="F:/本专业/计算机/笔记/嵌入式/嵌入式/LeetCode.assets/20210104235015226.png" alt="哈希表4" style="zoom:50%;" />

线性探测法（tableSize>=dataSize）将冲突的元素放到下一个空位中

<img src="F:/本专业/计算机/笔记/嵌入式/嵌入式/LeetCode.assets/20210104235109950.png" alt="哈希表5" style="zoom:50%;" />



## 哈希操作

1. 查找：当使用键进行查找时，哈希表会使用键的哈希值来确定其在哈希表中的位置，并进一步比较键的值来判断是否匹配。如果键相同，那么可以通过哈希值直接找到对应的位置，并返回存储在该位置上的值。
2. 插入：在向哈希表中插入键值对时，哈希表首先会计算键的哈希值，并根据哈希值找到对应的位置。然后，它会检查该位置上是否已经存在相同的键。如果存在相同的键，则可以选择更新现有的值，或者根据具体的实现策略来处理冲突。
3. 删除：当从哈希表中删除一个键值对时，哈希表会使用键的哈希值来定位该键所在的位置。如果在该位置上找到了匹配的键，就将其从哈希表中删除。



## 当两个对象映射到同一个哈希地址时，是否说明这两个对象相同

当两个对象产生哈希冲突时，它们被映射到了相同的哈希地址上，但并不能确定它们的内容是否相同。两个不同的对象完全可以具有相同的哈希值，因为哈希值只是一个对输入对象进行计算得出的结果。

要确定两个对象是否相同，通常需要使用其他方法，如比较它们的内容、引用或标识符等。哈希地址相同并不代表对象相同，只能说它们在哈希函数中产生了冲突。



## 哈希表如何解决键值冲突

哈希表（散列表）根据(Key value)直接进行访问的数据结构。映射函数叫做散列函数，存放记录的数组叫做散列表。

哈希值是通过哈希函数计算出来的，通过哈希函数计算出来的哈希值相同，就是哈希冲突，不能完全避免

解决方案：

1. 开放定址法：发现冲突后寻找下一个空闲散列表位置
2. 再哈希法：利用不同的哈希函数再次计算哈希值（多轮）
3. 链地址法：每个哈希表节点都有一个next指针，多个哈希表节点可以用next指针构成一个单向链表，被分配到同一个索引上的多个节点可以用这个单向链表连接起来，因而查找、插入和删除主要在同义词链中进行。
4. 公共溢出区法：冲突放入溢出表



## CMake是如何包含文件目录的

```cmake
target_include_directories(test PRIVATE ${YOUR_DIRECTORY})  #添加要包含的目录
set(SOURCES file.cpp file2.cpp ${YOUR_DIRECTORY}/file1.h ${YOUR_DIRECTORY}/file2.h)  #将头文件添加到当前目标的源文件列表中
add_executable(test ${SOURCES})
```



## extern c

C++不能直接调用C编译器编译的代码

在C++中可能调用C的代码段用关键字进行包裹，例子如下

extern "C" 修饰一段 C++ 代码，让编译器以处理 C 语言代码的方式来处理修饰的 C++ 代码。

```c++
// FUNC.h通用模板
#ifndef __INCvxWorksh /*防止该头文件被重复引用*/
#define __INCvxWorksh

#ifdef __cplusplus  //告诉编译器，这部分代码按C语言的格式进行编译，而不是C++的
extern "C"{
#endif
 
/* C语言实现的部分函数申明 */
    
/* C语言实现的部分函数申明 */
 
#ifdef __cplusplus
}
#endif

#endif /*end of __INCvxWorksh*/
```

extern "C"的主要作用就是为了能够正确实现C++代码调用其他C语言代码。加上extern "C"后，会指示编译器这部分代码按C语言（而不是C++）的方式进行编译。由于C++支持函数重载，因此编译器编译函数的过程中会将函数的参数类型也加到编译后的代码中，而不仅仅是函数名；而C语言并不支持函数重载，因此编译C语言代码的函数时不会带上函数的参数类型，一般只包括函数名。

```c++
//moduleA.h
int fun(int, int);
 
//moduleA.C
#include"moduleA"
int fun(int a, int b)
{
return a+b;
}
 
//moduleB.h
#ifdef __cplusplus //而这一部分就是告诉编译器，如果定义了__cplusplus(即如果是cpp文件， 
extern "C"{ //因为cpp文件默认定义了该宏),则采用C语言方式进行编译
#include"moduleA.h"
#endif
… //其他代码
 
#ifdef __cplusplus
}
#endif
 
//moduleB.cpp
#include"moduleB.h"
int main()
{
　　cout<<fun(2,3)<<endl;
}
```

总结

通常在C++ 中，假如需要使用C语言中的库文件的话，可以使用extern "C"去包含c编写的头文件



## 设计模式

单例模式

在整个系统生命周期内，保证一个类只能产生一个实例，确保该类的唯一性。成员函数均为static



工厂模式

抽象出一个工厂，工厂有不同的产线继承自工厂类，对于产品抽象出产品类



## 字符串string char*互转

```c
// char[] 转 char*
char ch[]="abcdef";
char *s = ch;

// char* 转 char[]
char *s="abcdef";
char ch[100];
strcpy(ch,s);

// string 转 char[]
string str= "abcdef";
char ch[20];
int i;
for( i=0;i<=str.length();i++){
    ch[i] = str[i];
    if(i==str.length()) c[i] = '\0';
}

// char[] 转 string
string str;
char ch[20] = "abcdef";
str = ch;

// string 转 char*
string str = "abcdef";
const char* p = (char*)str.data();  // data()仅返回字符串内容，而不含有结束符’\0’

string str=“abcdef”;
const char *p = str.c_str();
//使用char * p=(char*)str.c_str()效果相同

string str=“abcdef”+ '\0';
char *p= new char[str.length()+1];
str.copy(p,str.length(),0);  // 要想指针指向内容及地址不改变，使用该方式

// char* 转 string
string str;
char *p = "abcdef";
str = p;

char *p = "abcdef";
string str;
str.assign(p,strlen(p));  // 要想指针指向内容及地址不改变，使用该方式
```

## 1. 谈谈你对命名空间的理解

- namespace 是一个关键字：随着工程量的增加，变量命名上不可避免出现重名，防止名称冲突（在不同的命名空间中，即使 2 个变量名相同，也是 2 个不同的变量），在实际工作中，基本都使用标准命名空间
- 命名空间只能 **全局范围内** 定义，不能定义在函数内部
- 命名空间内， 可以存放 **变量、函数、结构体、类；也可以嵌套其他的命名空间**
- 命名空间可以 **匿名**（一般不这样使用），类似静态全局变量
- 命名空间是可以 **起别名** 的。

## 2. 谈谈指针与引用的区别

- 引用是给变量起别名，内部实现是指针常量 (int* const ref = &a), 其可以简单的理解为本体指针存放的是变量的地址
- 引用的本质是指针常量，其指向不可修改，而指针可以改变指向
- 引用创建的同时 **必须初始化** ，指针创建的时候可以不必初始化
- 引用不能为空，指针可以为 NULL
- “引用变量 ref“的内存单元保存的是"被引用变量 a "的地址
- sizeof（引用） = 指向变量的大小；sizeof（指针） = 指针本身大大小
- 引用使用的时候无需解引用，指针需要解引用
- 指针和引用“自增/自减运算“意义不一样
- 在同等需求下，函数传参，引用可以将一级指针

## 3. 谈谈你对内联函数的理解

- 宏函数缺陷 1：需要将实现“加 括号”，以保证优先级的完整性
- 宏函数缺陷 2：即使加了括号，有些情况，依然有缺陷
- 内联函数本身是一个真正的函数，但宏函数不是函数
- 内联函数具有普通函数的所有行为唯一不同处：内联函数会在适当的地方像定义宏一样展开，可以以空间换时间。因此，**内联函数既可以避免宏函数的缺陷，也可以避免普通函数入栈的时间浪费**
- 在普通函数函数面前加上 **inline** 关键字，否则视为普通函数
- 如果有函数声明，函数本身和声明必须同时加 **inline** 关键字，否则视为普通函数
- 任何在“类”内部定义的函数会自动成为内联函数
- 下列情况，普通函数即使指定为内联函数，编译器也可能考虑不按内联编译：
  - (1) 存在任何形式的循环语句
  - (2) 存在过多的条件判断语句
  - (3) 函数体过于庞大
  - (4) 对函数进行取址操作
- 使用方法建议：
  - (1) 内联仅仅只是给编译器一个建议，编译器不一定或接受这种建议
  - (2) 如果你没有将函数声明为内联函数，那么编译器也可能将此函数作内联编译（一个好的编译器将会内联小的，简单的函数）。因此，不用可以使用内联函数，可以交给编译器去自行处理。

## 4. 谈谈函数的重载条件

- 函数重载：在 C 语言中，函数名必须是唯一的，程序中不允许出现同名的函数。在 C++ 中是允许出现同名的函数，即在同一作用域内，具有相同函数名，不同参数列表的一组函数，称为函数重载
- 函数重载实现的原理：
  - 编译器为了实现函数重载，是做了一些幕后的工作，编译器用不同的参数类型来修饰不同的函数名，比如 void func(); 编译器可能会将函数名修饰成 func ，当编译器碰到 void func(int x)， 编译器可能将函数名修饰为 func_int, 当编译器碰到 void func(int x,char c), 编译器可能会将函数名修饰为 func_int_char ，这里使用“可能”这个字眼是因为编译器如何修饰重载的函数名称并没有一个统一的标准，所以不同的编译器可能会产生不同的内部名。
- 函数重载实现的条件：
  - 统一个作用域，参数的个数不同，参数的类型不同，参数的顺序不同

## 5. 谈谈 C 与 C++ 中 struct 的不同点

- C 语言中的 struct 只有数据
- C++ 中的 struct 不止有数据，还有函数

## 6. 如何理解 C++ 的密封性

- 封装特性主要包含两个方面，一个是属性和变量合成一个整体，一个是给属性和函数增加一个访问权限

## 7. 谈谈你对 C++ 构造和析构的理解

- 对象的初始化和清理也是两个非常重要的安全问题，一个对象或者变量没有初始化，对其使用后果是未知，同样的使用完一个变量，没有及时清理，也会造成一定的安全问题。 C++ 为了给我们提供这种问题的解决方案，构造函数和析构函数，这两个函数会将被编译器自动调用，完成对象初始化和对象的清理工作。
- 无论你是否喜欢，对象的初始化和清理工作是编译器强制我们要做的事情，即使你不提供初始化操作和清理操作，编译器也会给你增加默认的操作，只是这个默认初始化不会做任何事，所以编写类就应该顺便提供初始化函数。
- 构造函数主要作用在于创建对象时为对象的成员属性赋值。析构函数由编译器自动调用，无须手动调用。主要用于对象销毁前系统自动调用，执行一些清理工作。

## 8. 构造函数的分类

- 按参数类型：分为无参构造函数和有参构造函数
- 按类型分类：普通构造函数和拷贝构造函数（复制构造函数）

## 9. 构造函数的调用规则

- 默认情况下，C++ 编译器至少为我们写的类增加 3 个函数：默认构造函数（无参，函数体为空）、默认析构函数（无参，函数体为空）、默认拷贝构造函数，对类中非静态成员属性简单拷贝
- 如果用户定义构造函数， C++ 不会再提供任何默认构造函数；如果用户定义了普通构造（非拷贝）， C++ 不再提供默认无参构造，但是会提供默认拷贝构造。

## 10. 谈谈你对浅拷贝和深拷贝的区别

- 浅拷贝：同一类型的对象之间可以赋值，使得两个对象的成员变量的值相同，两个对象仍然是独立的两个对象，这种情况被称为浅拷贝。一般情况下，浅拷贝没有任何副作用，但是当类中有指针，并且指针指向动态分配的内存空间，析构函数做了动态内存释放的处理，会导致内存问题。
- 深拷贝：当类中有指针，并且此指针有动态分配空间，析构函数做了释放处理，往往需要自定义拷贝构造函数，自行给指针动态分配空间，深拷贝。

## 11. 谈谈啥叫对象成员以及对象成员的构造函数调用方式

- **在类中定义的数据成员一般是基本的数据类型。但是类中的成员也可以是对象，叫对象成员。**
- C++ 中对对象的初始化是非常重要的操作，当创建一个对象的时候， C++ 编译器必须确保调用了所有子对象的构造函数。如果所有的子对象有默认，编译器可以自动调用他们。但是如果子对象没有默认的构造函数，或者想指定调用某个构造函数怎么办？那么是否可以在类的构造函数直接调用子类的属性完成初始化呢？但是如果子类的成员属性是私有的，我们是没有办法访问并完成初始化的。
- 解决办法非常简单：对于子类调用构造函数， C++ 为此提供了专门的语法，即构造函数初始化列表。 **当调用构造函数时，首先按各对象成员在类定义中的顺序（和参数列表的顺序无关）一次调用它们的构造函数，对这些对象初始化，最后再调用本身的函数体。也就是说，先调用对象成员的构造函数，再调用本身的构造函数。析构函数和构造函数调用的顺序相反，先构造，在析构。**

## 12. 谈谈你对 explicit 的理解

- C++ 提供了关键字 explicit ，禁止通过构造函数进行的隐式转换。声明为 explicit 的构造函数不能在隐式转换中使用。

## 13. 探谈 c 中 malloc free 与 c++ 中的 new delete 有啥区别，或者在 C++ 在堆区申请对象是为啥推荐使用 new delete ？

- C++ 中的 malloc free 的问题：
  - (1) 程序员必须确定对象的长度
  - (2) malloc 返回一个 void 指针， C++ 不允许将 void 赋值给其他任何指针，必须强制
  - (3) malloc 可能申请内存失败，所以必须判断返回值来确保内存分配成功
  - (4) 用户在使用对象之前必须记住对他初始化，构造函数不能显示调用初始化（构造函数是由编译器调用的），用户有可能忘记调用初始化函数
  - 总结： C 的动态内存分配太复杂，容易令人混淆，是不可接受的
- C++ 中推荐使用 new，delete
  - new 操作符能确定在调用构造函数初始化之前，所有不用显式确定调用是否成功
  - delete 表达式先调用析构函数，然后释放内存

## 14. 谈谈你对 static 静态成员变量的理解

- 在一个类中，若将一个成员函数声明为 static ，这种成员称为静态成员变量。与一般的数据成员不同，无论建立了多少个对象，都只有一个静态数据的拷贝。静态成员变量，属于某个类，所有对象共享。
- 静态变量，是在编译阶段就分配空间，对象还没有创建时，就已经分配空间
- 注意：
  - (1) 静态成员变量必须在类中声明，在类外定义。
  - (2) 静态数据成员不属于某个对象，在为对象分配空间中不包括静态成员所占空间
  - (3) 静态数据成员可以通过类名或者对象名来引用

## 15. 谈谈你对 static 静态成员函数的理解

- 在类定义中，前面有 static 说明的成员函数称为静态成员函数。静态成员函数使用方式和静态变量一样，同样在对象没有创建前，即可通过类名调用。静态成员函数主要为了访问静态变量，但是，不能访问普通成员变量。
- 静态成员函数的意义，不在于信息共享，数据沟通，而在于管理静态数据成员，完成对静态数据成员的封装。
- 注意：
  - (1) 静态成员函数只能访问静态变量，不能访问普通成员变量
  - (2) 静态成员函数的使用和静态成员变量一样
  - (3) 静态成员函数也有访问权限
  - (4) 普通成员函数可以访问静态成员变量，也可以访问非静态成员变量

## 16. 谈谈你对 this 的理解

- 成员函数通过 this 指针即可知道操作的是那个对象的数据。 This 指针是一种隐含指针，它隐含余每个类的非静态成员函数中。 This 指针无需定义，直接使用即可。
- 注意：静态成员函数内部没有 this 指针，静态成员函数不能操作非静态成员变量

## 17. 谈谈你对友元的理解

- 类的主要特点之一是数据隐藏，即类的私有成员无法在类的无不（作用域之外）访问。但是，有时候需要在类的外部访问类的私有成员，怎么办？解决方法是使用友元函数，友元函数是一种特权函数， C++ 允许这个特权函数访问私有成员
  - 1. friend 关键字只出现在声明处
  - 2. 其他类，类成员函数，全局函数都可以声明为友元
  - 3. 友元函数不是类的成员，不带 this 指针
  - 4. 友元函数可以访问对象任意成员属性，包括私有属性
- 友元的注意事项：
  - (1) 友元关系不能被继承
  - (2) 友元关系是单向的，类 A 是类 B 的朋友，但类 B 不一定是类 A 的朋友。
  - (3) 友元关系不具有传递性。类 B 是类 A 的朋友，类 C 是类 B 的朋友，但类 C 不一定是类 A 的朋友。

##  C++11、C++14、C++17、C++20 新特性

1. C++11 新特新 - static_assert 编译时断言 - 新增加类型 long long ，unsigned long long，char16_t，char32_t，原始字符串 - auto - decltype - 委托构造函数 - constexpr - 模板别名 - alignas - alignof - 原子操作库 - nullptr - 显示转换运算符 - 继承构造函数 - 变参数模板 - 列表初始化 - 右值引用 - Lambda 表达式 - override、final - unique_ptr、shared_ptr - initializer_list - array、unordered_map、unordered_set - 线程支持库
2. C++14 新特新 - 二进制字面量 - 泛型 Lambda 表达式 - 带初始化/泛化的 Lambda 捕获 - 变量模板 - [[deprecated]]属性 - std::make_unique - std::shared_timed_mutex、std::shared_lock - std::quoted - std::integer_sequence - std::exchange
3. C++17 新特新 - 构造函数模板推导 - 结构化绑定 - 内联变量 - 折叠表达式 - 字符串转换 - std::shared_mutex
4. C++20 新特新 - 允许 Lambda 捕获 [=, this] - 三路比较运算符 - char8_t - 立即函数（consteval） - 协程 - constinit



## C++11中的atomic

C++11引入了一组原子类型（Atomic Types），用于解决多线程环境下的并发访问问题。原子类型保证了对变量的读写操作是原子的，即不会发生数据竞争。

```c++
#include <iostream>
#include <atomic>
#include <thread>

std::atomic<int> counter(0);

void incrementCounter() {
    for (int i = 0; i < 1000; ++i) {
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

int main() {
    std::thread t1(incrementCounter);
    std::thread t2(incrementCounter);

    t1.join();
    t2.join();

    std::cout << "Counter value: " << counter << std::endl;

    return 0;
}
```

# C++多线程



## pthread_create

创建子线程，并注册回调函数



## pthread_join

在主线程中调用，等待子线程执行完毕后，释放子线程资源，再执行join后的代码



## pthread_detach

主线程在调用pthread_detach(子线程ID) 与pthread_exit(NULL)后，不用等待Join才可释放子线程资源，在子线程结束运行前，主线程可以执行其他功能，子线程运行结束后资源由OS而非主线程释放

<img src="F:/本专业/计算机/笔记/嵌入式/嵌入式/嵌入式笔记.assets/image-20230306212906250.png" alt="image-20230306212906250" style="zoom:50%;" />



## pthread_cancel

在主线程中杀死子线程（通过系统调用，延迟杀死线程）



## pthread_equal

比较两个线程ID是否一致



## 父子进程fork()

当一个进程调用 `fork` 函数生成另一个进程，原进程就称为父进程，新生成的进程则称为子进程。

```c++
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char * argv[])
{
    int pid;
    /* fork another process */
    pid = fork();
    if (pid < 0) 
    { 
        /* error occurred */
        fprintf(stderr,"Fork Failed!");
        exit(-1);
    } 
    else if (pid == 0) 
    {
        /* child process */
        printf("This is Child Process!\n");
    } 
    else 
    {  
        /* parent process  */
        printf("This is Parent Process!\n");
        /* parent will wait for the child to complete*/
        wait(NULL);
        printf("Child Complete!\n");
    }
}
```

![image-20230704091925404](F:/本专业/计算机/笔记/嵌入式/嵌入式/嵌入式笔记.assets/image-20230704091925404.png)

可以建立一个新进程，把当前的进程分为父进程和子进程，新进程称为子进程，而原进程称为父进程。fork调用一次，返回两次，这两个返回分别带回它们各自的返回值，其中在父进程中的返回值是子进程的PID，而子进程中的返回值则返回 0。因此，可以通过返回值来判定该进程是父进程还是子进程。





## 孤儿进程

父进程释放，子进程还在，内核接管，没有危害



## 僵尸进程

子进程退出，父进程不知道，此时子进程还占用着资源

在 Linux 环境中，我们是通过 `fork` 函数来创建子进程的。创建完毕之后，父子进程独立运行，父进程无法预知子进程什么时候结束。通常情况下，子进程退出后，父进程会使用 `wait` 或 `waitpid` 函数进行回收子进程的资源，并获得子进程的终止状态。但是，如果父进程先于子进程结束，则子进程成为孤儿进程。孤儿进程将被 init 进程（进程号为1）领养，并由 init 进程对孤儿进程完成状态收集工作。而如果子进程先于父进程退出，同时父进程太忙了，无瑕回收子进程的资源，子进程残留资源（PCB）存放于内核中，变成僵尸（Zombie）进程

解决方案：

- 父进程在子进程退出时调用wait()/waitpid()函数来回收子进程的资源
- fork一个孙子进程，然后将子进程变为一个孤儿进程可以避免僵尸进程的产生
- 杀死这个僵尸进程的父进程。那么该僵尸进程就会被守护进程给领养。从而守护进程，会对这个僵尸进程的内核区资源进行回收。

僵尸进程其实已经就是退出的进程，因此无法再利用kill命令杀死僵尸进程。僵尸进程的罪魁祸首是父进程没有回收它的资源，那我们可以想办法它其它进程去回收僵尸进程的资源，这个进程就是 init 进程。因此，我们可以直接杀死父进程，init 进程就会很善良地把那些僵尸进程领养过来，并合理的回收它们的资源，那些僵尸进程就得到了妥善的处理了。

例如，如果 PID 5878 是一个僵尸进程，它的父进程是 PID 4809，那么要杀死僵尸进程 （5878），您可以结束父进程 （4809）：

```
$ sudo kill -9 4809  #4809 is the parent, not the zombie
```



## 守护进程

前台进程是在终端中运行的命令，那么该终端就为进程的控制终端，一旦这个终端关闭，这个进程也随着消失

- 而守护进程（Daemon），也就是我们平时说的后台进程，是运行在后台的一种特殊进程，不受终端控制，它不需要终端的交互
- 守护进程是一种特殊的后台进程，它通常在系统启动时自动启动，并在系统运行过程中一直运行，执行一些系统级别的任务，如监控系统资源、处理网络请求等。守护进程通常不与用户交互，也不需要终端，而是在后台默默地运行，直到系统关闭或者被显式地停止。



## 协程

C++20新增

协程不受操作系统调度，切换方便，轻量级

1. 依赖关系：线程是由操作系统内核进行调度管理的，并且每个线程通常拥有自己的独立堆栈和上下文。而协程则是由程序员在代码中显式地定义和管理的，没有操作系统参与调度。协程依赖于某种运行时环境或者特定的库来实现调度和切换。
2. 并发性能：线程属于操作系统层面的并发机制，它可以充分利用多核处理器的计算能力。每个线程都需要一定的系统资源来进行管理，因此**创建大量线程可能会导致资源消耗过大**。相比之下，协程是轻量级的，可以在单个线程中运行大量的协程，节省了线程切换的开销。
3. 切换机制：在**线程**之间进行**切换**时，需要进行上下文的保存和恢复，这是由操作系统**内核负责**完成的，并且通常涉及到**用户态和内核态之间的切换**。而**协程**的**切换**是在**用户态完成**的，切换开销更小。协程通过手动选择合适的切换点，在不同的协程之间进行切换，使得程序可以在合适的时机保存和恢复中间状态。
4. 同步方式：**线程**通常通过**共享内存或者消息传递**来进行通信和同步。而**协程**则通常通过**显式的调度和消息传递**机制来实现数据共享和同步。协程之间的切换是协作性的，需要各个协程自行决定何时让出执行权。

总的来说，线程更加底层和系统级别，可以充分利用多核处理器的并行计算能力，但线程数量受限于系统资源，并且线程切换开销较大。而协程是一种高级抽象，更适合处理大量的轻型任务，并且协程之间的切换开销较小。但协程需要依赖特定的运行时环境或库的支持，无法直接利用多核处理器的并行计算能力。

