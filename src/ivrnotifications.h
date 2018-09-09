#ifndef NODE_IVRNOTIFICATIONS_H
#define NODE_IVRNOTIFICATIONS_H

#include <nan.h>
#include <map>

#include <GLFW/glfw3.h>
#include <SDL_opengl.h>

#include <v8.h>
#include <openvr.h>


class IVRNotifications : public Nan::ObjectWrap {
    public:
        static NAN_MODULE_INIT(Init);
    
    private:
        explicit IVRNotifications(vr::IVRNotifications *self);
        ~IVRNotifications() = default;

        static NAN_METHOD(New);
        static NAN_METHOD(Check);
        static NAN_METHOD(Internals);

        static NAN_METHOD(CreateNotification);
        static NAN_METHOD(RemoveNotification);

        #ifdef TEST_METHODS
        static NAN_METHOD(TestHandle);
        #endif

        static inline Nan::Persistent<v8::Function>& constructor() {
            static Nan::Persistent<v8::Function> the_constructor;
            return the_constructor;
        }

        static bool checkError(vr::EVRNotificationError err);
};

#endif // NODE_IVRNOTIFICATIONS_H
