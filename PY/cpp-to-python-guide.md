# C++ 转 Python 学习指南

> 面向有 C++ 基础的开发者，通过对照方式系统学习 Python

---

## 第一章：变量与基本类型

### Python 基本数据类型

| 类型 | Python | C++ |
| ------ | -------- | ----- |
| 整型 | `int`（任意精度） | `int`（固定位数，通常 32 位） |
| 浮点 | `float`（双精度） | `double` 或 `float` |
| 布尔 | `bool`（`True`/`False`） | `bool`（`true`/`false`） |
| 字符串 | `str` | `std::string` |

### 动态类型 vs 静态类型

Python 是**动态类型**语言，变量不需要声明类型，类型在运行时确定且可以改变。

```python
# Python
x = 42       # int
x = "hello"  # 重新赋值成 str，合法
print(type(x))  # <class 'str'>
```

```cpp
// C++
int x = 42;
x = "hello";  // 编译错误！类型不可变
```

### 变量定义方式

```python
# Python：直接赋值即可
a = 10
b = 3.14
c = True
d = "hello"
e = None  # 空对象（NoneType 单例，不是空指针）
```

```cpp
// C++：需要显式声明类型
int a = 10;
double b = 3.14;
bool c = true;
std::string d = "hello";
int* e = nullptr;
```

### 类型推导对比

```python
# Python：完全动态推导
x = 42          # int
y = 3.14        # float
z = "hello"     # str
```

```cpp
// C++11 起支持 auto（编译时静态推导）
auto x = 42;        // int
auto y = 3.14;      // double
auto z = "hello";   // const char*
```

> C++ 的 `auto` 是编译期静态推导，类型确定后不可变；Python 的类型是运行时动态的，变量可以任意改变类型。

### Python 执行模型（CPython）

理解 Python 如何运行是 C++ 程序员的关键认知跃迁。

### 编译 vs 解释

| 特性 | Python | C++ |
| ------- | ------ | ----- |
| 编译方式 | 源码 → 字节码 → PVM 执行 | 源码 → 机器码 → CPU 执行 |
| 阶段 | `.py` → `.pyc`（字节码）→ PVM | `.cpp` → `.o` → 链接 → 可执行文件 |
| 中间表示 | 字节码（platform-independent） | 机器码（platform-specific） |
| 运行时 | 需要 Python 解释器 | 独立可执行文件 |

### Python 执行流程

```text
Python 源码 (.py)
    ↓ Parser（解析器）
抽象语法树 (AST)
    ↓ Compiler（编译器）
字节码 (.pyc / __pycache__/)
    ↓ PVM（Python Virtual Machine）
执行结果
```

```python
# 查看字节码
import dis

def add(a, b):
    return a + b

dis.dis(add)
# 输出示例：
#   0 LOAD_FAST    0 (a)
#   1 LOAD_FAST    1 (b)
#   2 BINARY_ADD
#   3 RETURN_VALUE
```

```cpp
// C++ 编译流程
// .cpp  →  汇编  →  机器码  →  可执行文件
// g++ -c main.cpp -o main.o
// g++ main.o -o main
// ./main （直接 CPU 执行，无需运行时环境）
```

```cpp
// C++ 查看汇编
// g++ -S main.cpp -o main.s
// main.s 中是汇编指令，直接对应 CPU
```

### Python 慢在哪里？

| 原因 | 说明 |
| ---- | --- |
| 解释执行 | 字节码解释比机器码慢 |
| 动态类型 | 每次操作需检查类型 |
| 动态分发 | 方法调用需查表（非直接跳转） |
| 对象模型开销 | 所有值都是对象，有引用计数等开销 |

> Python ≈ C++ 性能的 1/10 ~ 1/50，但开发效率高 2-5 倍。
> 性能瓶颈可用 C 扩展（Cython、pybind11）解决。

---

## 第二章：运算符体系

### 算术运算符

| 操作 | Python | C++ |
| ------ | -------- | ----- |
| 加法 | `+` | `+` |
| 减法 | `-` | `-` |
| 乘法 | `*` | `*` |
| 除法 | `/`（结果总是 float） | `/`（整数除法截断） |
| 整除 | `//` | 无对应，需手动实现 |
| 取模 | `%` | `%` |
| 幂运算 | `**` | `pow()` |
| 自增/自减 | 无 `++`/`--` | `++`/`--` |

```python
# Python
print(5 / 2)    # 2.5 (float)
print(5 // 2)   # 2 (整除，向下取整)
print(5 ** 3)   # 125 (幂运算)
print(-5 // 2)  # -3 (注意：向下取整)
```

```cpp
// C++
cout << 5 / 2;    // 2 (整数除法，截断)
cout << 5 / 2.0;  // 2.5
cout << pow(5, 3); // 125
```

### 逻辑运算符

| 操作 | Python | C++ |
| ------ | -------- | ----- |
| 逻辑与 | `and` | `&&` |
| 逻辑或 | `or` | `\|\|` |
| 逻辑非 | `not` | `!` |

```python
# Python
if a > 0 and b < 10:
if not flag:
if a == 0 or b == 0:
```

```cpp
// C++
if (a > 0 && b < 10)
if (!flag)
if (a == 0 || b == 0)
```

### 比较运算符

大部分一致：`==` `!=` `<` `>` `<=` `>=`

Python 支持**链式比较**：

```python
# Python
if 0 < x < 10:   # x > 0 and x < 10
    pass
```

```cpp
// C++：必须分开写
if (0 < x && x < 10) {}
```

### 位运算

两者一致：`&` `|` `^` `~` `<<` `>>`

### Python 特有运算符

```python
# in：判断是否在容器中
if "key" in my_dict:
    pass

# is：判断是否为同一对象（比较 identity，CPython 中类似地址比较但不完全等价）
if a is None:   # 等价于 a == None 但更推荐
    pass

# not in：不在容器中
if x not in my_list:
    pass
```

### 海象运算符 `:=`（赋值表达式，Python 3.8+）

`:=` 允许在表达式内部**同时完成赋值和判断**，减少重复计算：

```python
# 不用 := 的写法
data = get_data()
if data:
    print(f"Got: {data}")

# 用 := 的写法（赋值 + 判断在一行）
if (data := get_data()):
    print(f"Got: {data}")

# while 循环中的经典用法
while (line := file.readline()):
    print(line.strip())

# 列表推导式中避免重复计算
[expensive(x) for x in data if expensive(x) > 0]
# ↓ 用 := 只计算一次
[result for x in data if (result := expensive(x)) > 0]

# 正则匹配
if (match := re.search(r"\d+", text)):
    print(f"Found: {match.group()}")
```

```cpp
// C++ 没有直接对应（C++17 的 if-init 最接近）
if (auto data = getData(); !data.empty()) {
    std::cout << "Got: " << data;
}

// while 条件中赋值
std::string line;
while (std::getline(file, line)) {
    std::cout << line;
}

// C++ 没有能在表达式内部赋值的运算符
// 赋值是语句，不能嵌入表达式
```

> `:=` 常用于 `if`/`while` 条件 + 列表推导式，本质是**减少重复求值**。C++ 没有对应语法，但 C++17 的 `if (init; condition)` 起到了类似效果。

---

## 第三章：控制流

### 缩进语法 vs 花括号

**Python 用缩进表示代码块，C++ 用花括号 `{}`。**

```python
# Python
if x > 0:
    print("positive")
    if x > 10:
        print("large")
else:
    print("not positive")
```

