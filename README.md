目前是final Project

**已完成部分**
- assimp加载静态模型
- assimp加载动态模型

在**解决方案资源管理器右键**-**属性**添加对应的**包含文件**路径和**库文件**路径：

```
./include
./lib
```



![](README/include.png)

链接器-输入中添加：

```
glfw3.lib
assimp-vc143-mtd.lib
```

![](README/linker.png)

即可在`.sln`运行中运行，目前只实现了静态加载`assimp`和动画assimp。

有人来了test
