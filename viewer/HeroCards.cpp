// HeroCards bodies.

#include "HeroCards.hpp"

#include "titan/game/data/LogicData.cpp"

#include <QPainter>

HeroCards::HeroCards(QWidget* parent) : QWidget(parent) {
    setMinimumHeight(180);
}

void HeroCards::setData(const titan::LogicClientAvatar* avatar,
                        const titan::DataTables* tables) {
    avatar_ = avatar;
    tables_ = tables;
    update();
}

void HeroCards::paintEvent(QPaintEvent* event) {
    (void)event;
    QPainter p(this);
    p.fillRect(rect(), QColor(24, 26, 32));
    if (!avatar_) {
        p.setPen(Qt::gray);
        p.drawText(rect(), Qt::AlignCenter, "no avatar decoded");
        return;
    }
    QFont font = p.font();
    font.setPointSize(10);
    p.setFont(font);
    const int cardW = 150, cardH = 150, gap = 12;
    int x = gap, y = gap;
    const auto& heroSlots = avatar_->slots_[5]; // heroes (getHeroLevel source)
    int shown = 0;
    for (const auto& slot : heroSlots) {
        if (!slot.data_) continue;
        if (x + cardW > width() && x > gap) {
            x = gap;
            y += cardH + gap;
        }
        // Card body.
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(44, 48, 60));
        p.drawRoundedRect(x, y, cardW, cardH, 10, 10);
        // Name + level from the data tables.
        QString name = QString("%1:%2")
                           .arg(slot.data_->classId)
                           .arg(slot.data_->instanceId);
        QString sub = QString("x%1").arg(slot.count_);
        if (tables_) {
            const std::string nm = tables_->getName(slot.data_->classId,
                                                    slot.data_->instanceId);
            if (!nm.empty()) name = QString::fromStdString(nm);
            const titan::LogicData hero(tables_, slot.data_->classId,
                                        slot.data_->instanceId);
            sub += QString("  Lv %1").arg(avatar_->getHeroLevel(hero));
        }
        p.setPen(Qt::white);
        p.drawText(x + 10, y + 28, cardW - 20, 24, Qt::AlignLeft, name);
        p.setPen(QColor(255, 204, 0));
        QFont subFont = p.font();
        subFont.setPointSize(9);
        p.setFont(subFont);
        p.drawText(x + 10, y + 52, cardW - 20, 20, Qt::AlignLeft, sub);
        x += cardW + gap;
        if (++shown >= 64) break;
    }
    if (shown == 0) {
        p.setPen(Qt::gray);
        p.drawText(rect(), Qt::AlignCenter, "no heroes in slots");
    }
}