```cpp
// C++
if (x > 0) {
    cout << "positive";
    if (x > 10) {
        cout << "large";
    }
} else {
    cout << "not positive";
}
```

### if / elif / else

```python
# Python
score = 85
if score >= 90:
    grade = "A"
elif score >= 80:
    grade = "B"
elif score >= 70:
    grade = "C"
else:
    grade = "F"
```

```cpp
// C++
int score = 85;
std::string grade;
if (score >= 90) {
    grade = "A";
} else if (score >= 80) {
    grade = "B";
} else if (score >= 70) {
    grade = "C";
} else {
    grade = "F";
}
```

### for 循环

```python
# Python - range()
for i in range(5):       # 0,1,2,3,4
    print(i)
for i in range(2, 8):    # 2,3,4,5,6,7
    print(i)
for i in range(0, 10, 2): # 0,2,4,6,8
    print(i)

# Python - 遍历容器
for item in my_list:
    print(item)

# Python - 带索引遍历
for idx, val in enumerate(my_list):
    print(idx, val)
```

```cpp
// C++ 传统 for
for (int i = 0; i < 5; i++) {
    cout << i;
}

// C++ 范围 for (C++11)
for (const auto& item : my_list) {
    cout << item;
}
```

### while 循环

```python
# Python
i = 0
while i < 5:
    print(i)
    i += 1  # 注意：没有 ++ 运算符
```

```cpp
// C++
int i = 0;
while (i < 5) {
    cout << i;
    i++;
}
```

### break / continue

两者用法完全一致：

```python
# Python
for i in range(10):
    if i == 3:
        continue  # 跳过本次
    if i == 7:
        break     # 退出循环
```

```cpp
// C++
for (int i = 0; i < 10; i++) {
    if (i == 3) continue;
    if (i == 7) break;
}
```

---

## 第四章：函数体系

### 函数定义对比

```python
# Python
def add(a, b):
    return a + b

result = add(3, 5)  # 8
```

```cpp
// C++
int add(int a, int b) {
    return a + b;
}

int result = add(3, 5);  // 8
```

### 返回值

```python
# Python：无需声明返回类型，可返回多种类型
def process(x):
    if x > 0:
        return x * 2
    else:
        return "negative"  # 返回不同类型
```

```cpp
// C++：必须声明返回类型，类型固定
int process(int x) {
    if (x > 0) return x * 2;
    else return -1;  // 必须返回 int
}
```

### 参数传递

```python
# Python：所有参数都是"对象引用传递"
def modify(lst):
    lst.append(4)    # 修改传入的 list
    lst = [1, 2, 3]  # 重新绑定，不影响外部

my_list = [0]
modify(my_list)
print(my_list)  # [0, 4]
```

```cpp
// C++：值传递（拷贝）/ 引用传递
void modify(std::vector<int>& lst) {
    lst.push_back(4);  // 修改原始对象
    lst = {1, 2, 3};   // 也修改原始对象（因为引用）
}
```

### 默认参数

```python
# Python
def greet(name, greeting="Hello"):
    print(f"{greeting}, {name}")

greet("Alice")           # Hello, Alice
greet("Bob", "Hi")       # Hi, Bob
```

```cpp
// C++
void greet(const std::string& name,
           const std::string& greeting = "Hello") {
    std::cout << greeting << ", " << name;
}
```

### 可变参数

```python
# Python - *args 接收任意数量位置参数（元组）
def sum_all(*args):
    return sum(args)

print(sum_all(1, 2, 3, 4))  # 10

# Python - **kwargs 接收任意数量关键字参数（字典）
def print_kwargs(**kwargs):
    for k, v in kwargs.items():
        print(f"{k} = {v}")

print_kwargs(name="Alice", age=25)
```

```cpp
// C++：可变参数模板或 initializer_list
template<typename... Args>
auto sum_all(Args... args) {
    return (args + ...);  // C++17 fold expression
}
// 或使用 initializer_list
int sum_all(std::initializer_list<int> args) {
    int sum = 0;
    for (int x : args) sum += x;
    return sum;
}
```

### lambda 表达式

```python
# Python
add = lambda x, y: x + y
print(add(3, 4))  # 7

# 排序时使用
pairs = [(1, 2), (3, 1), (5, 0)]
pairs.sort(key=lambda p: p[1])  # 按第二个元素排序
```

```cpp
// C++ (C++11)
auto add = [](int x, int y) { return x + y; };
cout << add(3, 4);  // 7

std::vector<std::pair<int,int>> pairs = {{1,2}, {3,1}, {5,0}};
std::sort(pairs.begin(), pairs.end(),
    [](const auto& a, const auto& b) { return a.second < b.second; });
```

> Python lambda 只能写单行表达式，C++ lambda 可以有多条语句并支持捕获列表。

### 函数是一等公民（First-class Function）

Python 中函数是**对象**，可以像数据一样传递：

```python
# 函数可以赋值给变量
def greet(name):
    return f"Hello, {name}"

f = greet           # 函数对象赋值
print(f("Alice"))   # Hello, Alice

# 函数可以作为参数传递
def apply(func, value):
    return func(value)

print(apply(greet, "Bob"))  # Hello, Bob

# 函数可以嵌套定义
def make_multiplier(n):
    def multiplier(x):
        return x * n
    return multiplier       # 返回函数

double = make_multiplier(2)
print(double(5))            # 10
```

```cpp
// C++：函数指针 / std::function / lambda
int greet(const std::string& name) {
    std::cout << "Hello, " << name;
    return 0;
}

// 函数指针
int (*f)(const std::string&) = greet;

// std::function
std::function<int(const std::string&)> func = greet;

// 但是函数不能嵌套定义（lambda 可以）
auto make_multiplier(int n) {
    return [n](int x) { return x * n; };  // C++14
}
auto double_ = make_multiplier(2);
std::cout << double_(5);  // 10
```

### 闭包（Closure）

闭包是**捕获了外部环境变量的函数**：

```python
def counter(start=0):
    count = [start]       # 用 list 实现可变捕获
    def inc():
        count[0] += 1
        return count[0]
    return inc

c = counter(10)
print(c())  # 11
print(c())  # 12

# Python 3 中可以用 nonlocal
def counter2(start=0):
    count = start
    def inc():
        nonlocal count   # 声明 count 来自外层
        count += 1
        return count
    return inc
```

```cpp
// C++ lambda 通过捕获列表实现闭包
auto counter(int start = 0) {
    return [count = start]() mutable {
        return ++count;
    };
}

auto c = counter(10);
std::cout << c();  // 11
std::cout << c();  // 12
```

### LEGB 作用域规则

Python 查找变量的顺序：**Local → Enclosing → Global → Built-in**

```python
x = "global"          # G: Global

def outer():
    x = "enclosing"   # E: Enclosing
    def inner():
        x = "local"   # L: Local
        print(x)
    inner()

outer()  # "local"

# LEGB 查找顺序示例
def test():
    print(print)       # 先找 Local → Enclosing → Global → Built-in
                       # Built-in 找到 print 函数
```

```cpp
// C++：作用域查找规则
// 局部 → 类作用域 → 命名空间 → 全局
// 与 Python 不同，C++ 是编译期静态绑定

int x = 10;           // 全局

namespace MyNS {
    int x = 20;       // 命名空间
    void func() {
        int x = 30;   // 局部
        std::cout << x;  // 30（最内层优先）
    }
}
```

---

## 第五章：容器类型

### 结构对照表

