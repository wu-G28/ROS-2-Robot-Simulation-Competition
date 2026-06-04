# ROS-2-Robot-Simulation-Competition

> **第十九届先进机器人及仿真技术大赛 — 无人车组 — 科目一：事故勘探与清障**

基于 ROS2 Humble + Gazebo 的无人车事故勘探仿真系统。模拟封闭厂区灾情场景，实现 SLAM 建图导航、视觉标识识别、障碍物物理清障、AprilTag 定位标定等完整任务流程。

---

## 目录

- [比赛任务概述](#比赛任务概述)
- [系统架构](#系统架构)
- [任务流程设计](#任务流程设计)
- [代码模块说明](#代码模块说明)
- [自定义消息接口](#自定义消息接口)
- [配置文件说明](#配置文件说明)
- [URDF 车辆模型](#urdf-车辆模型)
- [编译与运行](#编译与运行)
- [比赛待办清单](#比赛待办清单)
- [技术栈](#技术栈)

---

## 比赛任务概述

### 故事背景

某封闭厂区发生事故，人员无法进入。无人车需独立进入厂区进行初步勘探。

### 任务流程

```
走廊搜寻          核心区入口             核心区内部
┌──────────────┐    ┌──────────────┐    ┌──────────────┐
│ 识别5个初级   │    │ 推开障碍物    │    │ 识别核心标识  │
│ 灾害标识      │───►│ 物理清障     │───►│ AprilTag定位 │
│ (走廊视觉识别) │    │ (入口清障交互) │    │ (核心区标定)  │
└──────────────┘    └──────────────┘    └──────────────┘
     15分                 20分                 20分
```

### 评分总览

| 评分项 | 分值 | 核心考点 |
|--------|:----:|---------|
| 环境构建与单车建图 | 15 | Gazebo 仿真环境搭建 + SLAM 2D 占据栅格地图 |
| 厂区走廊视觉识别 | 15 | 识别 5 个初级灾害标识，终端打印类别 |
| 核心区入口清障交互 | 20 | 物理碰撞推开障碍物，平稳驶入核心区 |
| 核心区标定与视觉识别 | 20 | 识别核心标识 + AprilTag + RViz Marker 标定 |
| 方案创新性 | 15 | 环境逼真度、推障机构、算法原创性 |
| 技术文档质量 | 15 | TF 树、节点流程图、技术原理说明 |

### 提交要求

| 材料 | 格式要求 |
|------|---------|
| 代码包 | 含 `start.sh` 一键启动脚本，参数独立 `.yaml` 配置，禁止硬编码坐标 |
| 演示视频 | MP4，≤5min，≤100M，一刀不剪，带系统时间，左右分屏 (Gazebo + RViz/终端) |
| 技术文档 | PDF，《技术实施报告》，含架构图/TF树/节点流程图 |

---

## 系统架构

```
                            ┌──────────────────────────────┐
                            │         start.sh             │
                            │      (一键启动脚本)            │
                            └─────────────┬────────────────┘
                                          │
              ┌───────────────────────────┼───────────────────────────┐
              │                           │                           │
              ▼                           ▼                           ▼
    ┌──────────────────┐     ┌──────────────────┐     ┌──────────────────┐
    │    Gazebo 仿真    │     │   RViz2 可视化    │     │   规划控制节点    │
    │  (厂区.world)     │     │  (地图+Marker+    │     │   (ROS2 Nodes)   │
    │  物理引擎+传感器  │     │   TF+路径显示)    │     │   感知+规划+控制  │
    └────────┬─────────┘     └────────┬─────────┘     └────────┬─────────┘
             │                        │                        │
             └────────────────────────┼────────────────────────┘
                                      │
                          ┌───────────┴───────────┐
                          │       ROS2 Humble      │
                          │   Topic / Service /    │
                          │   Action / TF2         │
                          └───────────────────────┘
```

### 节点拓扑图

```
                         ┌─────────────────┐
                         │   /clock         │ (Gazebo)
                         └────────┬────────┘
                                  │
  ┌───────────────────────────────┼───────────────────────────────┐
  │                               │                               │
  ▼                               ▼                               ▼
┌──────────┐  /scan   ┌──────────────┐  /camera  ┌──────────────┐
│  LiDAR   │─────────►│ slam_toolbox │◄─────────│   Camera     │
│ (Gazebo) │          │   (建图)      │           │  (Gazebo)    │
└──────────┘          └──────┬───────┘           └──────┬───────┘
                             │ /map                     │ /image_raw
                             ▼                          ▼
                      ┌──────────────┐          ┌──────────────┐
                      │   Nav2       │          │  vision_det  │
                      │ (导航栈)      │          │ (视觉识别)    │
                      └──────┬───────┘          └──────┬───────┘
                             │ /cmd_vel                │ /detection
                             ▼                          ▼
                      ┌──────────────┐          ┌──────────────┐
                      │  move_cmd    │          │ apriltag_det │
                      │ (运动控制)    │          │ (Tag检测)     │
                      └──────┬───────┘          └──────┬───────┘
                             │                         │
                             ▼                         ▼
                      ┌──────────────┐          ┌──────────────┐
                      │   TF2        │          │  rviz_marker │
                      │ (坐标广播)    │          │ (Marker标定)  │
                      └──────────────┘          └──────────────┘
```

### 项目目录结构

```
ROS-2-Robot-Simulation-Competition/
├── start.sh                             # 一键启动脚本（待创建）
│
├── src/
│   ├── base_msgs/                       # 自定义消息接口包 (ament_cmake)
│   │   ├── msg/                         # 消息定义 (11个)
│   │   │   ├── PNCMap.msg               #   PNC地图
│   │   │   ├── Referline.msg            #   参考线
│   │   │   ├── ReferlinePoint.msg       #   参考线点
│   │   │   ├── LocalPath.msg            #   局部路径
│   │   │   ├── LocalPathPoint.msg       #   局部路径点
│   │   │   ├── LocalSpeeds.msg          #   速度曲线
│   │   │   ├── LocalSpeedsPoint.msg     #   速度点
│   │   │   ├── LocalTrajectory.msg      #   合成轨迹
│   │   │   ├── LocalTrajectoryPoint.msg #   轨迹点
│   │   │   ├── ObsInfo.msg              #   障碍物信息
│   │   │   └── PlotInfo.msg             #   绘图数据
│   │   └── srv/                         # 服务定义 (2个)
│   │       ├── PNCMapService.srv        #   地图服务
│   │       └── GlobalPathServer.srv     #   全局路径服务
│   │
│   ├── planning/                        # 核心规划包 (ament_cmake)
│   │   ├── src/
│   │   │   ├── common/                  # 公共基础库
│   │   │   │   ├── config_reader/       #   YAML 配置读取器 ★完成
│   │   │   │   └── math/                #   曲线/多项式数学库
│   │   │   ├── pnc_map_creator/         # PNC 地图创建器 (服务端)
│   │   │   ├── global_planner/          # 全局路径规划器 (服务端)
│   │   │   ├── reference_line/          # 参考线生成 + OSQP 平滑
│   │   │   ├── decision_center/         # 决策中心 (障碍物避障)
│   │   │   ├── local_planner/           # 局部规划器
│   │   │   │   ├── local_path/          #   多项式路径规划 + 平滑
│   │   │   │   ├── local_speeds/        #   速度曲线规划 + 平滑
│   │   │   │   └── local_trajectory_combiner.*  # 轨迹合成
│   │   │   ├── vehicle_info/            # 车辆模型 (主车 + 障碍物)
│   │   │   ├── move_cmd/                # 运动指令 (主车 + 障碍物节点)
│   │   │   └── planning_process/        # 规划总流程编排 (主入口)
│   │   ├── launch/                      # ROS2 Launch 启动文件
│   │   ├── urdf/main_car/               # 车辆 URDF/Xacro 模型
│   │   │   ├── car.xacro                #   模型入口
│   │   │   ├── car_base.xacro           #   底盘
│   │   │   ├── car_wheels.xacro         #   四轮
│   │   │   ├── car_radar.xacro          #   激光雷达
│   │   │   └── car_camera.xacro         #   摄像头
│   │   ├── rviz/                        # RViz2 配置文件
│   │   └── worlds/                      # Gazebo 世界文件（待创建）
│   │
│   ├── data_plot/                       # 数据可视化包 (ament_python)
│   │   └── data_plot/data_plot.py       #   matplotlib 实时绘图
│   │
│   ├── vision_det/                      # 视觉检测包（待创建）
│   │   └── vision_det/                  #   YOLO/ResNet 灾害标识识别
│   │
│   ├── apriltag_det/                    # AprilTag 检测包（待创建）
│   │   └── apriltag_det/                #   AprilTag 识别 + RViz Marker
│   │
│   └── config/                          # 全局配置文件
│       ├── planning_dynamic_obs_config.yaml  # 规划参数
│       ├── nav2_params.yaml                  # 导航参数（待创建）
│       └── slam_params.yaml                  # SLAM 参数（待创建）
│
├── build/                               # 编译输出
├── install/                             # 安装目录
└── log/                                 # 日志
```

---

## 任务流程设计

### 整体状态机

```
                  ┌─────────────┐
                  │  系统初始化   │
                  │ (start.sh)   │
                  └──────┬──────┘
                         │
                         ▼
                  ┌─────────────┐
                  │  SLAM 建图   │
                  │ (slam_toolbox)│
                  └──────┬──────┘
                         │ /map 就绪
                         ▼
              ┌─────────────────────┐
              │  走廊导航 + 视觉识别  │ ◄── 阶段1: 走廊区
              │  (Nav2 + vision)     │     识别5个初级标识
              └──────────┬──────────┘
                         │ 到达核心区入口
                         ▼
              ┌─────────────────────┐
              │  障碍物检测 + 清障    │ ◄── 阶段2: 入口区
              │  (lidar + push)      │     物理推开障碍物
              └──────────┬──────────┘
                         │ 通道打通
                         ▼
              ┌─────────────────────┐
              │  核心区导航 + 标定    │ ◄── 阶段3: 核心区
              │  (AprilTag + Marker) │     识别标识+定位
              └──────────┬──────────┘
                         │ 任务完成
                         ▼
                  ┌─────────────┐
                  │  任务结束    │
                  │  输出结果    │
                  └─────────────┘
```

### 各阶段详解

#### 阶段 1 — 走廊视觉识别 (15分)

```
┌──────────────────────────────────────────────────┐
│ 无人车从起点出发，沿走廊自主导航                   │
│ 途中识别墙壁上 5 个初级灾害标识                   │
│ 终端打印: [ID] class_name confidence              │
│                                                  │
│ 标识由参赛队伍自行设计，图案互不相同               │
│ 得分: 每个正确识别 3 分，漏报/误报扣 3 分         │
└──────────────────────────────────────────────────┘
```

**技术要点**：
- Gazebo 中在走廊墙壁放置 5 个标识纹理
- 车载 RGB 相机采集图像 (`/camera/image_raw`)
- 视觉节点推理并打印类别到终端

#### 阶段 2 — 核心区入口清障 (20分)

```
┌──────────────────────────────────────────────────┐
│ 无人车到达核心区入口，发现门口被障碍物封堵         │
│ 通过 LiDAR 检测障碍物位置                         │
│ 切换推障机动模式：对准→加速→发力推开              │
│ 障碍物推开后，平稳驶入核心区                       │
│                                                  │
│ 关键: 不能物理穿模、卡死、或未能彻底推开          │
│ 得分: 成功推开并平稳进入得 20 分                  │
└──────────────────────────────────────────────────┘
```

**技术要点**：
- Gazebo 障碍物模型需支持物理碰撞（mass/inertia 参数）
- 推障前切换控制模式（导航控制器 → 推障控制器）
- 通过激光雷达实时检测障碍物是否已移开

#### 阶段 3 — 核心区标定与识别 (20分)

```
┌──────────────────────────────────────────────────┐
│ 进入核心区室内                                    │
│ 识别核心灾害标识（带 AprilTag 二维码阵列）         │
│ 解析 AprilTag 获取 ID 和相对位姿                   │
│ 转换为 map 坐标系，以 Marker 形式发布到 RViz       │
│                                                  │
│ 得分: 类别正确 + Marker 与实际布局吻合得 20 分     │
│ 扣分: Marker 乱飘、相对位置明显不符               │
└──────────────────────────────────────────────────┘
```

**技术要点**：
- 标识图案上嵌套 AprilTag（如 tag36h11 家族）
- `apriltag_ros` 或自行实现检测节点
- TF 变换: `camera_frame → tag_frame → map`
- 发布 `visualization_msgs/Marker` 到 RViz

---

## 代码模块说明

### 已实现模块

#### ConfigReader — 配置读取器 ★

| 结构体 | 字段 | YAML 路径 |
|--------|------|-----------|
| `VehicleStruct` | id, frame, length, width, x, y, pose_theta, speed_ori | `vehicle.main_car` |
| `PNCMapStruct` | frame, road_length, road_half_width, segment_len, speed_limit | `pnc_map` |
| `GlobalPathStruct` | type (0:Normal, 1:A*) | `global_path` |
| `ReferLineStruct` | type, front_size, back_size | `reference_line` |
| `LocalPathStruct` | curve_type (0一次/1三次/2五次), path_size | `local_path` |
| `LocalSpeedsStruct` | speed_size | `local_speed` |
| `DecisionStruct` | safe_dis_l, safe_dis_s | `decision` |
| `ProcessStruct` | obs_dis | `planning_process` |

采用**级联读取**策略：后续模块自动加载前置依赖配置，避免遗漏。

#### mathlibs — 数学工具库

| 类 | 依赖 | 用途 |
|----|------|------|
| `Curve` | — | 曲线抽象基类 |
| `PolynomialCurve` | Eigen3 | 多项式曲线拟合与求值 |

#### vehicle_info — 车辆信息模型

| 类 | 职责 |
|----|------|
| `VehicleInfo` | 车辆抽象基类 |
| `MainCarInfo` | 主车状态：位姿、速度、TF 帧 |
| `ObsInfo` | 障碍物状态：位姿、尺寸、Frenet 投影 |

#### 规划流水线（骨架已搭建，逻辑待实现）

| 模块 | 节点/库 | 职责 |
|------|---------|------|
| `pnc_map_creator` | pnc_map_server (节点) | 通过服务提供结构化道路地图 |
| `global_planner` | global_path_server (节点) | Normal/A* 全局路径规划 |
| `reference_line` | 动态库 | 参考线裁剪 + OSQP-Eigen 平滑 |
| `decision_center` | 动态库 | 障碍物检测 + 跟车/绕行/停车决策 |
| `local_planner` | 动态库 | 多项式路径 + 速度曲线 + 轨迹合成 |
| `move_cmd` | car/obs_move_cmd (节点) | TF 广播更新车辆位姿 |
| `planning_process` | planning_process (节点) | 总流程编排与状态管理 |

#### 模块依赖关系

```
config_reader ──► vehicle_info ──► decision_center
     │                │                  │
     ▼                ▼                  ▼
pnc_map_creator  reference_line    local_planner
     │                │                  │
     ▼                ▼                  ▼
global_planner   reference_line    move_cmd
     │           _smoother              │
     │                │                  │
     └────────────────┼──────────────────┘
                      ▼
               planning_process
```

### 待创建模块（比赛专用）

| 模块 | 类型 | 用途 |
|------|------|------|
| `vision_det` | Python 包 | 灾害标识视觉识别（YOLO/ResNet/模板匹配） |
| `apriltag_det` | Python 包 | AprilTag 检测 + TF 发布 + RViz Marker |
| `push_controller` | C++ 节点 | 推障机动专项控制 |
| `gazebo_world` | 资源 | 厂区走廊+核心室 .world 文件 |
| `disaster_models` | 资源 | 灾害标识+障碍物 Gazebo 模型 |

---

## 自定义消息接口

### 消息一览

| 消息 | 坐标系 | 核心字段 |
|------|:------:|---------|
| `PNCMap` | map | road_length, road_half_width, left/right/midline Marker |
| `Referline` | map | ReferlinePoint[] (pose, rs, rtheta, rkappa, rdkappa) |
| `ReferlinePoint` | map | 参考线投影: rs/rtheta/rkappa/rdkappa |
| `LocalPath` | map | LocalPathPoint[] (pose, s, l, theta, kappa, dkappa, Frenet导数) |
| `LocalPathPoint` | map | 完整路径点: 笛卡尔 + Frenet + 曲率及其导数 |
| `LocalSpeeds` | — | LocalSpeedsPoint[] (t, speed, acceleration, dacceleration) |
| `LocalSpeedsPoint` | — | 速度采样: t, s_2path, speed, acc, jerk |
| `LocalTrajectory` | map | 路径点 + 速度点 一一对应合成 |
| `LocalTrajectoryPoint` | map | = LocalPathPoint + LocalSpeedsPoint |
| `ObsInfo` | Frenet | obs_length/width, l, s, s_2path, t_in/t_out |
| `PlotInfo` | map | trajectory_info + obs_info[] |

### 服务接口

| 服务 | 请求 | 响应 |
|------|------|------|
| `PNCMapService` | `int32 map_type` | `PNCMap pnc_map` |
| `GlobalPathServer` | `int32 type` + `PNCMap pnc_map` | `nav_msgs/Path global_path` |

### Frenet 坐标系

本项目以 Frenet 坐标系为核心参考系，与笛卡尔坐标系配合使用：

| 符号 | 含义 |
|------|------|
| **s** | 沿参考线的纵向弧长位移 |
| **l** | 垂直于参考线的横向偏移 |
| **rs / rtheta / rkappa / rdkappa** | 投影点在参考线上的弧长/航向/曲率/曲率变化率 |
| **s_2path** | 障碍物投影到局部路径上的弧长（用于碰撞预测） |

---

## 配置文件说明

### 规划配置 — `src/config/planning_dynamic_obs_config.yaml`

```yaml
vehicle:                    # 车辆参数
  main_car:                 # 主车
    id: 0
    frame: "base_footprint"
    length: 3.0             # m
    width: 1.5              # m
    pose_x/y/theta: 0.0     # 初始位姿
    speed_ori: 1.0           # m/s

pnc_map:                    # 地图
  frame: "map"
  road_length: 1250.0       # m
  road_half_width: 4.0      # m
  segment_len: 0.5          # m
  speed_limit: 1.0           # m/s

global_path:
  type: 0                   # 0:Normal 沿中心线 | 1:A*

reference_line:
  type: 0                   # 0:Normal | 1:Stitch
  front_size: 200           # 前方参考点数
  back_size: 80             # 后方参考点数

local_path:
  curve_type: 2             # 0:一次 | 1:三次 | 2:五次多项式
  path_size: 80

local_speed:
  speed_size: 100

decision:
  safe_dis_l: 0.5           # 横向安全距离 (m)
  safe_dis_s: 20.0          # 纵向安全距离 (m)

planning_process:
  obs_dis: 100.0            # 障碍物检测半径 (m)
```

---

## URDF 车辆模型

```
                    ┌──────────┐
                    │  LiDAR   │ ← 半径 0.105m, 高度 0.08m
                    ├──────────┤
                    │ Top Plate│ ← 0.34×0.26×0.01m
        ┌───────────┼──────────┼───────────┐
        │  Camera   │   Plate  │  Screen   │
        │  前向      │  中层板  │  显示屏   │
        └───────────┴────┬─────┴───────────┘
                         │
              ┌──────────┴──────────┐
              │      base_link      │ ← 0.56×0.36×0.08m
              └──────────┬──────────┘
                         │
         ┌──────┬────────┼────────┬──────┐
         │ 前左  │  前右  │  后左  │ 后右  │ ← 四轮 0.075m半径
         └──────┘ └──────┘ └──────┘ └──────┘
```

| 组件 | 尺寸 (m) | Gazebo 插件 |
|------|---------|-------------|
| 底盘 | 0.56×0.36×0.08 | — |
| 四轮 | r=0.075, w=0.055 | `libgazebo_ros_diff_drive` |
| LiDAR | r=0.105, h=0.08 | `libgazebo_ros_ray_sensor` |
| Camera | 0.44×0.065×0.075 | `libgazebo_ros_camera` |

TF 变换链:
```
map → odom → base_footprint → base_link → lidar_link
                                        → camera_link
                                        → wheel_*
```

---

## 编译与运行

### 环境

| 组件 | 版本/名称 |
|------|----------|
| OS | Ubuntu 22.04 |
| ROS2 | Humble Hawksbill |
| 仿真器 | Gazebo (Ignition Fortress 或 Classic 11) |
| 构建 | colcon |

### 安装依赖

```bash
# ROS2 基础
sudo apt install ros-humble-desktop ros-humble-gazebo-ros-pkgs

# SLAM & 导航
sudo apt install ros-humble-slam-toolbox ros-humble-nav2-bringup

# AprilTag
sudo apt install ros-humble-apriltag-ros

# 规划依赖
sudo apt install libeigen3-dev ros-humble-osqp-eigen libyaml-cpp-dev

# Python 依赖
pip install numpy matplotlib opencv-python torch torchvision
```

### 编译

```bash
cd ROS-2-Robot-Simulation-Competition
colcon build --symlink-install
source install/setup.bash
```

### 运行

```bash
# 一键启动（待创建）
./start.sh

# 或分步启动:

# 1. Gazebo 仿真环境
ros2 launch planning gazebo_world.launch.py

# 2. SLAM 建图
ros2 launch planning slam.launch.py

# 3. 导航栈
ros2 launch planning nav2.launch.py

# 4. 视觉识别
ros2 run vision_det vision_det

# 5. AprilTag 检测
ros2 run apriltag_det apriltag_det

# 6. 规划总流程
ros2 run planning planning_process

# 7. RViz 可视化
ros2 launch planning display.launch.py
```

---

## 比赛待办清单

### 一、环境构建与单车建图 (15分)

- [ ] **1.1** 创建 Gazebo 厂区世界文件 (`factory.world`)：走廊 + 核心救援室
- [ ] **1.2** 放置墙壁、地面、入口门框等静态模型
- [ ] **1.3** 在走廊墙壁放置 5 个初级灾害标识纹理
- [ ] **1.4** 在核心区门口放置可推动的大质量障碍物模型
- [ ] **1.5** 集成 `slam_toolbox` 或 `cartographer` 生成 2D 占据栅格地图
- [ ] **1.6** 配置 Nav2 导航栈（全局/局部代价地图、规划器、控制器、行为树）
- [ ] **1.7** 编写 `start.sh` 一键启动脚本，所有参数从 `.yaml` 读取
- [ ] **1.8** 创建 `nav2_params.yaml` 和 `slam_params.yaml` 配置文件

### 二、厂区走廊视觉识别 (15分)

- [ ] **2.1** 自行设计 5 个互不相同的初级灾害标识图案
- [ ] **2.2** 为 URDF 车辆模型添加 RGB 相机 Gazebo 插件
- [ ] **2.3** 创建 `vision_det` 包（Python），实现相机图像订阅
- [ ] **2.4** 实现目标检测：YOLO/ResNet 或 OpenCV 模板匹配
- [ ] **2.5** 终端打印识别结果：`[ID] class_name confidence`
- [ ] **2.6** 训练/标注数据集，确保与其他队伍不雷同

### 三、核心区入口清障交互 (20分)

- [ ] **3.1** 设计障碍物 Gazebo 模型（mass/inertia 支持物理碰撞）
- [ ] **3.2** 实现 LiDAR 障碍物检测节点
- [ ] **3.3** 编写推障机动控制节点：对准 → 加速 → 发力推开 → 减速驶入
- [ ] **3.4** 实现导航模式与推障模式的平滑切换
- [ ] **3.5** 反复测试避免：物理穿模、卡死、障碍物未完全推开
- [ ] **3.6** 推障完成后自动恢复导航模式

### 四、核心区标定与视觉识别 (20分)

- [ ] **4.1** 设计核心灾害标识图案，嵌套 AprilTag 二维码阵列
- [ ] **4.2** 创建 `apriltag_det` 包，实现 AprilTag 检测（`apriltag_ros` 或自行实现）
- [ ] **4.3** 发布 `visualization_msgs/Marker` 到 RViz 实时标定
- [ ] **4.4** TF 变换：camera → tag → map，确保 Marker 与实际布局吻合不漂移
- [ ] **4.5** 计算并打印目标在 map 坐标系的相对坐标

### 五、方案创新性 (15分)

- [ ] **5.1** 优化 Gazebo 环境逼真度：纹理贴图、光照、粒子特效
- [ ] **5.2** 设计精妙的推障机构（如机械铲斗/推板，而非简单车头碰撞）
- [ ] **5.3** Nav2 自定义插件（规划器/控制器），体现算法原创性
- [ ] **5.4** 视觉识别采用自训练模型而非直接套用开源权重
- [ ] **5.5** 关键算法模块添加详细注释说明设计思路

### 六、技术文档质量 (15分)

- [ ] **6.1** 编写《技术实施报告》PDF
- [ ] **6.2** 绘制系统架构图
- [ ] **6.3** 绘制 TF 坐标变换树
- [ ] **6.4** 绘制核心节点流程图（节点、话题、服务、Action 拓扑）
- [ ] **6.5** 录制全流程一刀不剪演示视频（左右分屏，MP4 ≤5min ≤100M）

### 七、规划流水线完善（现有代码框架）

- [ ] **7.1** `planning_process.cpp` — 总流程状态机串联所有阶段
- [ ] **7.2** `global_path_server.cpp` — Normal/A* 全局路径算法
- [ ] **7.3** `reference_line_creator.cpp` + `smoother.cpp` — 参考线裁剪+OSQP平滑
- [ ] **7.4** `local_path_planner/smoother.cpp` — 多项式路径规划+平滑
- [ ] **7.5** `local_speeds_planner/smoother.cpp` — 速度曲线规划+平滑
- [ ] **7.6** `decision_center.cpp` — 障碍物避障决策
- [ ] **7.7** `local_trajectory_combiner.cpp` — 路径+速度轨迹合成
- [ ] **7.8** `car_move_cmd.cpp` / `obs_move_cmd.cpp` — TF运动指令
- [ ] **7.9** `pnc_map_server.cpp` — 地图服务完整实现
- [ ] **7.10** `planning_launch.py` — launch 文件整合所有节点
- [ ] **7.11** `data_plot.py` — matplotlib 数据可视化订阅
- [ ] **7.12** `config_reader.cpp:63` — 修复 `type` 字段读取为 `road_length`

### 实施优先级

```
第一优先 (跑通闭环):  1.1~1.8, 7.10
第二优先 (感知能力):  2.1~2.6, 4.1~4.2
第三优先 (核心动作):  3.1~3.6
第四优先 (标定定位):  4.3~4.5
第五优先 (规划算法):  7.1~7.12
第六优先 (展示包装):  5.1~5.5, 6.1~6.5
```

---

## 技术栈

| 层级 | 技术 | 用途 |
|------|------|------|
| 框架 | ROS2 Humble (rclcpp/rclpy) | 节点通信、服务、Action、TF2 |
| 仿真 | Gazebo | 物理引擎、传感器仿真、环境建模 |
| SLAM | slam_toolbox / cartographer | 2D 占据栅格地图构建 |
| 导航 | Nav2 (Behavior Tree) | 全局/局部路径规划、代价地图、恢复行为 |
| 视觉 | OpenCV + PyTorch/TensorFlow | 灾害标识目标检测与分类 |
| 标定 | apriltag_ros (AprilTag) | QR 阵列检测、相对位姿估计 |
| 规划 | 自研 PNC 流水线 | 参考线平滑(OSQP)、多项式路径、速度曲线、轨迹合成 |
| 优化 | Eigen3 + OsqpEigen | 线性代数 + 二次规划求解 |
| 可视化 | RViz2 + matplotlib | 3D 场景 + 数据曲线 |
| 建模 | URDF/Xacro + SDF | 车辆模型 + Gazebo 世界描述 |
| 配置 | yaml-cpp | 参数文件解析 |

---

## License

Apache-2.0 — 详见 [LICENSE](LICENSE)
