Ext.override(Ext.Element, {
    mask: function(msg, msgCls) {
            if(!Ext.isIE7 && this.getStyle("position") == "static") {
                this.setStyle("position", "relative");
            }
            //alert('pass');
            if (this._maskMsg) { this._maskMsg.remove(); }
            if (this._mask) { this._mask.remove(); }
            this._mask = Ext.DomHelper.append(this.dom, {cls: "ext-el-mask"}, true);
            
            this.addClass("x-masked");
            this._mask.setDisplayed(true);
            if (typeof msg == 'string') {
                this._maskMsg = Ext.DomHelper.append(this.dom, {cls:"ext-el-mask-msg", cn:{tag:'div'}},true);
                var mm = this._maskMsg;
                mm.dom.className = msgCls ? "ext-el-mask-msg "+msgCls:"ext-el-mask-msg";
                mm.dom.firstChild.innerHTML = msg;
                mm.setDisplayed(true);
                mm.center(this);
            }
            if (Ext.isIE && !(Ext.isIE7 && Ext.isStrict) && this.getStyle('height') == 'auto') {
                this._mask.setHeight(this.getHeight());
            }
            return this._mask;
    }
});

FilteredGridView = function(config){
	FilteredGridView.superclass.constructor.call(this, config);
};
Ext.extend(FilteredGridView, Ext.grid.GridView, {
	render: function(){
		FilteredGridView.superclass.render.apply(this, arguments);
		
		this.filterMenu = new Ext.menu.Menu();
		this.fmItems = {
			sep: this.hmenu.add("separator"),
			mi:  this.hmenu.add(new Ext.menu.CheckItem({
				id:"filters", 
				text: "Filter", 
				menu: this.filterMenu
			}))
		}
		this.fmItems.mi.on('checkchange', this.updateFilterStatus, this);
				
		this.delayedUpdate = new Ext.util.DelayedTask(this.updateFilters, this);
	},
	
	handleHdCtx: function(grid, index){
		FilteredGridView.superclass.handleHdCtx.apply(this, arguments);

		var filter = this.cm.config[index].filter;
		if(typeof filter == "string"){
			filter = this.cm.config[index].filter = new DefaultFilters[filter];
			filter.fgv = this;
		}
		this.activeFilter = filter;
		this.activeFilter.headerIndex = index;
		
		if(filter != null){
			this.fmItems.mi.setChecked(filter.enabled, true);
			this.filterMenu.removeAll();	
			filter.installMenu(this.fmItems.mi);
			
			this.fmItems.mi.show();
			this.fmItems.sep.show();
		} else {
			this.fmItems.mi.hide();
			this.fmItems.sep.hide();
		}
		
		this.hmenu.el.show();
	},
		
	updateFilters: function(){
		var filters = []
		var conf = this.cm.config;
		for(key in conf){
			var c = conf[key];
			var f = c.filter;
			if(f && f.enabled)
				filters.push(c.dataIndex + ":" + f.serialize());
		};
		var ds = this.grid.dataSource;
		ds.baseParams['filters'] = filters.join(";");
		console.log('Filters: ', ds.baseParams['filters']);
		ds.reload();
	},
	
	updateFilterStatus: function(item, enabled){
		this.activeFilter.enabled = enabled;
		this.updateFilterStyle(this.activeFilter.headerIndex);
		this.delayedUpdate.delay(500);
	},
	
	updateHeaders: function(){
		var r = FilteredGridView.superclass.updateHeaders.apply(this, arguments);
		
		for(var i=0, l=this.cm.getColumnCount(); i<l; i++)
			this.updateFilterStyle(i);
			
		return r;
	},
	
	updateFilterStyle: function(index){
		var h = Ext.get(this.getHeaderCell(index).firstChild);
		if(this.cm.config[index].filter.enabled)
			h.addClass(this.filteredHeaderClass || 'filtered');
		else
			h.removeClass(this.filteredHeaderClass || 'filtered');
	}
});

ColumnFilter = function(){};
ColumnFilter.prototype = {
	enabled: false,
	serialize: function(){
		return 'like:' + encodeURIComponent(this.value);
	},
	installMenu: function(rootMenu){}
}