| Python | C++ STL | 特性 |
| -------- | --------- | ------ |
| `list` | `std::vector` | 动态数组 + 指针数组（可存任意类型） |
| `tuple` | `std::pair` / `std::tuple` | 不可变序列 |
| `dict` | `std::unordered_map` | 哈希表，Python 3.7+ 有序（C++ 无序） |
| `set` | `std::unordered_set` | 哈希集合，元素唯一 |
| `list`（双端） | `std::deque` | Python list 本质是动态数组 |

### list vs vector

> Python list ≈ 动态数组 + `void*` 数组（可存任意类型）
> C++ `std::vector` 只能存单一类型

```python
# Python list（可以混合类型）
lst = [1, "hello", 3.14, [1, 2]]
lst.append(4)         # 尾插
lst.insert(0, 0)      # 头插
lst.pop()             # 尾删
lst.remove(2)         # 删除第一个匹配元素
print(lst[0])         # 索引访问
print(len(lst))       # 长度
lst.sort()            # 原地排序
reversed(lst)         # 反转迭代器
[1, 2] + [3, 4]      # [1,2,3,4] 拼接
```

```cpp
// C++ vector
std::vector<int> vec = {1, 2, 3};
vec.push_back(4);             // 尾插
vec.insert(vec.begin(), 0);   // 头插
vec.pop_back();               // 尾删（没有 remove by value）
vec.erase(std::remove(vec.begin(), vec.end(), 2), vec.end()); // 删除指定值
cout << vec[0];               // 索引访问
cout << vec.size();           // 长度
std::sort(vec.begin(), vec.end());  // 排序
std::reverse(vec.begin(), vec.end());
```

### dict vs unordered_map

> Python 3.7+ dict **保持插入顺序**
> C++ `std::unordered_map` **不保证顺序**

```python
# Python dict（3.7+ 有序）
d = {"name": "Alice", "age": 25}
d["city"] = "Beijing"     # 添加/修改
print(d["name"])          # 访问（KeyError 若不存在）
print(d.get("name"))      # 安全访问，不存在返回 None
print("name" in d)        # 检查键是否存在
d.pop("age")              # 删除
for k, v in d.items():    # 遍历（按插入顺序）
    print(k, v)
d.keys()                  # 所有键
d.values()                # 所有值
```

```cpp
// C++ unordered_map（无序）
std::unordered_map<std::string, int> m = {{"Alice", 25}};
m["Bob"] = 30;                  // 添加/修改
cout << m["Alice"];             // 访问（不存在则插入默认值）
auto it = m.find("Alice");
if (it != m.end()) cout << it->second;  // 安全访问
if (m.count("Alice"))           // 检查键是否存在
m.erase("Alice");               // 删除
for (const auto& [k, v] : m)    // C++17 遍历（顺序不确定）
    cout << k << v;
// 需用 std::map 才能保证有序（但 O(log n)）
```

### tuple vs pair

```python
# Python tuple
t = (1, "hello", 3.14)
print(t[0])          # 索引访问
a, b, c = t          # 解包
# 不可变：t[0] = 2  # 错误！
```

```cpp
// C++ pair / tuple
auto p = std::make_pair(1, "hello");
cout << p.first << p.second;

auto t = std::make_tuple(1, "hello", 3.14);
cout << std::get<0>(t);
auto [a, b, c] = t;  // C++17 结构化绑定
```

### set vs unordered_set

```python
# Python set
s = {1, 2, 3}
s.add(4)
s.remove(2)           # KeyError if not exists
s.discard(5)          # 不存在也不会报错
print(3 in s)         # 检查存在性
s1 | s2               # 并集
s1 & s2               # 交集
s1 - s2               # 差集
```

```cpp
// C++ unordered_set
std::unordered_set<int> s = {1, 2, 3};
s.insert(4);
s.erase(2);
if (s.count(3))                    // 检查存在性
std::set_union(s1.begin(), s1.end(), ...);  // 需要操作
```

---

## 第六章：字符串处理

### 字符串不可变性

**Python 字符串是不可变的**，每次修改都是创建新字符串。

```python
# Python
s = "hello"
# s[0] = "H"        # 错误！不可变
s = "H" + s[1:]     # 必须创建新字符串
```

```cpp
// C++ string 是可变的
std::string s = "hello";
s[0] = 'H';          // 可以直接修改
```

### 切片操作

Python 切片是强大特性，C++ 需要手动实现：

```python
# Python 切片 [start:stop:step]
s = "hello world"
print(s[0:5])      # "hello"
print(s[6:])       # "world"
print(s[::-1])     # "dlrow olleh" 反转
print(s[::2])      # "hlowrd"
```

```cpp
// C++ substr
std::string s = "hello world";
cout << s.substr(0, 5);   // "hello"
cout << s.substr(6);      // "world"
// 反转
std::reverse(s.begin(), s.end());  // 原地修改
// 步进需手动
```

### 拼接方式

```python
# Python 字符串拼接
s1 = "Hello"
s2 = "World"

# 方式1：+
s = s1 + ", " + s2

# 方式2：f-string（推荐）
s = f"{s1}, {s2}"

# 方式3：join（大量拼接时高效）
parts = ["Hello", "World"]
s = ", ".join(parts)

# 方式4：format
s = "{}, {}".format(s1, s2)
```

```cpp
// C++ 拼接
std::string s1 = "Hello", s2 = "World";

// + 运算符
std::string s = s1 + ", " + s2;

// +=
std::string s = s1;
s += ", " + s2;

// stringstream
std::ostringstream oss;
oss << s1 << ", " << s2;
```

### 常用函数

```python
# Python
s = "  Hello World  "
s.lower()          # "hello world"
s.upper()          # "HELLO WORLD"
s.strip()          # 去除首尾空白
s.split()          # ["Hello", "World"]
s.split(",")       # 按逗号分割
", ".join(["a", "b"])  # "a, b"
s.find("World")    # 返回索引，-1 表示未找到
s.replace("World", "Python")  # 替换
s.startswith("He") # True
s.endswith("ld")   # True
len(s)             # 长度
```

```cpp
// C++ (C++11/17)
std::string s = "  Hello World  ";
std::transform(s.begin(), s.end(), s.begin(), ::tolower);
// 没有直接的 split，需要自己实现或用 boost
size_t pos = s.find("World");
s.replace(pos, 5, "Python");
s.substr(start, length);
```

> Python 的字符串操作比 C++ 简洁得多，尤其是 `split`、`join`、`strip` 等在日常开发中高频使用。

---

## 第七章：类与面向对象

### 类定义

```python
# Python
class Dog:
    # 类变量（类似 C++ static）
    species = "Canis familiaris"

    # 构造函数
    def __init__(self, name, age):
        self.name = name    # 实例变量
        self.age = age

    # 实例方法
    def bark(self):
        print(f"{self.name} says woof!")

    # __str__ 类似 C++ 的 operator<< 或 toString
    def __str__(self):
        return f"Dog({self.name}, {self.age})"
```

```cpp
// C++
class Dog {
public:
    static std::string species;  // 类变量

    // 构造函数
    Dog(const std::string& name, int age)
        : name_(name), age_(age) {}

    // 实例方法
    void bark() const {
        std::cout << name_ << " says woof!\n";
    }

private:
    std::string name_;  // 实例变量
    int age_;
};
```

### self vs this

```python
# Python：self 是第一个参数，必须显式写出
class Foo:
    def __init__(self, x):
        self.x = x         # 必须用 self.x
    def show(self):
        print(self.x)      # 必须用 self.x

# 创建实例时不需要传 self
f = Foo(10)
```

