
Ext.form.Form.prototype.appendRow = function() {
  // Create a new layout object
  var layout = new Ext.form.Layout();
  console.log('TestForm This: %o %o', this, this.items.item(0));
  // Keep track of added fields that are form fields (isFormField)
  var fields = [];
  var columns = [];
  // Add all the fields on to the layout stack
  
  //layout.stack.push.apply(layout.stack, [column]);
    var appendEl = Ext.get(arguments[0]);
    var colWidth = Math.round(98 / (arguments.length - 1));
    colWidth = colWidth + "%";
  // Add only those fields that are form fields to the 'fields' array
  for(var i = 1; i < arguments.length; i++) {
    if(arguments[i].isFormField) {
      
      console.log('arguments: %o', arguments[i]);
      var myCol = new Ext.form.Column({width: colWidth});
      myCol.stack.push.apply(myCol.stack, [arguments[i]]);
      fields.push(myCol);
      fields.push(arguments[i]);
      columns.push(myCol);
    }
  }
layout.stack.push.apply(layout.stack, columns);
console.log(layout);
  // Render the layout
  layout.render(appendEl);
  // If we found form fields add them to the form's items collection and render the
  // fields into their containers created by the layout
  if(fields.length > 0) {
    this.items.addAll(fields);

    // Render each field
    for(var i = 0; i < fields.length; i++) {
      fields[i].render('x-form-el-' + fields[i].id);
    }
    
  }

  return this;
}; 

Ext.namespace('Ext.ux.FormBuilder');

Ext.ux.FormBuilder = function(el, config) {
    this.el = el;
    this.config = config;
    this.form = {};
    this.formConfig = {};
    this.addEvents({
	    "preprocess" : true,
		"postprocess" : true,
		"invaliddata" : true
		});
    Ext.ux.FormBuilder.superclass.constructor.call(this, {});
    console.log('This: %o', this);
    
};

