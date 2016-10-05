// node.h contains all of the function definitions needed to interface with
// Node and the V8 runtime as well as registration functions.
#include <node.h>

namespace node_cpp_tutorial {
  // Typing v8 in front of everything takes up a lot of extra space. We
  // want to bring these V8 objects directly into our namespace.
  using v8::FunctionCallbackInfo;
  using v8::Isolate;
  using v8::Local;
  using v8::Object;
  using v8::String;
  using v8::Value;

  void GetHello(const FunctionCallbackInfo<Value>& args) {
    // The Isolate pointer representing the V8 JS engine instance.
    Isolate* isolate = args.GetIsolate();

    // We set the return value of the function. Since we are returning a Node.js string, we need to covert our
    // C++ string value into a String object. We do this via the String::NewFromUtf8() method which takes the Isolate
    // and the C string value as arguments. This associates the String object with the V8 runtime pointed to by
    // the Isolate and sets the value of the String object to the passed in C string.
    args.GetReturnValue().Set(String::NewFromUtf8(isolate, "Hello World!"));
  }

  void init(Local<Object> exports) {
    // We register the C++ functions as methods to this addon. The second argument to
    // NODE_SET_METHOD is a string that defines what the method name will be in Node.js,
    // and the third argument is the name of the C++ function that will be called. It
    // is not required that they be the same, but it is good practice.
    NODE_SET_METHOD(exports, "getHello", GetHello);
  }

  // This is a C++ macro provided by node.h that registers the addon with
  // the V8 runtime, making it avaialble to Node.js when require'd in the code.
  NODE_MODULE(addon, init)
}
