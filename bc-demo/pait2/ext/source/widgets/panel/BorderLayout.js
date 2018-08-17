/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.layout.BorderLayout = Ext.extend(Ext.layout.ContainerLayout, {
    monitorResize:true,
    rendered : false,

    onLayout : function(ct, target){
        if(!this.rendered){
            target.position();
            target.addClass('x-border-layout-ct');
            var items = ct.items.items;
            for(var i = 0, len = items.length; i < len; i++) {
                var c = items[i];
                var pos = c.region;
                var collapsed = c.collapsed;
                c.collapsed = false;
                if(!c.rendered){
                    c.cls = c.cls ? c.cls +' x-border-panel' : 'x-border-panel';
                    c.render(target, i);
                }
                this[pos] = pos != 'center' && c.split ?
                    new Ext.layout.BorderLayout.SplitRegion(this, c.initialConfig, pos) :
                    new Ext.layout.BorderLayout.Region(this, c.initialConfig, pos);
                this[pos].render(target, c);
                if(collapsed){
                    this[pos].onCollapse(c, false);
                }
            }
            this.rendered = true;
        }

        var size = target.getViewSize();

        if(size.width < 20 || size.height < 20){ // display none?
            return;
        }


        var w = size.width, h = size.height;
        var centerW = w, centerH = h, centerY = 0, centerX = 0;

        var n = this.north, s = this.south, west = this.west, e = this.east, c = this.center;
        if(n && n.isVisible()){
            var b = n.getSize();
            var m = n.getMargins();
            b.width = w - (m.left+m.right);
            b.x = m.left;
            b.y = m.top;
            centerY = b.height + b.y + m.bottom;
            centerH -= centerY;
            n.applyLayout(b);
        }
        if(s && s.isVisible()){
            var b = s.getSize();
            var m = s.getMargins();
            b.width = w - (m.left+m.right);
            b.x = m.left;
            var totalHeight = (b.height + m.top + m.bottom);
            b.y = h - totalHeight + m.top;
            centerH -= totalHeight;
            s.applyLayout(b);
        }
        if(west && west.isVisible()){
            var b = west.getSize();
            var m = west.getMargins();
            b.height = centerH - (m.top+m.bottom);
            b.x = m.left;
            b.y = centerY + m.top;
            var totalWidth = (b.width + m.left + m.right);
            centerX += totalWidth;
            centerW -= totalWidth;
            west.applyLayout(b);
        }
        if(e && e.isVisible()){
            var b = e.getSize();
            var m = e.getMargins();
            b.height = centerH - (m.top+m.bottom);
            var totalWidth = (b.width + m.left + m.right);
            b.x = w - totalWidth + m.left;
            b.y = centerY + m.top;
            centerW -= totalWidth;
            e.applyLayout(b);
        }

        var m = c.getMargins();
        var centerBox = {
            x: centerX + m.left,
            y: centerY + m.top,
            width: centerW - (m.left+m.right),
            height: centerH - (m.top+m.bottom)
        };
        c.applyLayout(centerBox);

        //target.repaint();
    }
});

Ext.layout.BorderLayout.Region = function(layout, config, pos){
    this.layout = layout;
    this.position = pos;
    Ext.apply(this, config);
    this.margins = Ext.applyIf(this.margins || {}, this.defaultMargins);
    if(this.collapsible){
        this.cmargins = Ext.applyIf(this.cmargins || {},
                pos == 'north' || pos == 'south' ? this.defaultNSCMargins : this.defaultEWCMargins);
    }
};

