/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.grid.GridView.UIController = function(config){
    Ext.apply(this, config);
};

Ext.grid.GridView.UIController.prototype = {

    initUI : function(grid){
        grid.on("headerclick", this.onHeaderClick, this);

        if(grid.trackMouseOver){
            grid.on("mouseover", this.onRowOver, this);
	        grid.on("mouseout", this.onRowOut, this);
	    }
    },

    initEvents : function(){

    },

    onHeaderClick : function(g, index){
        if(this.headersDisabled || !this.cm.isSortable(index)){
            return;
        }
	    g.stopEditing();
        g.dataSource.sort(this.cm.getDataIndex(index));
    },

    onRowOver : function(e, t){
        var row;
        if((row = this.findRowIndex(t)) !== false){
            this.addRowClass(row, "x-grid3-row-over");
        }
    },

    onRowOut : function(e, t){
        var row;
        if((row = this.findRowIndex(t)) !== false && row !== this.findRowIndex(e.getRelatedTarget())){
            this.removeRowClass(row, "x-grid3-row-over");
        }
    },

    handleWheel : function(e){
        e.stopPropagation();
    },

    onRowSelect : function(row){
        this.addRowClass(row, "x-grid3-row-selected");
    },

    onRowDeselect : function(row){
        this.removeRowClass(row, "x-grid3-row-selected");
    },

    onCellSelect : function(row, col){
        var cell = this.getCell(row, col);
        if(cell){
            this.fly(cell).addClass("x-grid3-cell-selected");
        }
    },

    onCellDeselect : function(row, col){
        var cell = this.getCell(row, col);
        if(cell){
            this.fly(cell).removeClass("x-grid3-cell-selected");
        }
    },

    onColumnSplitterMoved : function(i, w){
        this.userResized = true;
        var cm = this.grid.colModel;
        cm.setColumnWidth(i, w, true);

        if(this.forceFit){
            this.fitColumns(true, false, i);
            this.updateAllColumnWidths();
        }else{
            this.updateColumnWidth(i, w);
        }
        
        //this.layout();
        this.grid.fireEvent("columnresize", i, w);
    },

    handleHdMenuClick : function(item){
        var index = this.hdCtxIndex;
        var cm = this.cm, ds = this.ds;
        switch(item.id){
            case "asc":
                ds.sort(cm.getDataIndex(index), "ASC");
                break;
            case "desc":
                ds.sort(cm.getDataIndex(index), "DESC");
                break;
            case "lock":
                var lc = cm.getLockedCount();
                if(cm.getColumnCount(true) <= lc+1){
                    this.onDenyColumnLock();
                    return;
                }
                if(lc != index){
                    cm.setLocked(index, true, true);
                    cm.moveColumn(index, lc);
                    this.grid.fireEvent("columnmove", index, lc);
                }else{
                    cm.setLocked(index, true);
                }
            break;
            case "unlock":
                var lc = cm.getLockedCount();
                if((lc-1) != index){
                    cm.setLocked(index, false, true);
                    cm.moveColumn(index, lc-1);
                    this.grid.fireEvent("columnmove", index, lc-1);
                }else{
                    cm.setLocked(index, false);
                }
            break;
            default:
                index = cm.getIndexById(item.id.substr(4));
                if(index != -1){
                    if(item.checked && cm.getColumnCount(true) <= 1){
                        this.onDenyColumnHide();
                        return false;
                    }
                    cm.setHidden(index, item.checked);
                }
        }
        return true;
    },

    beforeColMenuShow : function(){
        var cm = this.cm,  colCount = cm.getColumnCount();
        this.colMenu.removeAll();
        for(var i = 0; i < colCount; i++){
            this.colMenu.add(new Ext.menu.CheckItem({
                id: "col-"+cm.getColumnId(i),
                text: cm.getColumnHeader(i),
                checked: !cm.isHidden(i),
                hideOnClick:false
            }));
        }
    },

    handleHdCtx : function(g, index, e){
        e.stopEvent();
        var hd = this.getHeaderCell(index);
        this.hdCtxIndex = index;
        var ms = this.hmenu.items, cm = this.cm;
        ms.get("asc").setDisabled(!cm.isSortable(index));
        ms.get("desc").setDisabled(!cm.isSortable(index));
        if(this.grid.enableColLock !== false){
            ms.get("lock").setDisabled(cm.isLocked(index));
            ms.get("unlock").setDisabled(!cm.isLocked(index));
        }
        this.hmenu.show(hd, "tl-bl");
    },

    handleHdOver : function(e, t){
        var hd = this.findHeaderCell(t);
        if(hd && !this.headersDisabled){
            this.activeHd = hd;
            this.activeHdRegion = this.fly(hd).getRegion();
            if(this.grid.colModel.isSortable(this.getCellIndex(hd))){
               this.fly(hd).addClass("x-grid3-hd-over");
            }
        }
    },

    handleHdMove : function(e, t){
        if(this.activeHd && !this.headersDisabled){
            var hw = this.splitHandleWidth || 5;
            var r = this.activeHdRegion;
            var x = e.getPageX();
            var ss = this.activeHd.style;
            if(x - r.left <= hw){
                ss.cursor = 'e-resize';// col-resize not always supported
                ss.cursor = 'col-resize';
            }else if(r.right - x <= hw){
                ss.cursor = 'w-resize'; // col-resize not always supported
                ss.cursor = 'col-resize';
            }else{
                ss.cursor = '';
            }
        }
    },

    handleHdOut : function(e){
        var hd = this.findHeaderCell(e.getTarget());
        if(hd){
            this.activeHd = null;
            this.fly(hd).removeClass("x-grid3-hd-over");
            hd.style.cursor = '';
        }
    },

    handleSplitDblClick : function(e, t){
        return;
        var i = this.getCellIndex(t);
        if(this.grid.enableColumnResize !== false && this.cm.isResizable(i) && !this.cm.isFixed(i)){
            this.autoSizeColumn(i, true);
            this.layout();
        }
    }
};

