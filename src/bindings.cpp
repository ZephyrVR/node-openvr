#include "ivrsystem.h"
#include "ivrcompositor.h"
#include "ivroverlay.h"
#include "ivrnotifications.h"
#include "openvr.h"

#include <nan.h>

void Initialize(v8::Local<v8::Object> exports) {
  v8::Local<v8::Object> system = v8::Object::New(v8::Isolate::GetCurrent());
  system->Set(Nan::New("VR_Init").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_Init)->GetFunction());
  system->Set(Nan::New("VR_Shutdown").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_Shutdown)->GetFunction());
  system->Set(Nan::New("VR_IsHmdPresent").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_IsHmdPresent)->GetFunction());
  system->Set(Nan::New("VR_IsRuntimeInstalled").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_IsRuntimeInstalled)->GetFunction());
  system->Set(Nan::New("VR_RuntimePath").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_RuntimePath)->GetFunction());
  system->Set(Nan::New("VR_GetVRInitErrorAsSymbol").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_GetVRInitErrorAsSymbol)->GetFunction());
  system->Set(Nan::New("VR_GetVRInitErrorAsEnglishDescription").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_GetVRInitErrorAsEnglishDescription)->GetFunction());
  system->Set(Nan::New("VR_GetInitToken").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_GetInitToken)->GetFunction());
  IVRSystem::Init(system);
  exports->Set(Nan::New("system").ToLocalChecked(), system);

  v8::Local<v8::Object> compositor = v8::Object::New(v8::Isolate::GetCurrent());
  compositor->Set(Nan::New("NewCompositor").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(NewCompositor)->GetFunction());
  IVRCompositor::Init(compositor);
  exports->Set(Nan::New("compositor").ToLocalChecked(), compositor);

  v8::Local<v8::Object> overlay = v8::Object::New(v8::Isolate::GetCurrent());
  IVROverlay::Init(overlay);
  exports->Set(Nan::New("overlay").ToLocalChecked(), overlay);

  v8::Local<v8::Object> notifications = v8::Object::New(v8::Isolate::GetCurrent());
  IVRNotifications::Init(notifications);
  exports->Set(Nan::New("notifications").ToLocalChecked(), notifications);
}

NODE_MODULE(openvr, Initialize);
