## ttlib
ttlib 是一个侧重嵌入式软件的C语言库。了解更多请参考[在线文档](https://tangaoo.github.io/)。

### 初衷为解决下面两个问题。

- 应用程序在不同嵌入式平台迁移的问题。
  
  嵌入式硬件架构多，嵌入式 `OS` 也多，例如在 `ARM-Linux` 上运行的程序很难迁移到` M3-rtthread/uCos`上，然后在M3上正常运行的程序又很难运行到 `DSP-sys/bios` 上等等。不同的产品往往跑在不同的硬件架构不同的 `OS` 上面，这样使得一个公司的嵌入式软件产品维护难度大增。

- 嵌入式软件开发效率低的问题。
  
  嵌入式程序员大多喜欢从头开始，行业也少有标准库。自己造轮子不仅仅效率低，往往是bug的源头，也是可维护性的敌人。
  
  
### ttlib会包含哪些东西。（以下功能正在开发中，开发进度见changlog.md）

#### 平台库
- 平台库主要是对系统调用进行一次封装，对上层应用屏蔽 `OS` 特性。计划支持 `Linux`、`rt-thread`、`sys/bios`。
- 线程/任务
- 信号量
- 事件
- 互斥
- 自旋锁
- socket
- file
- time
- timer等
  
#### 流库
- 流库主要实现统一 `I/O` 操作接口，不管是可见大小的 `file` 流还是串口这样的纯流，提供阻塞与非阻塞操作方式。
- file流
- socket
- buffer
- 串口
- u口等
  
#### 动态内存库
- ttlib库的其他模块也基于此进行动态内存操作，这样就算在裸机上也能跑ttlib库
- 大块内存分配
- 固定内存分配
- 内存池
  
#### 容器库
- 提供哈希、链表、数组、队列、堆栈、最小最大堆等常用容器
- 迭代器支持

#### 算法库
- 提供各种排序算法：冒泡排序、堆排序、快速排序、插入排序。
- 提供各种查找算法：线性遍历、二分法搜索。
- 提供各种遍历、删除、统计算法
  
#### 实用工具库
- 实现base64/32编解码
- 实现crc32、md5等常用hash算法
- 实现debug输出、断言
- 实现日志模块
- 实现位操作相关接口，支持各种数据格式的解析，可以对8bits、16bits、32bits、64bits字段进行解析操作，并且同时支持大端、小端和本地端模式。
- 实现单例模块，可以对静态对象、实例对象进行快速的单例封装，实现全局线程安

#### 其他
- 等等