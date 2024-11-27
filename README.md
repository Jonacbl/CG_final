目前是final Project

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

即可运行。
