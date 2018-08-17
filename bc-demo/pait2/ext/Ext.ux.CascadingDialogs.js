Ext.namespace('Ext.ux.CascadingDialogs');

// set blank image to local file
Ext.BLANK_IMAGE_URL = '/<?=$self;?>javascripts/ext/resources/images/default/s.gif';

Ext.Resizable.prototype.destroy = function(removeEl) {
  this.proxy.remove();
  if(this.overlay) {
    this.overlay.removeAllListeners();
    this.overlay.remove();
  }
  var ps = Ext.Resizable.positions;
  for(var k in ps) {
    if(typeof ps[k] != "function" && this[ps[k]]) {
      var h = this[ps[k]];
      h.el.removeAllListeners();
      h.el.remove();
    }
  }
  if(removeEl) {
    this.el.update("");
    this.el.remove();
  }
};

Ext.ux.CascadingDialogs.checkBeforeOpen = function(btn) {
  console.log('clicked the %o button, scope: %o', btn, this);
  if (btn == 'yes') {
    var panel = Ext.ux.CascadingDialogs.layout.findPanel(this.tabid);
    panel.setUrl(this.url);
    panel.setTitle(this.title);
    panel.refresh();
  }
  this.temp = {};
};

Ext.ux.CascadingDialogs.openWindow = function (e) {

  console.log('Opening %o in new window', e.target);
  var winID = e.target.title.toString().replace(new RegExp('[^A-Za-z0-9]+', 'g'), '');
  var newWin = window.open(e.target.href, winID);
  newWin.focus();

};


Ext.ux.CascadingDialogs.refreshWSTree = function (f, e) {
	console.log('What is e: %o', e);
	var wsTree = Ext.PAIT.wsTree;
		wsTree.getNodeById('source').reload();
}

Ext.ux.CascadingDialogs.refreshWSTreeNode = function (e) {
	// refreshes the currently selected node
	console.log('What is e: %o', e);
	var wsTree = Ext.PAIT.wsTree;
  console.log('Selected Node is: %o', e.wsID);
  wsTree.getNodeById(e.treeNode).reload();
	wsTree.getNodeById(e.treeNode).expand();
}


Ext.ux.CascadingDialogs.openCenter = function (e, scope) {
  console.log('Clicked: %o', e);
  console.log('Target: %o', e.target);
  console.log('This: %o', this);
  scope = (App.layout)?App:scope;
  scope = (scope.layout)?scope:this;
  console.log('Scope: %o', scope);
  var maketab = true;
  var tabid = false;
  if (e.target.id && e.target.id.indexOf('ext-gen') == -1) {
    tabid = e.target.id+'tab';
    console.log('tabID: %o', tabid);
    if( !scope.layout.findPanel(tabid) ) {
      console.log('did not find tab.');
      maketab = true;
    } else {
      console.log('found tab.');
      maketab = false;
    }
  } else {
    maketab = true;
  }
  scope.layout.beginUpdate();
  if (maketab) {
    if (tabid) {
      var iframe = Ext.DomHelper.append(document.body,{tag: 'div', frameBorder: 0, id: tabid});
    } else {
      var iframe = Ext.DomHelper.append(document.body,{tag: 'div', frameBorder: 0});
    }
    var panel = new Ext.ContentPanel(iframe,{title: Ext.util.Format.ellipsis(e.target.title, 25), autoScroll:true, fitToFrame:true, closable:true});
    if (e.target.href) {
      panel.setUrl({url: e.target.href, scripts: true, text: 'Loading '+e.target.title+'...', callback: Ext.ux.CascadingDialogs.scanHrefs}, {}, true);
    } else {
      panel.setContent(e.target.html);
    } 
    scope.layout.add('center', panel); 
  } else {
    scope.layout.showPanel(tabid);
    var oldtab = scope.layout.findPanel(tabid);
    var oldTitle = oldtab.getTitle();
    if (oldTitle != Ext.util.Format.ellipsis(e.target.title,25)) {
      oldtab.setTitle(e.target.title);
    }
    if (e.target.html) {
      oldtab.setContent(e.target.html);
        } else if (e.target.href) {
      //console.log('Setting old tab  url to: %s based on %o', e.target.href, e);
      //oldtab.load({url: e.target.href, scripts: true, text: 'Loading '+e.target.title+'...', callback: Ext.ux.CascadingDialogs.scanHrefs}, {}, true);
    if (tabid) {
      var iframe = Ext.DomHelper.append(document.body,{tag: 'div', frameBorder: 0, id: tabid});
    } else {
      var iframe = Ext.DomHelper.append(document.body,{tag: 'div', frameBorder: 0});
    }
    var panel = new Ext.ContentPanel(iframe,{title: Ext.util.Format.ellipsis(e.target.title, 25), autoScroll:true, fitToFrame:true, closable:true});
    if (e.target.href) {
      panel.setUrl({url: e.target.href, scripts: true, text: 'Loading '+e.target.title+'...', callback: Ext.ux.CascadingDialogs.scanHrefs}, {}, true);
    } else {
      panel.setContent(e.target.html);
    } 
    scope.layout.add('center', panel); 
    
    } else {
    oldtab.refresh();
    }
    Ext.ux.CascadingDialogs.scanHrefs();     
  }
  scope.layout.endUpdate();
};