```cpp
// C++：this 是隐式指针，不需要写在参数列表
class Foo {
public:
    Foo(int x) : x_(x) {}
    void show() const {
        cout << this->x_;  // this 可省略
    }
private:
    int x_;
};
```

### 继承

```python
# Python
class Animal:
    def __init__(self, name):
        self.name = name

    def speak(self):
        pass

class Dog(Animal):  # 继承
    def speak(self):
        return f"{self.name} says woof!"

class Cat(Animal):
    def speak(self):
        return f"{self.name} says meow!"

# 多继承
class Hybrid(Dog, Cat):
    pass
```

```cpp
// C++
class Animal {
public:
    Animal(const std::string& name) : name_(name) {}
    virtual std::string speak() = 0;  // 纯虚函数
    virtual ~Animal() = default;

protected:
    std::string name_;
};

class Dog : public Animal {
public:
    using Animal::Animal;
    std::string speak() override {
        return name_ + " says woof!";
    }
};
```

### 多态

```python
# Python：鸭子类型（Duck Typing）
# 不需要继承也能实现多态
def make_sound(animal):
    print(animal.speak())  # 只要有 speak 方法就行

# 不相关的类只要有相同方法名即可
class Car:
    def speak(self):
        return "vroom!"

make_sound(Dog("Buddy"))  # Buddy says woof!
make_sound(Car())          # vroom!
```

```cpp
// C++：必须通过继承和虚函数实现多态
void make_sound(const Animal& animal) {
    cout << animal.speak();
}
// Car 如果不继承 Animal，编译错误
```

### 访问控制

| 语义 | Python | C++ |
| ------ | -------- | ----- |
| 公开 | `name` | `public:` |
| 保护 | `_name`（约定） | `protected:` |
| 私有 | `__name`（名称修饰） | `private:` |

```python
# Python：没有真正的私有，靠命名约定
class MyClass:
    def __init__(self):
        self.public = 1      # 公开
        self._protected = 2  # 约定保护（可访问）
        self.__private = 3   # 名称修饰（_MyClass__private）

m = MyClass()
print(m.public)         # OK
print(m._protected)     # OK（但应避免）
print(m._MyClass__private)  # 实际可访问
```

```cpp
// C++：强制执行访问控制
class MyClass {
public:
    int public_ = 1;
protected:
    int protected_ = 2;
private:
    int private_ = 3;
};
```

---

## 第八章：内存与对象模型

### Python 对象模型核心

### 一切都是对象

Python 中**一切（包括整数、函数、类、类型本身）都是对象**：

```python
# 在 Python 中，每个值都是一个对象
x = 42
print(type(x))           # <class 'int'>
print(isinstance(x, object))  # True

def foo(): pass
print(isinstance(foo, object))  # True（函数也是对象）

class MyClass: pass
print(isinstance(MyClass, object))  # True（类也是对象）
```

```cpp
// C++ 中 int 不是对象，只有 class/struct 实例才是对象
int x = 42;              // 基本类型，不是对象
std::string s = "hello"; // std::string 对象

// C++ 没有统一的对象基类
// 函数、类不是"对象"
```

### 变量是标签（Name Binding）

**Python 变量不是盒子，是贴在对象上的标签。**

```python
# Python：变量 = 名字绑定到对象
a = [1, 2, 3]   # 创建 list 对象，贴上标签 a
b = a           # 贴上另一个标签 b（同一个对象）
a = [4, 5, 6]   # 创建新 list，a 移过去（b 仍然指向旧对象）
print(b)        # [1, 2, 3] — b 没变！
```

```cpp
// C++：变量是内存位置
int a = 10;     // 在栈上分配 4 字节
int b = a;      // 复制值到新内存
a = 20;         // 修改 a 的内存
cout << b;      // 10 — 值拷贝的结果
```

### id() — 对象身份

```python
# id() 返回对象的唯一标识（CPython 中即内存地址）
a = [1, 2, 3]
b = a
print(id(a))           # 140234567890
print(id(b))           # 相同值（同一对象）
print(id(a) == id(b))  # True（is 的本质）
print(a is b)          # True

# 小整数缓存（CPython 实现细节）
x = 256
y = 256
print(x is y)  # True（小整数 [-5,256] 被缓存）

x = 1000
y = 1000
print(x is y)  # False（大整数不缓存）
```

```cpp
// C++ 地址比较
int a = 10;
int b = 10;
cout << (&a == &b);  // False（不同栈地址）

// 指针比较才类似
int* p = &a;
int* q = p;
cout << (p == q);    // True（指向同一地址）
```

### PyObject 结构（CPython 底层）

```c
typedef struct _object {
    Py_ssize_t ob_refcnt;   // 引用计数
    PyTypeObject* ob_type;  // 类型指针
} PyObject;

// 每个 Python 对象的前两个字段都如此
// 所有 Python 对象实际上都是 PyObject*
```

> Python 的变量本质上就是 `PyObject*` 指针，赋值 = 指针指向 + 引用计数调整。

### Python 变量是引用（Name Binding）

**Python 变量本质上是指针/引用（label），不是存储值的盒子（box）。**

```python
# Python
a = [1, 2, 3]
b = a             # b 是 a 的引用（浅拷贝）
b.append(4)
print(a)          # [1, 2, 3, 4] a 也被修改了！

# 深拷贝需要显式
import copy
c = copy.deepcopy(a)
c.append(5)
print(a)          # [1, 2, 3, 4] 不受影响
```

```cpp
// C++
std::vector<int> a = {1, 2, 3};
auto b = a;              // 值拷贝（b 是独立副本）
b.push_back(4);
cout << a.size();        // 3，a 不受影响

// 引用才能类似 Python 行为
auto& c = a;
c.push_back(4);
cout << a.size();        // 4
```

### 内存管理

| 特性 | Python | C++ |
| ------ | -------- | ----- |
| 分配 | 自动（全部在堆上 via CPython） | 自动（栈）/ 手动（堆） |
| 释放 | 引用计数 + 循环 GC | RAII / delete |
| 精确控制 | 无（无法强制释放） | 完全控制 |
| 构造函数/析构函数 | `__init__`（无析构保证） | 构造/析构精确调用 |
| 移动语义 | 无 | 有（move semantics） |

### 引用计数（Reference Counting）

Python 的主要 GC 机制是**引用计数**：

```python
import sys

a = []                    # refcount = 1
b = a                     # refcount = 2（a 和 b 都指向同一对象）
print(sys.getrefcount(a)) # 3（getrefcount 自己会 +1）
del b                     # refcount 减为 1
# 当 refcount 降到 0 时，对象立即被回收
```

### 循环引用（Cyclic GC）

引用计数无法处理**循环引用**：

```python
# 循环引用 — 引用计数无法释放
class Node:
    def __init__(self):
        self.next = None

a = Node()
b = Node()
a.next = b
b.next = a        # a → b → a 互相引用
del a
del b             # 引用计数不为 0，需 cyclic GC 回收
```

```cpp
// C++：循环引用用 weak_ptr 打破
struct Node {
    std::shared_ptr<Node> next;
};
auto a = std::make_shared<Node>();
auto b = std::make_shared<Node>();
a->next = b;
b->next = a;  // 内存泄漏！需用 weak_ptr
// 用 std::weak_ptr<Node> next; 打破循环
```

### del 不是 delete

```python
# del 只是移除名称绑定，不释放内存
a = [1, 2, 3]
b = a
del a          # 移除标签 a，对象仍然存在（b 还引用着）
print(b)       # [1, 2, 3] — 对象还在

# 只有引用计数归零时才真正释放
```

