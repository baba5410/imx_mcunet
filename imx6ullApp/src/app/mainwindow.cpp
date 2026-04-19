#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QPluginLoader>
#include <QCoreApplication>
#include <QGraphicsDropShadowEffect>
#include <QHBoxLayout>
#include <QLabel>
#include <QScroller>
#include <QScrollerProperties>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // 1. 初始化容器
    m_pageContainer = new QStackedWidget(this);
    setCentralWidget(m_pageContainer);

    // 2. 加载插件 (必须在 initUi 之前)
    loadPlugins();

    // 3. 构建 UI
    initUi();

    // 4. 窗口设置
    setWindowFlags(Qt::FramelessWindowHint);
    resize(SCREEN_WIDTH, SCREEN_HEIGHT);
}

MainWindow::~MainWindow() {
    // 程序退出时停止模块
    if (m_videoModule) {
        m_videoModule->stop();
        delete m_videoModule;   // 触发析构函数，释放 V4L2 资源
        m_videoModule = nullptr;
    }
}

void MainWindow::loadPlugins() {
    // 插件路径指向 build/release/plugins/
    QString pluginPath = QCoreApplication::applicationDirPath() + "/plugins/ov5640_module.so";

    QPluginLoader loader(pluginPath);
    QObject *instance = loader.instance();

    if (instance) {
        m_videoModule = qobject_cast<IModuleInterface*>(instance);
        if (m_videoModule) {
            // 初始化模块
            if (m_videoModule->init()) {
                m_videoWidget = m_videoModule->getWidget();
                qDebug() << "成功加载插件:" << m_videoModule->moduleName();
            }
        }
    } else {
        qWarning() << "插件加载失败:" << loader.errorString();
    }
}

void MainWindow::initUi() {
    // --- 首页构建 ---
    m_homePage = new QWidget();
    m_homePage->setStyleSheet("background-color: #F8F9FA;");

    QVBoxLayout *homeLayout = new QVBoxLayout(m_homePage);
    homeLayout->setContentsMargins(0, 30, 0, 0);

    QLabel *titleLabel = new QLabel("智能控制中心");
    titleLabel->setStyleSheet("font-size: 32px; font-weight: 600; color: #202124; margin-left: 40px; border:none;");
    homeLayout->addWidget(titleLabel);

    // 滑动区域
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setStyleSheet("background: transparent;");

    QWidget *cardContainer = new QWidget();
    QHBoxLayout *cardLayout = new QHBoxLayout(cardContainer);
    cardLayout->setContentsMargins(40, 0, 40, 20);
    cardLayout->setSpacing(25);

    // 添加 App 卡片 (视频监控对应 Index 2)
    cardLayout->addWidget(createAppCard("音乐播放", "🎵", 1));
    cardLayout->addWidget(createAppCard("视频监控", "📷", 2));
    cardLayout->addWidget(createAppCard("文件系统", "📁", 3));
    cardLayout->addWidget(createAppCard("系统设置", "⚙️", 4));
    cardLayout->addStretch();

    scrollArea->setWidget(cardContainer);
    homeLayout->addWidget(scrollArea);

    // 退出按钮
    QPushButton *exitBtn = new QPushButton("退出", m_homePage);
    exitBtn->setFixedSize(90, 45);
    exitBtn->move(SCREEN_WIDTH - 110, 20);
    exitBtn->setStyleSheet(
        "QPushButton { background-color: #EA4335; color: white; border-radius: 22px; font-weight: bold; border:none; }"
        "QPushButton:pressed { background-color: #B23121; }");
    connect(exitBtn, &QPushButton::clicked, qApp, &QApplication::quit);

    // --- 注册页面到容器 ---
    m_pageContainer->addWidget(m_homePage); // Index 0
    m_pageContainer->addWidget(createSubPage("音乐播放器", "音乐播放功能模块")); // Index 1

    // --- 视频监控页面 (Index 2) ---
    QWidget *videoPage = new QWidget();
    videoPage->setStyleSheet("background-color: black;");
    QVBoxLayout *vLayout = new QVBoxLayout(videoPage);
    vLayout->setContentsMargins(0, 0, 0, 0);

    // 视频返回按钮容器
    QWidget *videoHeader = new QWidget();
    videoHeader->setFixedHeight(50);
    QHBoxLayout *headerLayout = new QHBoxLayout(videoHeader);
    QPushButton *vBackBtn = new QPushButton("← 返回", videoHeader);
    vBackBtn->setFixedSize(100, 35);
    vBackBtn->setStyleSheet("background-color: #5F6368; color: white; border-radius: 15px; border:none;");

    // 点击返回停止采集
    connect(vBackBtn, &QPushButton::clicked, [this]() {
        if (m_videoModule) m_videoModule->pause(); // 切换页面时暂停采集
        m_pageContainer->setCurrentIndex(0);
    });
    headerLayout->addWidget(vBackBtn);
    headerLayout->addStretch();
    vLayout->addWidget(videoHeader);

    // 嵌入视频组件
    if (m_videoWidget) {
        m_videoWidget->setParent(videoPage);
        vLayout->addWidget(m_videoWidget);
    } else {
        QLabel *noVideo = new QLabel("未发现摄像头模块", videoPage);
        noVideo->setStyleSheet("color: white; font-size: 20px;");
        noVideo->setAlignment(Qt::AlignCenter);
        vLayout->addWidget(noVideo);
    }

    m_pageContainer->addWidget(videoPage); // Index 2
    m_pageContainer->addWidget(createSubPage("文件管理", "文件系统浏览")); // Index 3
    m_pageContainer->addWidget(createSubPage("系统设置", "内核版本: Linux 5.4.31")); // Index 4

    // 开启滑动支持
    QScroller::grabGesture(scrollArea->viewport(), QScroller::LeftMouseButtonGesture);
    QScrollerProperties prop = QScroller::scroller(scrollArea->viewport())->scrollerProperties();
    prop.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
    QScroller::scroller(scrollArea->viewport())->setScrollerProperties(prop);
}

