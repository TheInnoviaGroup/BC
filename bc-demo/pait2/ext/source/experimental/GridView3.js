/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.grid.GridView3 = function(config){
    Ext.grid.GridView3.superclass.constructor.call(this);
    Ext.apply(this, config);
};

Ext.extend(Ext.grid.GridView3, Ext.grid.AbstractGridView, {

    borderWidth: 2,
    scrollOffset: 17,
    autoFill:false,
    forceFit:false,

    init: function(grid){
        this.grid = grid;

        Ext.apply(this, this.ui);
        Ext.apply(this, this.modelHandler);
        Ext.apply(this, this.uiHandler);

        this.initData(grid.dataSource, grid.colModel);
        this.initUI(grid);
	},

	getColumnId : function(index){
	    return this.cm.getColumnId(index);
	},

    renderHeaders : function(){
	    var cm = this.cm, ts = this.templates;
        var ct = ts.hcell;

        var cb = [], sb = [], p = {};

        for(var i = 0, len = cm.getColumnCount(); i < len; i++){
            p.id = cm.getColumnId(i);
            p.value = cm.getColumnHeader(i) || "";
            p.style = this.getColumnStyle(i, true);
            cb[cb.length] = ct.apply(p);
        }
        return ts.header.apply({cells: cb.join(""), tstyle:'width:'+this.getTotalWidth()+';'});
	},

    beforeUpdate : function(){
        this.grid.stopEditing();
    },

    updateHeaders : function(){
        this.mainHd.dom.innerHTML = this.renderHeaders();
    },

    /**
     * Focuses the specified row..
     * @param {Number} row The row index
     */
    focusRow : function(row){
        this.focusCell(row, 0, false);
    },

    /**
     * Focuses the specified cell.
     * @param {Number} row The row index
     * @param {Number} col The column index
     */
    focusCell : function(row, col, hscroll){
        var el = this.ensureVisible(row, col, hscroll);
        if(el){
            this.focusEl.alignTo(el, "tl-tl");
            this.focusEl.focus.defer(1, this.focusEl);
        }
    },

    /** @ignore */
    ensureVisible : function(row, col, hscroll){
        return;
        if(typeof row != "number"){
            row = row.rowIndex;
        }
        if(row < 0 && row >= this.ds.getCount()){
            return;
        }
        col = (col !== undefined ? col : 0);
        while(this.cm.isHidden(col)){
            col++;
        }

        var el = this.getCell(row, col);
        if(!el){
            return;
        }
        var c = this.mainBody.dom;

        var ctop = parseInt(el.offsetTop, 10);
        var cleft = parseInt(el.offsetLeft, 10);
        var cbot = ctop + el.offsetHeight;
        var cright = cleft + el.offsetWidth;

        var ch = c.clientHeight - this.mainHd.dom.offsetHeight;
        var stop = parseInt(c.scrollTop, 10);
        var sleft = parseInt(c.scrollLeft, 10);
        var sbot = stop + ch;
        var sright = sleft + c.clientWidth;

        if(ctop < stop){
        	c.scrollTop = ctop;
        }else if(cbot > sbot){
            c.scrollTop = cbot-ch;
        }

        if(hscroll !== false){
            if(cleft < sleft){
                c.scrollLeft = cleft;
            }else if(cright > sright){
                c.scrollLeft = cright-c.clientWidth;
            }
        }
        return el;
    },

    /*
    hideColumn : function(col){
        var cid = this.getColumnId(col);
        this.css.updateRule(this.tdSelector+cid, "display", "none");
        this.layout();
    },

    unhideColumn : function(col){
        var cid = this.getColumnId(col);
        this.css.updateRule(this.tdSelector+cid, "display", "");
        this.layout();
    },*/

    insertRows : function(dm, firstRow, lastRow, isUpdate){
        if(firstRow == 0 && lastRow == dm.getCount()-1){
            this.refresh();
        }else{
            if(!isUpdate){
                this.fireEvent("beforerowsinserted", this, firstRow, lastRow);
            }
            //var s = this.getScrollState();

            this.renderRows(firstRow, lastRow);
            //this.restoreScroll(s);

            if(!isUpdate){
                this.fireEvent("rowsinserted", this, firstRow, lastRow);
                this.stripeRows(firstRow);
                this.layout();
            }
        }
    },

    deleteRows : function(dm, firstRow, lastRow){
        if(dm.getRowCount()<1){
            this.fireEvent("beforerefresh", this);
            this.mainBody.update("");
            this.fireEvent("refresh", this);
        }else{
            this.fireEvent("beforerowsdeleted", this, firstRow, lastRow);

            this.removeRows(firstRow, lastRow);

            this.stripeRows(firstRow);
            this.fireEvent("rowsdeleted", this, firstRow, lastRow);
        }
    },

    getColumnStyle : function(col, isHeader){
        var style = !isHeader ? (this.cm.config[col].css || '') : '';
        style += 'width:'+this.getColumnWidth(col)+';';
        if(this.cm.isHidden(col)){
            style += 'display:none;';
        }
        var align = this.cm.config[col].align;
        if(align){
            style += 'text-align:'+align+';';
        }
        return style;
    },

    getColumnWidth : function(col){
        var w = this.cm.getColumnWidth(col);
        if(typeof w == 'number'){
            return (Ext.isBorderBox ? w : w-this.borderWidth) + 'px';
        }
        return w;
    },

    getTotalWidth : function(){
        return this.cm.getTotalWidth()+'px';
    },

    fitColumns : function(preventRefresh, onlyExpand, omitColumn){
        var cm = this.cm, leftOver, dist, i;
        var tw = cm.getTotalWidth(false);
        var aw = this.grid.container.getWidth(true)-this.scrollOffset;
        var extra = aw - tw;

        if(extra === 0){
            return false;
        }

        var count = cm.getColumnCount();
        var vc = cm.getColumnCount(true);
        var ac = vc-(typeof omitColumn == 'number' ? 1 : 0);
        if(ac === 0){
            ac = 1;
            omitColumn = undefined;
        }

        if(extra > 0){
            if(extra < vc){ // just a little extra, so add to last column
                for(i = count-1; i >= 0; i--){
                    if(!cm.isHidden(i)){
                        cm.setColumnWidth(i, cm.getColumnWidth(i)+extra, true);
                        break;
                    }
                }
            }else{
                leftOver = extra % ac;
                dist = (extra - leftOver) / ac;
                for(i = 0; i < count; i++){
                    if(i !== omitColumn && !cm.isHidden(i)){
                        cm.setColumnWidth(i, cm.getColumnWidth(i)+dist+(--ac == 0 ? leftOver : 0), true);
                    }
                }
            }
        } else {
            extra *= -1;
            leftOver = extra % ac;
            dist = (extra - leftOver) / ac;
            for(i = 0; i < count; i++){
                if(i !== omitColumn && !cm.isHidden(i)){
                    cm.setColumnWidth(i, Math.max(1, cm.getColumnWidth(i)-(dist+(--ac == 0 ? leftOver : 0))), true);
                }
            }
        }
        if(ac != vc && (tw = cm.getTotalWidth(false)) > aw){
             cm.setColumnWidth(omitColumn, Math.max(1,
                     cm.getColumnWidth(omitColumn)- (tw-aw)), true);       
        }

        if(preventRefresh !== true){
            this.updateAllColumnWidths();
        }
        return true;
    },

    autoExpand : function(preventUpdate){
        var g = this.grid, cm = this.cm;
        if(!this.userResized && g.autoExpandColumn){
            var tw = cm.getTotalWidth(false);
            var aw = this.grid.container.getWidth(true)-this.scrollOffset;
            if(tw != aw){
                var ci = cm.getIndexById(g.autoExpandColumn);
                var currentWidth = cm.getColumnWidth(ci);
                var cw = Math.min(Math.max(((aw-tw)+currentWidth), g.autoExpandMin), g.autoExpandMax);
                if(cw != currentWidth){
                    cm.setColumnWidth(ci, cw, true);
                    if(preventUpdate !== true){
                        this.updateColumnWidth(ci, cw);
                    }
                }
            }

        }
    },

    renderRows : function(startRow, endRow){
        // pull in all the crap needed to render rows
        var g = this.grid, cm = g.colModel, ds = g.dataSource, stripe = g.stripeRows;
        var colCount = cm.getColumnCount();

        if(ds.getCount() < 1){
            return ["", ""];
        }

        // build a map for all the columns
        var cs = [];
        for(var i = 0; i < colCount; i++){
            var name = cm.getDataIndex(i);

            cs[i] = {
                name : (typeof name == 'undefined' ? ds.fields.get(i).name : name),
                renderer : cm.getRenderer(i),
                id : cm.getColumnId(i),
                locked : cm.isLocked(i),
                style : this.getColumnStyle(i)
            };
        }

        startRow = startRow || 0;
        endRow = typeof endRow == "undefined"? ds.getCount()-1 : endRow;

        // records to render
        var rs = ds.getRange(startRow, endRow);

        return this.doRender(cs, rs, ds, startRow, colCount, stripe);
    },

    renderBody : function(){
        var markup = this.renderRows();
        return this.templates.body.apply({rows: markup});
    },

    refreshRow : function(record){
        var ds = this.ds, index;
        if(typeof record == 'number'){
            index = record;
            record = ds.getAt(index);
        }else{
            index = ds.indexOf(record);
        }
        var cls = [];
        this.insertRows(ds, index, index, true);
        this.onRemove(ds, record, index+1, true);
        this.layout();
        this.fireEvent("rowupdated", this, index, record);
    },

    refresh : function(headersToo){
        this.fireEvent("beforerefresh", this);
        this.grid.stopEditing();
        var result = this.renderBody();
        //var t = new Date();
        this.mainBody.update(result);
        //alert(t.getElapsed())
        if(headersToo === true){
            this.updateHeaders();
            this.updateHeaderSortState();
        }
        this.stripeRows(0, true);
        this.layout();
        this.fireEvent("refresh", this);
    },

    updateHeaderSortState : function(){
        var state = this.ds.getSortState();
        if(!state){
            return;
        }
        this.sortState = state;
        var sortColumn = this.cm.findColumnIndex(state.field);
        if(sortColumn != -1){
            var sortDir = state.direction;
            this.updateSortIcon(sortColumn, sortDir);
        }
    },

    destroy : function(){
        return;
        if(this.colMenu){
            this.colMenu.removeAll();
            Ext.menu.MenuMgr.unregister(this.colMenu);
            this.colMenu.getEl().remove();
            delete this.colMenu;
        }
        if(this.hmenu){
            this.hmenu.removeAll();
            Ext.menu.MenuMgr.unregister(this.hmenu);
            this.hmenu.getEl().remove();
            delete this.hmenu;
        }
        if(this.grid.enableColumnMove){
            var dds = Ext.dd.DDM.ids['gridHeader' + this.grid.container.id];
            if(dds){
                for(var dd in dds){
                    if(!dds[dd].config.isTarget && dds[dd].dragElId){
                        var elid = dds[dd].dragElId;
                        dds[dd].unreg();
                        Ext.get(elid).remove();
                    } else if(dds[dd].config.isTarget){
                        dds[dd].proxyTop.remove();
                        dds[dd].proxyBottom.remove();
                        dds[dd].unreg();
                    }
                    if(Ext.dd.DDM.locationCache[dd]){
                        delete Ext.dd.DDM.locationCache[dd];
                    }
                }
                delete Ext.dd.DDM.ids['gridHeader' + this.grid.container.id];
            }
        }

        this.bind(null, null);
        Ext.EventManager.removeResizeListener(this.onWindowResize, this);
    },

    onDenyColumnHide : function(){

    },

    render : function(){

        var cm = this.cm;
        var colCount = cm.getColumnCount();

        if(this.grid.monitorWindowResize === true){
            Ext.EventManager.onWindowResize(this.onWindowResize, this, true);
        }

        if(this.autoFill){
            this.fitColumns(true, true);
        }else if(this.forceFit){
            this.fitColumns(true, false);
        }else if(this.grid.autoExpandColumn){
            this.autoExpand(true);
        }

        this.renderUI();

        this.layout();

        // two part rendering gives faster view to the user
        this.renderPhase2.defer(1, this);
    },

    renderPhase2 : function(){
        // render the rows now
        this.refresh();
    },

    onWindowResize : function(){
        if(!this.grid.monitorWindowResize || this.grid.autoHeight){
            return;
        }
        this.layout();
    },

    sortAscText : "Sort Ascending",
    sortDescText : "Sort Descending",
    lockText : "Lock Column",
    unlockText : "Unlock Column",
    columnsText : "Columns"
});