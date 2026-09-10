#!/usr/bin/env python3
"""Emit per-class files for macro/family-derived messages."""
import os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OUT = os.path.join(ROOT, "include", "titan", "messages")

FRIENDS = [
    ("AskForPlayingFacebookFriendsMessage", 10513, False, False),
    ("AskForPlayingGamecenterFriendsMessage", 10512, False, False),
    ("AskForPlayingKakaoFriendsMessage", 10514, False, False),
    ("AskForPlayingLineFriendsMessage", 10516, False, False),
    ("AskForPlayingSupercellFriendsMessage", 10517, False, False),
    ("AskForPlayingTencentFriendsMessage", 10515, True, True),
]

BINDS = [
    ("BindFacebookAccountMessage", 14201, 0, False, True),
    ("BindGamecenterAccountMessage", 14212, 3, False, True),
    ("BindGoogleServiceAccountMessage", 14262, 0, False, True),
    ("BindKakaoAccountMessage", 14202, 0, False, True),
    ("BindLineAccountMessage", 14203, 0, False, True),
    ("BindTencentAccountMessage", 14266, 0, True, False),
    ("BindYoozooAccountMessage", 14266, 0, True, False),
]

SETTINGS = [
    ("ChangeAllianceSettingsMessage", 14316),
    ("CreateAllianceMessage", 14301),
]


def pour(name, body):
    with open(os.path.join(OUT, name + ".hpp"), "w", encoding="utf-8", newline="\n") as fh:
        fh.write(body)


for cls, mid, vint, ref in FRIENDS:
    pour(cls, """#pragma once

// %s (%d) — AskForPlaying*Friends family (see Common.hpp).
// Split out of MsgBatch01.hpp; wire format unchanged.

#include "titan/messages/Common.hpp"

namespace titan {

class %s : public PlayingFriendsQuery {
public:
    int getMessageType() const override { return %d; }
    const char *getMessageTypeName() const override { return "%s"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeIds(%s, %s);
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeIds(%s, %s);
    }
};

} // namespace titan
""" % (cls, mid, cls, mid, cls,
       "true" if vint else "false", "true" if ref else "false",
       "true" if vint else "false", "true" if ref else "false"))

for cls, mid, blobs, vtail, force in BINDS:
    pour(cls, """#pragma once

// %s (%d) — Bind*Account family (see Common.hpp).
// Split out of MsgBatch02.hpp; wire format unchanged.

#include "titan/messages/Common.hpp"

namespace titan {

class %s : public BindAccountMessage {
public:
    int getMessageType() const override { return %d; }
    const char *getMessageTypeName() const override { return "%s"; }
    void encode() override {
        PiranhaMessage::encode();
        blobCount_ = %d;
        hasVintTail_ = %s;
        hasForce_ = %s;
        encodeBind();
    }
    void decode() override {
        PiranhaMessage::decode();
        blobCount_ = %d;
        hasVintTail_ = %s;
        hasForce_ = %s;
        decodeBind();
    }
};

} // namespace titan
""" % (cls, mid, cls, mid, cls, blobs,
       "true" if vtail else "false", "true" if force else "false", blobs,
       "true" if vtail else "false", "true" if force else "false"))

for cls, mid in SETTINGS:
    pour(cls, """#pragma once

// %s (%d) — AllianceSettings shape (see Common.hpp).
// Split out of MsgBatch03.hpp; wire format unchanged.

#include "titan/messages/Common.hpp"

namespace titan {

class %s : public AllianceSettingsBase {
public:
    int getMessageType() const override { return %d; }
    const char *getMessageTypeName() const override { return "%s"; }
    void encode() override {
        PiranhaMessage::encode();
        encodeSettings();
    }
    void decode() override {
        PiranhaMessage::decode();
        decodeSettings();
    }
};

} // namespace titan
""" % (cls, mid, cls, mid, cls))

print("derived:", len(FRIENDS) + len(BINDS) + len(SETTINGS))
