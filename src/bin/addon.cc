#include <nan.h>

 
//#include <napi.h>

#include <chrono>
#include <iostream>
#include <thread>

void trigger(char * event, v8::Local<v8::Function>& callback, int numArgs) {
    v8::Local<v8::Value> arguments[numArgs] = {Nan::New(event).ToLocalChecked()};
    Nan::AsyncResource resource("nan:makeCallback");
    resource.runInAsyncScope(Nan::GetCurrentContext()->Global(), callback, numArgs, arguments);
}

void CallEmit(const Nan::FunctionCallbackInfo<v8::Value> & info) {
  /*
  Nan::Env env = info.Env();
  Nan::Function emit = info[0].As<Nan::Function>();
  emit.Call({Nan::String::New(env, "start")});
  for (int i = 0; i < 3; i++) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    emit.Call(
        {Nan::String::New(env, "data"), Nan::String::New(env, "data ...")});
  }
  emit.Call({Nan::String::New(env, "end")});
  return Nan::String::New(env, "OK");
  */
    //v8::Local<v8::Context> context = info.GetIsolate()->GetCurrentContext();

    v8::Local<v8::Function> emitter = info[0].As<v8::Function>();
    trigger("start", emitter, 1);
}

// Init
void Init(v8::Local<v8::Object> exports) {

  v8::Local<v8::Context> context = exports->CreationContext();
  exports->Set(context,
               Nan::New("callEmit").ToLocalChecked(),
               Nan::New<v8::FunctionTemplate>(CallEmit)
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