DefaultFilters = {};
DefaultFilters['string'] = function(){};
Ext.extend(DefaultFilters['string'], ColumnFilter,{
	value: "",
	installMenu: function(rootItem){
		var rootMenu = rootItem.menu;
		
		var menu = new EditableMenuItem(this.value, {icon: 'images/find.gif'});
		menu.on('keyup', function(){
			this.value = menu.getValue();
			rootItem.setChecked(true);
			this.fgv.updateFilterStatus(null, true);
		}.createDelegate(this));
		rootMenu.add(menu);
	}
});
DefaultFilters['boolean'] = function(){};
Ext.extend(DefaultFilters['boolean'], ColumnFilter, {
	value: false,
	installMenu: function(rootItem){
		var rootMenu = rootItem.menu;
		
		var optionItems = [
			new Ext.menu.CheckItem({text: "Yes", group: 'boolean'}),
			new Ext.menu.CheckItem({text: "No", group: 'boolean', checked: true})];
		
		rootMenu.add(optionItems[0]);
		rootMenu.add(optionItems[1]);
		optionItems[0].setChecked(this.value, true);
		
		var f = function(){
				this.value = optionItems[0].checked;
				this.fgv.updateFilterStatus(null, true);
				rootItem.setChecked(true);
			}.createDelegate(this);
			
		for(var i=0; i<optionItems.length; i++){
			optionItems[i].on('click', f);
			optionItems[i].on('checkchange', f);
		}
	},
	serialize: function(){
		return 'eq:' + (this.value ? '1' : '0');
	}
});
DefaultFilters['null'] = function(){};
Ext.extend(DefaultFilters['null'], DefaultFilters['boolean'], {
	serialize: function(){
		return 'null:' + (this.value ? 'notnull' : 'null');
	}
});

DefaultFilters['date'] = function(){};
Ext.extend(DefaultFilters['date'], ColumnFilter, {
	installMenu: function(rootItem){
		var rootMenu = rootItem.menu;
		
		var dates = [
			new Ext.menu.CheckItem({text: "Before", menu: new Ext.menu.DateMenu()}),
			new Ext.menu.CheckItem({text: "After", menu: new Ext.menu.DateMenu()}),
			new Ext.menu.CheckItem({text: "On", menu: new Ext.menu.DateMenu()})
		];
		
		var keys = ['before', 'after', 'onDate'];
		for(var i=0; i<keys.length; i++)
			if(this[keys[i]]){
				dates[i].menu.picker.setValue(this[keys[i]]);
				dates[i].setChecked(true, true);
			}
		
		rootMenu.add(dates[1]);
		rootMenu.add(dates[0]);
		rootMenu.add("separator");
		rootMenu.add(dates[2]);
		
		for(var i=0; i<dates.length; i++){
			dates[i].menu.on('select', function(index){
				dates[index].setChecked(true);
				this.fgv.updateFilterStatus(null, true);
				rootItem.setChecked(true);
	
				this[keys[index]] = arguments[2];
				
				if(index == 2){
					dates[0].setChecked(false, true);
					dates[1].setChecked(false, true);
					delete this.before;
					delete this.after;
				} else {
					dates[2].setChecked(false, true);
					delete this.onDate;
				}
			}.createDelegate(this, i));
			
			dates[i].on('checkchange', function(index){
				delete this[keys[index]]
				if(!this[0] && !this[1] && !this[2])
					rootItem.setChecked(false);
			}.createDelegate(this, i));
		};
	},
	serialize: function(){
		var args = [];
		if(this.before)
			args = ['dlt:' + this.convertDate(this.before)];
		if(this.after)
			args.push('dgt:' + this.convertDate(this.after));
		if(this.onDate)
			args = ['deq:' + this.convertDate(this.onDate)];
		
		return args.join(':');
	},
	convertDate: function(date){
		date = new Date(date);
		return date.format('Y-m-d');
	}
});
DefaultFilters['numeric'] = function(){};
Ext.extend(DefaultFilters['numeric'], ColumnFilter, {
	installMenu: function(rootItem){
		var rootMenu = rootItem.menu;
		
		var keys = ['gt', 'lt', 'eq'];
		var fields = [
			new EditableMenuItem(this.gt || "", {icon: 'images/gt.gif', name:'gt'}),
			new EditableMenuItem(this.lt || "", {icon: 'images/lt.gif', name:'lt'}),
			new EditableMenuItem(this.eq || "", {icon: 'images/equals.gif', name:'eq'})
		];

		rootMenu.add(fields[0]);
		rootMenu.add(fields[1]);
		rootMenu.add("separator");
		rootMenu.add(fields[2]);
		
		for(var i=0; i<fields.length; i++)
			fields[i].on('keyup', function(index, input, e){
                            console.log('index %o, e %o, input %o', index, e, input, this);
				if(input.field.value.length > 0 && isFinite(input.field.value) && input.field.name.length > 0)
					this[input.field.name] = input.field.value;
				else
					delete this[input.field.name];
				
				rootItem.setChecked(false, true);
				for(var j=0; j<keys.length; j++)
					if(this[keys[j]]) rootItem.setChecked(true, true);

				this.fgv.updateFilterStatus(null, true);
			}.createDelegate(this, [i, fields[i]]));
            console.log('Fields: %o', fields);
	},
	serialize: function(){
		var args = [];
                console.log('this %o', this);
		if(this.lt)
			args.push('lt:' + this.lt);
		if(this.gt)
			args.push('gt:' + this.gt);
		if(this.eq)
			args.push('eq:' + this.eq);
		
		return args.join(':');
	}
});

