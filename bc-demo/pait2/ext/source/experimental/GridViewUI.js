/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.grid.GridView.Template = function(config){
    Ext.apply(this, config);

    var rc = this.rootCls;

    var ts = this.templates || {};
    if(!ts.master){
        ts.master = new Ext.Template(
                '<div class="', rc, '" hidefocus="true">',
                '<div class="', rc, '-topbar"></div>',
                '<div class="', rc, '-viewport">',
                '<div class="', rc, '-header"><div class="', rc, '-header-pop"><div class="', rc, '-header-pop-inner">&#160;</div></div><div class="', rc, '-header-inner">{header}</div><div class="x-clear"></div></div>',
                '<div class="', rc, '-body">{body}</div>',
                "</div>",
                '<div class="', rc, '-bottombar"></div>',
		          '<a href="#" class="', rc, '-focus" tabIndex="-1"></a>',
		          '<div class="', rc, '-resize-proxy">&#160;</div>',
                "</div>"
                );
    }

    if(!ts.header){
        ts.header = new Ext.Template(
                '<table border="0" cellspacing="0" cellpadding="0" style="{tstyle}">',
                '<tbody><tr class="', rc, '-hd-row">{cells}</tr></tbody>',
                "</table>"
                );
    }

    if(!ts.hcell){
        ts.hcell = new Ext.Template(
                '<td class="', rc, '-hd ', rc, '-cell ', rc, '-td-{id}" style="{style}"><div {attr} class="', rc, '-hd-inner ', rc, '-hd-{id}" unselectable="on">',
                '{value}<img class="', rc, '-sort-icon" src="', Ext.BLANK_IMAGE_URL, '" />',
                "</div></td>"
                );
    }

    if(!ts.body){
        ts.body = new Ext.Template('{rows}');
    }

    if(!ts.row){
        ts.row = new Ext.Template(
                '<div class="', rc, '-row {alt}" style="{tstyle}"><table class="', rc, '-row-table" border="0" cellspacing="0" cellpadding="0" style="{tstyle}">',
                '<tbody><tr>{cells}</tr>',
                        '<tr style="{bodyStyle}"><td colspan="{cols}" class="', rc, '-body-cell"><div class="', rc, '-row-body"></div></td></tr></tbody>',
                '</table></div>'
                );
    }

    if(!ts.cell){
        ts.cell = new Ext.Template(
                '<td class="', rc, '-col ', rc, '-cell ', rc, '-td-{id} {css}" style="{style}" tabIndex="0" {cellAttr}>',
                '<div class="', rc, '-cell-inner ', rc, '-col-{id}" unselectable="on" {attr}>{value}</div>',
                "</td>"
                );
    }

    for(var k in ts){
        var t = ts[k];
        if(t && typeof t.compile == 'function' && !t.compiled){
            t.disableFormats = true;
            t.compile();
        }
    }

    this.templates = ts;

    this.tdClass = ''+rc+'-cell';
    this.cellSelector = 'td.'+rc+'-cell';
    this.hdCls = rc+'-hd';
    this.rowSelector = 'div.'+rc+'-row';
    this.colRe = new RegExp(rc+"-td-([^\\s]+)", "");
};