```cpp
// C++ delete 直接释放内存
int* p = new int(42);
delete p;      // 立即释放，p 变成悬空指针
*p = 10;       // 未定义行为！
```

### weakref 避免循环引用

```python
import weakref

class Node:
    def __init__(self):
        self._next = None

    @property
    def next(self):
        return self._next()

    @next.setter
    def next(self, node):
        self._next = weakref.ref(node)  # 弱引用

a = Node()
b = Node()
a.next = b   # 弱引用，不会增加引用计数
b.next = a   # 弱引用
# 没有循环引用问题
```

```cpp
// C++：栈 vs 堆
void example() {
    int x = 42;              // 栈上分配，自动释放
    int* y = new int(42);    // 堆上分配，需手动释放
    delete y;

    std::vector<int> v;      // 数据在堆上，但对象管理在栈上（RAII）
    // 离开作用域自动释放
}
```

### 可变对象 vs 不可变对象

| 可变 (Mutable) | 不可变 (Immutable) |
| --------------- | ------------------- |
| `list`, `dict`, `set` | `int`, `float`, `bool`, `str`, `tuple` |

```python
# 不可变对象：修改会创建新对象
x = 5
y = x
x = x + 1
print(x, y)  # 6 5（y 不受影响）

# 可变对象：修改会影响所有引用
a = [1, 2, 3]
b = a
a.append(4)
print(a, b)  # [1,2,3,4] [1,2,3,4]
```

```cpp
// C++：所有类型都有值语义（除非使用引用/指针）
int x = 5;
int y = x;
x = x + 1;
cout << x << y;  // 6 5

std::vector<int> a = {1, 2, 3};
auto b = a;           // 值拷贝
a.push_back(4);
cout << a.size() << b.size();  // 4 3
```

---

## Python 性能模型 vs C++

### 为什么 Python 比 C++ 慢

| 原因 | 说明 | 量化影响 |
| ---- | --- | -------- |
| 解释执行 | 字节码解释 vs 机器码直接执行 | ~10x |
| 动态分发 | 每次方法调用需查虚表（更复杂） | ~2-5x |
| 动态类型检查 | 每个操作需检查类型 | ~2-3x |
| 对象模型开销 | 所有值都是 PyObject（内存占用大） | ~2-3x |
| 装箱拆箱 | int/float 也是对象，有分配开销 | ~2x |
| GIL | 全局解释器锁，限制多核利用 | 视情况 |

```python
# Python 动态类型检查的成本
a = 1
b = 2
c = a + b   # Python 实际做了：
# 1. 检查 a 的类型（PyLong_Check）
# 2. 检查 b 的类型（PyLong_Check）
# 3. 调用 PyLong_Add
# 4. 分配新 PyLong 对象
# 5. 返回指针
```

```cpp
// C++ 编译期确定类型
int a = 1, b = 2;
int c = a + b;   // 编译器直接生成 ADD 指令
// 没有类型检查，没有对象分配
// 一条 CPU 指令完成
```

### GIL（全局解释器锁）

```python
# GIL 让多线程无法利用多核
import threading
import time

def busy_work():
    for _ in range(10**7):
        pass

# 多线程在 CPU 密集型任务上反而可能更慢
start = time.time()
t1 = threading.Thread(target=busy_work)
t2 = threading.Thread(target=busy_work)
t1.start(); t2.start()
t1.join(); t2.join()
print("2 threads:", time.time() - start)

# 解决方案：
# - 多进程（multiprocessing）
# - C 扩展（不释放 GIL 就不受影响）
# - 异步（asyncio，适合 IO 密集型）
```

```cpp
// C++：没有 GIL，原生支持多线程
#include <thread>
#include <chrono>

void busy_work() {
    volatile int x = 0;
    for (int i = 0; i < 10000000; i++) x++;
}

// 真正并行，利用多核
auto t1 = std::thread(busy_work);
auto t2 = std::thread(busy_work);
t1.join(); t2.join();
```

### 性能优化策略

| 策略 | Python | C++ |
| ---- | ------ | --- |
| 内联 | 无（所有调用是动态的） | `inline` 关键字 |
| JIT | PyPy（非官方） | 编译器自动优化 |
| C 扩展 | Cython / pybind11 | 原生支持 |
| 循环优化 | 用列表推导式替代 for | 编译器自动向量化 |
| 多核 | multiprocessing（多进程） | std::thread（多线程） |

---

### 并发模型概述

| 方式 | Python | C++ |
| ---- | ------ | --- |
| 多线程 | threading（受 GIL 限制） | `std::thread`（真正并行） |
| 多进程 | multiprocessing（独立进程） | `fork` / 进程间通信 |
| 异步 | asyncio（协程，单线程） | 无标准协程（C++20 有） |
| 并行库 | concurrent.futures | TBB / OpenMP |

```python
# Python 异步（asyncio）
import asyncio

async def fetch_data(url):
    print(f"Fetching {url}...")
    await asyncio.sleep(1)  # 模拟 IO 等待
    return f"Data from {url}"

async def main():
    # 并发执行（单线程，非阻塞）
    results = await asyncio.gather(
        fetch_data("url1"),
        fetch_data("url2"),
        fetch_data("url3"),
    )
    print(results)

asyncio.run(main())  # 总共 ~1s，不是 3s
```

```cpp
// C++ 异步（C++20）
// 或者用 std::async / 线程池
#include <future>
#include <iostream>

std::string fetch_data(const std::string& url) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return "Data from " + url;
}

// C++ 需要手动管理线程/异步任务
auto f1 = std::async(fetch_data, "url1");
auto f2 = std::async(fetch_data, "url2");
auto f3 = std::async(fetch_data, "url3");
std::cout << f1.get() << f2.get() << f3.get();
```

---

## 第九章：关键字对照表

| Python | C++ | 说明 |
| -------- | ----- | ------ |
| `if` | `if` | 条件判断 |
| `elif` | `else if` | Python 缩写 |
| `else` | `else` | 否则 |
| `for` | `for` | 循环 |
| `while` | `while` | 循环 |
| `break` | `break` | 跳出循环 |
| `continue` | `continue` | 继续下一次循环 |
| `def` | `return_type function_name` | 函数定义 |
| `class` | `class` | 类定义 |
| `import` | `#include` | 导入模块/头文件 |
| `from ... import` | `using` | 选择性导入 |
| `True` / `False` | `true` / `false` | 布尔值（大小写不同） |
| `None` | `nullptr` / `NULL` | 空对象（NoneType 单例，不是空指针） |
| `and` / `or` / `not` | `&&` / `\|\|` / `!` | 逻辑运算符 |
| `in` | `find()` / `.count()` | 成员检查 |
| `is` | `==`（地址比较） | 对象身份比较（identity，非指针等价） |
| `lambda` | `[ captures ]` | 匿名函数 |
| `yield` | 无对应 | 生成器 |
| `with` | 无对应 | 上下文管理器 |
| `try` / `except` | `try` / `catch` | 异常处理 |
| `finally` | `finally` | 无论异常都执行 |
| `raise` | `throw` | 抛出异常 |
| `pass` | `;` / `{}` | 空操作占位符 |
| `del` | `delete` | 删除引用/元素 |
| `global` / `nonlocal` | 无对应 | 作用域声明 |
| `assert` | `assert` | 断言 |
| `async` / `await` | 无对应 | 异步编程 |
| `as` | `as`（C#）/ `namespace alias` | 别名 |

---

## 第十章：模块与工程结构

### import 机制

