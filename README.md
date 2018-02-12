# 聊天室
## 一、项目综述

实现Linux环境下的简单聊天室，基于c/s （客户端client/服务端server）模型，实现不同客户端之间通信。

## 二、开发环境

1. Linux开发环境，所有的API基于LinuxC（所有操作尽量使用LinuxC系统调用）
2. 编译器 ： gcc
3. 编辑器 ： Vim

## 三、安装说明

```
git clone https://github.com/SequinYF/CHATROOM_C
cd CHATROOM_C
make
run
```

## 四、代码实现功能

### 基础功能
1. 登录、注册
2. 好友管理（查看、添加、删除）
3. 聊天管理（私聊、群聊）
4. 聊天记录功能，日志功能（操作日志、错误日志)。

### 拓展技术
1. 使用多路复用技术（select/epoll）
2. 文件的传输、离线消息
3. 数据加密（密码、消息）
