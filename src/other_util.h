#ifndef NODE_OTHER_UTIL_H
#define NODE_OTHER_UTIL_H

#include <nan.h>
#include <openvr.h>
#include <v8.h>

;

#define SET_METHOD(name) target->Set(Nan::New(#name).ToLocalChecked(), Nan::New<v8::FunctionTemplate>(name)->GetFunction());

#define V8STR(prop, name) char * name = *Nan::Utf8String(prop);
#define V8INT(prop, name) uint32_t name = prop->Uint32Value();

inline vr::HmdMatrix34_t decodeVec3x4(const v8::Local<v8::Value> value) {
  vr::HmdMatrix34_t result;
  const auto matrix = value->ToObject();

  for (uint32_t rowIdx = 0; rowIdx < 3; ++rowIdx)
  {
    const auto row = Nan::Get(matrix, rowIdx).ToLocalChecked()->ToObject();
    for (uint32_t colIdx = 0; colIdx < 4; ++colIdx)
    {
      const auto cell = Nan::Get(row, colIdx).ToLocalChecked();
      result.m[rowIdx][colIdx] = static_cast<float>(cell->NumberValue());
    }
  }

  return result;
}

#endif