```python
# Python：导入模块
import math             # 导入整个模块
print(math.sqrt(16))

from math import sqrt   # 选择性导入
print(sqrt(16))

from math import *      # 导入所有（不推荐）

import math as m        # 别名
print(m.sqrt(16))
```

```cpp
// C++：包含头文件
#include <cmath>        // 包含头文件
std::sqrt(16);

using std::sqrt;        // using 声明
sqrt(16);
```

### module vs header/source

```python
# my_module.py (一个 .py 文件就是一个模块)
def hello():
    print("Hello from module")

class MyClass:
    pass

# main.py
import my_module
my_module.hello()
```

```cpp
// C++ 分离 header (.h) 和 source (.cpp)
// my_module.h
#pragma once
void hello();
class MyClass {};

// my_module.cpp
#include "my_module.h"
void hello() { std::cout << "Hello from module\n"; }

// main.cpp
#include "my_module.h"
hello();
```

### Python 包结构

```text
my_package/
├── __init__.py      # 包的初始化文件（可以是空文件）
├── module_a.py
├── module_b.py
└── sub_package/
    ├── __init__.py
    └── module_c.py
```

```python
# 使用包
from my_package import module_a
from my_package.sub_package import module_c
```

`__init__.py` 的作用：

1. **标记目录为 Python 包**（Python 3.3+ 可省略，但推荐保留）
2. 控制 `from package import *` 的行为
3. 执行包级别的初始化代码

```cpp
// C++ 工程结构（无统一包概念）
include/
├── my_library/
│   ├── module_a.h
│   └── sub_module/
│       └── module_c.h
src/
├── module_a.cpp
└── sub_module/
    └── module_c.cpp
```

### 工程组织差异

| 特性 | Python | C++ |
| ------ | -------- | ----- |
| 编译 | 解释执行，无需编译 | 需要编译+链接 |
| 文件扩展 | `.py` | `.h` / `.cpp` |
| 模块定义 | 一个文件即一个模块 | header + source 分离 |
| 可见性控制 | `_` 约定 / `__all__` | `#include` 控制 |
| 命名空间 | 模块名即命名空间 | `namespace` 关键字 |
| 构建工具 | 无需（解释器直接运行） | CMake / Make / Bazel |

### import 缓存与 sys.modules

```python
# Python 只加载模块一次，后续 import 使用缓存
import sys

print("math" in sys.modules)  # False（首次）
import math
print("math" in sys.modules)  # True（已缓存）

# 查看已加载的模块
print(sys.modules.keys())

# 强制重新加载（开发调试用）
import importlib
importlib.reload(math)  # 重新执行模块代码
```

```cpp
// C++：#pragma once 或 include guard 防止重复包含
// 编译器保证每个 .cpp 中头文件只被编译一次
// 但链接器不会"缓存"已翻译的模块
// 每次编译 .cpp 都会重新处理所有头文件
```

### 循环导入问题

```python
# a.py
from b import b_func

def a_func():
    return b_func()

# b.py
from a import a_func   # 循环导入！
# ImportError: cannot import name 'a_func'

# 解决方案 1：延迟导入（函数内 import）
# b.py
def b_func():
    from a import a_func  # 在运行时才导入
    return a_func()

# 解决方案 2：重构，提取公共模块到第三个文件
# common.py — 放共享代码
```

```cpp
// C++：前向声明解决循环依赖
// a.h
class B;  // 前向声明
class A { B* b_; };

// b.h
#include "a.h"  // 需要完整的 A 定义
class B { A a_; };
```

### 包管理器与环境隔离

```bash
# Python：pip + venv（类似 C++ 的 vcpkg / Conan）

# 创建虚拟环境（类似隔离的编译环境）
python -m venv myenv

# 激活
source myenv/bin/activate  # Linux/Mac
# myenv\Scripts\activate   # Windows

# 安装包
pip install requests
pip install numpy pandas

# 导出依赖
pip freeze > requirements.txt

# 从 requirements 安装
pip install -r requirements.txt
```

```cpp
// C++：Conan / vcpkg
// conanfile.txt
// [requires]
// fmt/10.0.0
// boost/1.83.0

// vcpkg.json
// {
//   "dependencies": ["fmt", "boost"]
// }
```

### 项目结构最佳实践

```text
my_project/
├── pyproject.toml       # 项目元数据与构建配置（替代 setup.py）
├── README.md
├── src/                 # 源码目录
│   └── my_project/
│       ├── __init__.py
│       ├── main.py
│       ├── module_a.py
│       └── utils/
│           ├── __init__.py
│           └── helpers.py
├── tests/               # 测试目录
│   ├── __init__.py
│   ├── test_module_a.py
│   └── test_utils.py
├── requirements.txt     # 依赖列表
└── .gitignore
```

```cpp
// C++ 常见项目结构
my_project/
├── CMakeLists.txt
├── include/
│   └── my_project/
│       ├── module_a.h
│       └── utils/
│           └── helpers.h
├── src/
│   ├── module_a.cpp
│   └── utils/
│       └── helpers.cpp
├── tests/
│   └── test_module_a.cpp
└── .gitignore
```

---

## 第十一章：异常处理

### try / except

```python
# Python
try:
    x = int(input("Enter a number: "))
    result = 10 / x
    print(result)
except ValueError:
    print("That's not a valid number!")
except ZeroDivisionError:
    print("Cannot divide by zero!")
except Exception as e:  # 捕获所有异常
    print(f"Unexpected error: {e}")
else:
    print("No exception occurred")  # 无异常时执行
finally:
    print("This always runs")
```

```cpp
// C++
try {
    int x = std::stoi(input);
    if (x == 0) throw std::runtime_error("Divide by zero");
    int result = 10 / x;
    std::cout << result;
} catch (const std::invalid_argument& e) {
    std::cout << "That's not a valid number!";
} catch (const std::runtime_error& e) {
    std::cout << e.what();
} catch (const std::exception& e) {
    std::cout << "Unexpected: " << e.what();
} catch (...) {
    std::cout << "Unknown exception";
}
// C++ 没有 else 和 finally 的直接对应
// finally 可以用 RAII 替代
```

### raise vs throw

```python
# Python：raise（无参数时重新抛出当前异常）
def validate_age(age):
    if age < 0:
        raise ValueError("Age cannot be negative")
    if age > 150:
        raise ValueError("Age is too large")

try:
    validate_age(-1)
except ValueError as e:
    print(f"Error: {e}")
    raise  # 重新抛出
```

```cpp
// C++：throw
void validate_age(int age) {
    if (age < 0)
        throw std::invalid_argument("Age cannot be negative");
    if (age > 150)
        throw std::invalid_argument("Age is too large");
}

try {
    validate_age(-1);
} catch (const std::invalid_argument& e) {
    std::cout << "Error: " << e.what();
    throw;  // 重新抛出
}
```

### 自定义异常

```python
# Python
class MyCustomError(Exception):
    """自定义异常"""
    def __init__(self, message, code):
        super().__init__(message)
        self.code = code

try:
    raise MyCustomError("Something went wrong", 500)
except MyCustomError as e:
    print(f"Code {e.code}: {e}")
```

```cpp
// C++
class MyCustomError : public std::exception {
public:
    MyCustomError(const std::string& msg, int code)
        : msg_(msg), code_(code) {}

    const char* what() const noexcept override {
        return msg_.c_str();
    }
    int code() const { return code_; }
private:
    std::string msg_;
    int code_;
};
```

---

## 第十二章：文件 IO

### 文件操作

