// HomeViewer bodies.

#include "HomeViewer.hpp"

#include "titan/game/LogicClientAvatar.hpp"
#include "titan/game/LogicClientHome.hpp"
#include "titan/game/LogicConfData.hpp"
#include "titan/game/LogicDailyData.hpp"
#include "titan/game/ForcedDrops.hpp"
#include "titan/gen/MessageFactory.hpp"
#include "titan/messages/OwnHomeDataMessage.hpp"
#include "titan/net/Messaging.hpp"

#include <QFileDialog>
#include <QFile>
#include <QHBoxLayout>
#include <QSplitter>
#include <QVBoxLayout>

using titan::ByteStream;
using titan::i32;
using titan::u8;

namespace {

// Decode one framed message; returns nullptr + reason on failure.
std::unique_ptr<titan::PiranhaMessage> decodeFrame(const std::vector<u8>& frame,
                                                   QString& reason) {
    using titan::net::kHeaderSize;
    if (frame.size() < static_cast<std::size_t>(kHeaderSize)) {
        reason = "frame shorter than 7-byte header";
        return nullptr;
    }
    u8 hdr[kHeaderSize];
    for (int i = 0; i < kHeaderSize; ++i) hdr[i] = frame[i];
    const titan::net::Header h = titan::net::readHeader(hdr);
    if (h.length < 0
        || static_cast<std::size_t>(h.length) != frame.size() - kHeaderSize) {
        reason = "header length mismatch";
        return nullptr;
    }
    auto m = titan::createMessageByType(h.type);
    if (!m) {
        reason = QString("unknown message type %1").arg(h.type);
        return nullptr;
    }
    m->setMessageVersion(h.version);
    if (h.length > 0) {
        m->stream().setBuffer(frame.data() + kHeaderSize, h.length);
    }
    try {
        m->decode();
    } catch (const std::exception& e) {
        reason = QString("decode failed: %1").arg(e.what());
        return nullptr;
    }
    return m;
}

QTreeWidgetItem* item(QTreeWidgetItem* parent, const QString& key,
                      const QString& value) {
    auto* it = new QTreeWidgetItem(parent, QStringList() << key << value);
    parent->addChild(it);
    return it;
}

void showHome(QTreeWidget* tree, const titan::OwnHomeDataMessage& m) {
    tree->clear();
    auto* root = new QTreeWidgetItem(tree, QStringList()
        << QString::fromStdString(m.getMessageTypeName())
        << QString("type=%1 ver=%2")
               .arg(m.getMessageType())
               .arg(m.getMessageVersion()));
    tree->addTopLevelItem(root);
    if (m.avatar_) {
        auto* av = item(root, "avatar", "");
        const auto& a = *m.avatar_;
        item(av, "name", QString::fromStdString(a.name_));
        item(av, "id", QString("%1/%2").arg(a.id1_.low).arg(a.id2_.low));
        item(av, "version", QString::number(a.kVersion));
    }
    if (m.home_) {
        auto* home = item(root, "home", "");
        const auto& h = *m.home_;
        item(home, "notifications", QString::number(h.notifications_.size()));
        item(home, "refs", QString::number(h.refs_.size()));
        if (h.daily_) {
            item(home, "daily.head0", QString::number(h.daily_->head_[0]));
        }
    }
    item(root, "f152", QString::number(m.f152_));
    tree->expandAll();
}

} // namespace

HomeViewer::HomeViewer(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("TitanBrawl36 — home viewer");
    resize(900, 600);

    auto* splitter = new QSplitter(this);
    tree_ = new QTreeWidget(splitter);
    tree_->setHeaderLabels({"field", "value"});
    hex_ = new QTextEdit(splitter);
    hex_->setReadOnly(true);
    hex_->setFontFamily("monospace");
    splitter->addWidget(tree_);
    splitter->addWidget(hex_);
    splitter->setSizes({550, 350});

    demoBtn_ = new QPushButton("Build demo OwnHomeData", this);
    loadBtn_ = new QPushButton("Load .bin capture…", this);
    connect(demoBtn_, &QPushButton::clicked, this, &HomeViewer::onDemo);
    connect(loadBtn_, &QPushButton::clicked, this, &HomeViewer::onLoad);

    auto* bar = new QWidget(this);
    auto* bl = new QHBoxLayout(bar);
    bl->addWidget(demoBtn_);
    bl->addWidget(loadBtn_);
    bl->addStretch();

    auto* central = new QWidget(this);
    auto* layout = new QVBoxLayout(central);
    layout->addWidget(splitter);
    layout->addWidget(bar);
    setCentralWidget(central);
    statusBar()->showMessage("ready");
}

void HomeViewer::showFrame(const std::vector<u8>& frame) {
    QString reason;
    auto m = decodeFrame(frame, reason);
    if (!m) {
        showError(reason);
        return;
    }
    QString hex;
    hex.reserve(static_cast<int>(frame.size()) * 3);
    for (std::size_t i = 0; i < frame.size(); ++i) {
        if (i > 0) hex += (i % 16 == 0) ? '\n' : ' ';
        hex += QString("%1").arg(frame[i], 2, 16, QChar('0')).toUpper();
    }
    hex_->setPlainText(hex);
    if (auto* home = dynamic_cast<titan::OwnHomeDataMessage*>(m.get())) {
        showHome(tree_, *home);
        statusBar()->showMessage(
            QString("decoded %1 (%2 bytes)").arg(home->getMessageTypeName()).arg(frame.size()));
    } else {
        tree_->clear();
        auto* root = new QTreeWidgetItem(
            tree_, QStringList() << QString::fromStdString(m->getMessageTypeName())
                                 << QString("type=%1").arg(m->getMessageType()));
        tree_->addTopLevelItem(root);
        statusBar()->showMessage("decoded non-home message");
    }
}

void HomeViewer::showError(const QString& what) {
    tree_->clear();
    hex_->clear();
    statusBar()->showMessage("error: " + what);
}

void HomeViewer::onDemo() {
    titan::OwnHomeDataMessage m;
    m.home_ = std::make_unique<titan::LogicClientHome>();
    m.home_->daily_ = std::make_unique<titan::LogicDailyData>();
    m.home_->daily_->head_[0] = 1234;
    m.home_->daily_->forced_ = std::make_unique<titan::ForcedDrops>();
    m.home_->conf_ = std::make_unique<titan::LogicConfData>();
    m.avatar_ = std::make_unique<titan::LogicClientAvatar>();
    m.avatar_->name_ = "Commander";
    m.avatar_->id1_ = titan::LogicLong{0, 1};
    m.avatar_->id2_ = titan::LogicLong{0, 2};
    m.f152_ = 1;
    showFrame(titan::net::encodeFrame(m));
}

void HomeViewer::onLoad() {
    const QString path = QFileDialog::getOpenFileName(
        this, "Open framed message capture", "", "Captures (*.bin);;All (*)");
    if (path.isEmpty()) return;
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly)) {
        showError("cannot open file");
        return;
    }
    const QByteArray data = f.readAll();
    showFrame(std::vector<u8>(data.begin(), data.end()));
}
