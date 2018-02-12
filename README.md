# 聊天室

实现Linux环境下的简单聊天室，基于c/s （客户端client/服务端server）模型，实现不同客户端之间通信。

| Author | Sequin小红九|
| :----------: | -----------|
| Email   | SequinYF@xiyoulinux.org   |
|[![csdn-logo]][csdn]|http://blog.csdn.net/sequin_yf|


## 开发环境

Linux开发环境，所有的API基于LinuxC（所有操作尽量使用LinuxC系统调用）

| 编译器 | gcc|
| ---------- | -----------|
| 编辑器   | Vim  |

## 安装说明

```
$ git clone https://github.com/SequinYF/CHATROOM_C
$ cd CHATROOM_C
$ make
```

## 实现功能

### 基础功能
1. 登录、注册
2. 好友管理（查看、添加、删除）
3. 聊天管理（私聊、群聊）
4. 聊天记录功能，日志功能（操作日志、错误日志)。
5. 消息提醒（好友上线、消息）

### 拓展技术
- 使用多路复用技术（select）
- 文件的传输、离线消息
- 数据加密（密码、消息）
