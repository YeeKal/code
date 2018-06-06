- 梯度: 方向导数最大的方向,其大小正好是此最大方向导数

$$\begin{align}
G(x,y)&=dx(i,j)+dy(i,j) \\
G_x&=f(x,y)-f(x-1,y) ,or \quad G_x=f(x+1,y)-f(x-1,y) \\
G_y&=f(x,y)-f(x,y-1),or \quad G_y=f(x,y+1)-f(x,y-1)
\end{align}$$

- 散度: 作用对像是向量场,导数

laplacian operator:

|0 | -1| 0|
|-1| 4 |-1|
|0 |-1 |0 |

- 散度+边界约束条件，实现图像重建


**ref**

1. [possion image editing](http://www.ipol.im/pub/art/2016/163/)