#chat

基本框架图

<img width="853" height="827" alt="屏幕截图 2025-09-09 200007" src="https://github.com/user-attachments/assets/de191ac8-71ad-4cfb-a25f-e64350914345" />

基本流程
GateServer收到客户端的登录请求会先检验账号密码，成功就会向StatusServer请求目标服务器ChatServer的ip&port
客户端事件请求一般直接发送请求至ChatServer，如果请求存在双方且对方在当前ChatServer就使用本地调用，如果不在当前ChatServer就使用rpc远程调用
客户端与服务端使用json通信
服务端之间通过rpc通信

GateServer   专注接入和转发，高性能处理连接。
StatusServer 用来保持多个ChatServer的负载均衡，这里简单的通过人数来均衡
ChatServer   处理客户端的交互请求

运行实例

注册
<img width="300" height="500" alt="屏幕截图 2025-09-09 202603" src="https://github.com/user-attachments/assets/6a784b17-c046-4df1-ad2c-7d1eeae5741a" />
<img width="300" height="500" alt="屏幕截图 2025-09-09 202609" src="https://github.com/user-attachments/assets/1cb307cf-1461-4e42-9f12-45ad1873f498" />
<img width="300" height="500" alt="屏幕截图 2025-09-09 202716" src="https://github.com/user-attachments/assets/cfd06649-eadd-4b5c-b64d-ea88a4f7bcc8" />
通过QTimer实现登陆成功跳转

添加好友
<img width="600" height="500" alt="屏幕截图 2025-09-09 203910" src="https://github.com/user-attachments/assets/9c145651-5f78-4c87-a63e-bd11edbf9a8b" />
<img width="600" height="500" alt="屏幕截图 2025-09-09 204016" src="https://github.com/user-attachments/assets/4ce605c3-f618-40c9-963b-a48ce293519e" />

聊天
<img width="600" height="700" alt="屏幕截图 2025-09-09 204116" src="https://github.com/user-attachments/assets/e64019d4-2fde-486f-b772-55a08700c62a" />
