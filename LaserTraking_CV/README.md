# LaserTracking_CV
## :book: 简介
该项目基于 Jetsonnano 搭载USB摄像头利用opencv库图像处理实现激光追踪的算法。
## :rocket: 操作 
### 摄像头配置
1. 配置命令
* 查看控制选项：
``` v4l2-ctl -d /dev/video0 -l ```
* 设置配置参数（设置白平衡为例）：
```v4l2-ctl -d /dev/video0 --set-ctrl=white_balance_auto=0 --set-ctrl=white_balance_temperature=XXXX```
2.  配置参考
```c++
jetson@yahboom:~/develop/smart-car/LaserTraking_CV/RGB_point_drawing/build3$ v4l2-ctl -d /dev/video0 -l
                     brightness 0x00980900 (int)    : min=-64 max=64 step=1 default=0 value=-64
                       contrast 0x00980901 (int)    : min=0 max=100 step=1 default=32 value=20
                     saturation 0x00980902 (int)    : min=0 max=100 step=1 default=64 value=64
                            hue 0x00980903 (int)    : min=-180 max=180 step=1 default=0 value=0
 white_balance_temperature_auto 0x0098090c (bool)   : default=1 value=0
                          gamma 0x00980910 (int)    : min=100 max=500 step=1 default=300 value=300
                           gain 0x00980913 (int)    : min=1 max=128 step=1 default=64 value=64
           power_line_frequency 0x00980918 (menu)   : min=0 max=2 default=1 value=1
      white_balance_temperature 0x0098091a (int)    : min=2800 max=6500 step=10 default=4600 value=4000
                      sharpness 0x0098091b (int)    : min=0 max=100 step=1 default=50 value=50
         backlight_compensation 0x0098091c (int)    : min=0 max=2 step=1 default=0 value=0
                  exposure_auto 0x009a0901 (menu)   : min=0 max=3 default=3 value=1
              exposure_absolute 0x009a0902 (int)    : min=50 max=10000 step=1 default=166 value=50
         exposure_auto_priority 0x009a0903 (bool)   : default=0 value=1
```


### 具体命令
1. 下载后进入新建文件夹名为buildx
``` linux
mkdir build1 
cd build1
```
2. 生成构建文件
```linux
cmake ..
```
3. 编译.cpp文件
```linux
make
```
4. 运行可执行文件
```linux
./red_point_drawing
```

