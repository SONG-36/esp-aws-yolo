### 2.2 核心功能代码设计

#### 2.2.1 任务初始化与调度核心

（1）设计目标

​	该模块负责系统启动后最早阶段的任务初始化与调度。其目标是：

​		保证各业务任务按照配置顺序自动注册并调度；

​		将任务创建逻辑从应用层剥离，统一封装；

​		提供灵活的任务扩展机制，使得新任务接入无需手动修改调度逻辑。

（2）核心结构体定义

​	在 `task_registry.h` 中定义统一的任务配置结构体：

```c
// middleware/core/task_registry.h

typedef void (*task_entry_fn_t)(void*);

typedef struct {
    const char*      name;       // 任务名称
    task_entry_fn_t  entry;      // 任务入口函数
    uint32_t         stack;      // 栈大小
    UBaseType_t      priority;   // 优先级
} task_config_t;
```

（3）任务数组登记机制

```c
// app/task_config.c
const task_config_t app_tasks[] = {
    { .name = "CameraTask",  .entry = camera_task,  .stack = 4096, .priority = 5 },
    { .name = "UploadTask",  .entry = upload_task,  .stack = 4096, .priority = 4 },
    // 可在此继续添加任务
};

const size_t app_tasks_count = sizeof(app_tasks) / sizeof(app_tasks[0]);
```

（4）统一调度入口

```c
// middleware/core/system_init.c
extern const task_config_t app_tasks[];
extern const size_t app_tasks_count;

void system_init(void) {
    for (size_t i = 0; i < app_tasks_count; ++i) {
        BaseType_t result = xTaskCreate(
            app_tasks[i].entry,
            app_tasks[i].name,
            app_tasks[i].stack,
            NULL,  // 可传参数
            app_tasks[i].priority,
            NULL   // 不保存 task handle
        );

        if (result != pdPASS) {
            ESP_LOGE("SYS_INIT", "Failed to create task: %s", app_tasks[i].name);
        } else {
            ESP_LOGI("SYS_INIT", "Task created: %s", app_tasks[i].name);
        }
    }
}
```



各 HAL 模块向上仅暴露统一函数，例如：

```c
// camera_hal.h
esp_err_t camera_init(void);
esp_err_t camera_capture_frame(uint8_t **buf, size_t *len);

```



HAL 接口函数内部通过预编译宏或配置文件自动选择对应平台实现，无需业务层干预。例如：

```c
#ifdef CONFIG_CAMERA_OV2640
#include "drivers/esp32-s3/ov2640/ov2640_driver.h"
#endif

esp_err_t camera_init(void) {
    return ov2640_init();
}
```

#### 2.2.3  App → Middleware 的任务注册机制

​	为提高任务创建的统一性与系统扩展能力，系统设计采用 **任务集中注册 + 中间件统一调度** 的机制，App 层无需直接调用 `xTaskCreate` 创建任务，只需登记任务配置结构体，中间件层在启动阶段自动完成任务创建与调度初始化。

​	App 层任务登记示例：

```c
const task_config_t app_tasks[] = {
    { "CameraTask", camera_task, 4096, 5 },
    { "UploadTask", upload_task, 4096, 4 },
    { "CmdParseTask", cmd_parse_task, 2048, 3 },
    { "AckTask", ack_task, 2048, 3 },
};

const size_t app_task_count = sizeof(app_tasks) / sizeof(app_tasks[0]);
```

#### 2.2.4 配置集中化与管理模块

​	在嵌入式系统中，不同任务的栈大小、优先级、日志等级、调试开关等配置项常常散落于各个模块，给维护带来困难。本项目采用 **配置集中化策略**，通过 `system_config.h` 等集中管理文件，统一定义系统关键参数，确保结构清晰、调优便利、版本控制有据可依。

（1）系统基础配置

| 宏名称              | 默认值         | 可选值/范围                                                  | 单位  | 说明                                                     |
| ------------------- | -------------- | ------------------------------------------------------------ | ----- | -------------------------------------------------------- |
| `SYSTEM_DEBUG_MODE` | `1`            | `0`: 关闭`1`: 开启                                           | -     | 系统是否处于调试模式。开启后启用调试日志等辅助信息。     |
| `SYSTEM_LOG_LEVEL`  | `ESP_LOG_INFO` | `ESP_LOG_NONE`, `ESP_LOG_ERROR`, `ESP_LOG_WARN`, `ESP_LOG_INFO`, `ESP_LOG_DEBUG`, `ESP_LOG_VERBOSE` | -     | 设置系统日志输出等级。建议发布版本设置为 WARN 或 ERROR。 |
| `MAX_TASK_COUNT`    | `16`           | 任意正整数（ < 32）                                          | count | 系统最多支持注册的任务数量。超出需修改任务数组大小。     |

