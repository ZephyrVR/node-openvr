#include "ivrnotifications.h"
#include <node.h>
#include "other_util.h"
#include "ivrsystem.h"
#include "ivroverlay.h"

using namespace v8;

#define CHECK_ERROR(err) { if (err != vr::VROverlayError_None) { Nan::ThrowError(vr::VROverlay()->GetOverlayErrorNameFromEnum(err)); return; } }
#define HND_OVERLAY(prop) IVROverlay::overlayHandleMap[prop->Uint32Value(Nan::GetCurrentContext()).FromJust()]

bool IVRNotifications::checkError(vr::EVRNotificationError err) {
    if (err != vr::VRNotificationError_OK) {
        return false;
    }

    return true;
}

NAN_MODULE_INIT(IVRNotifications::Init) {
    SET_METHOD(CreateNotification);
    SET_METHOD(RemoveNotification);
    SET_METHOD(Internals);
}

NAN_METHOD(IVRNotifications::Internals) {
    info.GetReturnValue().Set(
        #ifdef OVERLAY_D3D
        0
        #else
        1
        #endif
    );
}

NAN_METHOD(IVRNotifications::CreateNotification) {
    if (info.Length() != 6) {
        Nan::ThrowError("Wrong number of arguments.");
    }

    if (!info[2]->IsArrayBufferView()) {
        Nan::ThrowTypeError("Argument 3 must be an ArrayBufferView, such as a Uint8ClampedArray");
        return;
    }

    vr::EVRInitError eError;
    vr::IVRNotifications * notif = (vr::IVRNotifications *) vr::VR_GetGenericInterface(vr::IVRNotifications_Version, &eError);

    // Icon
    Local<ArrayBufferView> input = info[2].As<ArrayBufferView>();
    Nan::TypedArrayContents<uint8_t> buf(input);

    vr::NotificationBitmap_t icon;
    icon.m_nBytesPerPixel = 4;
    icon.m_nWidth = info[3]->Uint32Value(Nan::GetCurrentContext()).FromJust();
    icon.m_nHeight = info[4]->Uint32Value(Nan::GetCurrentContext()).FromJust();
    icon.m_pImageData = *buf;

    // Notification text
    V8STR(info[1], notificationText);

    // Send notification
    vr::VRNotificationId id;
    notif->CreateNotification(HND_OVERLAY(info[0]), 0, vr::EVRNotificationType_Transient, notificationText, vr::EVRNotificationStyle_Application, &icon, &id);

    info.GetReturnValue().Set(id);
}

NAN_METHOD(IVRNotifications::RemoveNotification) {
    if (info.Length() != 1) {
        Nan::ThrowError("Wrong number of arguments.");
    }

    vr::EVRInitError eError;
    vr::IVRNotifications * notif = (vr::IVRNotifications *) vr::VR_GetGenericInterface(vr::IVRNotifications_Version, &eError);

    V8INT(info[0], id);

    notif->RemoveNotification(id);
}