Ext.grid.GridView.Template.prototype = {
    rootCls : 'x-grid3',
    sortClasses : ["sort-asc", "sort-desc"],

    fly : function(el){
        if(!this._flyweight){
            this._flyweight = new Ext.Element.Flyweight(document.body);
        }
        this._flyweight.dom = el;
        return this._flyweight;
    },

    initElements : function(){
        var E = Ext.Element;
        
        var el = this.grid.container.dom.firstChild;
	    var cs = el.childNodes;

	    this.el = new E(el);
	    this.headerPanel = new E(el.firstChild);
	    this.headerPanel.enableDisplayMode("block");

        this.mainWrap = new E(cs[1]);
	    this.mainHd = new E(this.mainWrap.dom.firstChild);
	    this.innerHd = this.mainHd.dom.childNodes[1];
	    this.headerPop = new E(this.mainHd.dom.firstChild.firstChild);

        this.mainBody = new E(this.mainWrap.dom.childNodes[1]);

	    this.footerPanel = new E(cs[2]);
	    this.footerPanel.enableDisplayMode("block");

        this.focusEl = new E(cs[3]);
        this.focusEl.swallowEvent("click", true);

        this.resizeProxy = new E(cs[4]);
    },


    // extra panels for toolbars and such

    getHeaderPanel : function(doShow){
        if(doShow){
            this.headerPanel.show();
        }
        return this.headerPanel;
	},

	getFooterPanel : function(doShow){
        if(doShow){
            this.footerPanel.show();
        }
        return this.footerPanel;
	},

    // finder methods, used with delegation

    findCell : function(el){
        if(!el){
            return false;
        }
        return this.fly(el).findParent(this.cellSelector, 3);
    },

    findCellIndex : function(el, requiredCls){
        var cell = this.findCell(el);
        if(cell && (!requiredCls || this.fly(cell).hasClass(requiredCls))){
            return this.getCellIndex(cell);
        }
        return false;
    },

    getCellIndex : function(el){
        if(el){
            var m = el.className.match(this.colRe);
            if(m && m[1]){
                return this.cm.getIndexById(m[1]);
            }
        }
        return false;
    },

    findHeaderCell : function(el){
        var cell = this.findCell(el);
        return cell && this.fly(cell).hasClass(this.hdCls) ? cell : null;
    },

    findHeaderIndex : function(el){
        return this.findCellIndex(el, this.hdCls);
    },

    findRow : function(el){
        if(!el){
            return false;
        }
        return this.fly(el).findParent(this.rowSelector, 10);
    },

    findRowIndex : function(el){
        var r = this.findRow(el);
        return r ? r.rowIndex : false;
    },


    // getter methods for fetching elements dynamically in the grid

    getRow : function(row){
        return this.mainBody.dom.childNodes[row];
    },

    getCell : function(row, col){
        return this.mainBody.dom.getElementsByTagName('td')[col];
	},

    getHeaderCell : function(index){
	    return this.mainHd.dom.getElementsByTagName('td')[index];
	},


    // manipulating elements

    addRowClass : function(row, cls){
        var r = this.getRow(row);
        if(r){
            this.fly(r).addClass(cls);
        }
    },

    removeRowClass : function(row, cls){
        var r = this.getRow(row);
        if(r){
            this.fly(r).removeClass(cls);
        }
    },

    removeRow : function(row){
        var r = this.getRow(row);
        if(r){
            r.parentNode.removeChild(r);
        }
    },

    removeRows : function(firstRow, lastRow){
        var bd = this.mainBody.dom;
        for(var rowIndex = firstRow; rowIndex <= lastRow; rowIndex++){
            bd.removeChild(bd.childNodes[firstRow]);
        }
    },

    // scrolling stuff

    getScrollState : function(){
        var sb = this.mainBody.dom;
        return {left: sb.scrollLeft, top: sb.scrollTop};
    },

    restoreScroll : function(state){
        var sb = this.mainBody.dom;
        sb.scrollLeft = state.left;
        sb.scrollTop = state.top;
    },

    scrollToTop : function(){
        this.mainBody.dom.scrollTop = 0;
        this.mainBody.dom.scrollLeft = 0;
    },


    syncScroll : function(){
        this.mainBody.dom.scrollLeft=this.innerHd.scrollLeft = this.mainBody.dom.scrollLeft;
    },


    updateSortIcon : function(col, dir){
        var sc = this.sortClasses;
        var hds = this.mainHd.select('td').removeClass(sc);
        hds.item(col).addClass(sc[dir == "DESC" ? 1 : 0]);
    },

    updateAllColumnWidths : function(){
        var tw = this.getTotalWidth();
        var clen = this.cm.getColumnCount();
        var ws = [];
        for(var i = 0; i < clen; i++){
            ws[i] = this.getColumnWidth(i);
        }

        this.mainHd.dom.childNodes[1].firstChild.style.width = tw;
        for(var i = 0; i < clen; i++){
            var hd = this.getHeaderCell(i);
            hd.style.width = ws[i];
        }

        var ns = this.mainBody.dom.childNodes;
        for(var i = 0, len = ns.length; i < len; i++){
            ns[i].style.width = tw;
            ns[i].firstChild.style.width = tw;
            var row = ns[i].firstChild.rows[0];
            for(var j = 0; j < clen; j++){
                row.childNodes[j].style.width = ws[j];
            }
        }
    },

    updateColumnWidth : function(col, width){
        var w = this.getColumnWidth(col);
        var tw = this.getTotalWidth();

        this.mainHd.dom.childNodes[1].firstChild.style.width = tw;
        var hd = this.getHeaderCell(col);
        hd.style.width = w;

        var ns = this.mainBody.dom.childNodes;
        for(var i = 0, len = ns.length; i < len; i++){
            ns[i].style.width = tw;
            ns[i].firstChild.style.width = tw;
            ns[i].firstChild.rows[0].childNodes[col].style.width = w;
        }
    },

    updateColumnText : function(col, text){

    },

    afterMove : function(colIndex){
        //if(this.enableMoveAnim && Ext.enableFx){
        //    this.fly(this.getHeaderCell(colIndex).firstChild).highlight(this.hlColor);
        //}
    },

    doRender : function(cs, rs, ds, startRow, colCount, stripe){
        var ts = this.templates, ct = ts.cell, rt = ts.row;
        var tstyle = 'width:'+this.getTotalWidth()+';';
        // buffers
        var buf = [], cb, c, p = {}, rp = {tstyle: tstyle}, r;
        for(var j = 0, len = rs.length; j < len; j++){
            r = rs[j], cb = [], rowIndex = (j+startRow);
            for(var i = 0; i < colCount; i++){
                c = cs[i];
                p.id = c.id;
                p.css = p.attr = "";
                p.value = c.renderer(r.data[c.name], p, r, rowIndex, i, ds);
                p.style = c.style;
                if(p.value == undefined || p.value === "") p.value = "&#160;";
                if(r.dirty && typeof r.modified[c.name] !== 'undefined'){
                    p.css += ' x-grid-dirty-cell';
                }
                cb[cb.length] = ct.apply(p);
            }
            var alt = [];
            if(stripe && ((rowIndex+1) % 2 == 0)){
                alt[0] = "x-grid-row-alt";
            }
            if(r.dirty){
                alt[1] = " x-grid-dirty-row";
            }
            if(this.getRowClass){
                alt[2] = this.getRowClass(r, rowIndex);
            }
            rp.alt = alt.join(" ");
            rp.cells = cb.join("");

            buf[buf.length] =  rt.apply(rp);
        }
        return buf.join("");
    },

    stripeRows : function(startRow, skipStripe){
        if(this.ds.getCount() < 1){
            return;
        }
        skipStripe = skipStripe || !this.grid.stripeRows;
        startRow = startRow || 0;
        var rows = this.mainBody.dom.childNodes;
        var cls = ' x-grid-row-alt ';
        for(var i = startRow, len = rows.length; i < len; i++){
            var row = rows[i];
            row.rowIndex = i;
            if(!skipStripe){
                var isAlt = ((i+1) % 2 == 0);
                var hasAlt = (' '+row.className + ' ').indexOf(cls) != -1;
                if(isAlt == hasAlt){
                    continue;
                }
                if(isAlt){
                    row.className += " x-grid-row-alt";
                }else{
                    row.className = row.className.replace("x-grid-row-alt", "");
                }
            }
        }
    },

    renderUI : function(){
        var header = this.renderHeaders();
        var body = this.templates.body.apply({rows:''});

        var html = this.templates.master.apply({
            body: body,
            header: header
        });

        //this.updateColumns();

        this.grid.container.dom.innerHTML = html;

        this.initElements();

        
        this.mainHd.on("mouseover", this.handleHdOver, this);
        this.mainHd.on("mouseout", this.handleHdOut, this);
        this.mainHd.on("mousemove", this.handleHdMove, this);

        this.mainHd.on("dblclick", this.handleSplitDblClick, this,
                {delegate: "."+this.splitClass});

        this.mainBody.on('scroll', this.syncScroll,  this);
        if(this.grid.enableColumnResize !== false){
            new Ext.grid.GridView.SplitDragZone(this.grid, this.mainHd.dom);
        }

        if(this.grid.enableColumnMove){
            this.columnDrag = new Ext.grid.GridView.ColumnDragZone(this.grid, this.mainHd.dom);
            new Ext.grid.HeaderDropZone(this.grid, this.mainHd.dom);
        }

        /*
        if(this.grid.enableCtxMenu !== false){
            this.colMenu = new Ext.menu.Menu({id:this.grid.id + "-hcols-menu"});
            this.colMenu.on("beforeshow", this.beforeColMenuShow, this);
            this.colMenu.on("itemclick", this.handleHdMenuClick, this);

            this.hmenu = new Ext.menu.Menu({id: this.grid.id + "-hctx"});
            this.hmenu.add(
                {id:"asc", text: this.sortAscText, cls: "xg-hmenu-sort-asc"},
                {id:"desc", text: this.sortDescText, cls: "xg-hmenu-sort-desc"},
                "separator"
            );
            if(this.grid.enableColLock !== false){
                this.hmenu.add(
                    {id:"lock", text: this.lockText, cls: "xg-hmenu-lock"},
                    {id:"unlock", text: this.unlockText, cls: "xg-hmenu-unlock"},
                    "separator"
                );
            }
            this.hmenu.add(
                {id:"columns", text: this.columnsText, menu: this.colMenu}
            );
            this.hmenu.on("itemclick", this.handleHdMenuClick, this);

            this.grid.on("headercontextmenu", this.handleHdCtx, this);
        }

        if(this.grid.enableDragDrop || this.grid.enableDrag){
            var dd = new Ext.grid.GridDragZone(this.grid, {
                ddGroup : this.grid.ddGroup || 'GridDD'
            });
        }*/

        this.updateHeaderSortState();


    },

    layout : function(initialRender, is2ndPass){
        var g = this.grid;
        var c = g.container, cm = this.cm,
                expandCol = g.autoExpandColumn,
                gv = this;

        if(g.autoWidth){
            c.setWidth(cm.getTotalWidth()+c.getBorderWidth('lr'));
        }

        if(g.autoHeight){
            this.mainBody.dom.style.overflow = 'visible';
            return;
        }

        var scrollOffset = 16;
        
        var tbh = this.headerPanel.getHeight();
        var bbh = this.footerPanel.getHeight();

        var csize = c.getSize(true);

        this.el.setSize(csize.width, csize.height);

        this.headerPanel.setWidth(csize.width);
        this.footerPanel.setWidth(csize.width);

        var hdHeight = this.mainHd.getHeight();
        var vw = csize.width;
        var vh = csize.height - (hdHeight + tbh + bbh);

        this.mainBody.setSize(vw, vh);
        this.innerHd.style.width = (vw-16)+'px';

        this.headerPop.setHeight(this.innerHd.offsetHeight);

        this.autoExpand();

        if(this.forceFit){
            this.fitColumns(false, false);
        }

        return;


        var bt = this.getBodyTable();
        var ltWidth = hasLock ?
                      Math.max(this.getLockedTable().offsetWidth, this.lockedHd.dom.firstChild.offsetWidth) : 0;

        var scrollHeight = bt.offsetHeight;
        var scrollWidth = ltWidth + bt.offsetWidth;
        var vscroll = false, hscroll = false;

        this.scrollSizer.setSize(scrollWidth, scrollHeight+hdHeight);

        var lw = this.lockedWrap, mw = this.mainWrap;
        var lb = this.lockedBody, mb = this.mainBody;

        setTimeout(function(){
            var t = s.dom.offsetTop;
            var w = s.dom.clientWidth,
                h = s.dom.clientHeight;

            lw.setTop(t);
            lw.setSize(ltWidth, h);

            mw.setLeftTop(ltWidth, t);
            mw.setSize(w-ltWidth, h);

            lb.setHeight(h-hdHeight);
            mb.setHeight(h-hdHeight);

            if(initialRender){
                lw.show();
                mw.show();
            }
            //c.endMeasure();
        }, 10);
    }
};