Ext.ux.CascadingDialogs.scanHrefs =  function(e){
  console.log('Updating panel? %o %o %o', this, e, Ext.select('a.centerTarget'));
  /* Sometimes we get this, sometimes we get e */
  Ext.select('a.centerTarget').removeAllListeners();
  Ext.select('a.windowTarget').removeAllListeners();
  /* This line makes all a tags with a class of "centerTarget" open new tabs. */
  Ext.select('a.centerTarget').on('click', Ext.ux.CascadingDialogs.openCenter, this, {stopEvent: true, stopPropogation: true});
  /* This line makes all a tags with a class of "windowTarget" open new windows. */
  Ext.select('a.windowTarget').on('click', Ext.ux.CascadingDialogs.openWindow, this, {stopEvent: true, stopPropogation: true});
  console.log('Updated: %o', Ext.select('a.centerTarget'));
};

Ext.ux.CascadingDialogs.handleDialog = function (e, s, url, height, width) {
  if (e.type == 'confirm') {
    Ext.ux.CascadingDialogs.handleConfirm(e,s,url,height,width);
  } else if (e.type == 'alert') {
    Ext.ux.CascadingDialogs.handleAlert(e,s,url,height,width);
  } else if (e.type == 'html') {
    Ext.ux.CascadingDialogs.handleHTMLDialog(e,s,url,height,width);
  } else {
    Ext.ux.CascadingDialogs.handleFormDialog(e,s,url,height,width);
  }
};		

Ext.ux.CascadingDialogs.handleAlert = function(e,s,url,height,width) {
  //Here we ignore the url, height *and* width. Plbbt.
  var config = {
  title: 'Alert!',
  msg: 'There has been a problem.',
  buttons: Ext.MessageBox.OK
    };
  Ext.applyIf(e, config);
  console.log('E %o', e);
  var dlg = Ext.MessageBox.show(e);
  console.log('Alert %o', dlg);
  Ext.ux.CascadingDialogs.openDialog = dlg;
};

Ext.ux.CascadingDialogs.handleConfirm = function(e,s,url,height,width) {
  var config = {
    title: 'Confirm',
    msg: 'Please confirm your action.',
    buttons: Ext.MessageBox.OKCANCEL,
    fn: function(e) {
      if (e == "ok") {
      Ext.ux.CascadingDialogs.handleConfirmOK(url)
      }
    }
  };
  Ext.applyIf(e, config);
  console.log('E %o', e);
  var dlg = Ext.MessageBox.show(e);
  console.log('Alert %o', dlg);
  Ext.ux.CascadingDialogs.openDialog = dlg;
};

Ext.ux.CascadingDialogs.handleConfirmOK = function (e) {
  /* Fake this up to use the same form handler below so we only have to write this once. */
  Ext.Ajax.request({url: e, method: 'POST', callback: Ext.ux.CascadingDialogs.handleDlgResponse});
};

Ext.ux.CascadingDialogs.handleHTMLDialog = function(e,s,url,height,width) {
  Ext.Ajax.request({'url': e.href, method: 'POST',
	callback: function(f,q,r) {
	Ext.ux.CascadingDialogs.buildHTMLDialog(e,url,r,height,width);
      }
    });
};

Ext.ux.CascadingDialogs.buildHTMLDialog = function(e,s,r,height,width) {
  var config = {
  width: (width)?width:300,
  height: (height)?height:200,
  maxWidth: (width)?width:300,
  maxHeight: (height)?height:200,
  autoScroll: true,
    title: 'Confirm',
    msg: r.responseText,
    buttons: Ext.MessageBox.OKCANCEL,
    fn: function(e) {
      if (e == "ok") {
      Ext.ux.CascadingDialogs.handleConfirmOK(s)
      }
    }
  };
  Ext.applyIf(e, config);
  console.log('E %o', e);
  var dlg = Ext.MessageBox.show(e);
  console.log('Alert %o', dlg);
  Ext.ux.CascadingDialogs.openDialog = dlg;
}



