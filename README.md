<p align="left">
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/langs/C++/cyan?list=1" alt="languages"></a>
  <a href="https://github.com/ChunelFeng/CGraph"><img src="https://badgen.net/badge/os/MacOS,Linux,Windows/cyan?list=1" alt="os"></a>
  <a href="https://github.com/ChunelFeng/CGraph/stargazers"><img src="https://badgen.net/github/stars/ChunelFeng/CGraph?color=cyan" alt="stars"></a>
  <a href="https://github.com/ChunelFeng/CGraph/network/members"><img src="https://badgen.net/github/forks/ChunelFeng/CGraph?color=cyan" alt="forks"></a>
</p>

<h1 align="center">
  CGraph 说明文档
</h1>

>CGraph is a cross-platform `DAG`(Directed Acyclic Graph) framework based on C++17 without any 3rd-party.</br></br>
>You, with it, can build your own operators simply, and then describe any running schedules for them as you need, such as dependence, parallelling, loop, condition and aggregation. Some useful tools and plugins are also provided to improve your project.</br></br>
>Tutorials and contact information are shown as follows. Please get in touch with us for free if you need more about this repository.

## 一. 简介

本工程实现了一套无任何第三方依赖的跨平台图流程计算框架。通过`GPipeline`(流水线)底层调度，实现了依赖元素依次顺序执行、非依赖元素并发执行的调度功能。

使用者只需继承`GNode`(节点)类，实现子类的run()方法，并根据需要设定依赖关系，即可实现任务的图化执行。

同时，使用者还可以通过设定各种包含多节点信息的`GGroup`(组)，自行控制图的条件判断、循环和并发执行逻辑。

此外，还可以通过添加`GAspect`(切面)的方式，实现以上各种元素功能的横向扩展，或是通过引入各种`GAdapter`(适配器)对单个节点功能进行加强。

![CGraph Skeleton](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Skeleton.jpg)
<br>

## 二. 编译说明
* 本工程支持MacOS、Linux和Windows系统，无任何第三方依赖。使用CLion作为IDE的开发者，打开CMakeLists.txt文件作为工程，即可编译通过

* Linux环境开发者，在命令行模式下，输入以下指令，即可编译通过
  ```shell
  $ git clone https://github.com/ChunelFeng/CGraph.git
  $ cd CGraph
  $ cmake . -Bbuild
  $ cd build
  $ make
  ```

* 提供基于`Ubuntu 20.04.3 LTS`的Docker镜像。输入以下指令，即可获取并进入
  ```shell
  $ docker pull chunelfeng/cenv                         # 获取docker镜像
  $ docker run -it --name CGraphEnv chunelfeng/cenv     # 开启docker容器，并进入
  ```

## 三. 使用Demo

#### MyNode1.h
```cpp
#include "../../src/CGraph.h"

class MyNode1 : public CGraph::GNode {
public:
    CStatus run () override {
        CStatus status;
        CGraph::CGRAPH_ECHO("[%s], enter MyNode1 run function. Sleep for 1 second ... ", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(1)
        return status;
    }
};
```

#### MyNode2.h
```cpp
#include "../../src/CGraph.h"

class MyNode2 : public CGraph::GNode {
public:
    CStatus run () override {
        CStatus status;
        CGraph::CGRAPH_ECHO("[%s], enter MyNode2 run function. Sleep for 2 second ... ", this->getName().c_str());
        CGRAPH_SLEEP_SECOND(2)
        return status;
    }
};
```

#### demo.cpp
```cpp
#include "MyGNode/MyNode1.h"
#include "MyGNode/MyNode2.h"

using namespace CGraph;

void tutorial_simple() {
    /* 创建一个流水线，用于设定和执行流图信息 */
    GPipelinePtr pipeline = GPipelineFactory::create();
    GElementPtr a, b, c, d = nullptr;

    /* 注册节点，其中MyNode1和MyNode2必须为GNode的子类，否则无法通过编译。
     * MyNode1中run()执行内容为sleep(1s)
     * MyNode2中run()执行内容为sleep(2s) */
    CStatus status = pipeline->registerGElement<MyNode1>(&a, {}, "nodeA");    // 将名为nodeA，无执行依赖的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode2>(&b, {a}, "nodeB");    // 将名为nodeB，依赖a执行的node信息，注册入pipeline中
    status += pipeline->registerGElement<MyNode1>(&c, {a}, "nodeC");
    status += pipeline->registerGElement<MyNode2>(&d, {b, c}, "nodeD");    // 将名为nodeD，依赖{b,c}执行的node信息，注册入pipeline中
    if (!status.isOK()) {
        return;    // 使用时，请对所有CGraph接口的返回值做判定
    }

    /* 执行流图框架 */
    status = pipeline->process();
    GPipelineFactory::remove(pipeline);
}
```

![CGraph Demo](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Demo.jpg)
<br>
如上图所示，图结构执行的时候，首先执行`a`节点。`a`节点执行完毕后，并行执行`b`和`c`节点。`b`和`c`节点全部执行完毕后，再执行`d`节点。

## 四. 感谢
* 感谢《HelloGithub》期刊介绍和推荐：[HelloGithub 第70期](https://github.com/521xueweihan/HelloGitHub/blob/master/content/70/HelloGitHub70.md)

------------
#### 附录-1. 版本信息

[2022.01.23 - v1.8.3 - Chunel]
* 提供`function`适配器，实现函数式编程功能
* 提供线程优先级调度功能，提供线程绑定cpu执行功能
* 更新`tutorial`内容

[2022.01.31 - v1.8.4 - Chunel]
* 提供`node`(节点)异步执行的功能

[2022.02.03 - v1.8.5 - Chunel]
* 提供`daemon`(守护)功能，用于定时执行非流图中任务
* 更新`tutorial`内容

> 更多版本变更信息，请参考 [ChangeLog.md](https://github.com/ChunelFeng/CGraph/blob/main/ChangeLog.md) 文件

------------
#### 附录-2. 推荐阅读

* [纯序员给你介绍图化框架的简单实现——执行逻辑](http://www.chunel.cn/archives/cgraph-run-introduce)
* [纯序员给你介绍图化框架的简单实现——循环逻辑](http://www.chunel.cn/archives/cgraph-loop-introduce)
* [纯序员给你介绍图化框架的简单实现——参数传递](http://www.chunel.cn/archives/cgraph-param-introduce)
* [纯序员给你介绍图化框架的简单实现——条件判断](http://www.chunel.cn/archives/cgraph-condition-introduce)
* [纯序员给你介绍图化框架的简单实现——面向切面](http://www.chunel.cn/archives/cgraph-aspect-introduce)
* [纯序员给你介绍图化框架的简单实现——函数注入](http://www.chunel.cn/archives/cgraph-function-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（一）](http://www.chunel.cn/archives/cgraph-threadpool-1-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（二）](http://www.chunel.cn/archives/cgraph-threadpool-2-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（三）](http://www.chunel.cn/archives/cgraph-threadpool-3-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（四）](http://www.chunel.cn/archives/cgraph-threadpool-4-introduce)
* [纯序员给你介绍图化框架的简单实现——线程池优化（五）](http://www.chunel.cn/archives/cgraph-threadpool-5-introduce)

------------
#### 附录-3. 联系方式

* 微信： ChunelFeng
* 邮箱： chunel@foxmail.com
* 源码： https://github.com/ChunelFeng/CGraph
* 论坛： www.chunel.cn

![CGraph Author](https://github.com/ChunelFeng/CGraph/blob/main/doc/image/CGraph%20Author.jpg)
