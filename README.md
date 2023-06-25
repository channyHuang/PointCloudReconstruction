# PointCloudReconstruction
3d reconstruction from point cloud and pose

从点云中重建三维网格模型。

## input / output
输入：
* 激光姿态.pose文件，包含两行共6个数字，第一行三个数表示激光位置，第二行三个数表示激光旋转角度（相对世界坐标系）
* 点云.obj文件，激光姿态.pose文件对应的点云
以thermocolorlab.tar为样例输入。

输出：
* 非封闭三维风格模型
* 渲染显示

# dataset
[ASL](https://projects.asl.ethz.ch/datasets/doku.php?id=home)
[3D Scans](http://kos.informatik.uni-osnabrueck.de/3Dscans/)
[tinyobjloader](https://github.com/tinyobjloader/tinyobjloader)