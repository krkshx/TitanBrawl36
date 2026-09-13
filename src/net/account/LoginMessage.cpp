#pragma once
#include "../core/PiranhaMessage.cpp"
#include "../../titan/core/ByteStreamHelper.cpp"
#include <cstdint>
#include <string>

// Порядок encode строго по либе: _ZN12LoginMessage6encodeEv @ 0x483cd0,
// линейная цепочка 0x484b08->0x484c64 (31 BL), поля смапплены через сеттеры.
class LoginMessage : public PiranhaMessage {
public:
    LoginMessage() : PiranhaMessage(10) {}
    std::int32_t getMessageType() const override { return 10101; }
    std::int32_t getServiceNodeType() const override { return 1; }
    const char *getMessageTypeName() const override { return "LoginMessage"; }

    void encode() override {
        ByteStream *s = getByteStream();
        LogicLong id; // +136 setAccountId
        id.high = static_cast<std::int32_t>(accountId_ >> 32);
        id.low = static_cast<std::int32_t>(accountId_ & 0xFFFFFFFFll);
        s->writeLong(&id);
        // Свежий логин: setPassToken(null) -> на проводе -1, как в либе.
        s->writeString(passToken_.empty() ? nullptr : &passToken_); // +144 setPassToken
        s->writeInt(major_); // +152 setClientMajorVersion
        s->writeInt(1); // константа либы (mov w1,#1 между major и build)
        s->writeInt(build_); // +156 setClientBuild
        s->writeString(&resourceSha_); // +176 setResourceSha
        s->writeString(&device_); // +184 setDevice
        // +208 preferredLanguage (LogicLocaleData*): либа пишет объект =
        // ДВА VInt (classId, instanceId), сервер читает два VInt безусловно.
        // Значения сервер игнорирует, поэтому (0,0) — та же длина, тот же парс.
        s->writeVInt(0);
        s->writeVInt(0);
        s->writeString(&preferredDeviceLanguage_); // +192 setPreferredDeviceLanguage
        s->writeString(&osVersion_); // +216 setOSVersion
        s->writeBoolean(isAndroid_); // +272 setAndroid
        s->writeStringReference(imei_); // +224 setIMEI
        s->writeStringReference(androidId_); // +240 setAndroidID
        s->writeBoolean(isAdvertisingEnabled_); // +273 setAdvertisingEnabled
        s->writeString(&appleIFV_); // +256 setAppleIFV
        s->writeInt(rndKey_); // +268 setRndKey
        s->writeVInt(appStore_); // +264 setAppStore
        s->writeStringReference(clientVersion_); // +160 setClientVersion
        s->writeStringReference(tencentOpenId_); // +280 setTencentOpenId
        s->writeStringReference(tencentToken_); // +296 setTencentToken
        s->writeVInt(tencentPlatform_); // +312 setTencentPlatform
        s->writeStringReference(deviceVerifierResponse_); // +328 setDeviceVerifierResponse
        s->writeStringReference(appLicensingSignature_); // +344 setAppLicensingSignature
        s->writeStringReference(appLicensingSignedData_); // +360 setAppLicensingSignedData
        // +320 supercellIdToken: LogicCompressedString::encode, null -> writeBytes(0,-1).
        s->writeBytes(nullptr, -1);
        s->writeBoolean(updateMaintenanceMode_); // +376 setUpdateMaintenanceMode
        s->writeStringReference(yoozooOsdkTicket_); // +384 setYoozooOsdkTicket
        s->writeStringReference(yoozooDeviceId_); // +400 setYoozooDeviceId
    }

    void decode() override {}

    void setAccountId(std::int64_t v) { accountId_ = v; }
    void setPassToken(const std::string &v) { passToken_ = v; }
    void setVersion(std::int32_t major, std::int32_t build) { major_ = major; build_ = build; }
    void setResourceSha(const std::string &v) { resourceSha_ = v; }
    void setDevice(const std::string &v) { device_ = v; }
    void setPreferredDeviceLanguage(const std::string &v) { preferredDeviceLanguage_ = v; }
    void setOSVersion(const std::string &v) { osVersion_ = v; }
    void setAndroid(bool v) { isAndroid_ = v; }
    void setIMEI(const std::string &v) { imei_ = v; }
    void setAndroidId(const std::string &v) { androidId_ = v; }
    void setAdvertisingEnabled(bool v) { isAdvertisingEnabled_ = v; }
    void setAppleIFV(const std::string &v) { appleIFV_ = v; }
    void setRndKey(std::int32_t v) { rndKey_ = v; }
    void setAppStore(std::int32_t v) { appStore_ = v; }
    void setClientVersion(const std::string &v) { clientVersion_ = v; }

private:
    std::int64_t accountId_ = 0;
    std::string passToken_;
    std::int32_t major_ = 36;
    std::int32_t build_ = 218;
    std::string resourceSha_;
    std::string device_;
    std::string preferredDeviceLanguage_;
    std::string osVersion_;
    bool isAndroid_ = true;
    std::string imei_;
    std::string androidId_;
    bool isAdvertisingEnabled_ = false;
    std::string appleIFV_;
    std::int32_t rndKey_ = 0;
    std::int32_t appStore_ = 0;
    std::string clientVersion_ = "36.218";
    std::string tencentOpenId_;
    std::string tencentToken_;
    std::int32_t tencentPlatform_ = 0;
    std::string deviceVerifierResponse_;
    std::string appLicensingSignature_;
    std::string appLicensingSignedData_;
    bool updateMaintenanceMode_ = false;
    std::string yoozooOsdkTicket_;
    std::string yoozooDeviceId_;
};