Ext.ux.CascadingDialogs.handleDlgResponse = function(f,q,r) {
  console.log('hSC: %o %o', f,r);
  if (r.response) {
  var resp = Ext.util.JSON.decode(r.response.responseText);
  } else {
  var resp = Ext.util.JSON.decode(r.responseText);
  }
  if (Ext.ux.CascadingDialogs.openDialog) {
  Ext.ux.CascadingDialogs.openDialog.hide();
  }
  if (resp.console) {
    for (var i in resp.console) {
      console.log(resp.console[i]);
    }
  }
  if (resp.success) {
    if (resp.openWindow){
      Ext.ux.CascadingDialogs.openWindow(resp.openWindow);
    }
    if (resp.openCenter){
      Ext.ux.CascadingDialogs.openCenter(resp.openCenter);
    }
    if (resp.updateTree){
    	Ext.ux.CascadingDialogs.refreshWSTree(resp.updateTree);
    }
    if (resp.updateNode){
    	console.log('resp: %o', resp);
    	Ext.ux.CascadingDialogs.refreshWSTreeNode(resp.updateNode);
    }
    if (resp.updatePanel) {
      Ext.ux.CascadingDialogs.layout.beginUpdate();
      var panel = Ext.get(resp.updatePanel.id);
      panel.update('');
      console.log('Trying to set the fricking on update thingy');

      console.log('Panel: %o ID: %s', panel, resp.updatePanel.id); 
      //	        panel.setUrl({url: resp.updatePanel.url, scripts: true, text: 'Updating...', discardURL:true,callback: Ext.ux.CascadingDialogs.scanHrefs}, {}, true);

        Ext.Ajax.request({url: resp.updatePanel.url, method: 'POST', scope:panel, callback: function(e,f,g){this.update(g.responseText,true,Ext.ux.CascadingDialogs.scanHrefs);}});
//      panel.load({url: resp.updatePanel.url,text: "Updating...",scripts:true,nocache:false,callback: Ext.ux.CascadingDialogs.scanHrefs} );
      //   panel.refresh();

      Ext.ux.CascadingDialogs.layout.endUpdate();
    
    }
    if (resp.openDialog) {
      Ext.ux.CascadingDialogs.handleDialog(resp.openDialog.options, Ext.ux.CascadingDialogs, resp.openDialog.url, resp.openDialog.height, resp.openDialog.width);
    }
    if (resp.refreshWindow) {
      window.location.reload();
    }
  } else {
    Ext.MessageBox.alert(resp.errorTitle, resp.errorText);
  }


}

Ext.ux.CascadingDialogs.handleFormDialog = function (e, s, url, height, width) {
  console.log('Handle Dialog: %o %o %o', e, s, url);
  var useShim;
  var ua = navigator.userAgent.toLowerCase();
  if (Ext.isIE){
    useShim = true;
  }else{
    useShim = false;
  }
  var scroll = true;
  if (width || height) {
    scroll = false;
  }
  var dlg = new Ext.BasicDialog('dlghandler', {
    autoCreate: true,
	minHeight: 300,
	minWidth: 500,
	autoScroll: scroll,
	width: (width)?width:750,
	height: (height)?height:400,
	modal: false,
	closable:true,
	resizable:false,
	resizeHandles:false,
	proxyDrag:true,
	draggable:true,
	collapsible:false,
	title: (e.text)?e.text:e.title,
	shim: useShim
	});
  dlg.body.update('<div id="myDialog"></div><div id="myDLGProgress"></div>');
  dlg.on("show", function(d) {
      var div = Ext.get(d.el);
      div.setStyle("overflow", "auto");
				
      var text = div.select(".ext-mb-textarea", true);
      if (!text.item(0))
	text = div.select(".ext-mb-text", true);
      if (text.item(0))
	text.item(0).dom.select();
    });
  dlg.on("hide", function(d) {
     this.destroy(true);
    });
  dlg.show();
  var myForm = new Ext.ux.FormBuilder('myDialog',{'url' : url});
  dlg.myForm = myForm;		  
  myForm.fetchData();	
  Ext.ux.CascadingDialogs.openDialog = dlg;
  console.log('Dialog: %o   Form: %o', dlg, myForm);
};

Ext.ux.CascadingDialogs.handleDialogSubmit = function(f) {
  console.log('DLG Submit: %o', f);
  var form = f.scope.form;
  form.on('actioncomplete', Ext.ux.CascadingDialogs.handleSubmitComplete, form);
  form.on('actionfailed', Ext.ux.CascadingDialogs.handleSubmitComplete, form);
  console.log('Submitting!');
  if (Ext.isIE) {
  form.submit(); 
  } else {
  form.submit({waitMsg: 'Please wait...'}); 
  }
};

Ext.ux.CascadingDialogs.handleSubmitComplete = function(f, r) {
  console.log('Handling complete submit');
  Ext.ux.CascadingDialogs.handleDlgResponse(f, true, r);
};