Ext.layout.BorderLayout.Region.prototype = {
    collapsible : false,
    split:false,
    floatable: true,
    showPin: false,
    collapsed : false,
    //animFloat : false,
    defaultMargins : {left:0,top:0,right:0,bottom:0},
    defaultNSCMargins : {left:0,top:3,right:0,bottom:3},
    defaultEWCMargins : {left:3,top:0,right:3,bottom:0},
    minWidth:50,
    minHeight:50,

    render : function(ct, p){
        this.panel = p;
        p.el.enableDisplayMode('block');
        this.targetEl = ct;
        this.el = p.el;
        if(this.position != 'center'){
            p.on({
                beforecollapse: this.beforeCollapse,
                collapse: this.onCollapse,
                beforeexpand: this.beforeExpand,
                expand: this.onExpand,
                hide: this.onHide,
                show: this.onShow,
                scope: this
            });
            if(this.collapsible){
                p.collapseEl = 'el';
                p.slideAnchor = this.getSlideAnchor();
            }
            if(p.tools && p.tools.toggle){
                p.tools.toggle.addClass('x-tool-collapse-'+this.position);
                p.tools.toggle.addClassOnOver('x-tool-collapse-'+this.position+'-over');
            }
        }
    },

    getCollapsedEl : function(){
        if(!this.collapsedEl){
            if(!this.toolTemplate){
                var tt = new Ext.Template(
                     '<div class="x-tool x-tool-{id}">&#160</div>'
                );
                tt.disableFormats = true;
                tt.compile();
                Ext.layout.BorderLayout.Region.prototype.toolTemplate = tt;
            }
            this.collapsedEl = this.targetEl.createChild({
                cls: "x-layout-collapsed x-layout-collapsed-"+this.position
            });
            this.collapsedEl.enableDisplayMode('block');

            var t = this.toolTemplate.append(
                    this.collapsedEl.dom,
                    {id:'expand-'+this.position}, true);
            t.addClassOnOver('x-tool-expand-'+this.position+'-over');
            t.on('click', this.panel.expand, this.panel, {stopEvent:true});

            if(this.floatable !== false){
               this.collapsedEl.addClassOnOver("x-layout-collapsed-over");
               this.collapsedEl.on("click", this.collapseClick, this);
            }
        }
        return this.collapsedEl;
    },

    beforeCollapse : function(animate){
        if(this.splitEl){
            this.splitEl.hide();
        }
        this.getCollapsedEl().show();
        this.panel.el.setStyle('z-index', 100);
        this.collapsed = true;
        this.layout.layout();
    },

    onCollapse : function(animate){
        this.panel.el.setStyle('z-index', 1);
        this.getCollapsedEl().slideIn(this.panel.slideAnchor, {duration:.2});
    },

    beforeExpand : function(animate){
        var c = this.getCollapsedEl();
        this.el.show();
        if(this.position == 'east' || this.position == 'west'){
            this.panel.setSize(undefined, this.collapsedEl.getHeight());
        }else{
            this.panel.setSize(this.collapsedEl.getWidth(), undefined);
        }
        c.hide();
        c.dom.style.visibility = 'hidden';
        this.panel.el.setStyle('z-index', 100);
    },

    onExpand : function(){
        this.collapsed = false;
        if(this.splitEl){
            this.splitEl.show();
        }
        this.layout.layout();
        this.panel.el.setStyle('z-index', 1);
    },

    collapseClick : function(e){
        if(this.isSlid){
           e.stopPropagation();
           this.slideIn();
        }else{
           e.stopPropagation();
           this.slideOut();
        }
    },

    onHide : function(){
        if(this.collapsed){
            this.collapsedEl.hide();
        }else if(this.splitEl){
            this.splitEl.hide();
        }
    },

    onShow : function(){
        if(this.collapsed){
            this.collapsedEl.show();
        }else if(this.splitEl){
            this.splitEl.show();
        }
    },

    isVisible : function(){
        return !this.panel.hidden;
    },

    getMargins : function(){
        return this.collapsed ? this.cmargins : this.margins;
    },

    getSize : function(){
        return this.collapsed ? this.getCollapsedEl().getSize() : this.panel.getSize();
    },

    setPanel : function(panel){
        this.panel = panel;

    },

    getMinWidth: function(){
        return this.minWidth;
    },

    getMinHeight: function(){
        return this.minHeight;
    },

    applyLayoutCollapsed : function(box){
        this.collapsedEl.setLeftTop(box.x, box.y);
        this.collapsedEl.setSize(box.width, box.height);
    },

    applyLayout : function(box){
        if(this.collapsed){
            this.applyLayoutCollapsed(box);
        }else{
            this.panel.setPosition(box.x, box.y);
            this.panel.setSize(box.width, box.height);
        }
    },

    beforeSlide: function(){
        this.panel.beforeEffect();
    },

    afterSlide : function(){
        this.panel.afterEffect();
    },

    initAutoHide : function(){
        if(this.autoHide !== false){
            if(!this.autoHideHd){
                var st = new Ext.util.DelayedTask(this.slideIn, this);
                this.autoHideHd = {
                    "mouseout": function(e){
                        if(!e.within(this.el, true)){
                            st.delay(500);
                        }
                    },
                    "mouseover" : function(e){
                        st.cancel();
                    },
                    scope : this
                };
            }
            this.el.on(this.autoHideHd);
        }
    },

    clearAutoHide : function(){
        if(this.autoHide !== false){
            this.el.un("mouseout", this.autoHideHd.mouseout);
            this.el.un("mouseover", this.autoHideHd.mouseover);
        }
    },

    clearMonitor : function(){
        Ext.get(document).un("click", this.slideInIf, this);
    },

    // these names are backwards but not changed for compat
    slideOut : function(){
        if(this.isSlid || this.el.hasActiveFx()){
            return;
        }
        this.isSlid = true;
        var ts = this.panel.tools;
        if(ts && ts.toggle){
            ts.toggle.hide();
        }
        this.el.show();
        if(this.position == 'east' || this.position == 'west'){
            this.panel.setSize(undefined, this.collapsedEl.getHeight());
        }else{
            this.panel.setSize(this.collapsedEl.getWidth(), undefined);
        }
        this.restoreLT = [this.el.dom.style.left, this.el.dom.style.top];
        this.el.alignTo(this.collapsedEl, this.getCollapseAnchor());
        this.el.setStyle("z-index", 102);
        if(this.animFloat !== false){
            this.beforeSlide();
            this.el.slideIn(this.getSlideAnchor(), {
                callback: function(){
                    this.afterSlide();
                    this.initAutoHide();
                    Ext.get(document).on("click", this.slideInIf, this);
                },
                scope: this,
                block: true
            });
        }else{
            this.initAutoHide();
             Ext.get(document).on("click", this.slideInIf, this);
        }
    },

    afterSlideIn : function(){
        this.clearAutoHide();
        this.isSlid = false;
        this.clearMonitor();
        this.el.setStyle("z-index", "");
        this.el.dom.style.left = this.restoreLT[0];
        this.el.dom.style.top = this.restoreLT[1];

        var ts = this.panel.tools;
        if(ts && ts.toggle){
            ts.toggle.show();
        }
    },

    slideIn : function(cb){
        if(!this.isSlid || this.el.hasActiveFx()){
            Ext.callback(cb);
            return;
        }
        this.isSlid = false;
        if(this.animFloat !== false){
            this.beforeSlide();
            this.el.slideOut(this.getSlideAnchor(), {
                callback: function(){
                    this.el.hide();
                    this.afterSlide();
                    this.afterSlideIn();
                    Ext.callback(cb);
                },
                scope: this,
                block: true
            });
        }else{
            this.el.hide();
            this.afterSlideIn();
        }
    },

    slideInIf : function(e){
        if(!e.within(this.el)){
            this.slideIn();
        }
    },

    anchors : {
        "west" : "left",
        "east" : "right",
        "north" : "top",
        "south" : "bottom"
    },

    sanchors : {
        "west" : "l",
        "east" : "r",
        "north" : "t",
        "south" : "b"
    },

    canchors : {
        "west" : "tl-tr",
        "east" : "tr-tl",
        "north" : "tl-bl",
        "south" : "bl-tl"
    },

    getAnchor : function(){
        return this.anchors[this.position];
    },

    getCollapseAnchor : function(){
        return this.canchors[this.position];
    },

    getSlideAnchor : function(){
        return this.sanchors[this.position];
    },

    getAlignAdj : function(){
        var cm = this.cmargins;
        switch(this.position){
            case "west":
                return [0, 0];
            break;
            case "east":
                return [0, 0];
            break;
            case "north":
                return [0, 0];
            break;
            case "south":
                return [0, 0];
            break;
        }
    },

    getExpandAdj : function(){
        var c = this.collapsedEl, cm = this.cmargins;
        switch(this.position){
            case "west":
                return [-(cm.right+c.getWidth()+cm.left), 0];
            break;
            case "east":
                return [cm.right+c.getWidth()+cm.left, 0];
            break;
            case "north":
                return [0, -(cm.top+cm.bottom+c.getHeight())];
            break;
            case "south":
                return [0, cm.top+cm.bottom+c.getHeight()];
            break;
        }
    }
};