// private
// This is a support class used internally by the Grid components
Ext.grid.GridView.SplitDragZone = function(grid, hd){
    this.grid = grid;
    this.view = grid.getView();
    this.proxy = this.view.resizeProxy;
    Ext.grid.GridView.SplitDragZone.superclass.constructor.call(this, hd,
        "gridSplitters" + this.grid.container.id, {
        dragElId : Ext.id(this.proxy.dom), resizeFrame:false
    });
    this.scroll = false;
    this.hw = this.view.splitHandleWidth || 5;
};
Ext.extend(Ext.grid.GridView.SplitDragZone, Ext.dd.DDProxy, {

    b4StartDrag : function(x, y){
        this.view.headersDisabled = true;
        this.proxy.setHeight(this.view.mainWrap.getHeight());
        var w = this.cm.getColumnWidth(this.cellIndex);
        var minw = Math.max(w-this.grid.minColumnWidth, 0);
        this.resetConstraints();
        this.setXConstraint(minw, 1000);
        this.setYConstraint(0, 0);
        this.minX = x - minw;
        this.maxX = x + 1000;
        this.startPos = x;
        Ext.dd.DDProxy.prototype.b4StartDrag.call(this, x, y);
    },


    handleMouseDown : function(e){
        var t = this.view.findHeaderCell(e.getTarget());
        if(t){
            var xy = this.view.fly(t).getXY(), x = xy[0], y = xy[1];
            var exy = e.getXY(), ex = exy[0], ey = exy[1];
            var w = t.offsetWidth, adjust = false;
            if((ex - x) <= this.hw){
                adjust = -1;
            }else if((x+w) - ex <= this.hw){
                adjust = 0;
            }
            if(adjust !== false){
                this.cellIndex = this.view.getCellIndex(t)+adjust;
                this.split = t.dom;
                this.cm = this.grid.colModel;
                if(this.cm.isResizable(this.cellIndex) && !this.cm.isFixed(this.cellIndex)){
                    Ext.grid.GridView.SplitDragZone.superclass.handleMouseDown.apply(this, arguments);
                }
            }else if(this.view.columnDrag){
                this.view.columnDrag.callHandleMouseDown(e);
            }
        }
    },

    endDrag : function(e){
        var v = this.view;
        var endX = Math.max(this.minX, e.getPageX());
        var diff = endX - this.startPos;
        v.onColumnSplitterMoved(this.cellIndex, this.cm.getColumnWidth(this.cellIndex)+diff);
        setTimeout(function(){
            v.headersDisabled = false;
        }, 50);
    },

    autoOffset : function(){
        this.setDelta(0,0);
    }
});

Ext.grid.GridView.ColumnDragZone = function(grid, hd){
    Ext.grid.GridView.ColumnDragZone.superclass.constructor.call(this, grid, hd, null);
    this.proxy.el.addClass('x-grid3-col-dd');
};

Ext.extend(Ext.grid.GridView.ColumnDragZone, Ext.grid.HeaderDragZone, {
    handleMouseDown : function(e){

    },

    callHandleMouseDown : function(e){
        Ext.grid.GridView.ColumnDragZone.superclass.handleMouseDown.call(this, e);
    }
});




