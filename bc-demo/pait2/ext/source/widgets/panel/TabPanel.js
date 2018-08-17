/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.TabPanel2 = Ext.extend(Ext.Panel,  {

    monitorResize : true,

    tabWidth: 80,
    minTabWidth: 30,
    resizeTabs:false,

    tabPosition: 'top',
    elements: 'body',
    baseCls: 'x-tab-panel',
    frame:false,
    autoTabs : false,
    itemCls : 'x-tab-item',
    activeTab : null,
    headerAsText : false,
    tabMargin : 2,
    
    initComponent : function(){
        this.frame = false;
        Ext.TabPanel2.superclass.initComponent.call(this);
        this.addEvents({
            beforetabchange: true,
            tabchange : true,
            contextmenu: true
        });
        this.setLayout(new Ext.layout.CardLayout());
        if(this.tabPosition == 'top'){
            this.elements += ',header';
            this.stripTarget = 'header';
        }else {
            this.elements += ',footer';
            this.stripTarget = 'footer';
        }
        if(!this.stack){
            this.stack = Ext.TabPanel2.AccessStack();
        }
        this.initItems();
    },

    render : function(){
        Ext.TabPanel2.superclass.render.apply(this, arguments);
        if(this.activeItem !== undefined){
            var item = this.activeItem;
            delete this.activeItem;
            this.setActiveTab(item);
        }
    },

    onRender : function(ct, position){
        Ext.TabPanel2.superclass.onRender.call(this, ct, position);

        var st = this[this.stripTarget];
        
        this.stripWrap = st.createChild({cls:'x-tab-strip-wrap', cn:{
            tag:'ul', cls:'x-tab-strip x-tab-strip-'+this.tabPosition}});

        this.strip = new Ext.Element(this.stripWrap.dom.firstChild);
        
        this.strip.createChild({cls:'x-clear'});

        this.body.addClass('x-tab-panel-body-'+this.tabPosition);
        
        if(!this.itemTpl){
            var tt = new Ext.Template(
                 '<li class="{cls}" id="{id}"><a class="x-tab-strip-close" onclick="return false;"></a>',
                 '<a class="x-tab-right" href="#" onclick="return false;"><em class="x-tab-left">',
                 '<span class="x-tab-strip-inner"><span class="x-tab-strip-text">{text}</span></span>',
                 '</em></a></li>'
            );
            tt.disableFormats = true;
            tt.compile();
            Ext.TabPanel2.prototype.itemTpl = tt;
        }

        this.items.each(this.initTab, this);
    },

    initEvents : function(){
        Ext.TabPanel2.superclass.initEvents.call(this);
        this.on('add', this.onAdd, this);
        this.on('remove', this.onRemove, this);

        this.strip.on('mousedown', this.onStripMouseDown, this);
        this.strip.on('click', this.onStripClick, this);
        this.strip.on('contextmenu', this.onStripContextMenu, this);
    },

    findTargets : function(e){
        var item = null;
        var itemEl = e.getTarget('li');
        if(itemEl){
            item = this.getComponent(itemEl.id.split('__')[1]);
        }
        return {
            close : e.getTarget('.x-tab-strip-close'),
            item : item,
            el : itemEl
        };
    },

    onStripMouseDown : function(e){
        e.preventDefault();
        if(e.button != 0){
            return;
        }
        var t = this.findTargets(e);
        if(t.close){
            this.remove(t.item);
            return;
        }
        if(t.item){
            this.setActiveTab(t.item);
        }
    },

    onStripClick : function(e){
        var t = this.findTargets(e);
        if(!t.close && t.item & t.item != this.activeItem){
            this.setActiveTab(t.item);
        }
    },

    onStripContextMenu : function(e){
        e.preventDefault();
        var t = this.findTargets(e);
        if(t.item){
            this.fireEvent('contextmenu', this, item, e);
        }
    },

    initTab : function(item, index){
        var before = this.strip.dom.childNodes[index];
        var cls = item.closable ? 'x-tab-strip-closable' : '';
        if(item.disabled){
            cls += ' x-tab-strip-disabled';
        }
        var p = {
            id: this.id + '__' + item.id,
            text: item.title,
            cls: cls
        };
        var el = before ?
                 this.itemTpl.insertBefore(before, p) :
                 this.itemTpl.append(this.strip, p);

        item.on('disable', this.onItemDisabled, this);
        item.on('enable', this.onItemEnabled, this);
        item.on('titlechange', this.onItemTitleChanged, this);
    },

    onAdd : function(tp, item, index){
        this.initTab(item, index);
        if(this.resizeTabs && this.rendered){
            this.autoSizeTabs();
        }
    },

    onRemove : function(tp, item){
        var el = this.getTabEl(item);
        if(el){
            el.parentNode.removeChild(el);
        }
        this.stack.remove(item);
        if(item == this.activeItem){
            var next = this.stack.next();
            if(next){
                this.setActiveTab(next);
            }
        }
        if(this.resizeTabs && this.rendered){
            this.autoSizeTabs();
        }
    },

    onItemDisabled : function(item){
        var el = this.getTabEl(item);
        if(el){
            Ext.fly(el).addClass('x-tab-strip-disabled');
        }
        this.stack.remove(item);
    },

    onItemEnabled : function(item){
        var el = this.getTabEl(item);
        if(el){
            Ext.fly(el).removeClass('x-tab-strip-disabled');
        }
    },

    onItemTitleChanged : function(item){
        var el = this.getTabEl(item);
        if(el){
            Ext.fly(el).child('span.x-tab-strip-text', true).innerHTML = item.title;
        }
    },

    getTabEl : function(item){
        return document.getElementById(this.id+'__'+item.id);
    },

    onResize : function(){
        Ext.TabPanel2.superclass.onResize.apply(this, arguments);
        if(this.resizeTabs){
            this.autoSizeTabs();
        }
    },

    autoSizeTabs : function(){
        var count = this.items.length;
        var aw = this.stripWrap.dom.offsetWidth;
        if(!this.resizeTabs || count < 1 || !aw){ // !aw for display:none
            return;
        }
        var each = Math.max(Math.min(Math.floor((aw-4) / count) - this.tabMargin, this.tabWidth), this.minTabWidth); // -4 for float errors in IE
        var lis = this.stripWrap.dom.getElementsByTagName('li');
        for(var i = 0, len = lis.length; i < len; i++) {
            var li = lis[i];
            var inner = li.childNodes[1].firstChild.firstChild;
            var tw = li.offsetWidth;
            var iw = inner.offsetWidth;
            inner.style.width = (each - (tw-iw)) + 'px';
        }
    },

    setActiveTab : function(item){
        item = this.getComponent(item);
        if(this.fireEvent('beforetabchange', this, item, this.activeItem) === false){
            return;
        }
        if(!this.rendered){
            this.activeItem = item;
            return;
        }
        if(this.activeItem != item){
            if(this.activeItem){
                var oldEl = this.getTabEl(this.activeItem);
                if(oldEl){
                    Ext.fly(oldEl).removeClass('x-tab-strip-active');
                }
            }
            var el = this.getTabEl(item);
            Ext.fly(el).addClass('x-tab-strip-active');
            this.layout.setActiveItem(item);

            this.activeItem = item;
            this.stack.add(item);
            this.fireEvent('tabchange', this, item);
        }
    },

    getItem : function(item){
        return this.getComponent(item);
    }
});

Ext.TabPanel2.AccessStack = function(){
    var items = [];
    return {
        add : function(item){
            items.push(item);
            if(items.length > 10){
                items.shift();
            }
        },

        remove : function(item){
            var s = [];
            for(var i = 0, len = items.length; i < len; i++) {
                if(items[i] != item){
                    s.push(items[i]);
                }
            }
            items = s;
        },

        next : function(){
            return items.pop();
        }
    };
};

