// Pull in our compiled callback module. ./build/Release is the default directory
// gyp sets up to send the final compiled module.
var callback = require('./build/Release/callback.node');

console.log("Calling blocking-sleep.");
callback.sleepCallback(5000, () => {
    console.log("Done with blocking sleep.");
});

console.log("We are now past the sleepCallback call.\n\n");

console.log("Calling non-blocking sleep.");
callback.sleepCallbackAsync(5000, () => {
    console.log("Done with non-blocking sleep.");
});

console.log("We are now past the sleepCallbackAync call.");
