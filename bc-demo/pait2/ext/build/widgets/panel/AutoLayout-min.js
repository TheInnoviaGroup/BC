/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.layout.FitLayout=Ext.extend(Ext.layout.ContainerLayout,{monitorResize:true,onLayout:function(ct,_2){Ext.layout.FitLayout.superclass.onLayout.call(this,ct,_2);var _3=this.activeItem||ct.items.itemAt(0);if(_3){_3.setSize(_2.getSize(true));}}});Ext.layout.CardLayout=Ext.extend(Ext.layout.FitLayout,{renderHidden:true,setActiveItem:function(_4){_4=this.container.getComponent(_4);if(this.activeItem!=_4){if(this.activeItem){this.activeItem.hide();}this.activeItem=_4;_4.show();this.layout();}}});
