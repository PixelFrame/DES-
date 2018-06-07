# DES++
> by Pixel Frame

## 模块简介
1. DES_Main 加解密主程序
* encrypt 
    - 输入 (明文 : QWORD, 密钥 : QWORD)
    - 输出 (密文 : QWORD)
* encrypt*** (Hex, Bin, ANSI, Unicode, File)
    - 输入 (明文 : CString, 密钥 : QWORD, 加密模式 : int, 初始向量 : QWORD)
    - 输出 (密文 : std::vector<QWORD>)
* decrypt
    - 输入 (密文 : QWORD, 密钥 : QWORD)
    - 输出 (明文 : QWORD)
* decryptHex
    - 输入 (密文 : CString, 密钥 : QWORD, 加密模式 : int, 初始向量 : QWORD)
    - 输出 (明文 : std::vector<QWORD>)
    
2. DES_LookUp 查找表
3. DES_Convert 将各种格式字符串转换为QWORD
4. DES_Base64 BASE64加解密
5. DES++Dlg MFC窗口实现
6. CEditRes 窗口输入限制

## 注意事项
* 本工程为Unicode编码工程，因此对CString做了多次MultiByte转换，同时DES_Main输入的CString也应是CStringW
* DES++Dlg使用了大量冗余以及复杂的逻辑，不建议参考
* BASE64部分并非原创
* 窗口使用了Inziu Iosevka字体
