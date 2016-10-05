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
  using v8::Exception;
  using v8::Number;

  void AddTwoNumbers(const FunctionCallbackInfo<Value>& args) {
    // The Isolate pointer representing the V8 JS engine instance.
    Isolate* isolate = args.GetIsolate();

    // We require at least two parameters. If we have less than that, throw an exception.
    if (args.Length() < 2) {
      // When throwing the exception, we also pass it a v8::String object containing the message
      // we want for our exception. String::NewFromUtf8 reads the provided C string and makes it
      // usable to the V8 runtime.
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "You must pass two numbers.")));
      return;
    }

    // Since we are adding numbers, not objects or strings, we require that the first two arguments
    // be of Number type. If they aren't, we throw a type error with a message.
    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
      isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "You must pass in only number values.")));
      return;
    }

    // Once validation is complete, we convert the first two arguments to v8::Number objects. v8::Number provides
    // a Value() method that returns the actual numerical value as a C++ double type. The double type is used because
    // the number passed in could be a whole or factional number.
    double returnValue = args[0]->ToNumber()->Value() + args[1]->ToNumber()->Value();

    // We set the return value of the function. Since we are returning a Node.js number, we need to covert out
    // C++ double value into a Number object. We do this via the Number::New() method which takes the Isolate
    // and the double value as arguments. This associates the Number object with the V8 runtime pointed to by
    // the Isolate and sets the value of the Number object to what is contained in the returnValue double.
    args.GetReturnValue().Set(Number::New(isolate, returnValue));
  }

  void AddMultipleNumbers(const FunctionCallbackInfo<Value>& args) {
    // The Isolate pointer representing the V8 JS engine instance.
    Isolate* isolate = args.GetIsolate();

    // We require at least two parameters. If we have less than that, throw an exception.
    if (args.Length() < 2) {
      // When throwing the exception, we also pass it a v8::String object containing the message
      // we want for our exception. String::NewFromUtf8 reads the provided C string and makes it
      // usable to the V8 runtime.
      isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, "You must pass at least two numbers.")));
      return;
    }

    // The returnValue variable will serve as an accumulator. We will be adding each passed in number
    // into it and using its final value as the return of the function.
    double returnValue = 0;

    // Since this addon method takes a variable number of arguments, we must loop through each to
    // add in their value.
    for(int i = 0; i < args.Length(); i++) {
      // We require only numbers! If we find an argument that is not a number, we throw a type exception.
      if (!args[i]->IsNumber()) {
        isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "You must pass in only number values.")));
        return;
      }

      // After we validate that this argument is actually a number, convert it to a v8::Number object,
      // get its C++ double value via the Value() method, and add it to sum stored in returnValue;
      returnValue += args[i]->ToNumber()->Value();
    }

    // We set the return value of the function. Since we are returning a Node.js number, we need to covert out
    // C++ double value into a Number object. We do this via the Number::New() method which takes the Isolate
    // and the double value as arguments. This associates the Number object with the V8 runtime pointed to by
    // the Isolate and sets the value of the Number object to what is contained in the returnValue double.
    args.GetReturnValue().Set(Number::New(isolate, returnValue));
  }

  void init(Local<Object> exports) {
    // We register the C++ functions as methods to this addon. The second argument to
    // NODE_SET_METHOD is a string that defines what the method name will be in Node.js,
    // and the third argument is the name of the C++ function that will be called. It
    // is not required that they be the same, but it is good practice.
    NODE_SET_METHOD(exports, "addTwoNumbers", AddTwoNumbers);
    NODE_SET_METHOD(exports, "addMultipleNumbers", AddMultipleNumbers);
  }

  // This is a C++ macro provided by node.h that registers the addon with
  // the V8 runtime, making it avaialble to Node.js when require'd in the code.
  NODE_MODULE(addon, init)
}
