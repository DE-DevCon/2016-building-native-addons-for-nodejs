// Pull in our compiled argpassing module. ./build/Release is the default directory
// gyp sets up to send the final compiled module.
var argpassing = require('./build/Release/argpassing.node');

// Here, we call the addTwoNumbers method on our arg passing module. The module
// expects at least two number arguments. Any other arguments passed after those
// are ignored by the module code.
console.log("10 + 20 = " + argpassing.addTwoNumbers(10, 20));

// Here we call the addMultipleNumbers method on our arg passing module. This
// method also expects at least two numbers, but it supports a dynamic number
// of arguments, adding all numbers together. All arguments must be number types
// or the module will throw a type error as expected.
console.log("20 + 30 + 100 + 500 = " + argpassing.addMultipleNumbers(20, 30, 100, 500));
