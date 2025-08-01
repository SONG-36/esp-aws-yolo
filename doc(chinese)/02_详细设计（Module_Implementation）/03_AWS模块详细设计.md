# AWS模块详细设计

## 1 模块概述

​	本模块负责 ESP32-S3 边缘设备与 AWS 云端平台之间的数据交互与智能处理功能，是系统中实现“图像采集 → 云端识别 → 状态记录与展示”流程的核心组成部分。通过 AWS IoT Core、Lambda 函数、DynamoDB、S3 以及可选的 Web 展示界面，构建完整的边缘智能推理与云端协同架构。

### 1.1 功能描述

AWS 云端模块负责接收来自边缘设备（如 ESP32-S3）上传的图像数据，并完成如下处理流程：

- 通过 AWS IoT Core 接收 ESP32-S3 端通过 MQTT 协议上传的图像消息；
- 由 AWS Lambda 函数触发并解析图像内容，调用图像识别模型（如 YOLO）进行推理；
- 将识别结果存入 DynamoDB，用于后续状态查询与报警逻辑；
- （可选）将原始图像上传至 S3，并生成预签名访问链接供 Web 前端展示使用；
- （可选）在 Web 界面中展示识别状态与图像内容，供用户浏览与远程管理。

本模块通过服务编排方式完成“云端识别与反馈”的逻辑处理，是整个系统中云端智能判断的核心。

### 1.2 模块职责

本模块在系统中的职责包括但不限于：

- 建立与边缘设备的数据接收机制；
- 对上传的图像数据进行分类识别与状态判断；
- 存储每一次识别的状态、图像链接与事件标签；
- 为前端可视化界面提供数据支持（通过 API 或 Web SDK）；
- 对云端服务资源（如 DynamoDB、S3）进行统一封装与访问管理；
- 实现可配置的模型调用策略与报警逻辑。

### 1.3 模块的交互关系

| 对象模块         | 交互方向 | 接口/协议       | 内容说明                   |
| ---------------- | -------- | --------------- | -------------------------- |
| ESP32-S3 模块    | 输入     | MQTT / TLS      | 上传图像、设备状态         |
| 云端推理模型服务 | 内部调用 | Lambda 内部逻辑 | 图像识别、标签输出         |
| DynamoDB 模块    | 输出     | SDK / REST API  | 存储识别结果与元数据       |
| S3 模块（可选）  | 输出     | SDK / REST API  | 上传原始图像，生成访问链接 |
| Web 可视化模块   | 输出     | HTTP / SDK      | 展示识别结果与图像数据     |

### 1.4 模块内部任务列表与职责分工

| 子模块/任务名称        | 描述                                     | 所属平台     |
| ---------------------- | ---------------------------------------- | ------------ |
| MQTT 消息接收任务      | IoT Core 规则接收来自设备的图像消息      | AWS IoT Core |
| 图像识别与推理任务     | 解析消息并调用识别模型                   | AWS Lambda   |
| 识别结果状态判断任务   | 根据模型输出进行主人识别、异常检测等判断 | AWS Lambda   |
| 状态记录与图像存储任务 | 将结构化识别结果存入 DynamoDB / S3       | AWS SDK      |
| 可视化数据接口服务     | 提供前端所需数据结构与访问权限控制       | Web 端 / API |

## 2 云服务架构总览

本模块基于 AWS 云平台构建数据处理流程，主要服务包括：

- **AWS IoT Core**：用于接收 ESP32-S3 边缘设备上传的图像数据；
- **AWS Lambda**：用作事件处理核心，完成图像解码、识别模型调用与状态判断；
- **AWS DynamoDB**：用于存储识别结果、图像状态与设备信息；
- **Web 前端页面（可选）**：用于状态展示、图像回看与远程管理。

### 2.1 云服务整体架构图



### 2.2 服务职责说明

| 服务组件         | 所属平台 | 主要职责                                     |
| ---------------- | -------- | -------------------------------------------- |
| ESP32-S3         | 设备端   | 拍摄图像 → JPEG → Base64 编码 → MQTT 上传    |
| AWS IoT Core     | 云平台   | 接收 MQTT 消息；设置规则触发 Lambda          |
| AWS Lambda       | 云平台   | 解析消息 → 推理识别 → 状态判断 → 写入数据库  |
| DynamoDB         | 云平台   | 存储结构化识别结果（设备ID、时间戳、标签等） |
| S3（可选）       | 云平台   | 存储原始图像文件，提供预签名链接             |
| Web 界面（可选） | 用户端   | 可视化展示识别状态、图像内容、报警标记等     |

