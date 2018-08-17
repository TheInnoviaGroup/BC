/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.Container = function(config){
    Ext.Container.superclass.constructor.call(this, config);
    if(this.renderTo){
        this.render(this.renderTo);
        delete this.renderTo;
    }
};

Ext.extend(Ext.Container, Ext.BoxComponent, {
    autoDestroy: true,

    initComponent : function(){
        Ext.Container.superclass.initComponent.call(this);

        this.addEvents({
            'beforeadd':true,
            'beforeremove':true,
            'add':true,
            'remove':true
        });

        var items = this.items;
        if(items){
            delete this.items;
            if(items instanceof Array){
                this.add.apply(this, items);
            }else{
                this.add(items);
            }
        }
    },

    initItems : function(){
        if(!this.items){
            this.items = new Ext.util.MixedCollection(false, this.getComponentId);
            this.getLayout(); // initialize the layout
        }
    },

    setLayout : function(layout){
        if(this.layout && this.layout != layout){
            this.layout.setContainer(null);
        }
        this.layout = layout;
        layout.setContainer(this);
    },

    render : function(){
        Ext.Container.superclass.render.apply(this, arguments);
        if(this.layout){
            this.setLayout(this.layout);
        }
        this.doLayout();
    },

    getLayoutTarget : function(){
        return this.el;  
    },

    getComponentId : function(comp){
        return comp.id;
    },

    add : function(comp){
        if(!this.items){
            this.initItems();
        }
        var a = arguments, len = a.length;
        if(len > 1){
            for(var i = 0; i < len; i++) {
                this.add(a[i]);
            }
        }else{
            var c = this.lookupComponent(comp);
            var pos = this.items.length;
            if(this.fireEvent('beforeadd', this, c, pos) !== false){
                this.items.add(c);
                c.ownerCt = this;
                this.fireEvent('add', this, c, pos);
            }
        }
        return this;
    },

    insert : function(index, comp){
        if(!this.items){
            this.initItems();
        }
        var a = arguments, len = a.length;
        if(len > 1){
            for(var i = len; i >= 0; --i) {
                this.insert(index, a[i]);
            }
        }else{

            var c = this.lookupComponent(comp);
            if(this.fireEvent('beforeadd', this, c, index) !== false){
                this.items.insert(index, c);
                this.fireEvent('add', this, c, index);
            }
        }
        return this;
    },

    remove : function(comp){
        var c = this.getComponent(comp);
        if(this.fireEvent('beforeremove', this, c) !== false){
            this.items.remove(c);
            if(this.autoDestroy){
                c.destroy();
            }
            this.fireEvent('remove', this, c);
        }
    },

    getComponent : function(comp){
        if(typeof comp == 'object'){
            return comp;
        }
        return this.items.get(comp);
    },

    // private
    lookupComponent : function(comp){
        if(typeof comp == 'string'){
            return Ext.ComponentMgr.get(comp);
        }else if(!comp.events){
            return this.createComponent(comp);
        }
        return comp;
    },

    createComponent : function(config){
        return new Ext.Panel(config);
    },

    doLayout : function(){
        if(this.layout){
            this.layout.layout();
        }
        if(this.items){
            var cs = this.items.items;
            for(var i = 0, len = cs.length; i < len; i++) {
                var c  = cs[i];
                if(c.doLayout){
                    c.doLayout();
                }
            }
        }
    },

    getLayout : function(){
        if(!this.layout){
            var layout = new Ext.layout.ContainerLayout();
            this.setLayout(layout);
        }
        return this.layout;
    }
});