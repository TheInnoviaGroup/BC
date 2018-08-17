/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */


Ext.Window = function(config){
    Ext.Window.superclass.constructor.call(this, config);
};

Ext.extend(Ext.Window, Ext.Panel, {
    baseCls : 'x-window',
    frame:true,
    floating:true,
    collapsible:true,
    resizable:true,
    draggable:true,
    closable : true,
    constrain:true,

    minimizable : true,
    maximizable : true,

    minHeight: 80,
    minWidth: 200,

    initHidden : true,
    monitorResize : true,

    //tools:[{id:'minimize'},{id:'maximize'},{id:'restore', hidden:true},{id:'close'}],

    initComponent : function(){
        Ext.Window.superclass.initComponent.call(this);
        this.addEvents({
            resize: true,
            maximize : true,
            minimize: true,
            restore : true
        });
    },

    onRender : function(ct, position){
        Ext.Window.superclass.onRender.call(this, ct, position);

        // this element allows the Window to be focused for keyboard events
        this.focusEl = this.el.createChild({
                    tag: "a", href:"#", cls:"x-dlg-focus",
                    tabIndex:"-1", html: "&#160;"});
        this.focusEl.swallowEvent('click', true);

        this.proxy = this.el.createProxy("x-window-proxy");
        this.proxy.enableDisplayMode('block');

        if(this.modal){
            this.mask = this.container.createChild({cls:"ext-el-mask"});
            this.mask.enableDisplayMode("block");
        }
    },

    initEvents : function(){
        if(this.animateTarget){
            this.setAnimateTarget(this.animateTarget);
        }

        if(this.resizable){
            this.resizer = new Ext.Resizable(this.el, {
                minWidth: this.minWidth,
                minHeight:this.minHeight,
                handles: this.resizeHandles || "all",
                pinned: true,
                resizeElement : this.resizerAction
            });
            this.resizer.window = this;
            this.resizer.on("beforeresize", this.beforeResize, this);
        }

        if(this.draggable){
            this.header.addClass("x-window-draggable");
            this.dd = new Ext.Window.DD(this);
        }
        this.initTools();

        this.el.on("mousedown", this.toFront, this);
        this.manager = this.manager || Ext.WindowMgr;
        this.manager.register(this);
        this.hidden = true;
        if(this.maximized){
            this.maximized = false;
            this.maximize();
        }
    },

    initTools : function(){
        if(this.minimizable){
            this.addTool({
                id: 'minimize',
                on: {
                    'click' : this.onMinimize.createDelegate(this, [])
                }
            });
        }
        if(this.maximizable){
            this.addTool({
                id: 'maximize',
                on: {
                    'click' : this.maximize.createDelegate(this, [])
                }
            });
            this.addTool({
                id: 'restore',
                on: {
                    'click' : this.restore.createDelegate(this, [])
                },
                hidden:true
            });
            this.header.on('dblclick', this.toggleMaximize, this);
        }
        if(this.closable){
            this.addTool({
                id: 'close',
                on: {
                    'click' : this.onClose.createDelegate(this, [])
                }
            });
        }
    },

    resizerAction : function(){
        var box = this.proxy.getBox();
        this.proxy.hide();
        this.window.handleResize(box);
        return box;
    },

    beforeResize : function(){
        this.resizer.minHeight = Math.max(this.minHeight, this.getFrameHeight() + 40); // 40 is a magic minimum content size?
        this.resizer.minWidth = Math.max(this.minWidth, this.getFrameWidth() + 40);
    },

    // private
    handleResize : function(box){
        this.updateBox(box);
        this.focus();
        this.fireEvent("resize", this, box.width, box.height);
    },

    /**
     * Focuses the Window.  If a defaultButton is set, it will receive focus, otherwise the
     * Window itself will receive focus.
     */
    focus : function(){
        this.focusEl.focus.defer(10, this.focusEl);
    },

    setAnimateTarget : function(el){
        el = Ext.get(el);
        this.animateTarget = el;
    },

    beforeShow : function(){
        delete this.el.lastXY;
        delete this.el.lastLT;
        if(this.x === undefined){
            var xy = this.el.getAlignToXY(this.container, 'c-c');
            var pos = this.el.translatePoints(xy[0], xy[1]);
            this.x = pos.left;
            this.y = pos.top;
        }
        this.el.setLeftTop(this.x, this.y);
        this.expand(false);

        if(this.modal){
            Ext.get(document.body).addClass("x-body-masked");
            this.mask.setSize(Ext.lib.Dom.getViewWidth(true), Ext.lib.Dom.getViewHeight(true));
            this.mask.show();
        }
    },

    show : function(animateTarget, cb, scope){
        if(this.hidden === false){
            this.toFront();
            return;
        }
        if(this.fireEvent("beforeshow", this) === false){
            return;
        }
        if(cb){
            this.on('show', cb, scope, {single:true});
        }
        this.hidden = false;
        if(animateTarget !== undefined){
            this.setAnimateTarget(animateTarget);
        }
        this.beforeShow();
        if(this.animateTarget){
            this.animShow();
        }else{
            this.afterShow();
        }
    },

    afterShow : function(){
        this.proxy.hide();
        this.el.setStyle('display', 'block');
        this.el.show();

        if(this.monitorResize && this.constrain){
            Ext.EventManager.onWindowResize(this.onWindowResize, this);
            this.doConstrain();
        }
        if(this.layout){
            this.doLayout();
        }
        this.toFront();
        this.fireEvent("show", this);
    },

    // private
    animShow : function(){
        this.proxy.show();
        this.proxy.setBox(this.animateTarget.getBox());
        var b = this.getBox(false);
        b.callback = this.afterShow;
        b.scope = this;
        b.duration = .25;
        b.easing = 'easeNone';
        b.block = true;
        this.el.setStyle('display', 'none');
        this.proxy.shift(b);
    },


    hide : function(animateTarget, cb, scope){
        if(this.hidden || this.fireEvent("beforehide", this) === false){
            return;
        }
        if(cb){
            this.on('hide', cb, scope, {single:true});
        }
        this.hidden = true;
        if(animateTarget !== undefined){
            this.setAnimateTarget(animateTarget);
        }
        if(this.animateTarget){
            this.animHide();
        }else{
            this.el.hide();
            this.afterHide();
        }
    },

    afterHide : function(){
        this.proxy.hide();
        if(this.monitorResize && this.constrain){
            Ext.EventManager.removeResizeListener(this.onWindowResize, this);
        }
        if(this.modal){
            this.mask.hide();
            Ext.get(document.body).removeClass("x-body-masked");
        }
        this.fireEvent("hide", this);
    },

    animHide : function(){
        this.proxy.show();
        var tb = this.getBox(false);
        this.proxy.setBox(tb);
        this.el.hide();
        var b = this.animateTarget.getBox();
        b.callback = this.afterHide;
        b.scope = this;
        b.duration = .25;
        b.easing = 'easeNone';
        b.block = true;
        this.proxy.shift(b);
    },

    onWindowResize : function(){
        if(this.maximized){
            this.fitContainer();
        }
        this.doConstrain();
    },

    doConstrain : function(){
        if(this.constrain){
            var xy = this.el.getConstrainToXY(this.container, true,
                    {   right:this.el.shadowOffset,
                        left:this.el.shadowOffset,
                        bottom:this.el.shadowOffset});
            if(xy){
                this.setPosition(xy[0], xy[1]);
            }
        }
    },

    ghost : function(cls){
        var ghost = this.createGhost(cls);
        var box = this.getBox(true);
        ghost.setLeftTop(box.x, box.y);
        ghost.setWidth(box.width);
        this.el.hide();
        this.activeGhost = ghost;
        return ghost;
    },

    unghost : function(show, matchPosition){
        if(show !== false){
            this.el.show();
            this.focus();
        }
        if(matchPosition !== false){
            this.setPosition(this.activeGhost.getLeft(true), this.activeGhost.getTop(true));
        }
        this.activeGhost.hide();
        this.activeGhost.remove();
        delete this.activeGhost;
    },

    onMinimize : function(){
        this.fireEvent('minimize', this);
    },

    onClose : function(){
        if(this.fireEvent("beforeclose", this) === false){
            return;
        }
        this.hide(null, function(){
            this.fireEvent('close', this);
            this.destroy();
        }, this);
    },

    maximize : function(){
        if(!this.maximized){
            this.expand(false);
            this.restoreSize = this.getSize();
            this.restorePos = this.getPosition(true);
            this.tools.maximize.hide();
            this.tools.restore.show();
            this.maximized = true;
            this.el.disableShadow();

            if(this.dd){
                this.dd.lock();
            }
            if(this.collapsible){
                this.tools.toggle.hide();
            }
            this.el.addClass('x-window-maximized');
            this.container.addClass('x-window-maximized-ct');

            this.setPosition(0, 0);
            this.fitContainer();
            this.fireEvent('maximize', this);
        }
    },

    restore : function(){
        if(this.maximized){
            this.el.removeClass('x-window-maximized');
            this.tools.restore.hide();
            this.tools.maximize.show();
            this.setPosition(this.restorePos[0], this.restorePos[1]);
            this.setSize(this.restoreSize.width, this.restoreSize.height);
            delete this.restorePos;
            delete this.restoreSize;
            this.maximized = false;
            this.el.enableShadow(true);

            if(this.dd){
                this.dd.unlock();
            }
            if(this.collapsible){
                this.tools.toggle.show();
            }
            this.container.removeClass('x-window-maximized-ct');

            this.doConstrain();
            this.fireEvent('restore', this);
        }
    },

    toggleMaximize : function(){
        this[this.maximized ? 'restore' : 'maximize']();
    },

    fitContainer : function(){
        var vs = this.container.getViewSize();
        this.setSize(vs.width, vs.height);
    },

    // private
    // z-index is managed by the WindowManager and may be overwritten at any time
    setZIndex : function(index){
        if(this.modal){
            this.mask.setStyle("z-index", index);
        }
        this.el.setZIndex(++index);
        index += 5;

        if(this.resizer){
            this.resizer.proxy.setStyle("z-index", ++index);
        }

        this.lastZIndex = index;
    },

    /**
     * Aligns the window to the specified element
     * @param {String/HTMLElement/Ext.Element} element The element to align to.
     * @param {String} position The position to align to (see {@link Ext.Element#alignTo} for more details).
     * @param {Array} offsets (optional) Offset the positioning by [x, y]
     * @return {Ext.Window} this
     */
    alignTo : function(element, position, offsets){
        var xy = this.el.getAlignToXY(element, position, offsets);
        this.setPagePosition(xy[0], xy[1]);
        return this;
    },

    /**
     * Anchors this window to another element and realigns it when the window is resized or scrolled.
     * @param {String/HTMLElement/Ext.Element} element The element to align to.
     * @param {String} position The position to align to (see {@link Ext.Element#alignTo} for more details)
     * @param {Array} offsets (optional) Offset the positioning by [x, y]
     * @param {Boolean/Number} monitorScroll (optional) true to monitor body scroll and reposition. If this parameter
     * is a number, it is used as the buffer delay (defaults to 50ms).
     * @return {Ext.Window} this
     */
    anchorTo : function(el, alignment, offsets, monitorScroll){
        var action = function(){
            this.alignTo(el, alignment, offsets);
        };
        Ext.EventManager.onWindowResize(action, this);
        var tm = typeof monitorScroll;
        if(tm != 'undefined'){
            Ext.EventManager.on(window, 'scroll', action, this,
                {buffer: tm == 'number' ? monitorScroll : 50});
        }
        action.call(this);
        return this;
    },

    /**
     * Brings this Window to the front of any other visible Windows
     * @return {Ext.Window} this
     */
    toFront : function(){
        this.manager.bringToFront(this);
        this.focus();
        return this;
    },

    setActive : function(active){
        if(active){
            if(!this.maximized){
                this.el.enableShadow(true);
            }
            this.fireEvent('activate', this);
        }else{
            this.el.disableShadow();
            this.fireEvent('deactivate', this);
        }
    },

    /**
     * Sends this Window to the back (under) of any other visible Windows
     * @return {Ext.Window} this
     */
    toBack : function(){
        this.manager.sendToBack(this);
        return this;
    },

    /**
     * Centers this Window in the viewport
     * @return {Ext.Window} this
     */
    center : function(){
        var xy = this.el.getAlignToXY(this.container, 'c-c');
        this.setPagePosition(xy[0], xy[1]);
        return this;
    }
});

Ext.Window.DD = function(win){
    this.win = win;
    Ext.Window.DD.superclass.constructor.call(this, win.el.id, 'WindowDD-'+win.id);
    this.setHandleElId(win.header.id);
    this.scroll = false;
};

Ext.extend(Ext.Window.DD, Ext.dd.DD, {
    moveOnly:true,
    startDrag : function(){
        var w = this.win;
        this.proxy = w.ghost();
        if(w.constrain !== false){
            var so = w.el.shadowOffset;
            this.constrainTo(w.container, {right: so, left: so, bottom: so});
        }
    },
    b4Drag : Ext.emptyFn,

    onDrag : function(e){
        this.alignElWithMouse(this.proxy, e.getPageX(), e.getPageY());
    },

    endDrag : function(e){
        this.win.unghost();
    }
});