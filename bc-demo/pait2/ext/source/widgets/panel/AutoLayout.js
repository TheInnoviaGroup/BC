/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.layout.FitLayout = Ext.extend(Ext.layout.ContainerLayout, {
    monitorResize:true,    
    onLayout : function(ct, target){
        Ext.layout.FitLayout.superclass.onLayout.call(this, ct, target);
        var item = this.activeItem || ct.items.itemAt(0);
        if(item){
            item.setSize(target.getSize(true));
        }
    }
});

Ext.layout.CardLayout = Ext.extend(Ext.layout.FitLayout, {
    renderHidden : true,
    setActiveItem : function(item){
        item = this.container.getComponent(item);
        if(this.activeItem != item){
            if(this.activeItem){
                this.activeItem.hide();
            }
            this.activeItem = item;
            item.show();
            this.layout();
        }
    }
});
