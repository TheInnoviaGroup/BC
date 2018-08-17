/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.Viewport=Ext.extend(Ext.Container,{initComponent:function(){Ext.Viewport.superclass.initComponent.call(this);this.el=Ext.get(document.body);this.el.setStyle({overflow:"hidden",margin:"0",border:"0 none"});this.el.dom.scroll="no";this.allowDomMove=false;this.autoWidth=true;this.autoHeight=true;Ext.EventManager.onWindowResize(this.fireResize,this);this.renderTo=this.el;},fireResize:function(w,h){this.fireEvent("resize",w,h);}});
