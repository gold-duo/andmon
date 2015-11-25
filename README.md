**libandmon** A Linux native program. 

**Function** process monitor([NativeSubprocess](https://github.com/droidwolf/NativeSubprocess "NativeSubprocess")
[ProcessWatcher](https://github.com/droidwolf/NativeSubprocess/blob/master/example/src/main/java/com/droidwolf/example/ProcessWatcher.java "ProcessWatcher")),
uninstall feedback([NativeSubprocess](https://github.com/droidwolf/NativeSubprocess "NativeSubprocess")
                  [UninstallWatcher](https://github.com/droidwolf/NativeSubprocess/blob/master/example/src/main/java/com/droidwolf/example/UninstallWatcher.java "UninstallWatcher"))

**How to use**

1. import libandmon aar .
   add repositories ***maven { url 'https://oss.sonatype.org/content/repositories/snapshots' }***
   
   add dependencies ***compile 'com.github.droidwolf:libandmon:0.0.1-SNAPSHOT'***
   
2. call libandmon.start menthod

--------
**libandmon** 一个你(niu)们(meng)喜欢的linux原生程序. 

**功能** 进程监控([NativeSubprocess](https://github.com/droidwolf/NativeSubprocess "NativeSubprocess")。
[ProcessWatcher](https://github.com/droidwolf/NativeSubprocess/blob/master/example/src/main/java/com/droidwolf/example/ProcessWatcher.java "ProcessWatcher")),
卸载反馈([NativeSubprocess](https://github.com/droidwolf/NativeSubprocess "NativeSubprocess")
                  [UninstallWatcher](https://github.com/droidwolf/NativeSubprocess/blob/master/example/src/main/java/com/droidwolf/example/UninstallWatcher.java "UninstallWatcher"))

***NativeSubprocess是对linux原生进程的抽象封装，当初的目的是方便android java层实现其他业务逻辑。
由于示例用WatchDog导致一些人经常来问相关问题，没有google GCD的日子苦逼了广大的程序员哩。
NatvieSubprocess示例已经是几年前的技术，这种牛虻技术始终会面临厂商的扼杀滴。***

当然你们研究出新的方案麻烦告诉我^_^，欢迎加 **技术Q群 122699113**

**使用方法**

1. 引入libandmon aar .
   添加repositories ***maven { url 'https://oss.sonatype.org/content/repositories/snapshots' }***
   
   添加dependencies ***compile 'com.github.droidwolf:libandmon:0.0.1-SNAPSHOT'***

2. 调用libandmon.start方法
--------
**Authors**

droidwolf [droidwolf2006@gmail.com](mailto:droidwolf2006@gmail.com "droidwolf2006@gmail.com")


**License**

[Apache License, Version 2.0](http://www.apache.org/licenses/LICENSE-2.0 "Apache License, Version 2.0")

# Android Support(Does not support custom ROM)
Android Version | Support
--------------- | --------
2.3-4.x            | Yes
5.X             | Yes
6.0             | No
