/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.grid.GridView.EventListener = function(config){
    Ext.apply(this, config);
};

Ext.grid.GridView.EventListener.prototype = {
    initData : function(ds, cm){
        if(this.ds){
            this.ds.un("load", this.onLoad, this);
            this.ds.un("datachanged", this.onDataChange);
            this.ds.un("add", this.onAdd);
            this.ds.un("remove", this.onRemove);
            this.ds.un("update", this.onUpdate);
            this.ds.un("clear", this.onClear);
        }
        if(ds){
            ds.on("load", this.onLoad, this);
            ds.on("datachanged", this.onDataChange, this);
            ds.on("add", this.onAdd, this);
            ds.on("remove", this.onRemove, this);
            ds.on("update", this.onUpdate, this);
            ds.on("clear", this.onClear, this);
        }
        this.ds = ds;

        if(this.cm){
            this.cm.un("widthchange", this.onColWidthChange, this);
            this.cm.un("headerchange", this.onHeaderChange, this);
            this.cm.un("hiddenchange", this.onHiddenChange, this);
            this.cm.un("columnmoved", this.onColumnMove, this);
            this.cm.un("columnlockchange", this.onColumnLock, this);
        }
        if(cm){
            this.generateRules(cm);
            cm.on("widthchange", this.onColWidthChange, this);
            cm.on("headerchange", this.onHeaderChange, this);
            cm.on("hiddenchange", this.onHiddenChange, this);
            cm.on("columnmoved", this.onColumnMove, this);
            cm.on("columnlockchange", this.onColumnLock, this);
        }
        this.cm = cm;
    },

    onDataChange : function(){
        this.refresh();
        this.updateHeaderSortState();
    },

	onClear : function(){
        this.refresh();
    },

	onUpdate : function(ds, record){
        this.refreshRow(record);
    },

    onAdd : function(ds, records, index){
        this.insertRows(ds, index, index + (records.length-1));
    },

    onRemove : function(ds, record, index, isUpdate){
        if(isUpdate !== true){
            this.fireEvent("beforerowremoved", this, index, record);
        }
        this.removeRow(index);
        if(isUpdate !== true){
            this.stripeRows(index);
            this.layout();
            this.fireEvent("rowremoved", this, index, record);
        }
    },

    onLoad : function(){
        this.scrollToTop();
    },

    onColWidthChange : function(cm, col, width){
        this.updateColumnWidth(col, width);
    },
    
    onHeaderChange : function(cm, col, text){
        this.updateHeaderText(col, text);
    },

    onHiddenChange : function(cm, col, hidden){

    },
    
    onColumnMove : function(cm, oldIndex, newIndex){
        this.indexMap = null;
        var s = this.getScrollState();
        this.refresh(true);
        this.restoreScroll(s);
        this.afterMove(newIndex);
    },

    onColumnLock : Ext.emptyFn
};

