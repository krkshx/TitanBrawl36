#pragma once

// Alliance entries — reversed from libg_decrypted.so (ARM64).
// Replaces the pending placeholders for these classes.

#include "titan/core/DataReference.hpp"
#include "titan/core/LogicLong.hpp"
#include "titan/messages/Nested.hpp"

#include <optional>
#include <string>

namespace titan {

// PlayerDisplayData::encode @0x934728, decode @0x56a900 (stream ctor).
// Wire: string name, vint x4 (unk, thumbnailId, nameColorId, unk=-1).
class PlayerDisplayData : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        s.writeString(name_ ? &*name_ : nullptr);
        s.writeVInt(v16_);
        s.writeVInt(thumbnailId_);
        s.writeVInt(nameColorId_);
        s.writeVInt(v28_);
    }
    void decode(ByteStream& s) override {
        name_ = s.readString();
        v16_ = s.readVInt();
        thumbnailId_ = s.readVInt();
        nameColorId_ = s.readVInt();
        v28_ = s.readVInt();
    }
    std::optional<std::string> name_;
    i32 v16_ = 0, thumbnailId_ = 0, nameColorId_ = 0, v28_ = -1;
};

// AllianceHeaderEntry::encode @0x88a094, decode @0x8881cc.
// Wire: logiclong, string, dataref, vint x4, dataref, string, vint, bool
// (badge dataref + name string are out-of-order vs layout).
class AllianceHeaderEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id_.encode(s);
        s.writeString(name_ ? &*name_ : nullptr);
        DataReference::encodeNullable(s, dataRef1_);
        s.writeVInt(v24_);
        s.writeVInt(v28_);
        s.writeVInt(v32_);
        s.writeVInt(v36_);
        DataReference::encodeNullable(s, badgeRef_);
        s.writeString(description_ ? &*description_ : nullptr);
        s.writeVInt(v40_);
        s.writeBoolean(flag_);
    }
    void decode(ByteStream& s) override {
        id_ = LogicLong::decode(s);
        name_ = s.readString();
        dataRef1_ = DataReference::decodeNullable(s);
        v24_ = s.readVInt();
        v28_ = s.readVInt();
        v32_ = s.readVInt();
        v36_ = s.readVInt();
        badgeRef_ = DataReference::decodeNullable(s);
        description_ = s.readString();
        v40_ = s.readVInt();
        flag_ = s.readBoolean();
    }
    LogicLong id_;
    std::optional<std::string> name_, description_;
    std::optional<DataReference> dataRef1_, badgeRef_;
    i32 v24_ = 0, v28_ = 0, v32_ = 0, v36_ = 0, v40_ = 0;
    bool flag_ = false;
};

// AllianceMemberEntry::encode @0x2367d4, decode @0x64244c.
// Wire: logiclong, vint x2, vint x3, bool, PlayerDisplayData (always present).
class AllianceMemberEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        id_.encode(s);
        s.writeVInt(v40_);
        s.writeVInt(v44_);
        s.writeVInt(v60_);
        s.writeVInt(v64_);
        s.writeVInt(v68_);
        s.writeBoolean(flag_);
        display_.encode(s);
    }
    void decode(ByteStream& s) override {
        id_ = LogicLong::decode(s);
        v40_ = s.readVInt();
        v44_ = s.readVInt();
        v60_ = s.readVInt();
        v64_ = s.readVInt();
        v68_ = s.readVInt();
        flag_ = s.readBoolean();
        display_.decode(s);
    }
    LogicLong id_;
    i32 v40_ = 0, v44_ = 0, v60_ = 0, v64_ = 0, v68_ = 0;
    bool flag_ = false;
    PlayerDisplayData display_;
};

// AllianceFullEntry::encode @0x49a4bc, decode @0x253a58.
// Wire: header, string, vint count (-1=null) + member loop.
class AllianceFullEntry : public NestedEntry {
public:
    void encode(ByteStream& s) const override {
        header_.encode(s);
        s.writeString(description_ ? &*description_ : nullptr);
        if (isNull_) {
            s.writeVInt(-1);
            return;
        }
        s.writeVInt(static_cast<i32>(members_.size()));
        for (const auto& m : members_) m.encode(s);
    }
    void decode(ByteStream& s) override {
        header_.decode(s);
        description_ = s.readString();
        const i32 n = s.readVInt();
        members_.clear();
        isNull_ = (n < 0);
        for (i32 i = 0; i < n; ++i) {
            AllianceMemberEntry m;
            m.decode(s);
            members_.push_back(m);
        }
    }
    AllianceHeaderEntry header_;
    std::optional<std::string> description_;
    std::vector<AllianceMemberEntry> members_;
    bool isNull_ = false;
};

} // namespace titan
