### Final Project for Computer Graphic of SYSU in 2024 fall

**Group 17**

**Tutor: Prof. Chengying Gao, SYSU**

**Collaborators**：

- [Jonacbl](https://github.com/Jonacbl)
- [lagrange](https://github.com/lxl1437)
- [BaiHYF](https://github.com/BaiHYF)
- [Charles4731](https://github.com/Charles4731)

----

#### 实验内容

##### 小组选题
使用OpenGL搭建并渲染一个MC城堡的场景。

##### 实现内容
- **模型文件加载**：使用Assimp库加载静态模型文件。
- **动画模型加载**：使用Assimp库加载骨骼动画模型文件。
- **实例化**：使用实例化渲染大片重复内容。
- **天空盒背景**：使用立方体贴图实现天空盒背景。
- **Blinn-Phong光照**：实现Blinn-Phong光照明模型。
- **交互摄像机**：通过键盘输入可以实现照相机的移动，鼠标滚轮实现拉近效果。

##### 实验环境
- **操作系统**：Windows 11
- **开发工具**：Visual Studio 2022
- **编程语言**：C++17及以上
- **OpenGL版本**：OpenGL 3.3及以上
- **依赖库**：
    - **GLFW**：用于创建窗口和管理OpenGL上下文。
    - **glad**：OpenGL扩展加载库。
    - **glm**：用于矩阵、向量、变换等数学计算的库。
    - **assimp**：用于加载静态和动画模型文件。
    - **stb_image**：用于加载纹理图片。

#### 最终实现效果

加载了所有的所需要的静态模型：城堡、长椅等，动画模型：宝箱怪，实例化的草地和阳光明媚的天空盒。最终运行的结果如下：

![final scene](https://github.com/user-attachments/assets/5820360f-f468-4251-92b1-602393807193)

#### 相关参考网站

1. [LearnOpenGL CN](https://learnopengl-cn.github.io/) : 提供了`OpenGL`基本理论及其实现。
2. [Sketchfab](https://sketchfab.com/feed)：包含大量免费可下载的静态模型和动画模型。
3. [编译安装Assimp on CSDN](https://blog.csdn.net/guoguojune/article/details/130344278): 提供了assimp编译以及链接到`OpenGL`项目的教程。
4. [opengameart](https://opengameart.org)：包含大量免费可下载的天空盒。
