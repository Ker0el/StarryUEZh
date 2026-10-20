# 星空汉化（StarryUEZh）

给虚幻引擎编辑器做的中文显示插件。蓝图、材质、Niagara 的节点标题和引脚、细节面板、菜单都会显示中文，节点搜索支持中英文同时匹配。

适用于 **Unreal Engine 5.4、5.5、5.6、5.7、5.8（Win64 编辑器）**。插件是 Editor 模块，不会进入打包后的游戏。

每个引擎版本对应一个独立的压缩包（文件名里的 `UE5.x`），里面是同一个插件，只有 `Binaries\Win64` 下的 DLL 不同。装哪个版本就用哪个包，不能混用。预编译包在 GitHub 的 Releases 页面下载。

## 形态说明

这是标准的 UE Editor 插件。`Binaries\Win64\UnrealEditor-StarryUEZh.dll` 是 UE 插件的模块文件，由编辑器正常加载，和引擎自带插件一样；没有注入器，没有代理 DLL，不修改引擎目录，也不修改项目资产。

## 安装

把整个 `StarryUEZh` 文件夹放到项目的插件目录：

```
<你的项目>\Plugins\StarryUEZh
```

然后打开项目，在「编辑 > 插件」里启用 **Starry UE 汉化**，重启编辑器。

如果不想移动文件夹，也可以在 `.uproject` 里加一段（把路径换成插件实际所在目录）：

```json
"AdditionalPluginDirectories": [
  "D:/UnrealPlugins"
]
```

插件依赖引擎自带的 **Niagara** 插件，确保它是启用状态。

`Binaries\Win64\UnrealEditor-StarryUEZh.dll` 是编译好的版本，必须和引擎大版本对应（5.4 的包只能给 5.4 用）。换引擎版本或换平台时，用 `Source` 里的源码重新编译即可。

### 如果提示「模块缺失或引擎版本不同」

预编译插件除了 DLL，还需要同目录下的 `UnrealEditor.modules` 清单，zip 里已经带上了。确认这两个文件都在：

```
<工程>\Plugins\StarryUEZh\Binaries\Win64\UnrealEditor-StarryUEZh.dll
<工程>\Plugins\StarryUEZh\Binaries\Win64\UnrealEditor.modules
```

并且 `StarryUEZh.uplugin` 里是 `"Installed": true`。

如果仍然提示版本不同，说明工程用的引擎版本和这个包不对应。下载对应 `UE5.x` 的包，或者用 `Source` 里的源码针对你的引擎重新编译。

## 使用

- 默认中文显示。
- 工具栏上的 **星空汉化** 按钮（星星图标）：
  - **左键**：切换中文 / 英文
  - **右键**：重新加载词条、扫描词条、打开汉化面板
- 快捷键：`F1`。
- 材质编辑器、Niagara 编辑器的工具栏和 Window 菜单里也有同样的入口，蓝图编辑器在 Window 菜单里。
- 汉化面板：
  - 自动扫描并列出未翻译词条，可按分类查看、搜索、只看未翻译。
  - 手动编辑译文并保存，立即生效，不用重启编辑器。
  - 导出未翻译清单到 `Content/Dictionary/待翻译.json`，可以交给别的 AI 批量翻译。
  - 内置 DeepSeek 翻译：粘贴 API Key 后自动获取模型，选好模型点「开始翻译」即可；译文按批写入 `ai_zh.json`，格式不合格的条目会自动跳过。
  - API Key 保存在 Windows 凭据管理器，不写进配置文件。
- 词条改完不用重启编辑器：点右键菜单或面板里的 **重新加载词条** 即可生效。
- 控制台命令：
  - `StarryZh.Status` 查看状态
  - `StarryZh.On` 中文
  - `StarryZh.Off` 英文
  - `StarryZh.Reload` 重新加载词条
  - `StarryZh.ExportDictionary` 导出当前词条
  - `StarryZh.Harvest` 扫描词条
  - `StarryZh.DumpGaps` 导出未命中清单

## 覆盖范围

