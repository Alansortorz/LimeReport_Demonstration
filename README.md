# **`LimeReport_Demonstration`**
这是我自己用于学习和复现报表生成软件项目`LimeReport`的一个示例项目，本项目会将原`LimeReport`项目`QMake`构建系统替换成`CMake`构建系统，并使用`LimeReport`编译的库，调用库的函数现实各类函数的调用和调试。

原项目导航：`https://github.com/fralx/LimeReport`

## 0  前言

先简单介绍一下这个项目，这个项目叫做`LimeReport`，是一位俄罗斯小哥开发并开源的一个基于Qt和C++的报表生成软件。也就是基于提供的数据自动生成表格文档，相当于一个自动化的文档整理工具。很多的设备厂商会配备一个报表生成软件，用来生成设备数据报表，便于查看和分析。

## 1  环境配置

### 1.1 编译项目，获取库文件

编译项目过程很简单，直接下载源码，用`QtCreator`打开，直接编译即可在想要的`Demo`目录中得到相应的库函数。就像这样：

![使用QtCreator编译LimeReport项目得到的库函数的位置截图](2.Images/%E4%BD%BF%E7%94%A8QtCreator%E7%BC%96%E8%AF%91LimeReport%E9%A1%B9%E7%9B%AE%E5%BE%97%E5%88%B0%E7%9A%84%E5%BA%93%E5%87%BD%E6%95%B0%E7%9A%84%E4%BD%8D%E7%BD%AE%E6%88%AA%E5%9B%BE.jpg)

### 1.2  项目配置

如果是`QMake`的话，那么需要配置以下几点，主要包括：

- 将库文件`lib`复制到自己的项目中
- 在`pro`文件中添加相应的配置，具体如下：

```cmake
QT += printsupport qml 

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/librelease/ -llimereport
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llimereportd
 
INCLUDEPATH += $$PWD/librelease
DEPENDPATH += $$PWD/librelease
 
INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib
```

- 在项目中引入库并测试

```c++
#include "lib/include/lrreportengine.h"

LimeReport::ReportEngine *report;
report = new LimeReport::ReportEngine(this);  

report->designReport();
report->previewReport();
```

> 在`C++`中`this`指针是一个**指向当前对象的特殊指针**，在类的成员函数中使用这个指针可以访问当前对象的成员函数和成员变量。类的静态成员函数是没法使用this指针的，原因如下：
>
> - 类的静态成员函数内部没有`this`指针。
> - 静态成员函数是类的全局函数，属于整个类，只与类的静态成员变量及其它类的成员函数交互，所以从设计的角度来说它就没必要有this指针。

如何更改为`CMake`项目？

- 在`CMakeLists.txt`文件中添加如下内容：

```cmake
# Find the required Qt5 components
find_package(Qt5 COMPONENTS Core Gui PrintSupport Qml REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# Add Qt5 modules to target link libraries
target_link_libraries(YOU_PROJECT_NAME PRIVATE
    Qt5::Core
    Qt5::Gui
    Qt5::PrintSupport
    Qt5::Qml
)

if(WIN32)
    target_link_libraries(MIL_ReportView PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/limereportd.lib)
endif()
```

- 在主函数中添加操作函数文件`limereport.cpp`和`limereport.h`并配置相关操作类`LeReport`

```c++
#ifndef LIMEREPORT_H
#define LIMEREPORT_H

#include <QWidget>
#include "lib/include/lrreportengine.h"

class LmReport : QObject
{
    Q_OBJECT
public:
    LmReport(QWidget* parent = nullptr);
    ~LmReport();

    void LoadDesignReport();

private:
    LimeReport::ReportEngine *m_report;


};

#endif // LIMEREPORT_H
```

```c++
#include "limereport.h"

LmReport::LmReport(QWidget* parent)
{
    m_report = new LimeReport::ReportEngine(this);
}

LmReport::~LmReport()
{

}

void LmReport::LoadDesignReport()
{
    m_report->designReport();
}

```

主函数

```c++
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "limereport.h"
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    //QGuiApplication app(argc, argv);
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    //engine.load(url);

    LmReport lreport;
    lreport.LoadDesignReport();

    return app.exec();
}
```

- 【注意】在主函数中将`QGuiApplication`更改为`QApplication`，否则报错！！！

```c++
QGuiApplication app(argc, argv); --> QApplication app(argc, argv);
```

## 2 功能测试

在项目中新增两个文件，一个源文件`limereport.cpp`和头文件`limereport.h`，文件内容如下：

