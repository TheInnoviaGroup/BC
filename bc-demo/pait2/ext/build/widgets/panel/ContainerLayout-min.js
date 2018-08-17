/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.layout.ContainerLayout=function(_1){Ext.apply(this,_1);};Ext.layout.ContainerLayout.prototype={monitorResize:false,activeItem:null,layout:function(){this.onLayout(this.container,this.container.getLayoutTarget());},onLayout:function(ct,_3){var _4=ct.items.items;for(var i=0,_6=_4.length;i<_6;i++){var c=_4[i];if(!c.rendered){c.render(_3,i);if(this.renderHidden&&c!=this.activeItem){c.hide();}}}},onResize:function(){this.layout();},setContainer:function(ct){if(this.monitorResize){if(ct&&this.container&&ct!=this.container){this.container.un("resize",this.onResize,this);}ct.on("resize",this.onResize,this);}this.container=ct;}};
