<<<<<<< HEAD
目前是final Project

**Collaborators**：

- [Jonacbl](https://github.com/Jonacbl)
- [lagrange](https://github.com/lxl1437)
- [BaiHYF](https://github.com/BaiHYF)
- [Charles4731](https://github.com/Charles4731)



**已完成部分**

- assimp加载静态模型
- assimp加载动态模型
- skybox加载天空盒



在**解决方案资源管理器右键**-**属性**添加对应的**包含文件**路径和**库文件**路径：

```
./include
./lib
```

![](./README/include.png)

链接器-输入中添加：

```
glfw3.lib
assimp-vc143-mtd.lib
```

![](./README/linker.png)

即可在`.sln`运行中运行。
=======
目前是final Project

**Collaborators**：

- [Jonacbl](https://github.com/Jonacbl)
- [lagrange](https://github.com/lxl1437)
- [BaiHYF](https://github.com/BaiHYF)
- [Charles4731](https://github.com/Charles4731)



**已完成部分**

- assimp加载静态模型
- assimp加载动态模型
- skybox加载天空盒
- blinn-phong光照
- 交互摄像机



在**解决方案资源管理器右键**-**属性**添加对应的**包含文件**路径和**库文件**路径：

```
./include
./lib
```

![](./README/include.png)

链接器-输入中添加：

```
glfw3.lib
assimp-vc143-mtd.lib
```

![](./README/linker.png)

即可在`.sln`运行中运行。
>>>>>>> 7a654fa7f3b6f0fa457bf180f6e6cb58e7bca63b
