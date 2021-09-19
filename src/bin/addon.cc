#include <nan.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <string>

#include "sources/analyzer/RecognitionManager.h"

void trigger(char * event, v8::Local<v8::Function>& callback, int numArgs, int nSemantics) {
    v8::Local<v8::Value> arguments[numArgs] = {Nan::New(event).ToLocalChecked(),Nan::New(nSemantics)}; // .ToLocalChecked()
    Nan::AsyncResource resource("nan:makeCallback");
    resource.runInAsyncScope(Nan::GetCurrentContext()->Global(), callback, numArgs, arguments);
}

std::string castV8String(const Nan::FunctionCallbackInfo<v8::Value> &args)
{
  v8::Isolate* isolate = args.GetIsolate();
  v8::String::Utf8Value str(isolate, args[0]);
  return std::string(*str);
}

void expandMacros(const Nan::FunctionCallbackInfo<v8::Value> & info) {

    v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();
  
    v8::Local<v8::String> text = info[0]->ToString(context).ToLocalChecked();
    v8::Local<v8::Function> emit = info[1].As<v8::Function>();

    RecognitionManager * rm = new RecognitionManager();

    std::vector<Semantic *> * s_arr = rm->analyze(castV8String(info), false);

    if(s_arr != nullptr)
      trigger("success", emit, 2, s_arr->size());

    rm->~RecognitionManager();
}

// Init
void Init(v8::Local<v8::Object> exports) {

  v8::Local<v8::Context> context = exports->CreationContext();
  exports->Set(context,
               Nan::New("expandMacros").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(expandMacros)
                   ->GetFunction(context)
                   .ToLocalChecked());

}

NODE_MODULE(addon, Init)

/*
void Add(const Nan::FunctionCallbackInfo<v8::Value> & info) {
  v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();

  if (info.Length() < 2) {
    Nan::ThrowTypeError("Wrong number of arguments");
    return;
  }

  if (!info[0]->IsNumber() || !info[1]->IsNumber()) {
    Nan::ThrowTypeError("Wrong arguments");

    return;
  }

  double arg0 = info[0]->NumberValue(context).FromJust();
  double arg1 = info[1]->NumberValue(context).FromJust();
  v8::Local<v8::Number> num = Nan::New(arg0 + arg1);

  info.GetReturnValue().Set(num);
}

void Init(v8::Local<v8::Object> exports) {
  v8::Local<v8::Context> context = exports->CreationContext();
  exports->Set(context,
               Nan::New("add").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(Add)
                   ->GetFunction(context)
                   .ToLocalChecked());
}

NODE_MODULE(addon, Init)

*/