EditableMenuItem = function(text, config){
	EditableMenuItem.superclass.constructor.call(this);
	this.text = text;
	this.config = config;
	
	Ext.apply(this.events, {keyup: true});
};
Ext.extend(EditableMenuItem, Ext.menu.BaseItem, {
    itemCls : "x-menu-item",
    hideOnClick: false,
    onRender: function(){
        var s = document.createElement("div");
        s.className = this.itemCls;
        s.style.paddingRight = "10px";
        if(Ext.isGecko)
        	s.style.overflow     = 'auto';
        s.innerHTML = '<img src="' + this.config.icon + '" class="x-menu-item-icon" /><input type="text" name="'+this.config.name+'" style="width: 120px" class="x-menu-input-box" />';
        
        this.field = s.lastChild;
        this.field.value = this.text;
        this.el = s;
        this.relayEvents(Ext.get(this.field), ["keyup"]);
        EditableMenuItem.superclass.onRender.apply(this, arguments);
    },
    getValue: function(){
    	return this.field.value;
    }
});

Ext.grid.TableGrid = function(table, config) {
    config = config || {};
    var cf = config.fields || [], ch = config.columns || [];
    table = Ext.get(table);

    var ct = table.insertSibling();

    var fields = [], cols = [];
    var headers = table.query("thead th");
	for (var i = 0, h; h = headers[i]; i++) {
		var text = h.innerHTML;
		var name = 'tcol-'+i;
        fields.push(Ext.applyIf(cf[i] || {}, {
            name: name,
            mapping: 'td:nth('+(i+1)+')/@innerHTML'
        }));

		cols.push(Ext.applyIf(ch[i] || {}, {
			'header': text,
			'dataIndex': name,
			'width': h.offsetWidth,
			'tooltip': h.title,
            'sortable': true
        }));
	}

    var ds  = new Ext.data.Store({
        reader: new Ext.data.XmlReader({
            record:'tbody tr'
        }, fields)
    });

	ds.loadData(table.dom);
    var cm = new Ext.grid.ColumnModel(cols);

    if(config.width || config.height){
        ct.setSize(config.width || 'auto', config.height || 'auto');
    }
    if(config.remove !== false){
        table.remove();
    }

    Ext.grid.TableGrid.superclass.constructor.call(this, ct,
        Ext.applyIf(config, {
            'ds': ds,
            'cm': cm,
            'sm': new Ext.grid.RowSelectionModel(),
            autoHeight:true,
            autoWidth:true
        }
    ));
};

Ext.extend(Ext.grid.TableGrid, Ext.grid.Grid);

Ext.namespace('Ext.ux');

Ext.ux.AutoGrid = function(container, config){
    // cant render withot cm... workaround?
    if(!config.cm) {
        config.cm = new Ext.grid.ColumnModel([{header: ""}]);
    }
    
    Ext.ux.AutoGrid.superclass.constructor.call(this, container, config);
    
    // register the metachange event
    if(this.dataSource){
        this.dataSource.on("metachange", this.onMetaChange, this);
    }
};

Ext.extend(Ext.ux.AutoGrid, Ext.grid.EditorGrid, {   
    cellRenderers : {},
    
    addRenderer : function(name, fn) {
        this.cellRenderers[name] = fn;
    },
    
    onMetaChange : function(store, meta) {
        console.log("onMetaChange", store, meta);
        
        var field;
        var config = [];
        var autoExpand = false;
        for(var i=0; i<meta.fields.length; i++)
        {
            // loop for every dataIndex, only add fields with a header property
            field = meta.fields[i];
            if(field.header !== undefined){
                field.dataIndex = field.name;
                
                // search for cell render functions by [field.renderer] or _[field.name]
                if(this.cellRenderers[field.renderer]) {
                    field.renderer = this.cellRenderers[field.renderer];
                } else if(this.cellRenderers["_"+field.name]) {
                    field.renderer = this.cellRenderers["_"+field.name];
                }
                
                // add editors
                if(field.editor !== undefined){
                    var editor = field.editor;
                    switch (editor.type)
                    { 
                        case 'TextField' :
                            field.editor = new Ext.grid.GridEditor(new Ext.form.TextField(editor.config));
                            break;

                        case 'NumberField' :
                            field.editor = new Ext.grid.GridEditor(new Ext.form.NumberField(editor.config));
                            break;

                        case 'DateField' :
                            field.editor = new Ext.grid.GridEditor(new Ext.form.DateField(editor.config));
                            break;

                        case 'Checkbox' :
                            field.editor = new Ext.grid.GridEditor(new Ext.form.Checkbox(editor.config));
                            break;

                        default :
                            alert('type: unknow');
                    }
                }                

                // Auto assign an id if none given.
                if(!field.id) {
                    field.id = 'c' + i;
                }
                
                // if width is auto, set autoExpand variabel (should only be set after reconfigure for some reason)
                if(field.width == "auto") {
                    autoExpand = field.id;
                    field.width = 100;
                }
                                
                // add to the config (field.name is replaced by dataIndex)
                delete field.name;
                config[config.length] = field;                
            }
        }
        
        // Create the new cm, and update the gridview.
        var cm = new Ext.grid.ColumnModel(config);
        this.reconfigure(this.dataSource, cm);
        
        this.autoExpandColumn = autoExpand;        
    }
});