Ext.extend(Ext.ux.FormBuilder, Ext.util.Observable, {
	fetchData: function() {
	    this.conn = new Ext.data.Connection();
	    this.conn.request({
		    url: this.config.url
			, callback: this.processData
			, scope: this
			});
	},
	    processData: function(o, s, f) {
	    this.fireEvent('preprocess', f.responseText);
	    this.formConfig = Ext.util.JSON.decode(f.responseText);
	    console.log('Got json: %o', this.formConfig);
	    if (this.formConfig.console) {
		for (var i in this.formConfig.console) {
		    console.log(this.formConfig.console[i]);
		}
	    }
	    if (!this.formConfig.success || !this.formConfig.form || !this.formConfig.form.options || !this.formConfig.form.children) {
		this.fireEvent('invaliddata', this.formConfig);
		return;
	    }
       
	    /* Okay. We've got good data. Let's boogie! */
	    this.form = new Ext.form.Form(this.formConfig.form.options);
	    var i;
	    for (i in this.formConfig.form.children) {
		if (this.formConfig.form.children[i].type) {
		    console.log('Child: %o', this.formConfig.form.children[i]);
		    var call = "this.build"+this.formConfig.form.children[i].type+"(this.formConfig.form.children[i])";
		    console.log('Calling %s', call);
		    eval(call);
		}
	    }
	    for (i in this.formConfig.form.buttons) {
		if (this.formConfig.form.buttons[i].name) {
		    console.log('Button: %o', this.formConfig.form.buttons[i]);
		    var call = 'this.form.addButton(this.formConfig.form.buttons[i].name, '+this.formConfig.form.buttons[i].handler+', this)';
		    console.log('Calling %s', call);
		    eval(call);
		}
	    }
	    console.log('Form: %o', this.form);
	    this.form.render(this.el);
	    
    	
	},
	    buildHTML : function(f) {
	    this.form.add(
			  new Ext.Element(f.content)
			  );
	},

	    buildBookCombo : function(f) {
	    var defaults = {
		loadingText:'Searching...',
		typeAhead:true,
		width:250,
		hideTrigger: false,
		fieldLabel: 'Search',
		forceSelection: false,
		tpl: new Ext.Template('<div class="search-item"><h3>{name}</h3><span>{moddate}<br />by {creator}</span></div>'),
		displayField: 'name',
		valueField: 'bookID'
	    }
	    var ds = new Ext.data.Store({
		    proxy: new Ext.data.HttpProxy({
			    url: f.remotestore.proxy
			}),
		    reader: new Ext.data.JsonReader({root:"items",totalProperty:"totalCount",id:"bookID"},
						     [{name:"name",mapping:"name"},
							  {name:"bookID",mapping:"bookID"},
							  {name:"creator",mapping:"creator"},
	                                                  {name:"moddate",mapping:"moddate"}
                                                   ])
		});
		    f.options.store = ds;
		    Ext.apply(defaults, f.options);
		    var tf = new Ext.form.ComboBox(defaults);
		    
		    console.log('Adding %o', tf);
		    this.form.add(tf);
		    this.form.lastID = tf;	    
		    
		    
	},
	    buildContactCombo : function(f) {
	    var defaults = {
		loadingText:'Searching...',
		typeAhead:true,
		width:250,
		hideTrigger: false,
		forceSelection: false,
		displayField: 'name',
		valueField: 'id'
	    }
	    var ds = new Ext.data.Store({
		    proxy: new Ext.data.HttpProxy({
			    url: f.remotestore.proxy
			}),
		    reader: new Ext.data.JsonReader({root:"items",totalProperty:"totalCount",id:"id"},
						     [{name:"name",mapping:"name"},
							  {name:"id",mapping:"id"},
                                                   ])
		});
		    f.options.store = ds;
		    Ext.apply(defaults, f.options);
		    var tf = new Ext.form.ComboBox(defaults);
		    
		    console.log('Adding %o', tf);
		    this.form.add(tf);
		    this.form.lastID = tf;	    
		    
		    
	},

	    buildComboBox : function(f) {
	    var ds;
	    if (f.simplestore) {
		if (f.simplestore.data) {
		    eval('var mydata = '+f.simplestore.data);
		    f.simplestore.data = mydata;
		}
		ds = new Ext.data.SimpleStore(f.simplestore);
	    }
	    if (f.remotestore) {
		ds = new Ext.data.Store({
			proxy: new Ext.data.HttpProxy({
				url: f.remotestore.proxy
			    }),
			reader: new Ext.data.JsonReader(f.remotestore.reader, f.remotestore.map)
		    });
		
	    }
	    f.options.store = ds;
	    var tf = new Ext.form.ComboBox(f.options);

	    console.log('Adding %o', tf);
	    this.form.add(tf);
	    this.form.lastID = tf;	    
	
	},
	    buildTextField:  function(f) {
	
	    var tf = new Ext.form.TextField(f.options);
	    console.log('Adding %o', tf);
	    this.form.add(tf);
	    	    this.form.lastID = tf;
	
	},
	    buildHtmlEditor:  function(f) {
	
	    var tf = new Ext.form.HtmlEditor(f.options);
	    console.log('Adding %o', tf);
	    this.form.add(tf);
	    	    this.form.lastID = tf;
	    
	},
	    buildDateField:  function(f) {
	
	    var tf = new Ext.form.DateField(f.options);
	    console.log('Adding %o', tf);
	    this.form.add(tf);
	    	    this.form.lastID = tf;
	    
	},
	    buildTextArea: function(f) {
	
	    var tf = new Ext.form.TextArea(f.options);
	    console.log('Adding %o', tf);
	    this.form.add(tf);
	    	    this.form.lastID = tf;
	    
	},
	    buildCheckbox:  function(f) {
	
	    var tf = new Ext.form.Checkbox(f.options);
	    console.log('Adding %o', tf);
	    this.form.add(tf);
	    	    this.form.lastID = tf;
	    
	},
	    buildRadio:  function(f) {
	
	    var tf = new Ext.form.Radio(f.options);
	    console.log('Adding %o', tf);
	    this.form.add(tf);
	    	    this.form.lastID = tf;
	
	},
	    buildFieldSet: function(f) {
	    var fs = this.form.fieldset(f.options);
	    this.form.lastID = fs;
	    for (i in f.children) {
		if (f.children[i].type) {
		    console.log('FS Child: %o', f.children[i]);
		    var call = "this.build"+f.children[i].type+"(f.children[i])";
		    console.log('FS Calling %s', call);
		    eval(call);
		}
	    }
	    this.form.end(fs);
	},
	    buildColumn: function(f) {
	    var col = new Ext.form.Column();
	    this.form.start(col);
	    this.form.lastID = col;
	    for (i in f.children) {
		if (f.children[i].type) {
		    console.log('FS Child: %o', f.children[i]);
		    var call = "this.build"+f.children[i].type+"(f.children[i])";
		    console.log('FS Calling %s', call);
		    eval(call);
		}
	    }
	    this.form.end(col);
	},
    
	    Submit: function(f) {
	    console.log('Submit called with %o', f);
	    this.form.submit();
	},
	    Reset: function(f) {
	    console.log('Reset called with %o', f);
	    this.form.reset();
	}
	
    });



