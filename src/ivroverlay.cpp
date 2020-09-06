#include "ivroverlay.h"
// #include "util.h"

#include <node.h>
#include "other_util.h"
#include "ivrsystem.h"

using namespace v8;

#define CHECK_ERROR(err) { if (err != vr::VROverlayError_None) { Nan::ThrowError(vr::VROverlay()->GetOverlayErrorNameFromEnum(err)); return; } }
#define HND_OVERLAY(prop) overlayHandleMap[prop->Uint32Value(Nan::GetCurrentContext()).FromJust()]

bool IVROverlay::checkError(vr::VROverlayError err, const char *msg) {
    if (err != vr::VROverlayError_None) {
        Nan::ThrowError(vr::VROverlay()->GetOverlayErrorNameFromEnum(err));
        return false;
    }

    return true;
}

std::map<uint32_t, vr::VROverlayHandle_t> IVROverlay::overlayHandleMap;

GLFWwindow* IVROverlay::glWindow = NULL;
GLuint IVROverlay::bufferTexture;

vr::Texture_t IVROverlay::getTexture(uint8_t* buf, uint32_t width, uint32_t height) {
    if (bufferTexture == 0) {
		glGenTextures(1, &bufferTexture);

		glBindTexture(GL_TEXTURE_2D, bufferTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, buf);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		GLfloat fLargest;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);

		glBindTexture(GL_TEXTURE_2D, 0);
	} else {
		glBindTexture(GL_TEXTURE_2D, bufferTexture);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_BGRA, GL_UNSIGNED_BYTE, buf);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glFlush();
	glFinish();

    vr::Texture_t tex = {(void*)(uintptr_t)bufferTexture, vr::ETextureType::TextureType_OpenGL, vr::EColorSpace::ColorSpace_Auto};
    return tex;
}

NAN_MODULE_INIT(IVROverlay::Init) {
    SET_METHOD(CreateOverlay);
    SET_METHOD(CreateDashboardOverlay);
    SET_METHOD(ShowOverlay);
    SET_METHOD(Check);
    
    // target->Set("SetOverlayRaw", SetOverlayRaw);
    SET_METHOD(SetOverlayFromFile);
    // SET_METHOD(SetOverlayRaw);
    SET_METHOD(SetOverlayTextureFromBuffer);
    
    SET_METHOD(SetOverlayTransformTrackedDeviceRelative);
    SET_METHOD(SetOverlayWidthInMeters);
    SET_METHOD(SetOverlayMouseScale);
    // target->Set("SetOverlayWidthInMeters", SetOverlayWidthInMeters);

    SET_METHOD(Internals);
}

NAN_METHOD(IVROverlay::Internals) {
    info.GetReturnValue().Set(
        #ifdef OVERLAY_D3D
        0
        #else
        1
        #endif
    );
}

// IVROverlay::IVROverlay(vr::IVROverlay *self) : self_(self) {}

NAN_METHOD(IVROverlay::New) {
    if (!info.IsConstructCall()) {
        Nan::ThrowError("Use the `new` keyword when creating a new instance.");
        return;
    }

    if (!vr::VROverlay()) {
        Nan::ThrowError("Overlay was unable to fetch the shared instance.");
    }
}

NAN_METHOD(IVROverlay::Check) {
    if (!vr::VROverlay()) {
        info.GetReturnValue().Set(false);
        Nan::ThrowError("Overlay was unable to fetch the shared instance.");
        return;
    }

    info.GetReturnValue().Set(true);
}

NAN_METHOD(IVROverlay::CreateOverlay) {
    if (info.Length() != 2) {
        Nan::ThrowError("Wrong number of arguments.");
        return;
    }

    if (glWindow == NULL) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
        GLFWwindow* glWindow = glfwCreateWindow(1, 1, "node-openvr", nullptr, nullptr);
        glfwMakeContextCurrent(glWindow);
    }

    vr::VROverlayHandle_t hOverlay = 0U;

    V8STR(info[0], key);
    V8STR(info[1], name);
    
    vr::VROverlayError err = vr::VROverlay()->CreateOverlay(key, name, &hOverlay);
    CHECK_ERROR(err);

    // set bounds to correct OpenGL's texture draw
    err = vr::VROverlay()->SetOverlayTextureBounds(hOverlay, &(vr::VRTextureBounds_t{ 0.F, 1.F, 1.F, 0.F }));

    uint32_t oHndPtr = (uint32_t)hOverlay;
    overlayHandleMap.insert(std::pair<uint32_t, vr::VROverlayHandle_t>(oHndPtr, hOverlay));
    info.GetReturnValue().Set(oHndPtr);
}

