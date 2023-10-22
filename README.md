# stm32_study

### 小知识

###### VCC、VDD、VSS、VEE、GND

* VDD 是 CMOS 芯片电源，VSS 是 TTl 芯片电源

>* MOS 管的三个极分别为 G、D、S ，VDD 是正，VSS 是负
>
>* 三极管的三个极分别为 B、C、E，VCC 是正，VEE 是负

###### 串口各个引脚定义

> RS-232C接口定义(DB9)
> 引脚 定义 符号
> 1 载波检测 DCD（Data Carrier Detect）
> 2 接收数据 RXD（Received Data）
> 3 发送数据 TXD（Transmit Data）
> 4 数据终端准备好 DTR（Data Terminal Ready）
> 5 信号地 SG（Signal Ground）
> 6 数据准备好 DSR（Data Set Ready）
> 7 请求发送 RTS（Request To Send）
> 8 清除发送 CTS（Clear To Send）
> 9 振铃提示 RI（Ring Indicator）

### 系统结构

![image-20231006223350151](https://p.ipic.vip/g699ao.png)

### GPIO

![image-20231006223638190](https://p.ipic.vip/c3m6ke.png)

（1）GPIO_Mode_AIN 模拟输入

（2）GPIO_Mode_IN_FLOATING 浮空输入

（3）GPIO_Mode_IPD 下拉输入

（4）GPIO_Mode_IPU 上拉输入

（5）GPIO_Mode_Out_OD 开漏输出

（6）GPIO_Mode_Out_PP 推挽输出

（7）GPIO_Mode_AF_OD 复用开漏输出

（8）GPIO_Mode_AF_PP 复用推挽输出
------------------------------------------------
版权声明：本文为CSDN博主「strongerHuang」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/ybhuangfugui/article/details/52953533

### HC-05 蓝牙模块