```python
# Python
# 写入文件
with open("test.txt", "w", encoding="utf-8") as f:
    f.write("Hello, World!\n")
    f.writelines(["line1\n", "line2\n"])

# 读取文件
with open("test.txt", "r", encoding="utf-8") as f:
    content = f.read()           # 读取全部
    line = f.readline()          # 读取一行
    lines = f.readlines()        # 读取所有行

# 逐行读取（推荐，内存友好）
with open("test.txt", "r") as f:
    for line in f:
        print(line.strip())

# 追加
with open("test.txt", "a") as f:
    f.write("appended text\n")

# 二进制模式
with open("test.bin", "wb") as f:
    f.write(b"\x00\x01\x02")
```

```cpp
// C++
#include <fstream>
#include <string>

// 写入文件
std::ofstream out("test.txt");
out << "Hello, World!" << std::endl;
out.close();

// 读取文件
std::ifstream in("test.txt");
std::string content((std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>());

// 逐行读取
std::ifstream in2("test.txt");
std::string line;
while (std::getline(in2, line)) {
    std::cout << line << std::endl;
}

// 追加
std::ofstream out2("test.txt", std::ios::app);
out2 << "appended text" << std::endl;

// 二进制
std::ofstream out3("test.bin", std::ios::binary);
char data[] = {0x00, 0x01, 0x02};
out3.write(data, sizeof(data));
```

### with 语句

Python 的 `with` 语句自动管理资源（类似 C++ RAII）：

```python
# Python with 自动关闭文件
with open("test.txt", "r") as f:
    data = f.read()
# 自动调用 f.close()

# 等价于手动：
f = open("test.txt", "r")
try:
    data = f.read()
finally:
    f.close()
```

```cpp
// C++ RAII 自动管理
{
    std::ifstream f("test.txt");
    // 离开作用域自动调用析构函数关闭文件
}
```

> Python 的 `with` 语句和 C++ 的 RAII 思想一致，都保证资源正确释放。

---

## 第十三章：迭代器与生成器

### 迭代器

```python
# Python 迭代器
my_list = [1, 2, 3]
it = iter(my_list)     # 获取迭代器
print(next(it))        # 1
print(next(it))        # 2
print(next(it))        # 3
# print(next(it))      # StopIteration 异常

# for 循环本质上是迭代器
for x in my_list:      # 等价于 iter() + next()
    print(x)
```

```cpp
// C++ 迭代器
std::vector<int> vec = {1, 2, 3};
auto it = vec.begin();
std::cout << *it;   // 1
++it;
std::cout << *it;   // 2
++it;
std::cout << *it;   // 3

// 范围 for 循环（本质也是迭代器）
for (const auto& x : vec) {
    std::cout << x;
}
```

### 生成器（Generator）

Python 独有的强大特性，C++ 没有直接对应：

```python
# Python 生成器函数（使用 yield）
def countdown(n):
    while n > 0:
        yield n          # 暂停并返回值
        n -= 1
    yield "Done!"        # 可以 yield 多次

# 使用生成器
for value in countdown(3):
    print(value)         # 3, 2, 1
# 最后一个 "Done!" 不会被 for 循环捕获

# 生成器表达式（类似列表推导式但惰性）
squares = (x * x for x in range(10))
print(next(squares))  # 0
print(next(squares))  # 1
```

```cpp
// C++ 没有直接对应，需要手动实现或使用协程
// 用迭代器模拟（C++20 有协程）
class Countdown {
    int n;
public:
    Countdown(int n) : n(n) {}
    class iterator {
        int current;
    public:
        iterator(int n) : current(n) {}
        int operator*() const { return current; }
        iterator& operator++() { current--; return *this; }
        bool operator!=(const iterator& other) const {
            return current != other.current;
        }
    };
    iterator begin() const { return iterator(n); }
    iterator end() const { return iterator(0); }
};
```

### range 对比（不是生成器）

```python
# Python range 是惰性不可变序列（不是生成器）
r = range(1_000_000)   # 不占用大内存
print(r[0])             # 支持索引（生成器不支持）
print(len(r))           # 支持长度
print(500 in r)         # 支持 in 检查
for i in r:             # 可以迭代
    pass
```

```cpp
// C++ 无类似 range 的惰性序列
// for 循环直接迭代，不生成中间对象
for (int i = 0; i < 1'000'000; i++) {
    // ...
}

// 需要存储时才占用内存
std::vector<int> v(1'000'000);
std::iota(v.begin(), v.end(), 0);
```

---

## 第十四章：Python 特有机制

### 装饰器 (Decorator)

```python
# Python 装饰器：本质是函数，用于增强其他函数
def timer(func):
    import time
    def wrapper(*args, **kwargs):
        start = time.time()
        result = func(*args, **kwargs)
        print(f"{func.__name__} took {time.time()-start:.2f}s")
        return result
    return wrapper

# 使用装饰器
@timer
def slow_function():
    import time
    time.sleep(1)

slow_function()  # 自动计时，输出：slow_function took 1.00s

# 多个装饰器
@decorator1
@decorator2
def func():
    pass
# 等价于 func = decorator1(decorator2(func))
```

```cpp
// C++ 没有直接对应，可通过宏或模板模拟，但不优雅
// 最接近的是 Pythonic 的包装模式
template<typename Func>
auto timer(Func func) {
    return [func](auto&&... args) {
        auto start = std::chrono::high_resolution_clock::now();
        auto result = func(std::forward<decltype(args)>(args)...);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "took " 
                  << std::chrono::duration<double>(end - start).count()
                  << "s\n";
        return result;
    };
}
```

### 动态类型系统

```python
# Python 动态类型的双刃剑

# 优势：灵活
def process(data):
    if isinstance(data, list):
        return [x * 2 for x in data]
    elif isinstance(data, dict):
        return {k: v * 2 for k, v in data.items()}
    elif isinstance(data, str):
        return data.upper()

# 风险：运行时错误
def add(a, b):
    return a + b

add(1, 2)        # 3
add("hello", " ") # "hello "
add([1], [2])    # [1, 2]
# add(1, "hello")  # TypeError! 运行时才暴露
```

```cpp
// C++ 静态类型：编译期检查
// 重载实现类似动态行为
int process(const std::vector<int>& data);
std::string process(const std::string& data);

// 模板实现泛型
template<typename T>
auto add(const T& a, const T& b) -> decltype(a + b) {
    return a + b;
}
// add(1, "hello")  // 编译错误，类型安全
```

### 反射机制

```python
# Python 强大的反射能力

class Person:
    def __init__(self, name):
        self.name = name

p = Person("Alice")

# 动态获取对象信息
print(type(p))          # <class '__main__.Person'>
print(dir(p))           # 列出所有属性和方法
print(p.__class__)      # 获取类
print(p.__dict__)       # {'name': 'Alice'}

# 动态访问属性
print(getattr(p, "name"))            # Alice
print(getattr(p, "age", "default"))  # default（不存在时）
setattr(p, "age", 25)                # 动态设置属性
print(hasattr(p, "name"))            # True

# 动态调用方法
method = getattr(p, "name")
print(method)  # Alice（只是属性，不是方法）

# 动态创建类
MyClass = type("MyClass", (), {"x": 10})
```

```cpp
// C++ 反射非常有限（需借助 RTTI 或手动实现）
#include <typeinfo>
Person p("Alice");
std::cout << typeid(p).name();  // 实现定义的名字

// 没有 getattr / setattr 等动态能力
// 需要手动实现反射机制（如成员变量注册表）
```

### Duck Typing（鸭子类型）

