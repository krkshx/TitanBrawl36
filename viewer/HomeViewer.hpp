#pragma once

// Home data viewer: decodes OwnHomeData frames (demo-built or loaded from
// a .bin capture) and shows the fields. Qt only; core stays clean.

#include "titan/game/DataTables.hpp"

#include <QMainWindow>
#include <QTreeWidget>
#include <QTextEdit>
#include <QStatusBar>
#include <QPushButton>

#include <cstdint>
#include <vector>

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
    QPushButton* demoBtn_;
    QPushButton* loadBtn_;
    titan::DataTables tables_;
    bool tablesOk_ = false;
};