（2）RTOS任务参数配置

| 宏名称                            | 默认值 (Byte) | 说明             |
| --------------------------------- | ------------- | ---------------- |
| `CAMERA_TASK_STACK_SIZE`          | 4096          | 摄像头采集任务栈 |
| `UPLOAD_TASK_STACK_SIZE`          | 4096          | 图像上传任务     |
| `CMD_PARSE_TASK_STACK_SIZE`       | 2048          | 命令解析任务     |
| `ACK_TASK_STACK_SIZE`             | 2048          | 指令反馈任务     |
| `MQTT_CLIENT_TASK_STACK_SIZE`     | 4096          | MQTT 客户端任务  |
| `LOGGER_TASK_STACK_SIZE`          | 2048          | 日志打印任务     |
| `MONITOR_TASK_STACK_SIZE`         | 2048          | 堆栈监控任务     |
| `CAMERA_BUFFER_TASK_STACK_SIZE`   | 4096          | 图像缓存中转任务 |
| `UPLOADER_BRIDGE_TASK_STACK_SIZE` | 4096          | 上传桥接任务     |
| `DISPATCHER_TASK_STACK_SIZE`      | 2048          | 指令分发协调任务 |





## 3 Driver层设计





ESP32-S3 设备为边缘数据进口节点，主要职责包括：

- 接受相机数据，实时捕捉图像
- 培成为 Base64 格式并打包为 JSON 数据
- 通过 MQTT over TLS 将数据发送至 AWS IoT Core

**（1）图像采集模块**

​	本模块负责通过连接的摄像头采集图像帧，是系统的视觉入口。其采集动作可由 BMI270 触发事件启动，也可由周期性定时器触发采集任务。图像采集完成后，将图像帧通过队列发送至上传任务。

<img src="https:////wsl.localhost/Ubuntu-22.04/home/andy/esp32code/ESP32-YOLOCloudTank/ESP32-AWS-YOLOMinitor/doc(chinese)/pic/图像采集模块.png" alt="系统架构图" style="zoom: 50%;" />

​	调用关系：被 BMI270 触发或者定期触发 → 启动采集 → 输出图像帧 → 提交至上传队列
​	数据格式：输出为 RGB565（预编码）或 JPEG 图像缓存

**结构体初步设计：**

```c
typedef enum {
    TRIGGER_IMU = 0,
    TRIGGER_TIMER = 1
} trigger_source_t;

typedef struct {
    uint8_t* image_buf;        // 图像缓冲区指针（JPEG 格式）
    size_t image_size;         // 图像大小（字节数）
    trigger_source_t source;   // 触发采集的来源类型
    uint32_t timestamp_ms;     // 采集时间戳（ms）
} image_frame_t;
```

​	结构体 `image_frame_t` 定义了图像采集模块输出图像帧的基本格式，包含图像缓存地址、大小、采集时间戳及触发来源。图像采集完成后，该结构体将作为任务间通信数据单元，通过消息队列或事件组传递给图像上传模块。

**模块函数接口初步设计：**

```c
//返回是否成功采集；采集成功后，out_frame 中填入有效数据。
bool capture_image(image_frame_t* out_frame);
```

**（2）图像上传模块**

​	本模块负责将接收到的图像帧转码为 JPEG（如有需要），并通过 HTTP POST 上传至 AWS 云服务器。上传完成后，模块等待接收服务器的响应信息，并将其转发给解析模块进行后续处理。

<img src="https:////wsl.localhost/Ubuntu-22.04/home/andy/esp32code/ESP32-YOLOCloudTank/ESP32-AWS-YOLOMinitor/doc(chinese)/pic/图像上传模块.png" alt="系统架构图" style="zoom: 50%;" />

​	调用关系：从图像采集队列接收 → 上传 → 接收 HTTP 响应 → 推送给 JSON 解析模块
​	网络协议：HTTP 1.1 + TCP/IP，POST 文件内容（Content-Type: image/jpeg）

**结构体初步设计：**

```c
//输入结构体：描述图像上传的基本输入，包括图像数据、目标服务器地址等
typedef struct {
    uint8_t* jpeg_data;        // 图像数据指针
    size_t jpeg_size;          // 图像字节数
    char server_url[128];      // 上传服务器 URL（含 IP 或域名）
    char auth_token[64];       // 鉴权用 token（可选）
    uint32_t timestamp_ms;     // 图像采集时间戳（便于日志记录）
} upload_image_request_t;

//输出结构体（上传响应）：描述 HTTP 上传完成后的响应情况
typedef struct {
    int http_status_code;      // HTTP 返回码，如 200、403 等
    bool success;              // 是否成功（状态码为 200~299）
    char response_body[256];   // 可选：返回的 JSON 内容或消息
    uint32_t elapsed_ms;       // 上传耗时（用于性能评估）
} upload_result_t;

//状态码枚举（可用于内部逻辑判断）：模块执行的宏观状态
typedef enum {
    UPLOAD_OK = 0,
    UPLOAD_FAIL_NETWORK = -1,
    UPLOAD_FAIL_HTTP = -2,
    UPLOAD_TIMEOUT = -3,
    UPLOAD_INVALID_PARAM = -4,
} upload_status_t;
```

