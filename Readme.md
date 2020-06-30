## Deformation 

- Reproduced paper:

	Olga Sorkine, Daniel Cohen-Or, et al.  2004. Laplacian Surface Editing.  In Proceedings of the Eurographics/ACM SIGGRAPH Symposium on Geometry Processing, 175–184.


### Introduction

- Laplacian surface editing

- constant Laplacian

$$
\phi(v_i) = v_i - \frac{1}{d_i} \sum_{j\in N_i} v_j
$$

- Energy function used

  $$
  E(V') = \sum_{i=1}^n ||\delta_i - \phi(v'_i)||^2 + \sum_{i=m}^n||v'_i-u_i||^2
  $$

#### External library

- [GLFW](https://www.glfw.org/) and [Glad](https://glad.dav1d.de/)

- [Assimp](https://www.assimp.org/)

- [glm](https://github.com/g-truc/glm)

- [Eigen](https://eigen.tuxfamily.org/) 

#### display

- Armadillo  model: 494 vertices in ROI

![2](Readme.assets/2.png)

- Bunny model: 1815 vertices in ROI

![3](Readme.assets/3.png)

