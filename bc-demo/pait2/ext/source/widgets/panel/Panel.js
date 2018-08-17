/*
 * Ext JS Library 1.0.1
 * Copyright(c) 2006-2007, Ext JS, LLC.
 * licensing@extjs.com
 * 
 * http://www.extjs.com/license
 */

Ext.Panel = function(config){
    Ext.Panel.superclass.constructor.call(this, config);
};

Ext.extend(Ext.Panel, Ext.Container, {
    baseCls : 'x-panel',
    collapsedCls : 'x-panel-collapsed',
    elements : 'header,body,footer',
    maskDisabled: true,
    collapsed : false,
    animCollapse:Ext.enableFx,
    toolTarget : 'header',
    headerAsText:true,
    buttonAlign:'right',
    /**
     * @cfg {Number} minButtonWidth Minimum width of all buttons (defaults to 75)
     */
    minButtonWidth:75,

    collapseEl : 'bwrap',
    slideAnchor : 't',

    initComponent : function(){
        Ext.Panel.superclass.initComponent.call(this);
        this.addEvents({
            bodyresize : true,
            titlechange: true,
            collapse : true,
            expand:true,
            beforecollapse : true,
            beforeexpand:true,

            // these 4 events are only fired if used within
            // a container that supports them
            beforeclose: true,
            close: true,
            activate: true,
            deactivate: true
        });

        // shortcuts
        if(this.tbar){
            this.elements += ',tbar';
            if(typeof this.tbar == 'object'){
                this.topToolbar = this.tbar;
            }
            delete this.tbar;
        }
        if(this.bbar){
            this.elements += ',bbar';
            if(typeof this.bbar == 'object'){
                this.bottomToolbar = this.bbar;
            }
            delete this.bbar;
        }

        if(this.buttons){
            var btns = this.buttons;
            delete this.buttons;
            for(var i = 0, len = btns.length; i < len; i++) {
                this.addButton(btns[i]);
            }
        }
    },

    // private, notify box this class will handle heights
    deferHeight:true,

    createElement : function(name, pnode){
        if(name !== 'bwrap' && this.elements.indexOf(name) == -1){
            return;
        }
        if(this[name]){
            pnode.appendChild(this[name].dom);
        }else{
            if(this[name+'Cfg']){
                this[name] = Ext.fly(pnode).createChild(this[name+'Cfg']);
            }else{
                var el = document.createElement('div');
                el.className = this[name+'Cls'];
                this[name] = Ext.get(pnode.appendChild(el));
            }
        }
    },

    onRender : function(ct, position){
        Ext.Panel.superclass.onRender.call(this, ct, position);

        this.createClasses();

        if(this.el){ // existing markup
            this.header = this.el.down('.'+this.headerCls);
            this.bwrap = this.el.down('.'+this.bwrapCls);
            var cp = this.bwrap ? this.bwrap : this.el;
            this.tbar = cp.down('.'+this.tbarCls);
            this.body = cp.down('.'+this.bodyCls);
            this.bbar = cp.down('.'+this.bbarCls);
            this.footer = cp.down('.'+this.footerCls);
        }else{
            this.el = ct.createChild({
                id: this.id,
                cls: this.baseCls
            }, position);
        }
        var el = this.el, d = el.dom;
        if(this.cls){
            this.el.addClass(this.cls);
        }
        // This block allows for maximum flexibility and performance when using existing markup

        // framing requires special markup
        if(this.frame){
            el.insertHtml('afterBegin', String.format(Ext.Element.boxMarkup, this.baseCls));

            this.createElement('header', d.firstChild.firstChild.firstChild);
            this.createElement('bwrap', d);

            // append the mid and bottom frame to the bwrap
            var bw = this.bwrap.dom;
            var ml = d.childNodes[1], bl = d.childNodes[2];
            bw.appendChild(ml);
            bw.appendChild(bl);

            var mc = bw.firstChild.firstChild.firstChild;
            this.createElement('tbar', mc);
            this.createElement('body', mc);
            this.createElement('bbar', mc);
            this.createElement('footer', bw.lastChild.firstChild.firstChild);
        }else{
            this.createElement('header', d);
            this.createElement('bwrap', d);

            // append the mid and bottom frame to the bwrap
            var bw = this.bwrap.dom;
            this.createElement('tbar', bw);
            this.createElement('body', bw);
            this.createElement('bbar', bw);
            this.createElement('footer', bw);

            if(!this.header){
                this.body.addClass(this.bodyCls + '-noheader');
            }
        }

        this.bwrap.enableDisplayMode('block');

        if(this.header){
            this.header.unselectable();
        }

        if(this.iconCls){
            this.header.addClass('x-panel-icon '+this.iconCls);
        }

        if(this.ownerCt){
            this.el.addClass(this.baseCls + '-nested');
        }

        // for tools, we need to wrap any existing header markup
        if(this.header && this.headerAsText){
            this.header.dom.innerHTML =
                '<span class="' + this.headerTextCls + '">'+this.header.dom.innerHTML+'</span>';
        }
        if(this.floating){
            this.el = new Ext.Layer(
                typeof this.floating == 'object' ? this.floating : {
                    shadow:this.shadow||'sides', constrain:false, shim: this.shim === false ? false : undefined
                }, this.el
            );
        }
        if(this.collapsible){
            this.tools = this.tools || [];
            this.tools.unshift({
                id: 'toggle',
                on : {
                    'click' : this.toggleCollapse,
                    'scope': this
                }
            });
        }
        if(this.tools){
            var ts = this.tools;
            this.tools = {};
            this.addTool.apply(this, ts);
        }else{
            this.tools = {};
        }

        if(this.buttons && this.buttons.length > 0){
            // tables are required to maintain order and for correct IE layout
            var tb = this.footer.createChild({cls:'x-panel-btns-ct', cn: {
                cls:"x-panel-btns x-panel-btns-"+this.buttonAlign,
                html:'<table cellspacing="0"><tbody><tr></tr></tbody></table><div class="x-clear"></div>'
            }}, null, true);
            var tr = tb.getElementsByTagName('tr')[0];
            for(var i = 0, len = this.buttons.length; i < len; i++) {
                var b = this.buttons[i];
                var td = document.createElement('td');
                td.className = 'x-panel-btn-td';
                b.render(tr.appendChild(td));
            }
        }

        if(this.tbar && this.topToolbar){
            if(this.topToolbar instanceof Array){
                this.topToolbar = new Ext.Toolbar(this.topToolbar);
            }
            this.topToolbar.render(this.tbar);
        }
        if(this.bbar && this.bottomToolbar){
            if(this.bottomToolbar instanceof Array){
                this.bottomToolbar = new Ext.Toolbar(this.bottomToolbar);
            }
            this.bottomToolbar.render(this.bbar);
        }
    },

    // must be called before rendering
    addButton : function(config, handler, scope){
        var bc = {
            handler: handler,
            scope: scope,
            minWidth: this.minButtonWidth,
            hideParent:true
        };
        if(typeof config == "string"){
            bc.text = config;
        }else{
            Ext.apply(bc, config);
        }
        var btn = new Ext.Button(null, bc);
        if(!this.buttons){
            this.buttons = [];
        }
        this.buttons.push(btn);
        return btn;
    },

    addTool : function(){
        if(!this.toolTemplate){
            // initialize the global tool template on first use
            var tt = new Ext.Template(
                 '<div class="x-tool x-tool-{id}">&#160</div>'
            );
            tt.disableFormats = true;
            tt.compile();
            Ext.Panel.prototype.toolTemplate = tt;
        }
        for(var i = 0, a = arguments, len = a.length; i < len; i++) {
            var tc = a[i];
            var t = this.toolTemplate.insertFirst(this[this.toolTarget], tc, true);
            this.tools[tc.id] = t;
            t.enableDisplayMode('block');
            if(tc.on){
                t.on(tc.on);
            }
            if(tc.hidden){
                t.hide();
            }
            t.addClassOnOver('x-tool-'+tc.id+'-over');
        }
    },

    afterRender : function(){
        if(this.floating && !this.hidden && !this.initHidden){
            this.el.show();
        }
        if(this.title){
            this.setTitle(this.title);
        }
        if(this.autoScroll){
            this.body.dom.style.overflow = 'auto';
        }
        if(this.html){
            this.body.update(this.html);
            delete this.html;
        }
        if(this.contentEl){
            this.body.dom.appendChild(Ext.getDom(this.contentEl));
        }
        if(this.loadContent){
            this.load.defer(10, this, [this.loadContent]);
            delete this.loadContent;
        }
        if(this.collapsed){
            this.collapsed = false;
            this.collapse(false);
        }
        Ext.Panel.superclass.afterRender.call(this); // do sizing calcs last
        this.initEvents();
    },

    initEvents : function(){

    },

    beforeEffect : function(){
        if(this.floating){
            this.el.beforeAction();
        }
        if(Ext.isGecko){
            this.body.clip();
        }
    },

    afterEffect : function(){
        this.syncShadow();
        if(Ext.isGecko){
            this.body.unclip();
        }
    },

    // private - wraps up an animation param with internal callbacks
    createEffect : function(a, cb, scope){
        var o = {
            scope:scope,
            block:true
        };
        if(a === true){
            o.callback = cb;
            return o;
        }else if(!a.callback){
            o.callback = cb;
        }else { // wrap it up
            o.callback = function(){
                cb.call(scope);
                Ext.callback(a.callback, a.scope);
            };
        }
        return Ext.applyIf(o, a);
    },

    collapse : function(animate){
        if(this.collapsed || this.el.hasFxBlock() || this.fireEvent('beforecollapse', this, animate) === false){
            return;
        }
        var a = animate === true || (animate !== false && this.animCollapse);
        this.beforeEffect();
        if(a){
            this[this.collapseEl].slideOut(this.slideAnchor, this.createEffect(a, this.afterCollapse, this));
        }else{
            this[this.collapseEl].hide();
            this.afterCollapse();
        }
        return this;
    },

    afterCollapse : function(){
        this.collapsed = true;
        this.el.addClass(this.collapsedCls);
        this.afterEffect();
        this.fireEvent('collapse', this);
    },

    expand : function(animate){
        if(!this.collapsed || this.el.hasFxBlock() || this.fireEvent('beforeexpand', this, animate) === false){
            return;
        }
        var a = animate === true || (animate !== false && this.animCollapse);
        this.el.removeClass(this.collapsedCls);
        this.beforeEffect();
        if(a){
            this[this.collapseEl].slideIn(this.slideAnchor, this.createEffect(a, this.afterExpand, this));
        }else{
            this[this.collapseEl].show();
            this.afterExpand();
        }
        return this;
    },

    afterExpand : function(){
        this.collapsed = false;
        this.afterEffect();
        this.fireEvent('expand', this);
    },

    toggleCollapse : function(animate){
        this[this.collapsed ? 'expand' : 'collapse'](animate);
        return this;
    },

    onDisable : function(){
        if(this.rendered && this.maskDisabled){
            this.el.mask();
        }
        Ext.Panel.superclass.onDisable.call(this);
    },

    onEnable : function(){
        if(this.rendered && this.maskDisabled){
            this.el.unmask();
        }
        Ext.Panel.superclass.onEnable.call(this);
    },

    onResize : function(w, h){
        if(w !== undefined || h !== undefined){
            if(typeof w == 'number'){
                this.body.setWidth(w - this.getFrameWidth());
            }else if(w == 'auto'){
                this.body.setWidth(w);
            }

            if(typeof h == 'number'){
                this.body.setHeight(h - this.getFrameHeight());
            }else if(h == 'auto'){
                this.body.setHeight(h);
            }
            this.syncShadow();
            this.fireEvent('bodyresize', this, w, h);
        }
    },

    onPosition : function(){
        this.syncShadow();
    },

    getFrameWidth : function(){
        var w = this.el.getFrameWidth('lr');

        if(this.frame){
            var l = this.bwrap.dom.firstChild;
            w += (Ext.fly(l).getFrameWidth('l') + Ext.fly(l.firstChild).getFrameWidth('r'));
            var mc = this.bwrap.dom.firstChild.firstChild.firstChild;
            w += Ext.fly(mc).getFrameWidth('lr');
        }
        return w;
    },

    getFrameHeight : function(){
        var h  = this.el.getFrameWidth('tb');
        h += (this.tbar ? this.tbar.getHeight() : 0) +
             (this.bbar ? this.bbar.getHeight() : 0);

        if(this.frame){
            var hd = this.el.dom.firstChild;
            var ft = this.bwrap.dom.lastChild;
            h += (hd.offsetHeight + ft.offsetHeight);
            var mc = this.bwrap.dom.firstChild.firstChild.firstChild;
            h += Ext.fly(mc).getFrameWidth('tb');
        }else{
            h += (this.header ? this.header.getHeight() : 0) +
                (this.footer ? this.footer.getHeight() : 0);
        }
        return h;
    },

    getInnerWidth : function(){
        return this.getSize().width - this.getFrameWidth();
    },

    getInnerHeight : function(){
        return this.getSize().height - this.getFrameHeight();
    },

    syncShadow : function(){
        if(this.floating){
            this.el.sync(true);
        }
    },

    getLayoutTarget : function(){
        return this.body;
    },

    setTitle : function(title){
        this.title = title;
        if(this.header && this.headerAsText){
            this.header.child('span').update(title);
            this.fireEvent('titlechange', this, title);
        }
        return this;
    },

    /**
     * Get the {@link Ext.UpdateManager} for this panel. Enables you to perform Ajax updates.
     * @return {Ext.UpdateManager} The UpdateManager
     */
    getUpdateManager : function(){
        return this.body.getUpdateManager();
    },

     /**
     * Loads this content panel immediately with content from XHR.
     * @param {Object/String/Function} config A config object containing any of the following options:
<pre><code>
panel.load({<br/>
    url: "your-url.php",<br/>
    params: {param1: "foo", param2: "bar"}, // or a URL encoded string<br/>
    callback: yourFunction,<br/>
    scope: yourObject, //(optional scope)  <br/>
    discardUrl: false, <br/>
    nocache: false,<br/>
    text: "Loading...",<br/>
    timeout: 30,<br/>
    scripts: false<br/>
});
</code></pre>
     * The only required property is url. The optional properties nocache, text and scripts
     * are shorthand for disableCaching, indicatorText and loadScripts and are used to set their
     * associated property on this panel UpdateManager instance.
     * @return {Ext.Panel} this
     */
    load : function(){
        var um = this.body.getUpdateManager();
        um.update.apply(um, arguments);
        return this;
    },

    beforeDestroy : function(){
        Ext.Element.uncache(
            this.header,
            this.tbar,
            this.bbar,
            this.footer,
            this.body
        );
    },

    createClasses : function(){
        this.headerCls = this.baseCls + '-header';
        this.headerTextCls = this.baseCls + '-header-text';
        this.bwrapCls = this.baseCls + '-bwrap';
        this.tbarCls = this.baseCls + '-tbar';
        this.bodyCls = this.baseCls + '-body';
        this.bbarCls = this.baseCls + '-bbar';
        this.footerCls = this.baseCls + '-footer';
    },

    createGhost : function(cls, useShim, preventAppend){
        var el = document.createElement('div');
        el.className = 'x-panel-ghost ' + (cls ? cls : '');
        if(this.header){
            el.appendChild(this.el.dom.firstChild.cloneNode(true));
        }
        Ext.fly(el.appendChild(document.createElement('ul'))).setSize(this.bwrap.getSize());
        if(!preventAppend){
            this.container.dom.appendChild(el);
        }
        if(useShim !== false && this.el.useShim !== false){
            var layer = new Ext.Layer({shadow:false, useDisplay:true, constrain:false}, el);
            layer.show();
            return layer;
        }else{
            return new Ext.Element(el);
        }
    }
});
