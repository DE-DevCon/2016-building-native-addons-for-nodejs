// node.h contains all of the function definitions needed to interface with
// Node and the V8 runtime as well as registration functions.
#include <node.h>
#include <uv.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace node_cpp_tutorial {
  // Typing v8 in front of everything takes up a lot of extra space. We
  // want to bring these V8 objects directly into our namespace.
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Persistent;
  using v8::Object;
  using v8::String;
  using v8::Value;
  using v8::Exception;
  using v8::Number;
  using v8::Null;
  using v8::Function;
  using v8::HandleScope;

  // This structure is used to keep important data available to the code running on different
  // threads during asyncronous sleep.
  struct WorkInfo {
      uv_work_t request;
      uint32_t sleepTime;
      Persistent<Function> jsCallback;
  };

  void SleepCallback(const FunctionCallbackInfo<Value>& args) {
    // The Isolate pointer representing the V8 JS engine instance.
    Isolate* isolate = args.GetIsolate();

    if (args.Length() < 2) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "You must pass in the sleep time and a callback function")));
        return;
    }

    if (!args[0]->IsUint32()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Your first argument must be an integer.")));
        return;
    }

    if (!args[1]->IsFunction()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Your second argument must be a callback function.")));
        return;
    }

    uint32_t sleepTime = args[0]->ToUint32()->Value();
#ifdef _WIN32
    Sleep(sleepTime);
#else
    usleep(sleepTime * 1000);
#endif

    // Fire the callback function that was provided. Pass the callback the total
    // time we have slept.
    Local<Function> cb = Local<Function>::Cast(args[1]);
    cb->Call(Null(isolate), 0, 0);
  }

  void SleepAsync(uv_work_t* request) {
      WorkInfo* info = static_cast<WorkInfo*>(request->data);
#ifdef _WIN32
      Sleep(info->sleepTime);
#else
      usleep(info->sleepTime * 1000);
#endif
  }

  void SleepAsyncComplete(uv_work_t* request, int status) {
      Isolate* isolate = Isolate::GetCurrent();
      HandleScope handleScope(isolate);
      WorkInfo* info = static_cast<WorkInfo*>(request->data);

      Local<Function> callback = Local<Function>::New(Isolate::GetCurrent(), info->jsCallback);
      callback->Call(isolate->GetCurrentContext()->Global(), 0, 0);
  }

  void SleepCallbackAsync(const FunctionCallbackInfo<Value>& args) {
    // The Isolate pointer representing the V8 JS engine instance.
    Isolate* isolate = args.GetIsolate();

    if (args.Length() < 2) {
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "You must pass in the sleep time and a callback function")));
        return;
    }

    if (!args[0]->IsUint32()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Your first argument must be an integer.")));
        return;
    }

    if (!args[1]->IsFunction()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Your second argument must be a callback function.")));
        return;
    }

    WorkInfo* info = new WorkInfo();
    info->sleepTime = args[0]->ToUint32()->Value();
    info->request.data = info;

    Local<Function> callback = Local<Function>::Cast(args[1]);
    info->jsCallback.Reset(isolate, callback);

    uv_queue_work(uv_default_loop(), &info->request, SleepAsync, SleepAsyncComplete);
  }

  void init(Local<Object> exports) {
    // We register the C++ functions as methods to this addon. The second argument to
    // NODE_SET_METHOD is a string that defines what the method name will be in Node.js,
    // and the third argument is the name of the C++ function that will be called. It
    // is not required that they be the same, but it is good practice.
    NODE_SET_METHOD(exports, "sleepCallback", SleepCallback);
    NODE_SET_METHOD(exports, "sleepCallbackAsync", SleepCallbackAsync);
  }

  // This is a C++ macro provided by node.h that registers the addon with
  // the V8 runtime, making it avaialble to Node.js when require'd in the code.
  NODE_MODULE(addon, init)
}