- 蓝图：节点标题、引脚名、函数/事件/变量节点、调色板分类
- 材质：材质节点标题、引脚名、材质函数、节点搜索；305 个材质表达式的标题、359 个表达式类名、1700 个材质函数（含 MaterialX / MX_ 节点）都已补全，`Constant3Vector (0,0,0)`、`Texture Sample 'Tex'` 这类动态后缀会保留原值
- Niagara：节点标题、参数、Particle State / Emitter State / System State、调色板搜索；267 个模块、290 个动态输入的名称已补全（`Drag`、`SpawnRate` 这类模块名和参数名都有中文）
- 细节面板：属性名、分类、枚举值、属性提示和函数提示
- 编辑器菜单、工具栏、提示文字、富文本提示框
- 节点搜索：中文和英文都能搜
- 第三方插件：会读取插件自己的 `Content/Localization`；也会扫描项目和已启用插件里的资产描述、类名、属性提示，未收录的字符串可以在汉化面板里查看、手动补充或交给内置 AI 翻译

## 原理

- 中文模式：加载引擎自带的 `zh-Hans` 本地化资源，再在 Slate 的纯文本显示出口替换硬编码的英文（材质节点标题等）。
- 英文模式：加载引擎自带的 `en` 本地化资源，显示层替换关闭。
- 不修改引擎源码，不修改资产名和资产路径，不写 Blueprint/材质资产，不参与打包。
- 资产名和变量名有保护：AssetRegistry 里的资产名不会被当作界面词条翻译。例外是词库里明确收录的界面名称（材质函数、Niagara 模块名和分类名等），它们本身就是资产名，需要按词条显示中文。

## 已知限制

1. 只覆盖 Slate 显示层的文字。画布直绘的文字（少数自绘 HUD）暂时保持原样。
2. 切换语言后，**已经打开的材料/蓝图资产编辑器建议重新打开一次**。部分节点标签在打开时会把当前语言写死，重开后才会按新语言重建。
3. 极少数和界面词条同名的用户变量名可能被显示层替换（只影响显示，不影响数据）。
4. 第三方插件的专有名词如果没进词库，会保持英文。可以把自己的词条加到 `Content/Dictionary/manual_zh.json`，或者用汉化面板的内置 AI 翻译。
5. 5.4 到 5.8 都有对应编译产物，五个版本都做过模块加载验证；5.8 另外做了界面验收。更老的版本（5.3 及以下）没有适配，函数导出名可能不同，需要按对应版本调整 `StarryHooks.cpp` 里的修饰名。

## 目录

```
StarryUEZh.uplugin
Binaries\Win64\UnrealEditor-StarryUEZh.dll     预编译模块（Release 里按引擎版本提供）
Binaries\Win64\UnrealEditor.modules            模块清单
Config\FilterPlugin.ini
Content\Dictionary\zh_cn.json                  官方 en/zh-Hans 对照词库
Content\Dictionary\zh_cn_extra.json            扩展词条（界面分类、Niagara）
Content\Dictionary\zh_cn_material.json         材质节点基名补全
Content\Dictionary\zh_cn_niagara.json          Niagara 模块和参数
Content\Dictionary\zh_cn_niagara_engine.json   Niagara 引擎提供属性
Content\Dictionary\zh_cn_material_funcs.json   材质函数
Content\Dictionary\zh_cn_auto.json             自动扫描生成的长尾词条
Content\Dictionary\glossary.json               固定译法术语表
Content\Dictionary\notranslate.json            保持英文的品牌和缩写
Content\Dictionary\manual_zh.json              手工修正词条（优先级最高）
Content\Dictionary\ai_zh.json                  内置 AI 翻译结果
Source\StarryUEZh\                             插件源码
ThirdParty\Detours\                            UE 5.4-5.7 用的 Detours 头文件和静态库
```

## 许可证

- 代码（`Source`、`Config` 等）：MIT，见 `LICENSE`。
- 词库（`Content/Dictionary/*.json`）：CC BY-SA 4.0，见 `LICENSE-DICTIONARY`。
- `ThirdParty/Detours`：Microsoft Detours v4.0.1，MIT，见 `ThirdParty/Detours/LICENSE.md`。
- 词库里的部分条目来自 Unreal Engine 自带的 zh-Hans 本地化资源，版权归 Epic Games 所有；本项目与 Epic Games 无关联。详见 `NOTICE.md`。

## English

StarryUEZh is a Chinese localization plugin for the Unreal Engine 5.4-5.8 editor. It translates Blueprint, Material and Niagara node titles, pins, details panels, menus and tooltips at the Slate display layer, supports Chinese/English switching and bilingual node search, and does not modify source code, asset names, asset paths or packaged games. Prebuilt packages for each engine version are available in Releases. The code is MIT licensed; the dictionary data is CC BY-SA 4.0.
