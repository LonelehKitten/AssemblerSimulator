'use strict';

const EventEmitter = require('events').EventEmitter;
const addon = require('bindings')('ASMR.node');
//const addon = require('./build/Release/ASMR.node');
const emitter = new EventEmitter();

/*
let myPromise = new Promise(function(myResolve, myReject) {
  addon.requestTest();
  myResolve(); // when successful
  myReject();  // when error
});
  
myPromise.then(
  function(value) {  },
  function(error) {  }
);

addon.requestEndTest();
*/

emitter.on("log", (data) => {
    console.log(JSON.parse(data).message);
})

addon.init(emitter.emit.bind(emitter));

console.log("-- post init")

addon.requestEndTest();

console.log("-- referencial")