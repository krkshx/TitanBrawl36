#pragma once

// Home data viewer: decodes OwnHomeData frames (demo-built or loaded from
// a .bin capture) and shows the fields. Qt only; core stays clean.

#include "HeroCards.hpp"
#include "titan/core/PiranhaMessage.cpp"
#include "titan/game/data/DataTables.cpp"

#include <QMainWindow>
#include <QTabWidget>
#include <QTreeWidget>
#include <QTextEdit>
#include <QStatusBar>
#include <QPushButton>

#include <cstdint>
#include <memory>
#include <vector>

namespace titan {
class PiranhaMessage;
}

class HomeViewer : public QMainWindow {
    Q_OBJECT
public:
    explicit HomeViewer(QWidget* parent = nullptr);

private slots:
    void onDemo();
    void onLoad();

private:
    void showFrame(const std::vector<std::uint8_t>& frame);
    void showError(const QString& what);

    QTreeWidget* tree_;
    QTextEdit* hex_;
    HeroCards* cards_;
    QTreeWidget* notifs_;
    QTreeWidget* shop_;
    QPushButton* demoBtn_;
    QPushButton* loadBtn_;
    std::unique_ptr<titan::PiranhaMessage> last_;
    titan::DataTables tables_;
    bool tablesOk_ = false;
};