```c++
#ifndef LIMEREPORT_H
#define LIMEREPORT_H

#include <QDebug>
#include <QWidget>
#include <QFileInfo>
#include <QGestureEvent>
#include <QPinchGesture>
#include "lib/include/lrreportengine.h"
#include "lib/include/lrcallbackdatasourceintf.h"

class limereport : public QObject
{
    Q_OBJECT

public:
    limereport(QWidget* parent = nullptr); // 修改默认参数为nullptr
    virtual ~limereport();

public slots:
    void designReport();        //设计报表
    void previewReport();       //预览报表

private:
    LimeReport::ReportEngine *m_report;
    LimeReport::PreviewReportWidget *m_preview;
};

#endif


```

头文件内容如下：

```c++
#include "limereport.h"

limereport::limereport(QWidget* parent) // 使用初始化列表初始化 m_report
{
    m_report = new LimeReport::ReportEngine(this);
    m_preview = m_report->createPreviewWidget();

    // 配置模板文件数据
    QFileInfo fileInfo("E:/Other/Workspace/3DPrint/MetalForgeX/MILReportView/MIL_ReportViewer_Qt5/MIL_ReportView/ReportViewCtrl/Report/lrxml/WorkData.lrxml");
    if(fileInfo.isFile())
    {
        m_report->loadFromFile("E:/Other/Workspace/3DPrint/MetalForgeX/MILReportView/MIL_ReportViewer_Qt5/MIL_ReportView/ReportViewCtrl/Report/lrxml/WorkData.lrxml");
        m_preview->refreshPages();
    }
    m_preview->setScalePercent(50);
}

limereport::~limereport()
{
    delete m_report;
}

void limereport::designReport()
{
    m_report->designReport();
}

void limereport::previewReport()
{
    m_preview->show();
}
```

在主函数中调用

```c++
limereport lreport;
lreport.designReport();
```

如果没有文件，将会看到以下界面：

![Limereport设计报表界面](2.Images/Limereport%E8%AE%BE%E8%AE%A1%E6%8A%A5%E8%A1%A8%E7%95%8C%E9%9D%A2.jpg)

## 3  重要功能实现

### 3.1  报表设计功能

#### 3.1.1  报表组成

关于报表设计主要包含以下内容：

- Toolbars 工具栏
- Report Elements Bar 报表元素栏
- Object Browser 对象浏览
- Data Browser 数据浏览
- Report Page 报表页面
- Report Structure Browser 报表结构浏览

报表设计中常用的快捷键：

| 序号 |          快捷键           |        描述        |
| :--: | :-----------------------: | :----------------: |
|  1   |         Ctrl + N          |      新建报表      |
|  2   |         Ctrl + O          |      加载报表      |
|  3   |         Ctrl + S          |      保存报表      |
|  4   |     Ctrl + Shift + S      |       另存为       |
|  5   |         Ctrl + P          |      预览报告      |
|  6   |         Ctrl + Z          |        回退        |
|  7   |     Ctrl + Shift + Z      |        恢复        |
|  8   |         Ctrl + C          |        复制        |
|  9   |         Ctrl + X          |        剪切        |
|  10  |         Ctrl + V          |        粘贴        |
|  11  |       Ctrl + Arrows       |   移动选择的对象   |
|  12  |      Shift + Arrows       | 修改选择对象的大小 |
|  13  |            Del            |   删除选择的对象   |
|  14  | Shift + Left mouse button |    创建选择区域    |

#### 3.1.2 数据源

`LimeReport`提供这么几种数据源

- 变量，
- 使用数据库连接的基于 SQL 的数据集。可以通过以下方式建立数据库连接：
  - 可以立即从报告生成器建立连接。
  - 可以通过外部应用程序建立连接。
- 程序中创建一个类并让它继承自 QAbstractItemModel，然后实现这个类的各种方法和功能，从而使外部的数据集可以在 Qt 应用程序中使用。
- 使用信号-槽（Signal-Slot）方法进行数据传输和与报告生成器的链接。

Sqlite



报表预览功能



报表自动生成



设置背景颜色



### 3.1  文件预览

模板文件：.lrxml



数据文件：.lrd

待定

待定



待定



### 3.2  模板设计



### 3.3  数据加载



从`Sqlite`读取数据



循环读取数据



模板设计

模板设计的使用技巧

关于License

## 参考资料

1. https://blog.csdn.net/moudide/article/details/122936070

