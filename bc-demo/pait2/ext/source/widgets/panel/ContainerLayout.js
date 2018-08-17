/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.layout.ContainerLayout = function(config){
    Ext.apply(this, config);
};

Ext.layout.ContainerLayout.prototype = {
    monitorResize:false,
    activeItem : null,

    layout : function(){
        this.onLayout(this.container,
                this.container.getLayoutTarget());
    },

    onLayout : function(ct, target){
        var items = ct.items.items;
        for(var i = 0, len = items.length; i < len; i++) {
            var c = items[i];
            if(!c.rendered){
                c.render(target, i);
                if(this.renderHidden && c != this.activeItem){
                    c.hide();
                }
            }
        }
    },

    onResize: function(){
        this.layout();
    },

    setContainer : function(ct){
        if(this.monitorResize){
            if(ct && this.container && ct != this.container){
                this.container.un('resize', this.onResize, this);
            }
            ct.on('resize', this.onResize, this);
        }
        this.container = ct;
    }
};