```python
# Python："如果它走路像鸭子，叫起来像鸭子，那它就是鸭子"
class Duck:
    def quack(self):
        print("Quack!")

class Person:
    def quack(self):
        print("I'm quacking like a duck!")

def make_it_quack(thing):
    thing.quack()  # 不关心类型，只要有 quack 方法

make_it_quack(Duck())    # Quack!
make_it_quack(Person())  # I'm quacking like a duck!
```

```cpp
// C++：必须通过继承/接口
class Quackable {
public:
    virtual void quack() = 0;
};

class Duck : public Quackable { ... };
class Person : public Quackable { ... };

void make_it_quack(Quackable& thing) {
    thing.quack();
}
```

> 鸭子类型的优势是灵活、代码量少；代价是错误在运行时暴露，IDE 支持较弱。

---

## Python 常见陷阱（C++ 程序员必看）

### 陷阱 1：可变默认参数

```python
# 陷阱：默认参数在函数定义时求值，不是调用时
def add_item(item, lst=[]):   # [] 创建一次，被所有调用共享
    lst.append(item)
    return lst

print(add_item(1))  # [1]
print(add_item(2))  # [1, 2] — 不是 [2]！
print(add_item(3))  # [1, 2, 3]

# 正确做法
def add_item(item, lst=None):
    if lst is None:
        lst = []
    lst.append(item)
    return lst
```

```cpp
// C++：默认参数在调用处求值
// 不存在共享问题（每次调用创建新 vector）
std::vector<int> add_item(int item,
                           std::vector<int> lst = {}) {
    lst.push_back(item);
    return lst;
}
// 每次调用 lst 都是新的
```

### 陷阱 2：浅拷贝 vs 深拷贝

```python
# Python：list 复制默认是浅拷贝
original = [[1, 2], [3, 4]]
shallow = original.copy()      # 只有外层是新的
shallow[0].append(99)
print(original)                # [[1, 2, 99], [3, 4]] — 内层被改了！

# 深拷贝需要显式
import copy
deep = copy.deepcopy(original)
deep[0].append(100)
print(original)                # 不受影响
```

```cpp
// C++：默认是深拷贝（值语义）
std::vector<std::vector<int>> original = {{1,2}, {3,4}};
auto copy = original;          // 完整深拷贝
copy[0].push_back(99);
cout << original[0].size();    // 2（不受影响）
```

### 陷阱 3：`+=` vs `append` 对不可变对象的差异

```python
# str 是不可变对象
a = "hello"
b = a
a += " world"    # 创建新字符串，a 指向新对象
print(b)          # "hello"（b 不受影响）

# list 是可变对象
x = [1, 2]
y = x
x += [3, 4]      # 直接修改原列表（+= 调用 __iadd__）
print(y)          # [1, 2, 3, 4]（y 也变了！）

# 但 x = x + [3, 4] 是创建新列表
x = x + [5, 6]   # 创建新对象
print(y)          # [1, 2, 3, 4]（y 不受影响）
```

```cpp
// C++：运算符行为一致，没有这种陷阱
std::string a = "hello";
auto b = a;
a += " world";  // 值语义，b 不受影响

std::vector<int> x = {1, 2};
auto y = x;
x.insert(x.end(), {3, 4});  // y 不受影响（值拷贝）
```

### 陷阱 4：is 比较 vs == 比较

```python
# is 比较对象身份，== 比较值
a = [1, 2, 3]
b = [1, 2, 3]
print(a == b)  # True（值相等）
print(a is b)  # False（不同对象）

# 小整数缓存导致的陷阱
x = 256
y = 256
print(x is y)  # True（CPython 缓存 [-5, 256]）

x = 1000
y = 1000
print(x is y)  # False（大整数未缓存）

# 字符串驻留
s1 = "hello_world"
s2 = "hello_world"
print(s1 is s2)  # 可能是 True（CPython 驻留）
```

```cpp
// C++ 明确区分：== 调用 operator==，地址用 &a == &b
int a = 256;
int b = 256;
cout << (a == b);   // True（值相等）
cout << (&a == &b); // False（不同地址）
```

### 陷阱 5：for 循环中修改列表

```python
# 不要在遍历列表时修改它
lst = [1, 2, 3, 4, 5]
for x in lst:
    if x % 2 == 0:
        lst.remove(x)   # 导致跳过元素！
print(lst)  # [1, 3, 5] — 可能不符合预期

# 正确做法：遍历副本
for x in lst[:]:       # 或 list(lst)
    if x % 2 == 0:
        lst.remove(x)
```

```cpp
// C++ 同样的问题，但更明显（迭代器失效）
std::vector<int> v = {1, 2, 3, 4, 5};
for (auto it = v.begin(); it != v.end(); ) {
    if (*it % 2 == 0)
        it = v.erase(it);  // 必须更新迭代器
    else
        ++it;
}
```

### 陷阱 6：dict 键必须是可哈希的

```python
# dict 的键必须是 hashable 的
d = {}
d[[1, 2]] = "list"  # TypeError: unhashable type: 'list'

# 可哈希：int, str, tuple（内容不可变）
# 不可哈希：list, dict, set（可变类型不能做键）

# 解决方案：转成 tuple
d[tuple([1, 2])] = "works"
```

```cpp
// C++ unordered_map 键需要 std::hash 特化
// std::vector 默认不支持做键
// 需自定义哈希或使用 std::map
std::map<std::vector<int>, std::string> m;
m[{1, 2}] = "works";  // std::map 用 operator<，不需要哈希
```

---

## 附录：学习路线总结

### 学习路径建议

```text
第一阶段：基础语法（1-2 周）
├── 变量与类型（第一章）
├── 运算符（第二章）
├── 控制流（第三章）
├── 函数（第四章）
└── 字符串（第六章）

第二阶段：核心数据结构（1-2 周）
├── list / tuple / dict / set（第五章）
├── 容器操作与推导式
└── 迭代器（第十三章）

第三阶段：面向对象（1 周）
├── 类与对象（第七章）
├── 继承与多态
└── 特殊方法（__xxx__）

第四阶段：工程实践（1 周）
├── 模块与包（第十章）
├── 异常处理（第十一章）
├── 文件 IO（第十二章）
└── 项目结构

第五阶段：Python 进阶（持续）
├── 生成器（第十三章）
├── 装饰器（第十四章）
├── 上下文管理器
├── 异步编程（async/await）
└── 常用标准库
```

### C++ 转 Python 常见误区

| 误区 | 正解 |
| ------ | ------ |
| 认为 `==` 比较对象内容 | Python 的 `==` 比较值，`is` 比较身份 |
| 用 `++` 自增 | Python 没有 `++`，用 `+= 1` |
| 写大段 `if...else if` | 用 `elif` |
| 纠结变量类型 | 动态类型，关注行为而不是类型 |
| 用 `for (int i = 0; ...)` | 用 `for x in iterable` |
| 复制对象用 `=` | 深拷贝用 `copy.deepcopy()` |
| 担心内存泄漏 | Python GC 自动管理，关注引用循环 |
| 写 getter/setter | 用 `@property` 装饰器 |

### 推荐 Python 学习资源

1. **官方文档**：docs.python.org（全面权威）
2. **PEP 8**：Python 编码规范
3. **Python Tutor**：可视化代码执行过程（理解对象模型）
4. **Flake8 / mypy**：代码质量工具
5. **pytest**：测试框架

---

> **总结：Python 的哲学是"简单优雅"，C++ 的哲学是"零开销抽象"。**
> 从 C++ 转到 Python，关键在于适应动态类型思维和 Pythonic 的编程风格。
