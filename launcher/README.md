# 本地静态服务器使用说明

此目录提供在 Windows 上快速本地预览 `html/` 网站的脚本。

## 文件说明
- `serve.ps1`：PowerShell 版静态服务器（基于 .NET HttpListener）。
- `serve.bat`：一键启动脚本，方便双击运行。

## 启动方式（一）双击批处理
1. 直接双击 `serve.bat`。
2. 默认在 `http://127.0.0.1:5500/` 提供服务，根目录为项目 `html/`。

## 启动方式（二）命令行
- 使用默认端口：
  ```bat
  test\serve.bat
  ```
- 指定端口（例如 8080）：
  ```bat
  test\serve.bat 8080
  ```
- 直接用 PowerShell 运行并指定端口与根目录：
  ```powershell
  powershell -NoProfile -ExecutionPolicy Bypass -File test\serve.ps1 -Port 8080 -Root ..\html
  ```

## 使用说明
- 访问地址：`http://127.0.0.1:端口/`（默认 5500）。
- 目录索引：若访问目录路径（如 `/` 或以 `/` 结尾），脚本会自动返回该目录下的 `index.html`。
- 缓存：默认设置为不缓存，便于开发调试。

## 常见问题
- 端口占用：若 5500 被占用，改用 `test\serve.bat 5501` 或其他端口。
- 执行策略限制：若 PowerShell 提示执行策略限制，使用批处理启动或在命令行加 `-ExecutionPolicy Bypass` 参数。
- 权限：如仍无法启动，请尝试以管理员身份运行终端。 