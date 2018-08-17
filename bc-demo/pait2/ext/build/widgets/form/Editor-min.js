/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.form.HtmlEditor=Ext.extend(Ext.form.Field,{validationEvent:false,deferHeight:true,editorTbEnabled:false,enableFormat:true,enableFontSize:true,enableColors:true,enableAlignments:true,enableLists:true,enableSourceEdit:true,sourceEditMode:false,onFocus:Ext.emptyFn,defaultAutoCreate:{tag:"textarea",style:"width:500px;height:300px;",autocomplete:"off"},initialized:false,onRender:function(ct,_2){Ext.form.HtmlEditor.superclass.onRender.call(this,ct,_2);this.el.dom.style.border="0 none";this.el.addClass("x-hidden");if(Ext.isIE){this.el.applyStyles("margin-top:-1px;margin-bottom:-1px;");}this.wrap=this.el.wrap({cls:"x-html-editor-wrap",cn:{cls:"x-html-editor-tb"}});var _3=this;function btn(id,_5,_6){return {id:id,cls:"x-btn-icon x-edit-"+id,enableToggle:_5!==false,scope:_3,handler:_6||_3.relayCmd,clickEvent:"mousedown",tooltip:_3.buttonTips[id]||undefined};}var tb=new Ext.Toolbar(this.wrap.dom.firstChild);if(this.enableFormat){tb.add(btn("bold"),btn("italic"),btn("underline"));}if(this.enableFontSize){tb.add("-",btn("increasefontsize",false,this.adjustFont),btn("decreasefontsize",false,this.adjustFont));}if(this.enableColors){tb.add("-",{id:"forecolor",cls:"x-btn-icon x-edit-forecolor",clickEvent:"mousedown",tooltip:_3.buttonTips["forecolor"]||undefined,menu:new Ext.menu.ColorMenu({focus:Ext.emptyFn,value:"000000",plain:true,selectHandler:function(cp,_9){this.execCmd("forecolor",Ext.isSafari||Ext.isIE?"#"+_9:_9);this.deferFocus();},scope:this,clickEvent:"mousedown"})},{id:"backcolor",cls:"x-btn-icon x-edit-backcolor",clickEvent:"mousedown",tooltip:_3.buttonTips["backcolor"]||undefined,menu:new Ext.menu.ColorMenu({focus:Ext.emptyFn,value:"FFFFFF",plain:true,selectHandler:function(cp,_b){if(Ext.isGecko){this.execCmd("useCSS",false);this.execCmd("hilitecolor",_b);this.execCmd("useCSS",true);this.deferFocus();}else{this.execCmd(Ext.isOpera?"hilitecolor":"backcolor",Ext.isSafari||Ext.isIE?"#"+_b:_b);this.deferFocus();}},scope:this,clickEvent:"mousedown"})});}if(this.enableAlignments){tb.add("-",btn("justifyleft"),btn("justifycenter"),btn("justifyright"));}if(!Ext.isSafari){if(this.enableLists){tb.add("-",btn("insertorderedlist"),btn("insertunorderedlist"));}if(this.enableSourceEdit){tb.add("-",btn("sourceedit",true,this.toggleSourceEdit));}}tb.items.each(function(_c){if(_c.id!="sourceedit"){_c.disable();}});this.tb=tb;var _d=document.createElement("iframe");_d.name=Ext.id();_d.frameBorder="no";_d.src="javascript:false";this.wrap.dom.appendChild(_d);this.iframe=_d;this.doc=Ext.isIE?_d.contentWindow.document:(_d.contentDocument||window.frames[_d.name].document);this.win=Ext.isIE?_d.contentWindow:window.frames[_d.name];this.doc.designMode="on";this.doc.open();this.doc.write("<html><head><style type=\"text/css\">body{border:0;margin:0;padding:3px;cursor:text;}</style></head><body></body></html>");this.doc.close();var _e={run:function(){if(this.doc.body||this.doc.readyState=="complete"){this.doc.designMode="on";Ext.TaskMgr.stop(_e);this.initEditor.defer(10,this);}},interval:10,duration:10000,scope:this};Ext.TaskMgr.start(_e);if(!this.width){this.setSize(this.el.getSize());}},onResize:function(w,h){Ext.form.HtmlEditor.superclass.onResize.apply(this,arguments);if(this.el&&this.iframe){if(typeof w=="number"){var aw=w-this.wrap.getFrameWidth("lr");this.el.setWidth(this.adjustWidth("textarea",aw));this.iframe.style.width=aw+"px";}if(typeof h=="number"){var ah=h-this.wrap.getFrameWidth("tb")-this.tb.el.getHeight();this.el.setHeight(this.adjustWidth("textarea",ah));this.iframe.style.height=ah+"px";}}},toggleSourceEdit:function(btn){this.sourceEditMode=btn.pressed;if(btn.pressed){this.tb.items.each(function(_14){if(_14.id!="sourceedit"){_14.disable();}});this.syncValue();this.iframe.className="x-hidden";this.el.removeClass("x-hidden");this.el.focus();}else{if(this.initialized){this.tb.items.each(function(_15){_15.enable();});}this.pushValue();this.iframe.className="";this.el.addClass("x-hidden");this.deferFocus();}this.setSize(this.wrap.getSize());},adjustSize:Ext.BoxComponent.prototype.adjustSize,getResizeEl:function(){return this.wrap;},getPositionEl:function(){return this.wrap;},initEvents:function(){this.originalValue=this.getValue();},markInvalid:Ext.emptyFn,clearInvalid:Ext.emptyFn,setValue:function(v){Ext.form.HtmlEditor.superclass.setValue.call(this,v);this.pushValue();},cleanHtml:function(_17){_17=String(_17);if(_17.length>5){var _18=_17.toLowerCase();if(Ext.isSafari){_17=_17.replace(/\sclass="(?:Apple-style-span|khtml-block-placeholder)"/gi,"");}}if(_17=="&nbsp;"){_17="";}return _17;},syncValue:function(){if(this.initialized){var bd=(this.doc.body||this.doc.documentElement);var _1a=bd.innerHTML;if(Ext.isSafari){var bs=bd.getAttribute("style");var m=bs.match(/text-align:(.*?);/i);if(m&&m[1]){_1a="<div style=\""+m[0]+"\">"+_1a+"</div>";}}this.el.dom.value=this.cleanHtml(_1a);}},pushValue:function(){if(this.initialized){var v=this.el.dom.value;if(v.length<1){v="&nbsp;";}(this.doc.body||this.doc.documentElement).innerHTML=v;}},deferFocus:function(){this.focus.defer(10,this);},focus:function(){if(this.win){this.win.focus();}else{this.el.focus();}},initEditor:function(){var _1e=(this.doc.body||this.doc.documentElement);var ss=this.el.getStyles("font-size","font-family","background-image","background-repeat");ss["background-attachment"]="fixed";_1e.bgProperties="fixed";Ext.DomHelper.applyStyles(_1e,ss);Ext.EventManager.on(this.doc,{"mousedown":this.updateTb,"dblclick":this.updateTb,"click":this.updateTb,"keyup":this.updateTb,buffer:100,scope:this});if(Ext.isGecko){Ext.EventManager.on(this.doc,"keypress",this.applyCommand,this);}if(Ext.isIE||Ext.isSafari||Ext.isOpera){Ext.EventManager.on(this.doc,"keydown",this.fixTab,this);}this.initialized=true;this.pushValue();},onDestroy:function(){if(this.rendered){this.tb.items.each(function(_20){if(_20.menu){_20.menu.removeAll();if(_20.menu.el){_20.menu.el.destroy();}}_20.destroy();});this.wrap.dom.innerHTML="";this.wrap.remove();}},initEditorTb:function(){this.editorTbEnabled=true;this.tb.items.each(function(_21){_21.enable();});if(Ext.isGecko){var s=this.win.getSelection();if(!s.focusNode||s.focusNode.nodeType!=3){var r=s.getRangeAt(0);r.selectNodeContents((this.doc.body||this.doc.documentElement));r.collapse(true);this.deferFocus();}try{this.execCmd("useCSS",true);this.execCmd("styleWithCSS",false);}catch(e){}}},adjustFont:function(btn){var _25=btn.id=="increasefontsize"?1:-1;if(Ext.isSafari){_25*=2;}var v=parseInt(this.doc.queryCommandValue("FontSize")||3,10);v=Math.max(1,v+_25);this.execCmd("FontSize",v+(Ext.isSafari?"px":0));},updateTb:function(){if(!this.editorTbEnabled){this.initEditorTb();return;}var _27=this.tb.items.map,doc=this.doc;_27.bold.toggle(doc.queryCommandState("bold"));_27.italic.toggle(doc.queryCommandState("italic"));_27.underline.toggle(doc.queryCommandState("underline"));_27.justifyleft.toggle(doc.queryCommandState("justifyleft"));_27.justifycenter.toggle(doc.queryCommandState("justifycenter"));_27.justifyright.toggle(doc.queryCommandState("justifyright"));if(!Ext.isSafari){_27.insertorderedlist.toggle(doc.queryCommandState("insertorderedlist"));_27.insertunorderedlist.toggle(doc.queryCommandState("insertunorderedlist"));}this.syncValue();},relayCmd:function(btn){this.win.focus();this.execCmd(btn.id,null);this.updateTb();this.deferFocus();},execCmd:function(cmd,_2b){this.doc.execCommand(cmd,false,_2b===undefined?null:_2b);},applyCommand:function(e){if(e.ctrlKey){var c=e.getCharCode(),cmd;if(c>0){c=String.fromCharCode(c);switch(c){case "b":cmd="bold";break;case "i":cmd="italic";break;case "u":cmd="underline";break;}if(cmd){this.win.focus();this.execCmd(cmd);this.deferFocus();e.preventDefault();}}}},fixTab:function(e){if(e.getKey()==e.TAB){e.stopEvent();if(Ext.isIE){var r=this.doc.selection.createRange();if(r){r.collapse(true);r.pasteHTML("&nbsp;&nbsp;&nbsp;&nbsp;");this.deferFocus();}}else{if(Ext.isOpera){this.win.focus();this.execCmd("InsertHTML","&nbsp;&nbsp;&nbsp;&nbsp;");this.deferFocus();}else{this.execCmd("InsertText","\t");this.deferFocus();}}}},getToolbar:function(){return this.tb;},buttonTips:{bold:{title:"Bold (Ctrl+B)",text:"Make the selected text bold.",cls:"x-html-editor-tip"},italic:{title:"Italic (Ctrl+I)",text:"Make the selected text italic.",cls:"x-html-editor-tip"},underline:{title:"Underline (Ctrl+U)",text:"Underline the selected text.",cls:"x-html-editor-tip"},increasefontsize:{title:"Grow Text",text:"Increase the font size.",cls:"x-html-editor-tip"},decreasefontsize:{title:"Shrink Text",text:"Decrease the font size.",cls:"x-html-editor-tip"},backcolor:{title:"Text Highlight Color",text:"Change the background color of the selected text.",cls:"x-html-editor-tip"},forecolor:{title:"Font Color",text:"Change the color of the selected text.",cls:"x-html-editor-tip"},justifyleft:{title:"Align Text Left",text:"Align text to the left.",cls:"x-html-editor-tip"},justifycenter:{title:"Center Text",text:"Center text in the editor.",cls:"x-html-editor-tip"},justifyright:{title:"Align Text Right",text:"Align text to the right.",cls:"x-html-editor-tip"},insertunorderedlist:{title:"Bullet List",text:"Start a bulleted list.",cls:"x-html-editor-tip"},insertorderedlist:{title:"Numbered List",text:"Start a numbered list.",cls:"x-html-editor-tip"},sourceedit:{title:"Source Edit",text:"Switch to source editing mode.",cls:"x-html-editor-tip"}}});