Ext.layout.BorderLayout.SplitRegion = function(layout, config, pos){
    Ext.layout.BorderLayout.SplitRegion.superclass.constructor.call(this, layout, config, pos);
    // prevent switch
    this.applyLayout = this.applyFns[pos];
};

Ext.extend(Ext.layout.BorderLayout.SplitRegion, Ext.layout.BorderLayout.Region, {
    splitTip : "Drag to resize.",
    collapsibleSplitTip : "Drag to resize. Double click to hide.",
    useSplitTips : false,
    splitSettings : {
        north : {
            orientation: Ext.SplitBar.VERTICAL,
            placement: Ext.SplitBar.TOP,
            maxFn : 'getVMaxSize',
            minProp: 'minHeight',
            maxProp: 'maxHeight'
        },
        south : {
            orientation: Ext.SplitBar.VERTICAL,
            placement: Ext.SplitBar.BOTTOM,
            maxFn : 'getVMaxSize',
            minProp: 'minHeight',
            maxProp: 'maxHeight'
        },
        east : {
            orientation: Ext.SplitBar.HORIZONTAL,
            placement: Ext.SplitBar.RIGHT,
            maxFn : 'getHMaxSize',
            minProp: 'minWidth',
            maxProp: 'maxWidth'
        },
        west : {
            orientation: Ext.SplitBar.HORIZONTAL,
            placement: Ext.SplitBar.LEFT,
            maxFn : 'getHMaxSize',
            minProp: 'minWidth',
            maxProp: 'maxWidth'
        }
    },

    applyFns : {
        west : function(box){
            if(this.collapsed){
                return this.applyLayoutCollapsed(box);
            }
            this.panel.setPosition(box.x, box.y);
            var sw = this.splitEl.dom.offsetWidth;
            this.splitEl.dom.style.left = (box.x+box.width-sw)+'px';
            this.splitEl.dom.style.top = (box.y)+'px';
            this.splitEl.dom.style.height = (box.height)+'px';
            this.panel.setSize(box.width-sw, box.height);
        },
        east : function(box){
            if(this.collapsed){
                return this.applyLayoutCollapsed(box);
            }
            var sw = this.splitEl.dom.offsetWidth;
            this.panel.setPosition(box.x+sw, box.y);
            this.splitEl.dom.style.left = (box.x)+'px';
            this.splitEl.dom.style.top = (box.y)+'px';
            this.splitEl.dom.style.height = (box.height)+'px';
            this.panel.setSize(box.width-sw, box.height);
        },
        north : function(box){
            if(this.collapsed){
                return this.applyLayoutCollapsed(box);
            }
            var sh = this.splitEl.dom.offsetHeight;
            this.panel.setPosition(box.x, box.y);
            this.splitEl.dom.style.left = (box.x)+'px';
            this.splitEl.dom.style.top = (box.y+box.height-sh)+'px';
            this.splitEl.dom.style.width = (box.width)+'px';
            this.panel.setSize(box.width, box.height-sh);
        },
        south : function(box){
            if(this.collapsed){
                return this.applyLayoutCollapsed(box);
            }
            var sh = this.splitEl.dom.offsetHeight;
            this.panel.setPosition(box.x, box.y+sh);
            this.splitEl.dom.style.left = (box.x)+'px';
            this.splitEl.dom.style.top = (box.y)+'px';
            this.splitEl.dom.style.width = (box.width)+'px';
            this.panel.setSize(box.width, box.height-sh);
        }
    },

    render : function(ct, p){
        Ext.layout.BorderLayout.SplitRegion.superclass.render.call(this, ct, p);
        this.splitEl = ct.createChild({
            cls: "x-layout-split x-layout-split-"+this.position, html: "&#160;"
        });

        var s = this.splitSettings[this.position];

        this.split = new Ext.SplitBar(this.splitEl.dom, p.el, s.orientation);
        this.split.placement = s.placement;
        this.split.getMaximumSize = this[s.maxFn].createDelegate(this);
        this.split.minSize = this.minSize || this[s.minProp];
        this.split.on("beforeapply", this.onSplitMove, this);
        this.split.useShim = this.useShim === true;
        this.maxSize = this.maxSize || this[s.maxProp];

        if(this.useSplitTips){
            this.splitEl.dom.title = this.collapsible ? this.collapsibleSplitTip : this.splitTip;
        }
        if(this.collapsible){
            this.splitEl.on("dblclick", this.onCollapse,  this);
        }
    },

    getSize : function(){
        if(this.collapsed){
            return this.collapsedEl.getSize();
        }
        var s = this.panel.getSize();
        if(this.position == 'north' || this.position == 'south'){
            s.height += this.splitEl.dom.offsetHeight;
        }else{
            s.width += this.splitEl.dom.offsetWidth;
        }
        return s;
    },

    getHMaxSize : function(){
         var cmax = this.maxSize || 10000;
         var center = this.layout.center;
         return Math.min(cmax, (this.el.getWidth()+center.el.getWidth())-center.getMinWidth());
    },

    getVMaxSize : function(){
        var cmax = this.maxSize || 10000;
        var center = this.layout.center;
        return Math.min(cmax, (this.el.getHeight()+center.el.getHeight())-center.getMinHeight());
    },

    onSplitMove : function(split, newSize){
        var s = this.panel.getSize();
        if(this.position == 'north' || this.position == 'south'){
            this.panel.setSize(s.width, newSize);
        }else{
            this.panel.setSize(newSize, s.height);
        }
        this.layout.layout();
        return false;
    },

    getSplitBar : function(){
        return this.split;
    },

    animateCollapse : function(){
        this.beforeSlide();
        this.el.setStyle("z-index", 20000);
        var anchor = this.getSlideAnchor();
        this.el.slideOut(anchor, {
            callback : function(){
                this.el.setStyle("z-index", "");
                this.collapsedEl.slideIn(anchor, {duration:.3});
                this.afterSlide();
                this.el.setLocation(-10000,-10000);
                this.el.hide();
                this.fireEvent("collapsed", this);
            },
            scope: this,
            block: true
        });
    },

    animateExpand : function(){
        this.beforeSlide();
        this.el.alignTo(this.collapsedEl, this.getCollapseAnchor(), this.getExpandAdj());
        this.el.setStyle("z-index", 20000);
        this.collapsedEl.hide({
            duration:.1
        });
        this.el.slideIn(this.getSlideAnchor(), {
            callback : function(){
                this.el.setStyle("z-index", "");
                this.afterSlide();
                if(this.split){
                    this.split.el.show();
                }
                this.fireEvent("invalidated", this);
                this.fireEvent("expanded", this);
            },
            scope: this,
            block: true
        });
    }
});