#include <QtWidgets/QMainWindow>
#include <QListWidget>

#include <QLayout>
#include <QGroupBox>

#include <QCloseEvent>

#include <QMessageBox>
#include <QDialog>
#include <QDir>

#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QSlider>

#include <QVideoWidget>
#include <QMediaPlayer>
#include <QAudioInput>
#include <QAudioOutput>

#include <QProcess>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadVideoTemplates(const QString& videoFolder, const QString& thumbFolder, QGroupBox* groupBox);
    void generateThumbnail(const QString& videoPath, const QString& outputPath);

public slots:
    void setSliderPosition(int position);
    void updateMediaPosition(int position);
    void updateMediaDuration(int duration);

private:
    QTextEdit* contents = nullptr;
    QPushButton* btnPlay = nullptr;
    QPushButton* btnStop = nullptr;
    QPushButton* btnSave = nullptr;
    QSlider* slider = nullptr;
    QMediaPlayer* player = nullptr;

    QString ffmpegPath = "./ffmpeg/bin/ffmpeg.exe";
    QString videoFolderPath = "./videos";
    QString thumbFolderPath = "./temp";

    QListWidget* supportedFeatures;
    QListWidget* pinnedFeatures;

protected:
    void closeEvent(QCloseEvent* event) override;
};
