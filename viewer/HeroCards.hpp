#pragma once

// Hero cards widget: paints one card per owned hero (from the avatar's
// hero slots) with name, level and count from the data tables.
// Pure Qt view over decoded protocol data; art is placeholder rects
// (per-hero art lives in .sc files — a later wave).

#include "titan/game/data/DataTables.cpp"
#include "titan/game/avatar/LogicClientAvatar.cpp"

#include <QWidget>

class HeroCards : public QWidget {
    Q_OBJECT
public:
    explicit HeroCards(QWidget* parent = nullptr);

    void setData(const titan::LogicClientAvatar* avatar,
                 const titan::DataTables* tables);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    const titan::LogicClientAvatar* avatar_ = nullptr;
    const titan::DataTables* tables_ = nullptr;
};
