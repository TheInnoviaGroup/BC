/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.WindowGroup=function(){var _1={};var _2=[];var _3=null;var _4=function(d1,d2){return (!d1._lastAccess||d1._lastAccess<d2._lastAccess)?-1:1;};var _7=function(){_2.sort(_4);var _8=Ext.DialogManager.zseed;for(var i=0,_a=_2.length;i<_a;i++){var _b=_2[i];if(_b&&!_b.hidden){_b.setZIndex(_8+(i*10));}}_c();};var _d=function(_e){if(_e!=_3){if(_3){_3.setActive(false);}_3=_e;if(_e){_e.setActive(true);}}};var _c=function(){for(var i=_2.length-1;i>=0;--i){if(!_2[i].hidden){_d(_2[i]);return;}}_d(null);};return {zseed:9000,register:function(win){_1[win.id]=win;_2.push(win);win.on("hide",_c);},unregister:function(win){delete _1[win.id];win.un("hide",_c);_2.remove(win);},get:function(id){return typeof id=="object"?id:_1[id];},bringToFront:function(win){win=this.get(win);if(win!=_3){win._lastAccess=new Date().getTime();_7();}return win;},sendToBack:function(win){win=this.get(win);win._lastAccess=-(new Date().getTime());_7();return win;},hideAll:function(){for(var id in _1){if(_1[id]&&typeof _1[id]!="function"&&_1[id].isVisible()){_1[id].hide();}}},getBy:function(fn,_17){var r=[];for(var i=_2.length-1;i>=0;--i){var win=_2[i];if(fn.call(_17||win,win)!==false){r.push(win);}}return r;}};};Ext.WindowMgr=new Ext.WindowGroup();