Ext.form.VTypes["hostnameVal1"] = /^[a-zA-Z][-.a-zA-Z0-9]{0,254}$/;
Ext.form.VTypes["hostnameVal2"] = /^[a-zA-Z]([-a-zA-Z0-9]{0,61}[a-zA-Z0-9]){0,1}([.][a-zA-Z]([-a-zA-Z0-9]{0,61}[a-zA-Z0-9]){0,1}){0,}$/;
Ext.form.VTypes["ipVal"] = /^([1-9][0-9]{0,1}|1[013-9][0-9]|12[0-689]|2[01][0-9]|22[0-3])([.]([1-9]{0,1}[0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])){2}[.]([1-9][0-9]{0,1}|1[0-9]{2}|2[0-4][0-9]|25[0-4])$/;
Ext.form.VTypes["netmaskVal"] = /^(128|192|224|24[08]|25[245].0.0.0)|(255.(0|128|192|224|24[08]|25[245]).0.0)|(255.255.(0|128|192|224|24[08]|25[245]).0)|(255.255.255.(0|128|192|224|24[08]|252))$/;
Ext.form.VTypes["portVal"] = /^(0|[1-9][0-9]{0,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5])$/;
Ext.form.VTypes["multicastVal"] = /^((22[5-9]|23[0-9])([.](0|[1-9][0-9]{0,1}|1[0-9]{2}|2[0-4][0-9]|25[0-5])){3})|(224[.]([1-9][0-9]{0,1}|1[0-9]{2}|2[0-4][0-9]|25[0-5])([.](0|[1-9][0-9]{0,1}|1[0-9]{2}|2[0-4][0-9]|25[0-5])){2})|(224[.]0[.]([1-9][0-9]{0,1}|1[0-9]{2}|2[0-4][0-9]|25[0-5])([.](0|[1-9][0-9]{0,1}|1[0-9]{2}|2[0-4][0-9]|25[0-5])))$/;
Ext.form.VTypes["usernameVal"] = /^[a-zA-Z][-_.a-zA-Z0-9]{0,30}$/;
Ext.form.VTypes["passwordVal1"] = /^.{6,31}$/;
Ext.form.VTypes["passwordVal2"] = /[^a-zA-Z].*[^a-zA-Z]/;
Ext.form.VTypes["hostname"]=function(v){
 if(!Ext.form.VTypes["hostnameVal1"].test(v)){
  Ext.form.VTypes["hostnameText"]="Must begin with a letter and not exceed 255 characters"
  return false;
 }
 Ext.form.VTypes["hostnameText"]="L[.L][.L][.L][...] where L begins with a letter, ends with a letter or number, and does not exceed 63 characters";
 return Ext.form.VTypes["hostnameVal2"].test(v);
}
Ext.form.VTypes["hostnameText"]="Invalid Hostname"
Ext.form.VTypes["hostnameMask"]=/[-.a-zA-Z0-9]/;
Ext.form.VTypes["ip"]=function(v){
 return Ext.form.VTypes["ipVal"].test(v);
}
Ext.form.VTypes["ipText"]="1.0.0.1 - 223.255.255.254 excluding 127.x.x.x"
Ext.form.VTypes["ipMask"]=/[.0-9]/;
Ext.form.VTypes["netmask"]=function(v){
 return Ext.form.VTypes["netmaskVal"].test(v);
}
Ext.form.VTypes["netmaskText"]="128.0.0.0 - 255.255.255.252"
Ext.form.VTypes["netmaskMask"]=/[.0-9]/;
Ext.form.VTypes["port"]=function(v){
 return Ext.form.VTypes["portVal"].test(v);
}
Ext.form.VTypes["portText"]="0 - 65535"
Ext.form.VTypes["portMask"]=/[0-9]/;
Ext.form.VTypes["multicast"]=function(v){
 return Ext.form.VTypes["multicastVal"].test(v);
}
Ext.form.VTypes["multicastText"]="224.0.1.0 - 239.255.255.255"
Ext.form.VTypes["multicastMask"]=/[.0-9]/;
Ext.form.VTypes["username"]=function(v){
 return Ext.form.VTypes["usernameVal"].test(v);
}
Ext.form.VTypes["usernameText"]="Username must begin with a letter and cannot exceed 255 characters"
Ext.form.VTypes["usernameMask"]=/[-_.a-zA-Z0-9]/;
Ext.form.VTypes["password"]=function(v){
 if(!Ext.form.VTypes["passwordVal1"].test(v)){
  Ext.form.VTypes["passwordText"]="Password length must be 6 to 31 characters long";
  return false;
 }
 Ext.form.VTypes["passwordText"]="Password must include atleast 2 numbers or symbols";
 return Ext.form.VTypes["passwordVal2"].test(v);
}
Ext.form.VTypes["passwordText"]="Invalid Password"
Ext.form.VTypes["passwordMask"]=/./;

