这才是真正的tcp

tcp为什么不是四次握手？如果第三次client发送给server的ack丢了怎么办？为什么是四次挥手？如果第四次client发送给server的ack丢了怎么办？

你这个问题其实抓住了 TCP 建立和断开的**核心区别**：

> **为什么建立连接只需要 3 次，而断开连接却需要 4 次？**
> 关键不在于“ACK 丢了怎么办”，而在于：**建立连接时，SYN 和 ACK 可以合并；断开连接时，双方的 FIN 是两个独立的事情。**

------

# 一、先理解：TCP 建立连接到底在确认什么？

三次握手：

```text
Client                              Server

  SYN, seq=x  -------------------->

             <--------------------  SYN, seq=y
                                  ACK=x+1

  ACK=y+1    -------------------->
```

实际上确认了两件事情：

```text
Client → Server：我能发给你
Server → Client：我能发给你
Client → Server：我知道你能发给我
```

注意第二步：

```text
Server → Client
SYN + ACK
```

这里的 **SYN 和 ACK 可以放在同一个 TCP 报文里**。

所以：

```text
Client：我要建立连接
Server：我同意，而且我也要建立连接
Client：收到
```

只需要：

```text
SYN
SYN + ACK
ACK
```

因此是三次。

------

# 二、那为什么不是四次握手？

理论上当然可以设计成四次：

```text
Client → Server：SYN
Server → Client：ACK
Server → Client：SYN
Client → Server：ACK
```

但是完全没必要。

因为：

```text
Server：ACK
Server：SYN
```

这两个动作可以合并成：

```text
Server → Client：SYN + ACK
```

所以 TCP 选择了三次。

------

# 三、那么你问的关键来了：第三次 ACK 丢了怎么办？

比如：

```text
Client                              Server

SYN ------------------------------>

     <----------------------------- SYN + ACK

ACK --------X  （丢失）
```

这时候怎么办？

**Server 不会认为连接建立失败。**

因为 Server 在发送：

```text
SYN + ACK
```

之后，会进入：

```text
SYN-RECEIVED
```

并等待 Client 的 ACK。

如果 ACK 丢了：

```text
Client                    Server

  ACK --------X

                           等待 ACK
                           ↓
                       超时重传 SYN+ACK
                           ↓
                  SYN+ACK ------------>
```

Client 收到重复的 SYN+ACK 后，会再次发送 ACK。

所以最终：

```text
Client                              Server

SYN ------------------------------>

     <----------------------------- SYN + ACK

ACK --------X

     <----------------------------- SYN + ACK（重传）

ACK ------------------------------>
```

连接仍然可以建立。

------

# 四、这里有一个非常重要的思想

你可能会产生一个疑问：

> “第三次 ACK 都丢了，Server 怎么知道 Client 收到了 SYN+ACK？”

答案是：

**不知道。**

TCP 不要求 Server 在这一瞬间就百分之百知道。

它采用的是：

> **超时 + 重传 + 状态机**

也就是说：

```text
ACK 没来
↓
我暂时认为可能丢了
↓
等待
↓
超时
↓
重新发送 SYN+ACK
↓
如果 Client 还活着，就再次 ACK
```

这就是 TCP 可靠性的基本思想之一。

------

# 五、现在看四次挥手，情况完全不一样

TCP 断开连接：

```text
Client                              Server

FIN ------------------------------>

     <----------------------------- ACK

     <----------------------------- FIN

ACK ------------------------------>
```

为什么需要四次？

因为这里有一个关键区别：

## FIN 和 ACK 不能像 SYN 和 ACK 那样简单地合并

例如 Client：

```text
FIN
```

表达的是：

> “我以后不再发送数据了。”

但这并不意味着：

```text
Server：我也不发送数据了。
```

Server 可能还有数据没发送完。

所以 Server 收到 FIN 后：

```text
Server：
你的数据我收完了。

但是：
我这边可能还有数据没发完。
```

于是先：

```text
ACK
```

等 Server 自己的数据发送完以后，再：

```text
FIN
```

所以出现：

```text
Client → Server：FIN
Server → Client：ACK
Server → Client：FIN
Client → Server：ACK
```

这就是四次挥手。

------

# 六、你会发现一个非常关键的区别

### 三次握手

Server 收到 Client 的 SYN：

```text
Client：我要建立连接
```

Server 可以立刻：

```text
ACK：我收到你的请求
+
SYN：我也要建立连接
```

这两个动作天然可以同时发生。

所以：

