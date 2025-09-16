# b3063 · 项目导航

本仓库提供一个简洁优美的静态导航页与资源目录结构，便于快速浏览项目内容。

## 快速访问
- 主页（静态导航页）：[`html/index.html`](html/index.html)

## 目录导航
- 🧩 **嵌入式代码 embedded_code**：存放嵌入式代码。
  - 路径：[`embedded_code/`](embedded_code/)
- 📈 **图形资源 grah**：与图形、图表或可视化相关的资源集合。
  - 路径：[`grah/`](grah/)
- 🧭 **HTML 页面 html**：网页文件与导航页所在目录。
  - 路径：[`html/`](html/)
- 🖼️ **图片资源 pic**：项目图片、截图与图标等静态资源。
  - 路径：[`pic/`](pic/)
- 🎬 **视频资源 video**：演示与素材视频文件。
  - 路径：[`video/`](video/)

## 预览方式
由于浏览器的本地文件安全限制，推荐使用本地静态服务器预览：
- 如果你已安装任意静态服务器（如 VS Code Live Server、`python -m http.server`、`node` 的 `http-server` 等），请将站点根指向 `html/`，然后访问：
  - `http://127.0.0.1:<port>/`（首页会加载 `index.html`）
- 或直接在文件浏览器中双击打开 `html/index.html`（部分浏览器对本地资源加载有限制，若样式或脚本未生效，请改用本地服务器）。

## 说明
- 本 README 由 `html/index.html` 的导航内容转换而来，以便在 GitHub 上直观浏览项目结构与入口链接。 