Ext.form.VTypes["phoneVal"] = /^(\d{3}[-]?){1,2}(\d{4})( x\d+)*$/;
Ext.form.VTypes["phoneMask"] = /[\d x-]/;
Ext.form.VTypes["phoneText"] = 'Not a valid phone number.  Must be in the format 123-4567 x123 or 123-456-7890 x123 (dashes and extensions optional)';
Ext.form.VTypes["phone"]=function(v){
           return Ext.form.VTypes["phoneVal"].test(v);
     }
     
Ext.form.VTypes["ssnVal"] = /^(\d{3})-(\d{2})-(\d{4})$/;
Ext.form.VTypes["ssnMask"] = /[\d-]/;
Ext.form.VTypes["ssnText"] = 'Not a valid Social Security Number. Must be of the format xxx-yy-zzzz';
Ext.form.VTypes["ssn"]=function(v){
           return Ext.form.VTypes["ssnVal"].test(v);
     }

Ext.namespace('Ext.ux.HTMLForm');

Ext.ux.HTMLForm = function(id, callback) {
    var formEle = Ext.get(id);
    var form = new Ext.form.BasicForm(id);
	//frm.render();
	
	var fields = form.getValues()
	console.log('Form: %o Ele: %o  Fields: %o', form,formEle,fields);
	var i = 0;
	var j = formEle.dom.length;
	for (key in formEle.dom)
	{
           // alert(key);
            if (i < j) {
                i++;
		var elem = Ext.get(key);
		console.log(key);
		if (! elem && key && formEle.dom[key] && formEle.dom[key].id) {
		elem = Ext.get(formEle.dom[key].id);
		}
		var vtype = false;
                var stateGroup = false;
		var FallowBlank = false;
		if (elem && elem.dom.attributes.getNamedItem('vtype')) {
		    vtype = elem.dom.attributes.getNamedItem('vtype').nodeValue;
		    console.log('Got vtype ',vtype, elem);
		}
		if (elem && elem.dom.attributes.getNamedItem('allowBlank')) {
		    FallowBlank = true;
		}
                if (elem && elem.dom.attributes.getNamedItem('stateGroup')) {
                    stateGroup = elem.dom.attributes.getNamedItem('stateGroup').nodeValue;
                    console.log('Setting stateGroup ', stateGroup, elem);
                }
                
		var config;
		console.log('Elem: %o   Key: %o', elem, key);
		if (elem && elem.hasClass('Combobox')) {
                    if (! Ext.isIE) {
                        //alert('CB: '+elem.dom.name);
		    config = {
				transform:elem.dom.name,
				typeAhead: true,
				triggerAction: 'all',
				emptyText: 'Please choose one...',
				width: (elem.getWidth()>145)?145:elem.getWidth(),
				allowBlank: FallowBlank,
				forceSelection:true
			};
		    if (vtype) {
			config.vtype = vtype;
		    }
                    if (stateGroup) {
                        config.stateGroup = stateGroup;
                    }
                    
		   var cb = new Ext.form.ComboBox(config);
                    }
		} else if (elem && (elem.hasClass('DateField'))) {
                    //alert('DF: '+elem.dom.name);
		    config = {
			    format:'m/d/Y'
			};
			if (vtype) {
			config.vtype = vtype;
		    }
		    if (stateGroup) {
                        config.stateGroup = stateGroup;
                    }
			var df = new Ext.form.DateField(config);
			df.applyTo(elem.dom.name);
		} else if (elem && elem.hasClass('Checkbox'))
		{
		    config = {
			
			};
			if (vtype) {
			config.vtype = vtype;
		    }
		    if (stateGroup) {
                        config.stateGroup = stateGroup;
                    }
			var df = new Ext.form.Checkbox(config);
			console.log('Checkbox ',df,elem);
			
			df.applyTo(elem);
                } else if (elem && elem.hasClass('Radio'))
		{
		    config = {
			
			};
			if (vtype) {
			config.vtype = vtype;
		    }
		    if (stateGroup) {
                        config.stateGroup = stateGroup;
                    }
			var df = new Ext.form.Radio(config);
			console.log('Checkbox ',df,elem);
			
			df.applyTo(elem);
		}
		else if (elem) {
		    config = {
			allowBlank:FallowBlank,
			disableKeyFilter: true,
			width: 145
		    };
		    if (vtype) {
			config.vtype = vtype;
		    }
		    if (stateGroup) {
                        config.stateGroup = stateGroup;
                    }
		    if (elem.dom.type == 'textarea') {
			var tf = new Ext.form.TextArea(config);
		    tf.applyTo(elem.dom.name);
		    } else if (elem && elem.dom.name) {
                        console.log('Creating TF: ',elem,elem.dom.name);
		    var tf = new Ext.form.TextField(config);
                    //alert('TF: '+elem.dom.name);
		    tf.applyTo(elem.dom.name);
                    }
		}
		
		if (elem && elem.hasClass('resizable'))
		{
            var dwrapped = new Ext.Resizable(elem, {
                wrap:true,
                pinned:true,
                width:145,
                height:150,
                minWidth:145,
                minHeight: 50,
		handles: false,
                dynamic: true
            });
		}
	}
	}
	Ext.fly(id).un("submit", form.onSubmit, form);
	var map;
	if (callback) {
	    Ext.fly(id).on("submit", callback, form);
            map = new Ext.KeyMap(id, {
                key: Ext.EventObject.ENTER, 
                fn: callback,
                scope: form
            });
	}
	
	//form.render();
        return form;
};

Ext.namespace('Ext.ux.Test');
Ext.ux.Test = function(e,f,g,h) {
    //e.stopEvent();
    console.log('Test: ',this,e,f,g,h);
    
}