QWidget *MainWindow::createAppCard(QString name, QString icon, int pageIndex) {
    QPushButton *card = new QPushButton();
    card->setFixedSize(180, 260);
    card->setStyleSheet("QPushButton { background-color: white; border: 1px solid #DADCE0; border-radius: 20px; }"
                        "QPushButton:pressed { background-color: #E8F0FE; border: 2px solid #4285F4; }");

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(10);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 30));
    card->setGraphicsEffect(shadow);

    QVBoxLayout *layout = new QVBoxLayout(card);
    QLabel *iconLabel = new QLabel(icon);
    iconLabel->setStyleSheet("font-size: 60px; background: transparent; border:none;");
    iconLabel->setAlignment(Qt::AlignCenter);

    QLabel *nameLabel = new QLabel(name);
    nameLabel->setStyleSheet("font-size: 18px; color: #3C4043; background: transparent; border:none;");
    nameLabel->setAlignment(Qt::AlignCenter);

    layout->addStretch();
    layout->addWidget(iconLabel);
    layout->addSpacing(15);
    layout->addWidget(nameLabel);
    layout->addStretch();

    // 点击卡片切换逻辑
    connect(card, &QPushButton::clicked, [this, pageIndex]() {
        // 如果点击的是视频模块
        if (pageIndex == 2 && m_videoModule) {
            m_videoModule->start(); // 开启 V4L2 采集
        }
        m_pageContainer->setCurrentIndex(pageIndex);
    });

    return card;
}

QWidget *MainWindow::createSubPage(QString title, QString description) {
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: white;");

    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(40, 40, 40, 40);

    QPushButton *backBtn = new QPushButton("← 返回", page);
    backBtn->setFixedSize(100, 40);
    backBtn->setStyleSheet("QPushButton { background-color: #F1F3F4; border-radius: 20px; color: #5F6368; font-weight: bold; border:none; }");
    connect(backBtn, &QPushButton::clicked, [this]() { m_pageContainer->setCurrentIndex(0); });

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet("font-size: 36px; font-weight: bold; color: #1A73E8; margin-top: 20px;");

    QLabel *descLabel = new QLabel(description);
    descLabel->setStyleSheet("font-size: 20px; color: #5F6368;");

    layout->addWidget(backBtn);
    layout->addSpacing(30);
    layout->addWidget(titleLabel);
    layout->addWidget(descLabel);
    layout->addStretch();

    return page;
}