**模块函数接口初步设计：**

```c
// 发起图像上传
upload_status_t upload_image(const upload_image_request_t* request, upload_result_t* result);
```

**（3）云端数据解析模块**

​	本模块负责接收来自图像上传模块或网络通信模块返回的 HTTP 响应内容（通常为 JSON 格式），并解析其中的识别结果，例如：目标类别（label）、位置信息（bounding box）、置信度（confidence）等。解析后的数据将用于控制指令下发（UART）或结果展示（LCD），也会被记录到日志模块中。

<img src="https:////wsl.localhost/Ubuntu-22.04/home/andy/esp32code/ESP32-YOLOCloudTank/ESP32-AWS-YOLOMinitor/doc(chinese)/pic/云端数据解析模块.png" alt="系统架构图" style="zoom: 60%;" />

**结构体初步设计：**

```c
//识别对象信息结构体
typedef struct {
    char label[32];       // 类别名称，如 "person", "car"
    int x_min, y_min;     // 目标区域左上角坐标
    int x_max, y_max;     // 目标区域右下角坐标
    float confidence;     // 识别置信度
} detection_result_t;

//整体识别结果结构体
#define MAX_DETECTIONS 10

typedef struct {
    detection_result_t results[MAX_DETECTIONS]; // 最大支持 N 个目标
    int count;                                  // 实际检测目标数
    bool valid;                                 // 是否有效结果
    uint32_t request_timestamp;                 // 对应上传图像的时间戳
} parsed_response_t;
```

**模块函数接口初步设计：**

```c
bool parse_aws_response_json(const char* json_str, parsed_response_t* out_result);
//参数说明：json_str: HTTP 响应中的 JSON 字符串内容
//out_result: 结构化输出结果，供系统使用返回值表示解析是否成功。
```

**（4）在线语音识别模块**



**结构体初步设计：**

```c
//语音识别请求结构体
typedef struct {
    int sample_rate;         // 采样率（Hz），如 16000
    int duration_ms;         // 录音时长（ms）
    uint8_t* audio_buffer;   // 音频数据指针（PCM）
    size_t buffer_size;      // 音频数据字节数
    char api_key[64];        // Google API 密钥
} asr_request_t;

//语音识别结果结构体
typedef struct {
    char transcript[128];    // 识别文本结果（如 "move forward"）
    float confidence;        // 置信度（如 0.92）
    bool success;            // 是否识别成功
} asr_result_t;

//状态码
typedef enum {
    ASR_OK = 0,
    ASR_FAIL_NETWORK = -1,
    ASR_FAIL_API_KEY = -2,
    ASR_NO_SPEECH = -3,
    ASR_PARSE_ERROR = -4
} asr_status_t;

```

**模块函数接口初步设计：**

```c
bool google_asr_process(const asr_request_t* request, asr_result_t* result);
```

**（5）UART通信模块（S3）**

​	

**结构体初步设计：**

```c
//UART 指令数据结构（传入）
typedef struct {
    char command_str[64];   // 发送给下位机的命令字符串
    uint8_t priority;       // 命令优先级（可用于队列排序）
    uint32_t timestamp_ms;  // 发出时间戳
} uart_command_t;

//串口发送状态码
typedef enum {
    UART_TX_OK = 0,
    UART_TX_BUSY = -1,
    UART_TX_FAIL = -2,
    UART_TX_TIMEOUT = -3
} uart_status_t;

```

**模块函数接口初步设计：**

```c
//实现内容：格式封装 → 串口发送 → 状态返回
uart_status_t uart_send_command(const uart_command_t* cmd);
```

**（6）LCD显示模块**

​	

**结构体初步设计：**

```c
//显示事件枚举
typedef enum {
    LCD_EVENT_IDLE = 0,
    LCD_EVENT_PERSON_DETECTED,
    LCD_EVENT_COMMAND_LEFT,
    LCD_EVENT_COMMAND_STOP,
    LCD_EVENT_ERROR,
    LCD_EVENT_DEBUG_PRINT,  // 调试模式下文本输出
} lcd_event_t;

//显示任务消息结构体
typedef struct {
    lcd_event_t event_type;        // 显示类型
    char message[64];              // 可选文字信息（调试或命令提示）
    uint32_t timestamp_ms;         // 系统时间戳
} lcd_display_msg_t;

```

