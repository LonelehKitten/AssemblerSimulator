'use strict';

const EventEmitter = require('events').EventEmitter;
const addon = require('bindings')('ASMR.node');
//const addon = require('./build/Release/ASMR.node');
const emitter = new EventEmitter();

let myPromise = new Promise(function(myResolve, myReject) {
  addon.requestTest();
  myResolve(); // when successful
  myReject();  // when error
});
  
myPromise.then(
  function(value) { /* code if successful */ },
  function(error) { /* code if some error */ }
);

addon.requestEndTest();

console.log("-- referencial")