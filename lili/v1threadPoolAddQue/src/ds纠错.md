```c++
先说结论：你的担心是对的，而且已经实际发生了。我用 timeout 跑了一下，程序最后卡死（exit=124），并且出现大量重复值 10 和 0。
回答第 58 行的疑问
} else {
    notFull_.wait(ul);   // 被唤醒后，函数直接返回，没有 push
}
notFull_.wait(ul) 返回（被消费者唤醒）后，函数什么都没做就结束了。也就是说：
- 生产者想存的那个 elem，在进入 else 分支前已经从参数传进来了；
- 唤醒后没有执行 que_.push(elem)，所以这个元素被丢掉了；
- 外层 while 继续下一轮，打印 "producer add task i"，但实际并没存进去（日志和事实不符）。
正确写法是唤醒后重新检查条件并补上 push，而且必须用 while（防止虚假唤醒 spurious wakeup）：
void taskQueue::pushElem(int elem) {
    unique_lock<mutex> ul(mutex_);
    notFull_.wait(ul, [this]{ return !isFull(); }); // 等价于 while(!isFull()) wait
    que_.push(elem);
    notEmpty_.notify_one();
}
其他问题
1. popElem 取错端点（严重） taskQueue.cpp:72-73：que_.back() 取队尾，但 queue::pop() 删的是队首。读写不一致，所以你会看到重复的 10/0。应为 que_.front()。
2. popElem 也用 if 而非 while taskQueue.cpp:69：虚假唤醒时会在空队列上 back()/pop()，属未定义行为。同样改成：
notEmpty_.wait(ul, [this]{ return !isEmpty(); });
int elem = que_.front();
que_.pop();
notFull_.notify_one();
3. 丢元素导致死锁/挂起：生产者丢了 1 个任务，只存进 19 个，消费者却要取 20 个 → 最终永久等待。这就是卡死原因。
4. 成员初始化顺序警告 taskQueue.cpp:16：-Wreorder。初始化列表顺序应和头文件声明顺序一致（queSize_, mutex_, notFull_, notEmpty_, que_），把 que_ 挪到最后。
5. pushElem 的 ul.unlock() 无必要：notEmpty_.notify_one() 可以在持锁时调用，提前解锁对正确性没帮助，反而容易出错。让 unique_lock 析构自动解锁即可。
6. curSize() 返回 int 但 que_.size() 是 size_t：比较时有符号/无符号转换，建议返回 size_t 或用 static_cast<int>。
7. isFull()/isEmpty()/curSize() 可加 const。
8. a.out 被提交进源码目录（src/a.out），建议加进 .gitignore 并删掉。
按上面的 while/谓词 + front() 修正后即可正确运行；需要我直接改吗？
```