**模块函数接口初步设计：**

```c
//根据 msg->event_type 决定是显示图片（正常模式）还是打印文字（调试模式）
void lcd_display_event(const lcd_display_msg_t* msg);

//模式切换接口
typedef enum { LCD_MODE_NORMAL, LCD_MODE_DEBUG } lcd_mode_t;
void lcd_set_mode(lcd_mode_t mode);
```

**（7）日志记录模块**

​	

**结构体初步设计：**

```c
//日志等级枚举
typedef enum {
    LOG_LEVEL_ERROR = 0,
    LOG_LEVEL_WARN,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG
} log_level_t;

//日志记录结构体
typedef struct {
    log_level_t level;
    char module[16];
    char message[128];
    uint32_t timestamp_ms;
} log_entry_t;
```

**模块函数接口初步设计：**

```c
void log_print(log_level_t level, const char* module, const char* fmt, ...);

//快捷调用宏封装
#define LOGE(mod, fmt, ...) log_print(LOG_LEVEL_ERROR, mod, fmt, ##__VA_ARGS__)
#define LOGW(mod, fmt, ...) log_print(LOG_LEVEL_WARN,  mod, fmt, ##__VA_ARGS__)
#define LOGI(mod, fmt, ...) log_print(LOG_LEVEL_INFO,  mod, fmt, ##__VA_ARGS__)
#define LOGD(mod, fmt, ...) log_print(LOG_LEVEL_DEBUG, mod, fmt, ##__VA_ARGS__)

//模式切换控制接口
typedef enum {
    LOG_OUTPUT_SERIAL,
    LOG_OUTPUT_LCD,
    LOG_OUTPUT_STORAGE
} log_output_mode_t;

void log_set_output_mode(log_output_mode_t mode);
void log_set_level(log_level_t level);

```

**（8）BMI270触发模块**

​	

**结构体初步设计：**

```c
//触发事件类型枚举
typedef enum {
    TRIG_NONE = 0,
    TRIG_MOTION,
    TRIG_SHAKE,
    TRIG_ORIENTATION_CHANGE
} trigger_event_t;

//触发事件数据结构
typedef struct {
    trigger_event_t type;       // 触发类型
    uint32_t timestamp_ms;      // 事件时间戳
    int16_t accel_x, accel_y, accel_z;  // 原始加速度数据（可选）
} imu_trigger_event_t;
```

**模块函数接口初步设计：**

```c
bool imu_init(void);  // 初始化 BMI270 驱动与中断
bool imu_get_event(imu_trigger_event_t* out_event); // 获取最新触发事件
```





#### 1.3.2 系统内存计算

| 模块         | 占用内存             | 说明                                           |
| ------------ | -------------------- | ---------------------------------------------- |
| 图像缓冲     | ~128 KB（2帧双缓冲） | 驱动层采集原始 YUV/RGB 数据，保存在 PSRAM      |
| 上传缓冲     | ~64 KB               | 与上传任务绑定，支持失败重传机制               |
| 任务栈总占用 | ~18 KB（任务栈）     | 5个主要任务，栈空间分配如上                    |
| JSON 缓冲区  | ~4 KB                | 用于临时存储推理返回值，result_parse_task 使用 |
| 控制命令缓存 | ~1 KB                | motor_control_tx_task 用于 UART ACK 管理       |
| 日志缓存     | ~4 KB                | 仅调试版本开启，可关闭以节省资源               |

#### 1.3.3 任务栈监控机制

​	为保障系统稳定性，所有任务启动后将周期性检测栈水位，避免溢出风险：

```c
// FreeRTOS 任务中使用
UBaseType_t stack_left = uxTaskGetStackHighWaterMark(NULL);
if (stack_left < 100) {
    ESP_LOGW("STACK", "Task %s nearing overflow: %d words left", pcTaskGetName(NULL), stack_left);
}
```

​	在调试版本中，配合 `vTaskList()` 或 ESP-IDF 的 CLI 工具 `esp_timer_dump()` 可输出所有任务运行状态、CPU 占用、剩余堆栈空间等。

#### 1.3.4 异常处理与资源保护

| 异常情形          | 处理机制                                                     |
| ----------------- | ------------------------------------------------------------ |
| 栈溢出            | 注册 FreeRTOS 钩子函数 `vApplicationStackOverflowHook()` 打印任务名并重启系统 |
| 上传失败          | 记录日志 → 指定次数后丢弃 / 重传                             |
| JSON 异常         | result_parse_task 记录异常 → 进入 fallback 状态              |
| 控制命令 ACK 超时 | 自动触发 resend，3 次失败后上报 Log                          |

