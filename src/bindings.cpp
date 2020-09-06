#include "ivrsystem.h"
#include "ivrcompositor.h"
#include "ivroverlay.h"
#include "ivrnotifications.h"
#include "openvr.h"

#include <nan.h>

void Initialize(v8::Local<v8::Object> exports) {
  v8::Local<v8::Object> system = v8::Object::New(v8::Isolate::GetCurrent());
  system->Set(Nan::GetCurrentContext(), Nan::New("VR_Init").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_Init)->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());
  system->Set(Nan::GetCurrentContext(), Nan::New("VR_Shutdown").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_Shutdown)->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());
  system->Set(Nan::GetCurrentContext(), Nan::New("VR_IsHmdPresent").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_IsHmdPresent)->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());
  system->Set(Nan::GetCurrentContext(), Nan::New("VR_IsRuntimeInstalled").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_IsRuntimeInstalled)->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());
  system->Set(Nan::GetCurrentContext(), Nan::New("VR_RuntimePath").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_RuntimePath)->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());
  system->Set(Nan::GetCurrentContext(), Nan::New("VR_GetVRInitErrorAsSymbol").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_GetVRInitErrorAsSymbol)->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());
  system->Set(Nan::GetCurrentContext(), Nan::New("VR_GetVRInitErrorAsEnglishDescription").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_GetVRInitErrorAsEnglishDescription)->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());
  system->Set(Nan::GetCurrentContext(), Nan::New("VR_GetInitToken").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(VR_GetInitToken)->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());
  IVRSystem::Init(system);
  exports->Set(Nan::GetCurrentContext(), Nan::New("system").ToLocalChecked(), system);

  v8::Local<v8::Object> compositor = v8::Object::New(v8::Isolate::GetCurrent());
  compositor->Set(Nan::GetCurrentContext(), Nan::New("NewCompositor").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(NewCompositor)->GetFunction(Nan::GetCurrentContext()).ToLocalChecked());
  IVRCompositor::Init(compositor);
  exports->Set(Nan::GetCurrentContext(), Nan::New("compositor").ToLocalChecked(), compositor);

  v8::Local<v8::Object> overlay = v8::Object::New(v8::Isolate::GetCurrent());
  IVROverlay::Init(overlay);
  exports->Set(Nan::GetCurrentContext(), Nan::New("overlay").ToLocalChecked(), overlay);

  v8::Local<v8::Object> notifications = v8::Object::New(v8::Isolate::GetCurrent());
  IVRNotifications::Init(notifications);
  exports->Set(Nan::GetCurrentContext(), Nan::New("notifications").ToLocalChecked(), notifications);
}

NODE_MODULE(openvr, Initialize);
