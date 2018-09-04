#ifndef NODE_IVROVERLAY_H
#define NODE_IVROVERLAY_H

#include <nan.h>
#include <map>

#include <GLFW/glfw3.h>
#include <SDL_opengl.h>

#include <v8.h>
#include <openvr.h>


class IVROverlay : public Nan::ObjectWrap {
    public:
        static NAN_MODULE_INIT(Init);
        // static v8::Local<v8::Object> NewInstance(vr::IVROverlay *overlay);
    
    private:
        explicit IVROverlay(vr::IVROverlay *self);
        ~IVROverlay() = default;

        static std::map<uint32_t, vr::VROverlayHandle_t> overlayHandleMap;
        
        static GLFWwindow* glWindow;
        static GLuint bufferTexture;

        static NAN_METHOD(New);
        static NAN_METHOD(Check);
        static NAN_METHOD(Internals);

        static NAN_METHOD(CreateOverlay);
        static NAN_METHOD(CreateDashboardOverlay);

        // static NAN_METHOD(SetOverlayAlpha);
        static NAN_METHOD(SetOverlayTextureFromBuffer);
        static NAN_METHOD(SetOverlayFromFile);
        // static NAN_METHOD(SetOverlayTransformAbsolute);
        static NAN_METHOD(SetOverlayTransformTrackedDeviceRelative);
        static NAN_METHOD(SetOverlayWidthInMeters);
 
        static NAN_METHOD(ShowOverlay);

        #ifdef TEST_METHODS
        static NAN_METHOD(TestHandle);
        #endif

        static inline Nan::Persistent<v8::Function>& constructor() {
            static Nan::Persistent<v8::Function> the_constructor;
            return the_constructor;
        }


        // vr::IVROverlay * const self_;
        static bool checkError(vr::VROverlayError err, const char* v);
        static vr::Texture_t getTexture(uint8_t* buffer, uint32_t width, uint32_t height);
}


#endif