NAN_METHOD(IVROverlay::CreateDashboardOverlay) {
    if (info.Length() != 3) {
        Nan::ThrowError("Wrong number of arguments.");
        return;
    }

    if (glWindow == NULL) {
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
        GLFWwindow* glWindow = glfwCreateWindow(1, 1, "node-openvr", nullptr, nullptr);
        glfwMakeContextCurrent(glWindow);
    }

    vr::VROverlayHandle_t hOverlay = 0U;
    vr::VROverlayHandle_t hOverlayThumbnail = 0U;

    V8STR(info[0], key);
    V8STR(info[1], name);
    V8STR(info[2], thumbnailPath);
    
    vr::VROverlayError err = vr::VROverlay()->CreateDashboardOverlay(key, name, &hOverlay, &hOverlayThumbnail);
    CHECK_ERROR(err);

    // set bounds to correct OpenGL's texture draw
    err = vr::VROverlay()->SetOverlayTextureBounds(hOverlay, &(vr::VRTextureBounds_t{ 0.F, 1.F, 1.F, 0.F }));
    err = vr::VROverlay()->SetOverlayFromFile(hOverlayThumbnail, thumbnailPath);

    uint32_t oHndPtr = (uint32_t)hOverlay;
    overlayHandleMap.insert(std::pair<uint32_t, vr::VROverlayHandle_t>(oHndPtr, hOverlay));
    info.GetReturnValue().Set(oHndPtr);
}

NAN_METHOD(IVROverlay::ShowOverlay) {
    if (info.Length() != 1) {
        Nan::ThrowError("Wrong number of arguments.");
        return;
    }

    vr::VROverlayError err;    
    err = vr::VROverlay()->ShowOverlay(HND_OVERLAY(info[0]));
    
    CHECK_ERROR(err);
}

NAN_METHOD(IVROverlay::SetOverlayFromFile) {
    if (info.Length() != 2) {
        Nan::ThrowError("Wrong number of arguments.");
        return;
    }

    V8STR(info[1], path);

    vr::VROverlayError err;
    err = vr::VROverlay()->SetOverlayFromFile(HND_OVERLAY(info[0]), path);
    CHECK_ERROR(err);
}

NAN_METHOD(IVROverlay::SetOverlayTextureFromBuffer) {
    if (info.Length() < 4) {
        Nan::ThrowError("Wrong number of arguments.");
        return;
    }

    if (!info[1]->IsArrayBufferView()) {
        Nan::ThrowTypeError("Argument 2 must be an ArrayBufferView, such as a Uint8ClampedArray");
        return;
    }

    Local<ArrayBufferView> input = info[1].As<ArrayBufferView>();

    uint32_t width = info[2]->Uint32Value(Nan::GetCurrentContext()).FromJust();
    uint32_t height = info[3]->Uint32Value(Nan::GetCurrentContext()).FromJust();

    Nan::TypedArrayContents<uint8_t> buf(input);

    vr::Texture_t tex = getTexture(*buf, width, height);
    vr::VROverlayError err;
    err = vr::VROverlay()->SetOverlayTexture(HND_OVERLAY(info[0]), &tex);
    
    CHECK_ERROR(err);
}

NAN_METHOD(IVROverlay::SetOverlayTransformTrackedDeviceRelative) {
    vr::TrackedDeviceIndex_t trackedDevice = info[1]->Uint32Value(Nan::GetCurrentContext()).FromJust();
    vr::HmdMatrix34_t transform = decodeVec3x4(info[2]);

    vr::VROverlayError err;
    err = vr::VROverlay()->SetOverlayTransformTrackedDeviceRelative(HND_OVERLAY(info[0]), trackedDevice, &transform);
    checkError(err, "SetOverlayTransformTrackedDeviceRelative failed");
}

NAN_METHOD(IVROverlay::SetOverlayWidthInMeters) {
    if (info.Length() != 2) {
        Nan::ThrowError("Wrong number of arguments.");
        return;
    }

    uint32_t width = info[1]->Uint32Value(Nan::GetCurrentContext()).FromJust();

    vr::VROverlayError err;
    err = vr::VROverlay()->SetOverlayWidthInMeters(HND_OVERLAY(info[0]), width);
    checkError(err, "SetOverlayWidthInMeters failed");
}

NAN_METHOD(IVROverlay::SetOverlayMouseScale) {
    if (info.Length() != 3) {
        Nan::ThrowError("Wrong number of arguments.");
        return;
    }

    vr::HmdVector2_t vecWindowSize =
    {
        info[1]->Uint32Value(Nan::GetCurrentContext()).FromJust(),
        info[2]->Uint32Value(Nan::GetCurrentContext()).FromJust()
    };
    vr::VROverlayError err;
    err = vr::VROverlay()->SetOverlayMouseScale(HND_OVERLAY(info[0]), &vecWindowSize);
    checkError(err, "SetOverlayMouseScale failed");
}