```text
ACK + SYN
```

可以合并。

------

### 四次挥手

Server 收到 Client 的 FIN：

```text
Client：我不发数据了
```

Server：

```text
ACK：我知道了
```

但是：

```text
Server：我自己什么时候不发？
```

**不一定现在。**

可能：

```text
Server 还有数据
↓
继续发送数据
↓
发送完
↓
FIN
```

因此：

```text
ACK
```

和：

```text
FIN
```

在时间上可能不是一回事。

所以通常需要四个报文。

------

# 七、那么你第二个问题：第四次 ACK 丢了怎么办？

例如：

```text
Client                              Server

FIN ------------------------------>

     <----------------------------- ACK

     <----------------------------- FIN

ACK --------X
```

此时 Server 已经发送 FIN，并进入：

```text
FIN-WAIT / LAST-ACK
```

准确地说，如果 Server 是主动发送 FIN 的一方，它会进入 `LAST-ACK`。

Server 等待：

```text
Client 的 ACK
```

但是 ACK 丢了。

于是：

```text
等待
 ↓
超时
 ↓
Server 重传 FIN
```

也就是：

```text
Client                              Server

     <----------------------------- FIN

ACK --------X

     <----------------------------- FIN（重传）

ACK ------------------------------>
```

所以最终还是能够正常关闭。

------

# 八、但是这里有一个非常重要的细节：为什么 Client 不需要无限等待？

因为 TCP 有状态机和超时机制。

Server：

```text
FIN
↓
等待 ACK
↓
ACK 没收到
↓
重传 FIN
↓
继续等待
```

Client：

```text
收到 FIN
↓
发送 ACK
↓
进入 TIME_WAIT
```

Client 进入 `TIME_WAIT` 的一个非常重要原因，就是：

> **防止最后一个 ACK 丢失后，Server 重传 FIN 时，Client 已经把连接状态彻底清掉了。**

所以：

```text
Server                         Client

FIN ------------------------->

      <----------------------- ACK

                         TIME_WAIT
```

假如最后 ACK 丢了：

```text
FIN ------------------------->

      <----------------------- ACK
                X（ACK丢失）

FIN ------------------------->

      <----------------------- ACK
```

因为 Client 还处于 `TIME_WAIT`，所以它还能识别：

```text
这是刚才那个连接的 FIN 重传
```

然后重新 ACK。

------

# 九、所以你可以把整个过程记成这张图

```text
              TCP 建立连接

Client                                      Server

SYN ---------------------------------------->

             <------------------------ SYN + ACK

ACK ---------------------------------------->


为什么 3 次？
        ↓
SYN 和 ACK 可以合并
        ↓
Server：我收到你的 SYN，同时告诉你我的 SYN


------------------------------------------------


              TCP 断开连接

Client                                      Server

FIN ---------------------------------------->

             <----------------------------- ACK

             <----------------------------- FIN

ACK ---------------------------------------->


为什么 4 次？
        ↓
双方关闭发送方向是独立的
        ↓
Server 收到 FIN 后不一定马上能 FIN
        ↓
可能还有数据需要发送
```

------

# 十、再把你问的两个“丢包”放到一起

### 第三次握手 ACK 丢失

```text
Client                         Server

ACK --------X

                               等待 ACK
                                  ↓
                               超时
                                  ↓
                         重传 SYN + ACK
                                  ↓
Client 再 ACK -------------------->
```

**靠 SYN+ACK 重传解决。**

------

### 第四次挥手 ACK 丢失

```text
Client                         Server

        <---------------------- FIN

ACK --------X

                               等待 ACK
                                  ↓
                               超时
                                  ↓
        <---------------------- FIN（重传）

ACK --------------------------->
```

**靠 FIN 重传解决。**

而 Client 的：

```text
TIME_WAIT
```

保证了即使最后 ACK 丢失，Server 重传 FIN 时，Client 仍然能够响应。

------

## 最后记住一句最核心的话

不要死记：

> **三次握手，四次挥手。**

应该理解成：

> **建立连接时，双方都需要“开始通信”，Server 的 SYN 和对 Client SYN 的 ACK 可以一起发，所以 3 次。**

> **断开连接时，双方的“停止发送”是相互独立的，一方停止发送并不代表另一方也准备停止，所以通常需要 4 次。**

而**丢包并不会改变这个基本次数**，TCP 靠的是：

```text
状态机
  +
超时
  +
重传
  +
TIME_WAIT
```

来把丢失的报文补回来。