/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.StyleAnim=function(_1){var o=_1;if(typeof _1=="string"){o={};var re=Ext.StyleAnim.styleRE;var m;while((m=re.exec(_1))!=null){o[m[1]]=m[2];}}this.styles=o;if(!Ext.StyleAnim.measureEl){var el=document.createElement("div");el.style.position="absolute";el.style.top="-500px";el.style.left="-500px";el.style.width="0px";el.style.visibility="hidden";document.body.appendChild(el);Ext.StyleAnim.measureEl=Ext.get(el);}};Ext.StyleAnim.styleRE=/\s?([a-z\-]*)\:([^;]*);?/gi;Ext.StyleAnim.measurables=["border","border-width","border-left","border-right","border-top","border-bottom","border-left-width","border-right-width","border-top-width","border-bottom-width","padding-left","padding-right","padding-top","padding-bottom"];Ext.StyleAnim.copyStyles=["border-left-width","border-right-width","border-top-width","border-bottom-width","padding-left","padding-right","padding-top","padding-bottom"];Ext.StyleAnim.prototype={apply:function(el,_7){var o=this.styles;var el=Ext.get(el);var _9=["width","height"];if(el.autoBoxAdjust&&!el.isBorderBox()){var up=Ext.Element.unitPattern;var _b;for(var i=0,_d=_9.length;i<_d;i++){var _e=_9[i];if(o[_e]!==undefined){var m=String(o[_e]).match(up);if(!m||m[1]=="px"){if(!_b){_b=this.getAdjustments(el,o);}var v=Math.max(0,(parseInt(o[_e],10)||0)-_b[0]);o[_e]=v;}}}}else{for(var i=0,_d=_9.length;i<_d;i++){var _e=_9[i];if(o[_e]!==undefined){o[_e]=Math.max(0,parseInt(o[_e],10)||0);}}}var _e={};for(var k in o){if(typeof o[k]!="function"){_e[k]={to:o[k]};}}alert(Ext.util.JSON.encode(_e));new YAHOO.util.Anim(el.dom,_e,0.5).animate();return this;},getAdjustments:function(el,o){var mel=Ext.StyleAnim.measureEl;var ms=Ext.StyleAnim.measurables;mel.copyStyles(el,Ext.StyleAnim.copyStyles);for(var i=0,len=ms.length;i<len;i++){var s=ms[i];if(o[s]){mel.setStyle(s,o[s]);}}return [mel.getBorderWidth("lr")+mel.getPadding("lr"),mel.getBorderWidth("tb")+mel.getPadding("tb")];}};Ext.ClassAnim=function(_19){var _1a=Ext.util.CSS.getRule(_19);var s=_1a.style;var _1c={};for(var key in s){if(s[key]&&typeof s[key]!="function"&&String(s[key]).indexOf(":")<0&&s[key]!="false"){_1c[key]=s[key];}}Ext.ClassAnim.superclass.constructor.call(this,_1c);};Ext.extend(Ext.ClassAnim,Ext.StyleAnim);
