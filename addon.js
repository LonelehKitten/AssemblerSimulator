'use strict';

const EventEmitter = require('events').EventEmitter;
const addon = require('bindings')('addon.node');
//const addon = require('./build/Release/addon.node');
const emitter = new EventEmitter();

emitter.on('success', (evt) => {
  console.log('### START ... ' + evt);
});
emitter.on('data', (evt) => {
  console.log(evt);
});

emitter.on('end', () => {
  console.log('### END ###');
});

addon.expandMacros('ADD AX, DX\n MUL SI', emitter.emit.bind(emitter));
//addon.callEmit('hello');

/*
var addon = require('bindings')('addon.node')

console.log('This should be eight:', addon.add(3, 5))

*/
