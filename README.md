# stm32f103硬件课设
这是一个基于stm32f103单片机的硬件课设项目，主题为智能家居，主要包括传感器模块、显示器模块、语音识别模块、风扇模块、灯光模块等.
- 通过传感器读取当前环境的温度、湿度、光强等等
- 通过显示屏显示当前的环境参数
- 通过语音识别模块识别语音 转化成对应的命令
  - "开窗"指令 控制系统拉窗帘(开)
  - "关窗"指令 控制系统拉窗帘(关)
  - "开灯"指令 控制系统打开LED灯
  - "关灯"指令 控制系统关闭LED灯
  - "开启风扇"指令 控制系统开启风扇
  - "关闭风扇"指令 控制系统关闭风扇
- 通过蜂鸣器在环境温度过高时进行报警，并开启风扇进行降温，如果此时光强超过阈值，则同时会拉窗帘(关)。