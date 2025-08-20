#include "mainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Text-To-Speech");

    auto centralWidget = new QWidget();
    this->setCentralWidget(centralWidget);

    auto leftLayout = new QVBoxLayout();
    auto rightLayout = new QVBoxLayout();

    auto font = QFont("Time New Roman", 13);
    font.setBold(true);

    btnPlay = new QPushButton("Play", this);
    btnPlay->setFont(font);

    btnStop = new QPushButton("Stop", this);
    btnStop->setFont(font);

    btnSave = new QPushButton("Save", this);
    btnSave->setFont(font);

    auto grTemplates = new QGroupBox("Templates");
    grTemplates->setFont(font);
    loadVideoTemplates(videoFolderPath, thumbFolderPath, grTemplates);

    auto grContents = new QGroupBox("Contents");
    grContents->setFont(font);
    auto layoutContents = new QHBoxLayout(grContents);
    contents = new QTextEdit(this);
    font.setBold(false);
    contents->setFont(font);
    layoutContents->addWidget(contents);
    grContents->setLayout(layoutContents);

    auto layoutButton = new QVBoxLayout(grContents);
    layoutButton->setAlignment(Qt::AlignVCenter);
    auto add = new QPushButton(">>");
    connect(add, &QPushButton::clicked, this, [this]() {
        auto items = supportedFeatures->selectedItems();
        for (auto* item : items) {
            supportedFeatures->takeItem(supportedFeatures->row(item));
            pinnedFeatures->addItem(item);
        }
    });
    auto del = new QPushButton("<<");
    connect(del, &QPushButton::clicked, this, [this]() {
        auto items = pinnedFeatures->selectedItems();
        for (auto* item : items) {
            pinnedFeatures->takeItem(pinnedFeatures->row(item));
            supportedFeatures->addItem(item);
        }
    });
    layoutButton->addWidget(add);
    layoutButton->addSpacing(10);
    layoutButton->addWidget(del);

    supportedFeatures = new QListWidget;
    pinnedFeatures = new QListWidget;

    supportedFeatures->setSelectionMode(QAbstractItemView::ExtendedSelection);
    supportedFeatures->setFont(font);
    pinnedFeatures->setSelectionMode(QAbstractItemView::ExtendedSelection);
    pinnedFeatures->setFont(font);

    QString text;
    for (int i = 1; i < 51; i++) {
        text = "Feature " + QString::number(i);
        QListWidgetItem* item = new QListWidgetItem(QIcon("nsicon.png"), text);
        supportedFeatures->addItem(item);
    }

    layoutContents->addSpacing(15);
    layoutContents->addWidget(supportedFeatures);
    layoutContents->addSpacing(15);
    layoutContents->addLayout(layoutButton);
    layoutContents->addSpacing(15);
    layoutContents->addWidget(pinnedFeatures);

    slider = new QSlider(Qt::Horizontal);
    connect(slider, &QSlider::sliderMoved, this, &MainWindow::setSliderPosition);

    player = new QMediaPlayer();
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::updateMediaPosition);
    connect(player, &QMediaPlayer::durationChanged, this, &MainWindow::updateMediaDuration);
    
    auto layoutMediaControl = new QHBoxLayout();
    layoutMediaControl->addWidget(btnPlay);
    layoutMediaControl->addWidget(btnStop);
    layoutMediaControl->addWidget(btnSave);
    layoutMediaControl->addWidget(slider);

    leftLayout->addWidget(grTemplates);
    rightLayout->addWidget(grContents);
    rightLayout->addLayout(layoutMediaControl);

    auto mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);
    centralWidget->setLayout(mainLayout);
}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent* event)
{
    auto reply = QMessageBox::information( this, "Information", "Are you sure you want to exit ?", QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::generateThumbnail(const QString& videoPath, const QString& thumbFolder)
{
    QStringList arguments;
    arguments << "-y"               // Override if exist
        << "-i" << videoPath        // Input video
        << "-ss" << "00:00:05"      // Frame at time of capture
        << "-vframes" << "1"        // Only capture 1 frame
        << thumbFolder;             // Output image

    QProcess process;
    process.start(ffmpegPath, arguments);
    process.waitForFinished(-1);    // Waiting process successfull
}

void MainWindow::loadVideoTemplates(const QString& videoFolder, const QString& thumbFolder, QGroupBox* groupBox)
{
    QDir videoDir(videoFolder);
    QDir thumbDir(thumbFolder);
    if (!thumbDir.exists()) thumbDir.mkpath(thumbFolder);

    QStringList filters;
    filters << "*.mp4";
    QFileInfoList videoFiles = videoDir.entryInfoList(filters, QDir::Files);

    QVBoxLayout* layout = new QVBoxLayout(groupBox);

    for (const QFileInfo& videoFile : videoFiles) {
        QString thumbPath = thumbFolder + "/" + videoFile.baseName() + ".jpg";

        // Create thumbnail if not exist
        if (!QFile::exists(thumbPath)) {
            generateThumbnail(videoFile.absoluteFilePath(), thumbPath);
        }

        // Display thumbnail
        QLabel* thumbLabel = new QLabel(this);
        QPixmap pix(thumbPath);
        thumbLabel->setPixmap(pix.scaled(320, 320, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        thumbLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(thumbLabel);
    }

    groupBox->setLayout(layout);

    for (const QFileInfo& fileInfo : thumbDir.entryInfoList(QDir::Files)) {
        QFile::remove(fileInfo.absoluteFilePath());
    }
}

void MainWindow::setSliderPosition(int position) 
{
    slider->setValue(position);
}

void MainWindow::updateMediaPosition(int position) 
{
    player->setPosition(position);
}

void MainWindow::updateMediaDuration(int duration) 
{
    slider->setRange(0, duration);
}
