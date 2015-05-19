/**
 * Created with JetBrains WebStorm.
 * User: jvaranam
 * Date: 18/2/13
 * Time: 10:17 AMAM
 * To change this template use File | Settings | File Templates.
 */
var preloaderID;
var errorMessage = "Unexpected server error occured. Please try again later.";
var VirtualViewer = function(){
    var that = this;
    VirtualViewer.BROWSER_TYPE = browsertype();
    VirtualViewer.CAMERA_MIN_FOV = 65;
    VirtualViewer.CAMERA_MAX_FOV = 65;
    VirtualViewer.CAMERA_FOV = 65;
    VirtualViewer.COMPARE_MODE = 1;
    VirtualViewer.EVENT_APP_STATE = null;
    VirtualViewer.APP_STATE_NORMALSCREEN = 1;

    var _holderId = null;      /*id for the image holder */
    var _containerDiv = null;
    var _folderPath = null; /* image folder path */
    var _width = null;   /* dev width*/
    var _height = null; /* dev height */
    var _widthNor = null;
    var _heightNor = null;
    var _compereMode = false; /*  compered mode for the views */
    var _fov = 65; /*  fov  */
    var _fovMax = 65;  /*fov max */
    var _fovMin = 65;  /* fov min*/
    var _preloaderId = null; /* Preloader id*/
    var _viewer = null;
    var _panoViewer = null;
    var _interval = null;
	
	

    var swfKRPano = function(){
        if (typeof deconcept == "undefined")var deconcept = {};
        if (typeof deconcept.util == "undefined")deconcept.util = {};
        if (typeof deconcept.SWFObjectUtil == "undefined")deconcept.SWFObjectUtil = {};
        deconcept.SWFObject = function (a, b, c, e, f, g) {
            this.params = {};
            this.variables = {};
            this.attributes = [];
            this.vchk = function (d) {
                d = ("" + d).toLowerCase();
                return d != "menu"
            };
            a && this.setAttribute("swf", a);
            b && this.setAttribute("id", b);
            c && this.setAttribute("width", c);
            e && this.setAttribute("height", e);
            f && this.setAttribute("version", new deconcept.PlayerVersion(f.toString().split(".")));
            this.installedVer = deconcept.SWFObjectUtil.getPlayerVersion();
            if (!window.opera && document.all && this.installedVer.major > 7)deconcept.SWFObject.doPrepUnload =
                true;
            g && this.addParam("bgcolor", g)
        };
        deconcept.SWFObject.prototype = {setAttribute:function (a, b) {
            this.attributes[a] = b
        }, getAttribute:function (a) {
            return this.attributes[a]
        }, addParam:function (a, b) {
            this.params[a] = b
        }, getParams:function () {
            return this.params
        }, addVariable:function (a, b) {
            this.variables[a] = b
        }, getVariable:function (a) {
            return this.variables[a]
        }, getVariables:function () {
            return this.variables
        }, getVariablePairs:function () {
            var a = [], b, c = this.getVariables();
            for (b in c)if (this.vchk(b))a[a.length] = b + "=" + c[b];
            return a
        }, getSWFHTML:function () {
            var a =
                "";
            if (navigator.plugins && navigator.mimeTypes && navigator.mimeTypes.length) {
                a = '<embed type="application/x-shockwave-flash" src="' + this.getAttribute("swf") + '" width="' + this.getAttribute("width") + '" height="' + this.getAttribute("height") + '" style="' + this.getAttribute("style") + '"';
                a += ' id="' + this.getAttribute("id") + '" name="' + this.getAttribute("id") + '" ';
                var b = this.getParams();
                for (var c in b)if (this.vchk(c))a += [c] + '="' + b[c] + '" ';
                b = this.getVariablePairs().join("&");
                if (b.length > 0)a += 'flashvars="' + b + '"';
                a +=
                    "/>"
            } else {
                a = '<object id="' + this.getAttribute("id") + '" classid="clsid:D27CDB6E-AE6D-11cf-96B8-444553540000" width="' + this.getAttribute("width") + '" height="' + this.getAttribute("height") + '" style="' + this.getAttribute("style") + '">';
                a += '<param name="movie" value="' + this.getAttribute("swf") + '" />';
                b = this.getParams();
                for (c in b)if (this.vchk(c))a += '<param name="' + c + '" value="' + b[c] + '" />';
                b = this.getVariablePairs().join("&");
                if (b.length > 0)a += '<param name="flashvars" value="' + b + '" />';
                a += "</object>"
            }
            return a
        },
            write:function (a) {
                if (this.installedVer.versionIsValid(this.getAttribute("version"))) {
                    (typeof a == "string" ? document.getElementById(a) : a).innerHTML = this.getSWFHTML();
                    return true
                }
                return false
            }};
        deconcept.SWFObjectUtil.getPlayerVersion = function () {
            var a = new deconcept.PlayerVersion([0, 0, 0]);
            if (navigator.plugins && navigator.mimeTypes.length) {
                var b = navigator.plugins["Shockwave Flash"];
                if (b && b.description)a = new deconcept.PlayerVersion(b.description.replace(/([a-zA-Z]|\s)+/, "").replace(/(\s+r|\s+b[0-9]+)/, ".").split("."))
            } else {
                try {
                    b = new ActiveXObject("ShockwaveFlash.ShockwaveFlash")
                } catch (c) {
                }
                if (b != null)a = new deconcept.PlayerVersion(b.GetVariable("$version").split(" ")[1].split(","))
            }
            return a
        };
        deconcept.PlayerVersion = function (a) {
            this.major = a[0] != null ? parseInt(a[0]) : 0;
            this.minor = a[1] != null ? parseInt(a[1]) : 0;
            this.rev = a[2] != null ? parseInt(a[2]) : 0
        };
        deconcept.PlayerVersion.prototype.versionIsValid = function (a) {
            if (this.major < a.major)return false;
            if (this.major > a.major)return true;
            if (this.minor < a.minor)return false;
            if (this.minor > a.minor)return true;
            if (this.rev < a.rev)return false;
            return true
        };
        deconcept.util = {getRequestParameter:function (a) {
            var b = document.location.search || document.location.hash;
            if (a == null)return b;
            if (b) {
                b = b.substring(1).split("&");
                for (var c = 0; c < b.length; c++)if (b[c].substring(0, b[c].indexOf("=")) == a)return b[c].substring(b[c].indexOf("=") + 1)
            }
            return""
        }};
        deconcept.SWFObjectUtil.cleanupSWFs = function () {
            for (var a = document.getElementsByTagName("OBJECT"), b = a.length - 1; b >= 0; b--) {
                a[b].style.display = "none";
                for (var c in a[b])if (typeof a[b][c] == "function")a[b][c] = function () {
                }
            }
        };
        if (deconcept.SWFObject.doPrepUnload)if (!deconcept.unloadSet) {
            deconcept.SWFObjectUtil.prepUnload = function () {
                __flash_unloadHandler = function () {
                };
                __flash_savedUnloadHandler = function () {
                };
                window.attachEvent("onunload", deconcept.SWFObjectUtil.cleanupSWFs)
            };
            window.attachEvent("onbeforeunload", deconcept.SWFObjectUtil.prepUnload);
            deconcept.unloadSet = true
        }
        var getQueryParamValue = deconcept.util.getRequestParameter, SWFObject = deconcept.SWFObject;
        function SWFkrpanoMouseWheel2(l) {
            function m(a) {
                if (!a) {
                    a = d.event;
                    a.target = a.srcElement
                }
                n = a ? a.target : null
            }

            function o(a) {
                if (!a) {
                    a = d.event;
                    a.target = a.srcElement
                }
                var c = 0, i = g.length;
                for (c = 0; c < i; c++) {
                    var e = g[c];
                    if (e) {
                        var f = h[e.id];
                        if (f && e.needfix) {
                            var b = f.getBoundingClientRect();
                            e = f == n;
                            b = a.clientX >= b.left && a.clientX < b.right && a.clientY >= b.top && a.clientY < b.bottom;
                            if ((f == a.target || e) && b == false)try {
                                f.externalMouseEvent2 && f.externalMouseEvent2(0, "mouseUp")
                            } catch (q) {
                            }
                        }
                    }
                }
                return true
            }

            function j(a) {
                if (!a) {
                    a = d.event;
                    a.target = a.srcElement
                }
                var c = 0, i = false;
                if (a.wheelDelta) {
                    c = a.wheelDelta / 120;
                    if (d.opera && k)c /= 4 / 3
                } else if (a.detail) {
                    c = -a.detail;
                    if (k == false)c /= 3
                }
                if (c) {
                    var e = 0, f = g.length;
                    for (e = 0; e < f; e++) {
                        var b = g[e];
                        if (b)if (b = h[b.id])if (b == a.target) {
                            try {
                                if (b.jswheel)b.jswheel(c); else if (b.externalMouseEvent)b.externalMouseEvent(c); else if (b.enable_mousewheel_js_bugfix) {
                                    b.enable_mousewheel_js_bugfix();
                                    b.externalMouseEvent && b.externalMouseEvent(c)
                                }
                            } catch (q) {
                            }
                            i = true;
                            break
                        }
                    }
                }
                if (i) {
                    a.stopPropagation && a.stopPropagation();
                    a.preventDefault &&
                    a.preventDefault();
                    a.cancelBubble = true;
                    a.cancel = true;
                    return a.returnValue = false
                }
            }

            var d = window, h = document, k = navigator.appVersion.toLowerCase().indexOf("mac") >= 0, g = SWFkrpanoMouseWheel2.instances, n = null;
            if (!g) {
                g = SWFkrpanoMouseWheel2.instances = [];
                if (d.addEventListener) {
                    d.addEventListener("DOMMouseScroll", j, false);
                    d.addEventListener("mousewheel", j, false);
                    h.addEventListener("mousedown", m, false);
                    h.addEventListener("mouseup", o, false)
                } else {
                    if (d.opera)d.attachEvent("onmousewheel", j); else d.onmousewheel = h.onmousewheel =
                        j;
                    h.onmousedown = m;
                    h.onmouseup = o
                }
            }
            var p = ("" + l.params.wmode).toLowerCase();
            g.push({id:l.getAttribute("id"), needfix:k || !!d.chrome || p == "opaque" || p == "transparent"})
        }
        ;
        this.createkrpanoJSviewer = function(c, a, b, e, d, i, l) {
            function m() {
                var h = null;
                if (typeof krpanoJS === "undefined")h = l ? "ERROR:<br/><br/>Adobe Flashplayer 10 or higher needed!<br/><br/><br/><br/>" : "ERROR:<br/><br/>HTML5 Version not available!<br/><br/><br/><br/>"; else if (krpanojs_init(f) == false)h = "LICENSE ERROR";
                if (h)document.getElementById(f.htmltarget).innerHTML = '<table width="100%" height="100%"><tr valign="middle"><td><center>' + h + "</center></td></tr></table>"
            }

            if (e === undefined)e = "";
            var j = false;
            if (d === undefined ||
                d == true) {
                if (!window.krpanoreg || !window.krpanokey) {
                    document.write('<script src="' + e + 'krpanoiphone.license.js" type="text/javascript" charset="UTF-8"><\/script>');
                    j = true
                }
                if (typeof krpanoJS === "undefined") {
                    document.write('<script src="' + e + 'krpanoiphone.js" type="text/javascript"><\/script>');
                    j = true
                }
            }
            var f = {};
            f.params = {};
            f.params.id = c ? c : "krpanoSWFObject";
            f.params.width = a ? a : "100%";
            f.params.height = b ? b : "100%";
            f.params.basepath = e;
            f.params.bgcolor = i;
            f.vars = {};
            f.addVariable = function (h, n) {
                f.vars[String(h).toLowerCase()] =
                    n
            };
            f.addParam = function (h, n) {
                f.params[String(h).toLowerCase()] = n
            };
            f.passQueryParameters = function () {
                var h = document.location.search || document.location.hash;
                if (h) {
                    h = h.substring(1).split("&");
                    for (var n = 0; n < h.length; n++) {
                        var o = h[n], g = o.indexOf("=");
                        if (g == -1)g = o.length;
                        var k = o.substring(0, g);
                        o = o.substring(g + 1);
                        f.addVariable(k, o)
                    }
                }
            };
            f.embed = function (h) {
                f.htmltarget = h;
                j ? window.addEventListener("load", m, false) : m(null)
            };
            return f
        }
        function createkrpanoSWFviewer(c, a, b, e, d, i, l) {
            if (typeof a === "undefined")a = "krpanoSWFObject";
            if (typeof b === "undefined")b = "100%";
            if (typeof e === "undefined")e = "100%";
            if (typeof d === "undefined")d = "#000000";
            var m = navigator.userAgent, j = m.toLowerCase(), f = false, h = 0, n = "", o = false;
            if (j.indexOf("ipad") >= 0 || j.indexOf("iphone") >= 0 || j.indexOf("ipod") >= 0)f = true; else {
                h = deconcept.SWFObjectUtil.getPlayerVersion().major;
                if (h == 0) {
                    f = createPanoViewer().isHTML5possible();
                    o = true
                }
            }
            if (i === false)f = false;
            if (f) {
                i = "./";
                m = c.lastIndexOf("/");
                if (m >= 0)i = c.slice(0, m + 1);
                c = true;
                if (l && l.html5licenseload === false)c = false;
                return createkrpanoJSviewer(a, b, e, i, c, d, o)
            }
            if (h >= 9) {
                for (; ;)if (document.getElementById(a))a += String.fromCharCode(48 + Math.floor(9 * Math.random())); else break;
                if(!g){
                    var g = new SWFObject(c, a, b, e, "9.0.28", d);
                }
                g.addParam("allowFullScreen", "true");
                g.addParam("allowScriptAccess", "always");
                g.setAttribute("style", "outline:none;");
                g.addVariable("browser.useragent", m);
                g.embed = function (k) {
                    if(k) jQuery(k).empty();
                    g.write(k) && SWFkrpanoMouseWheel2(g)
                };
                g.passQueryParameters =
                    function () {
                        var k = document.location.search || document.location.hash;
                        if (k) {
                            k = k.substring(1).split("&");
                            for (var r = 0; r < k.length; r++) {
                                var p = k[r], q = p.indexOf("=");
                                if (q == -1)q = p.length;
                                var s = p.substring(0, q);
                                p = p.substring(q + 1);
                                g.addVariable(s, p)
                            }
                        }
                    };
                return g
            } else n = 'Adobe Flashplayer 10 or higher needed<br/><br/><br/><a href="http://www.adobe.com/go/getflashplayer/" target="_blank" style="text-decoration:none;"><small>...click here to download...</small></a><br/><br/>';
            g = {};
            g.addVariable = function () {
            };
            g.addParam =
                function () {
                };
            g.passQueryParameters = function () {
            };
            g.embed = function (k) {
                document.getElementById(k).innerHTML = '<table width="100%" height="100%"><tr valign="middle"><td><center>ERROR:<br/><br/>' + n + "<br/><br/></center></td></tr></table>"
            };
            return g
        }
        var createswf = createkrpanoSWFviewer;
        function embedpano(c) {
            c || (c = {});
            var a = c.swf ? c.swf : "krpano.swf", b = c.xml ? c.xml : a.split(".swf").join(".xml"), e = c.id ? c.id : "krpanoSWFObject", d = c.target ? c.target : null, i = c.width ? c.width : "100%", l = c.height ? c.height : "100%", m = c.bgcolor ? c.bgcolor : "#000000";
            if (d) {
                a = createPanoViewer({swf:a, id:e, width:i, height:l, bgcolor:m});
                a.addVariable("xml", b);
                c.useHTML5 && a.useHTML5(c.useHTML5);
                c.html5 && a.useHTML5(c.html5);
                c.wmode && a.addParam("wmode", c.wmode);
                c.passQueryParameters && a.passQueryParameters();
                if (c.vars)for (var j in c.vars)typeof c.vars[j] !=
                    "function" && a.addVariable(j, c.vars[j]);
                a.embed(d)
            } else alert("ERROR: embedpano() - target needed")
        }
        var embedPanoViewer = embedpano;
        this.createPanoViewer = function(c) {
            c || (c = {});
            if(!a){
                var a = {};
                a.pswfpath = c.swf ? c.swf : "krpano.swf";
            }
            a.pxml = c.xml ? c.xml : a.pswfpath.split(".swf").join(".xml");
            a.pid = c.id ? c.id : "krpanoSWFObject";
            a.ptarget = c.target ? c.target : null;
            a.pwidth = c.width ? c.width : "100%";
            a.pheight = c.height ? c.height : "100%";
            a.bgcolor = c.bgcolor ? c.bgcolor : "#000000";
            a.pvars = [];
            a.pparams = [];
            a.HTML5 = "auto";
            if (c.useHTML5)a.HTML5 = c.useHTML5;
            if (c.html5)a.HTML5 = c.html5;
            a.useHTML5 = function (b) {
                a.HTML5 = b
            };
            a.isFlashpossible = function () {
                return deconcept.SWFObjectUtil.getPlayerVersion().major >=
                    9
            };
            a.isHTML5possible = function () {
                var b, e = navigator.userAgent, d = e.toLowerCase();
                if (d.indexOf("ipad") >= 0 || d.indexOf("iphone") >= 0 || d.indexOf("ipod") >= 0)return true;
                if (d.indexOf("safari") > 0) {
                    b = d.indexOf("version");
                    if (b > 0) {
                        b = parseInt(d.slice(b + 8));
                        if (b >= 5)return true
                    }
                    b = d.indexOf("chrome");
                    if (b > 0) {
                        b = parseInt(d.slice(b + 7));
                        if (b >= 20)return true
                    }
                    b = d.indexOf("android");
                    if (b > 0) {
                        b = parseInt(d.slice(b + 8));
                        if (b >= 4)return true
                    }
                } else {
                    b = d.indexOf("firefox");
                    if (b > 0) {
                        b = parseInt(d.slice(b + 8));
                        if (b >= 10)return true
                    }
                    b = e.indexOf("MSIE ");
                    if (b > 0) {
                        b = parseInt(e.slice(b + 5));
                        if (b >= 10)return true
                    }
                }
                return false
            };
            a.setSWFPath = function (b) {
                a.pswfpath = b;
                if (a.pxml == "krpano.swf")a.pxml = a.pswfpath.split(".swf").join(".xml")
            };
            a.setViewerID = function (b) {
                a.pid = b
            };
            a.setSize = function (b, e) {
                a.pwidth = b;
                a.pheight = e
            };
            a.isDevice = function (b) {
                var e = "all", d = navigator.userAgent.toLowerCase();
                if (d.indexOf("ipad") >= 0)e += "|ipad";
                if (d.indexOf("iphone") >= 0)e += "|iphone";
                if (d.indexOf("ipod") >= 0)e += "|ipod";
                if (d.indexOf("android") >= 0)e += "|android";
                b = String(b).toLowerCase().split("|");
                if (b == null)return true;
                var i = b.length;
                for (d = 0; d < i; d++)if (e.indexOf(b[d]) >= 0)return true;
                return false
            };
            a.addVariable = function (b, e) {
                b = String(b).toLowerCase();
                if (b == "xml" || b == "pano")a.pxml = e; else a.pvars[b] = e
            };
            a.addParam = function (b, e) {
                a.pparams[b] = e
            };
            a.passQueryParameters = function () {
                var b = document.location.search || document.location.hash;
                if (b) {
                    b = b.substring(1).split("&");
                    for (var e = 0; e < b.length; e++) {
                        var d = b[e], i = d.indexOf("=");
                        if (i == -1)i = d.length;
                        var l = d.substring(0, i);
                        d = d.substring(i + 1);
                        if (l == "useHTML5" ||
                            l.toLowerCase() == "html5")a.HTML5 = d; else a.addVariable(l, d)
                    }
                }
            };
            a.embed = function (b) {
                if (b)a.ptarget = b;
                if (a.ptarget) {
                    b = null;
                    b = String(a.HTML5).toLowerCase();
                    if (b == "always" || b == "force" || (b == "whenpossible" || b == "prefer") && a.isHTML5possible()) {
                        b = "./";
                        var e = a.pswfpath.lastIndexOf("/");
                        if (e >= 0)b = a.pswfpath.slice(0, e + 1);
                        e = true;
                        if (c.html5licenseload === false)e = false;
                        b = createkrpanoJSviewer(a.pid, a.pwidth, a.pheight, b, e, a.bgcolor)
                    } else b = createkrpanoSWFviewer(a.pswfpath, a.pid, a.pwidth, a.pheight, a.bgcolor, b != "never" &&
                        b != "no" && b != "false", c);
                    for (var d in a.pparams)typeof a.pparams[d] != "function" && b.addParam(d, a.pparams[d]);
                    for (var i in a.pvars)typeof a.pvars[i] != "function" && b.addVariable(i, a.pvars[i]);
                    b.addVariable("xml", a.pxml);
                    b.embed(a.ptarget)
                } else alert("ERROR: createPanoViewer.embed() - target needed")
            };
            c.wmode && a.addParam("wmode", c.wmode);
            c.passQueryParameters && a.passQueryParameters();
            return a
        }
    }
    function showPreloader(_preloaderId){
        preloaderID = _preloaderId
        _width = (jQuery(_containerDiv).width()) + "px";
        _height = (jQuery(_containerDiv).height()) + "px";
        jQuery("#"+_preloaderId).css({left:(jQuery(_containerDiv).width()/2)+'px'});
        jQuery("#"+_preloaderId).css({top:(jQuery(_containerDiv).height()/2)+'px'});
        jQuery("#"+_preloaderId).show();
    }
    hidePreloader = function(){
        jQuery("#"+_preloaderId).hide();
    }
		
    function ajaxCall(_paramPath){
        var returnval;
					jQuery.ajax({
							async: false,
							url: _paramPath,
							success: function (a) {
									returnval =  true;
							},
							statusCode: {
									404: function() {
											returnval =  false;
									}
							}
					});
					return returnval;
				
				}
		
		
	
	
	function dispatcheventHandler(){
		jQuery("#"+preloaderID).hide();
		jQuery("#vrholder").trigger("ioError", errorMessage);
		return false;
	}

    var KRPano = function(){
        var that = this;
        var swfPano = new swfKRPano();
        var krPanoSWFObjectId = null;
        this.unload = function(_paramContainer){
            var krPano = jQuery('#' + krPanoSWFObjectId);
            if (krPano && krPano.unload) {
                krPano.unload();
            }
        }
				
        this.loadImage = function (vrFolderPath, wegGlRend, container, _preloaderId) {
            that.krPanoSWFObjectId = "krpano" + container.id;
            that.unload()
            _preloaderId = _preloaderId;
            showPreloader(_preloaderId)  /*Show preloader */
			var xmlPath = vrFolderPath + "/pano.xml";
            /*if(! ajaxCall(xmlPath)){
                 return dispatcheventHandler();
            }*/

			var imagePath = vrFolderPath + vrFolderPath.substring(vrFolderPath.lastIndexOf('/'), vrFolderPath.length);
			if(imagePath.indexOf('#') !== -1){
				return dispatcheventHandler();
			}
			
            var hlookat = null;
            var vlookat = null;
            			
            jQuery(container).empty();
            if(VirtualViewer.BROWSER_TYPE == 3){
                if(_panoViewer == null){
                    _panoViewer = swfPano.createPanoViewer({ swf: "/sites/all/modules/custom/custom_seat_view/swfs/pano.swf", id: "krpano"+container.id, xml: xmlPath, target: container, preloaderId:_preloaderId });
                }else{
                    _panoViewer.pwidth = _width;
                    _panoViewer.pheight = _height;
                }
                _panoViewer.addParam("wmode", "transparent");
                _panoViewer.addParam("allowFullScreen", "true");
            }else if(VirtualViewer.BROWSER_TYPE == 4){  
				if(! ajaxCall(imagePath+"_Right.jpg")){ return dispatcheventHandler(); }
				if(! ajaxCall(imagePath+"_Left.jpg")){ return dispatcheventHandler(); }
				if(! ajaxCall(imagePath+"_Up.jpg")){ return dispatcheventHandler(); }
				if(! ajaxCall(imagePath+"_Down.jpg")){ return dispatcheventHandler(); }
				if(! ajaxCall(imagePath+"_Front.jpg")){ return dispatcheventHandler(); }
				if(! ajaxCall(imagePath+"_Back.jpg")){ return dispatcheventHandler(); }
                if(_panoViewer == null){
                   _panoViewer = swfPano.createkrpanoJSviewer("krpano"+container.id,_preloaderId,null,null,false);
                }
                _panoViewer.addVariable("simulatedevice", "iphone");
                _panoViewer.addParam("width", _width);
                _panoViewer.addParam("height", _height);
            }
            _panoViewer.addVariable("id","krpano"+container.id)
            _panoViewer.addVariable("xml", xmlPath);
            _panoViewer.addVariable("debugmode", "false");
            _panoViewer.addVariable("showErrors", "false");
            _panoViewer.addVariable("view.hlookat", 0);
            _panoViewer.addVariable("view.vlookat", 0);
            _panoViewer.addVariable("view.fov", 65);
            _panoViewer.addVariable("view.fovmin", 65);
            _panoViewer.addVariable("view.fovmax", 65);
            _panoViewer.addParam("width", "100%");
            _panoViewer.addParam("height", "100%");
            _panoViewer.embed(container.id);
            this.resetView = function (){
                var krpanoDivContainer = jQuery("#krpano"+container.id).get(0);
                if(krpanoDivContainer){
                    krpanoDivContainer.set("view.hlookat", 0);
                    krpanoDivContainer.set("view.vlookat",0 );
                    Utils.log("krpano  "+krpanoDivContainer.get("view.fovmax")+"  "+krpanoDivContainer.get("view.fovmin")+" "+krpanoDivContainer.get("view.hlookatmin")+ "FOV: " + krpanoDivContainer.get("view.fov"));
                }
            }
        }
    }


    var  CubicVRCLS = function () {
        var position = new THREE.Vector3(0, 0, 0);
        var target = new THREE.Vector3(0, 0, 1);// set the z to 1 to flip the camera.
        var isFirstTime = true;
        var cubeWidth, cubeHeight, cubeDepth, cubeSegments_width, cubeSegments_height, cubeMaterials;
        var MOUSE_SPEED_FACTOR = 0.1;
        this.WIDTH;
        this.HEIGHT;
        this.renderer = null;
        this.scene = null;
        var that = this;
        var c = null;
        this.camera;
        this.animationId = null;

        this.unload = function(container){
            if(container){
                c = document.getElementById(container.id + "canvas");
                if(c){
                    var canvas;
                    var gl;

                    function start() {
                        canvas = c;

                        initWebGL(canvas);      // Initialize the GL context

                        // Only continue if WebGL is available and working

                        if (gl) {

                            gl.deleteBuffer(gl.getParameter(gl.ARRAY_BUFFER_BINDING));
                            gl.clearColor(0.0, 0.0, 0.0, 1.0);  // Set clear color to black, fully opaque
                            gl.clearDepth(1.0);                 // Clear everything
                            gl.enable(gl.DEPTH_TEST);           // Enable depth testing
                            gl.depthFunc(gl.LEQUAL);			              // Near things obscure far things
                            gl.clear(gl.COLOR_BUFFER_BIT|gl.DEPTH_BUFFER_BIT);

                        }
                    }
                    function initWebGL(canvas) {
                        gl = null;

                        try {
                            gl = canvas.getContext("webgl") || canvas.getContext("experimental-webgl");
                        }
                        catch(e) {
                        }

                        // If we don't have a GL context, give up now

                        if (!gl) {
                            //alert("Unable to initialize WebGL. Your browser may not support it.");
                            var messageModel = new MessageModel();
                            messageModel.set('title', "");
                            messageModel.set('message', "Unable to initialize WebGL. Your browser may not support it.");
                            messageModel.set('viewType', "warning");
                           // eventDispatcher.trigger(Constants.EVENT_SHOW_POP_UP,messageModel);
                        }
                    }
                    start();
                    //var canvasremove = jQuery('#'+container.id+'canvas');

                }
                if(c){
                    var elem = jQuery('#'+container.id + "canvas");
                    //c.parentNode.removeNode(c);
                    elem.detach();
                    elem.remove();
                    delete elem;
                    elem = null;
                }
            }
            cancelAnimationFrame( that.animationId );
        }
        this.loadImage = function (vrFolderPath, useWebGlRenderer, container, _preloaderId) {
            that.unload(container);
            showPreloader(_preloaderId);
            /*Setting up the image base path*/
            var imagePath = vrFolderPath + vrFolderPath.substring(vrFolderPath.lastIndexOf('/'), vrFolderPath.length);
			
			if(imagePath.indexOf('#') !== -1){
				return dispatcheventHandler();
			}
			
			
			if(! ajaxCall(imagePath+"_Right.jpg")){ return dispatcheventHandler(); }
			if(! ajaxCall(imagePath+"_Left.jpg")){ return dispatcheventHandler(); }
			if(! ajaxCall(imagePath+"_Up.jpg")){ return dispatcheventHandler(); }
			if(! ajaxCall(imagePath+"_Down.jpg")){ return dispatcheventHandler(); }
			if(! ajaxCall(imagePath+"_Front.jpg")){ return dispatcheventHandler(); }
			if(! ajaxCall(imagePath+"_Back.jpg")){ return dispatcheventHandler(); }
			
			
			position = new THREE.Vector3(0, 0, 0);
            target = new THREE.Vector3(0, 0, 1);// set the z to 1 to flip the camera.

            var isUserInteracting;
            var lon = 90, lat = 0, phi = 0, theta = 0, loadImageCounter = 0,olon,olat;
            cubeWidth = 500, cubeHeight = 500, cubeDepth = 500, cubeSegments_width = 7, cubeSegments_height = 7, cubeMaterials = 7;

            /*Setting the event listeners on the container*/
            container.addEventListener('mousedown', onDocumentMouseDown, false);
            container.addEventListener('mousemove', onDocumentMouseMove, false);
            container.addEventListener('mouseout', onDocumentMouseOut, false);
            container.addEventListener('mouseup', onDocumentMouseOut, false);
           /* if (Constants.CAMERA_ZOOM_ENABLED) {
                if (window.navigator.userAgent.toLowerCase().indexOf("fire") > 0) {
                    container.addEventListener('DOMMouseScroll', onDocumentMouseWheel, false);
                } else {
                    container.addEventListener('mousewheel', onDocumentMouseWheel, false);
                }
            }*/
            /*Setting up the reset view event handler*/
          //  eventDispatcher.bind(Constants.EVENT_RESET_PANO, resetView);

            this.WIDTH = jQuery(container).width();
            this.HEIGHT = jQuery(container).height();

            /* JVI: 24/7/12 2:26 PM */
            // make the mouse move according to the width of the stage
            MOUSE_SPEED_FACTOR = 360 / this.WIDTH;

            /*Preloader stuff*/
            /*var preloader = jQuery("#" + _preloaderId);
            preloader.show();
            preloader.width(this.WIDTH);
            preloader.height(this.HEIGHT);
            preloader.find('.Loader_Container').css({"top":"0%"});*/
            /*Removing the prvious canvas from the id*/
            //jQuery("#" + container.id + " canvas").remove();

            /*Creating the Renderer*/
            if (useWebGlRenderer) {
                //renderer = new THREE.WebGLRenderer();
                if(c){
                    this.renderer = new THREE.WebGLRenderer({antialias: false,
                        canvas: c,
                        clearColor: 0x000000,
                        clearAlpha: 0,
                        maxLights: 4,
                        stencil: false,
                        preserveDrawingBuffer: false});
                }else{
                    this.renderer = new THREE.WebGLRenderer();
                }
            } else {
                this.renderer = new THREE.CanvasRenderer();
            }

            /*Set the render size*/
            this.renderer.setSize(this.WIDTH, this.HEIGHT);
            /*giving the id for the canvas div tag*/
            this.renderer.domElement.setAttribute('id', container.id + 'canvas');
            /*Attach renderer to the container*/
            jQuery(container).append(this.renderer.domElement);

            //this.unload(container);

            /*Setup Camera*/
            var VIEW_ANGLE = 60, ASPECT = this.WIDTH / this.HEIGHT, NEAR = 0.1, FAR = 10000;

            /*Create a scene*/
            this.scene = new THREE.Scene();
            this.scene.id = container.id + "canvas";

            /*Create the camera*/
            this.camera = new THREE.PerspectiveCamera(VIEW_ANGLE, ASPECT, NEAR, FAR);
            /*Put the camera inside the cube*/
            this.camera.position.z = 0;

            /*Add the camera to the scene*/
            this.scene.add(this.camera);

            

            /*Creating the materials for the cube*/
            var materials = [];
            materials.push(loadMaterial(imagePath + "_Right.jpg"));
            materials.push(loadMaterial(imagePath + "_Left.jpg"));
            materials.push(loadMaterial(imagePath + "_Up.jpg"));
            materials.push(loadMaterial(imagePath + "_Down.jpg"));
            materials.push(loadMaterial(imagePath + "_Front.jpg"));
            materials.push(loadMaterial(imagePath + "_Back.jpg"));

            /*Creating the cube*/
            var cube = new THREE.Mesh(new THREE.CubeGeometry(cubeWidth, cubeHeight, cubeDepth, cubeSegments_width, cubeSegments_height, cubeMaterials, materials), new THREE.MeshFaceMaterial());
            cube.doubleSided = true;
            cube.scale.x = -1;
            this.scene.add(cube);

            this.resetView = function() {
                isFirstTime = true;
                isUserInteracting = false;
                loadImageCounter = 0;
                lon = 90, lat = 0, phi = 0, theta = 0;
                dx = 0;
                dy = 0;
                olon = lon;
                olat = lat;
                position.x = 0;
                position.y = 0;
            }


            that.resetView()
            /*Start animation loop*/
            animate();

            function loadMaterial(imgPath) {
                var texture = THREE.ImageUtils.loadTexture(imgPath, {}, function () {
                    texture.mapping.needsUpdate = true;
                    loadImageCounter++;
                    if (loadImageCounter > 5) {
                        /*preloader.hide();
                        preloader.width(this.WIDTH);
                        preloader.height(this.HEIGHT);*/
                        hidePreloader()
                    }
                });
                texture.mapping.needsUpdate = true;
                return mat(texture);
            }
            function mat(tex){
                var texture = tex;
                var mat = new THREE.MeshBasicMaterial({map:texture, overdraw:true});
                return mat;
            }

            function onDocumentMouseOut(event) {
                isUserInteracting = false;
            }

            function onDocumentMouseDown(event) {
                event.preventDefault();
                isUserInteracting = true;
                //isFirstTime = false
                onPointerDownPointerX = event.clientX;
                onPointerDownPointerY = event.clientY;
                onPointerDownLon = lon;
                onPointerDownLat = lat;
            }

            function onDocumentMouseMove(event) {
                event.preventDefault();
                if (isUserInteracting) {
                    isFirstTime = false;
                    var dx = (onPointerDownPointerX - event.clientX) * MOUSE_SPEED_FACTOR * -1;//0.063 * -1;
                    var dy = (event.clientY - onPointerDownPointerY) * MOUSE_SPEED_FACTOR * -1;//0.063 * -1;
                    position.x = dx + onPointerDownLon; // reversed dragging direction (thanks @mrdoob!)
                    position.y = dy + onPointerDownLat;
                    /* JVI: 9/7/12 3:15 PM */
                    // for lock pano compire view
                   // eventDispatcher.trigger("LockPano_MouseMove", {value:container, positionX:position.x, positionY:position.y});
                }
            }

            function onDocumentMouseUp(event) {
                event.preventDefault();
                isUserInteracting = false;
            }

            function onDocumentMouseWheel(event) {
                event.preventDefault();
                var delta = 0;
                if (event.detail) {
                    delta = -event.detail / 3;
                }
                else {
                    delta = event.wheelDelta / 120;
                }
                var camFov = camera.fov - delta * 1; //20-05-2012
                if (camFov >= VirtualViewer.CAMERA_MIN_FOV && camFov < VirtualViewer.CAMERA_MAX_FOV) {
                    camera.fov = camFov;
                    camera.updateProjectionMatrix();
                    //isLoadFirst = false;
                    /* JVI: 9/7/12 3:16 PM */
                    //mouse wheel on lock pano (zoom)
                    //eventDispatcher.trigger("LockPano_MouseWheel", {value:container, camera:camera.fov});
                }
            }

            function animate() {
                that.animationId = requestAnimationFrame(animate);
                render();
            }

            function render() {
                olon = lon;
                olat = lat;
                if (isFirstTime) {
                    lat = Math.max(-85, Math.min(85, lat));
                    position.x = position.y = 0;
                    phi = Math.ceil((90 - lat) * Math.PI / 180);
                    theta = Math.ceil(lon * Math.PI / 180);
                    target.x = cubeWidth * Math.round(Math.sin(phi)) * Math.round(Math.cos(theta));
                    target.y = cubeHeight * Math.round(Math.cos(phi));
                    target.z = cubeDepth * Math.round(Math.sin(phi)) * Math.round(Math.sin(theta));
                    that.camera.lookAt(target);
                }
                else {

                    lon = lon + (position.x - olon) * .12;
                    lat = lat + (position.y - olat) * .12;
                    lat = Math.max(-85, Math.min(85, lat));
                    phi = (90 - lat) * Math.PI / 180;
                    theta = lon * Math.PI / 180;
                    target.x = cubeWidth * Math.sin(phi) * Math.cos(theta);
                    target.y = cubeHeight * Math.cos(phi);
                    target.z = cubeDepth * Math.sin(phi) * Math.sin(theta);
                    that.camera.lookAt(target);
                }

                that.renderer.render(that.scene, that.camera);
            }

            /* JVI: 9/7/12 3:16 PM */
            //LockPano_MouseMove
           // eventDispatcher.bind("LockPano_MouseMove", lockPano_mousemove);
            function lockPano_mousemove(event) {
                if (event.value.id != container.id && VirtualViewer.COMPARE_MODE == 1) {
                    position.x = event.positionX;
                    position.y = event.positionY;
                    /* JVI: 20/7/12 2:47 PM */
                    // when compare mode is on then for moving the other pano isFirstTime is make false due to this. on render time the values will get changed
                    isFirstTime = false;
                }
            }

            /* JVI: 9/7/12 3:16 PM */
            //LockPano_MouseWheel
          //  eventDispatcher.bind("LockPano_MouseWheel", lockPano_mousewheel);
            function lockPano_mousewheel(event) {
                if (event.value.id != container.id && VirtualViewer.COMPARE_MODE == 1) {
                    camera.fov = event.camera;
                    camera.updateProjectionMatrix();
                }
            }
        }
    };
    switch(VirtualViewer.BROWSER_TYPE){
        case "0":
        case "1":
        case "2":
            _viewer = new CubicVRCLS();
            break;
        case "3":
        case "4":
            _viewer = new KRPano();
            break;
    }
    that.loadViewer = function(_paramHolderId, _paramFolderPath, _paramPreloaderId){
        _holderId = _paramHolderId;
        _folderPath = _paramFolderPath;
        _preloaderId = _paramPreloaderId;
        _width = jQuery("#"+_holderId).width();
        _height = jQuery("#"+_holderId).height();
        _containerDiv =  jQuery("#" + _holderId).get(0);
        if(!fullScreenApi.isFullScreen()){
            _widthNor = (jQuery(_containerDiv).width()) + "px";
            _heightNor = (jQuery(_containerDiv).height()) + "px";
        }
        _viewer.loadImage(_folderPath,true,_containerDiv,_preloaderId);
    }

    that.fullscreenViewer = function(){
       if(fullScreenApi.supportsFullScreen){
           if(fullScreenApi.isFullScreen()){
               fullScreenApi.requestFullScreen(_containerDiv);
           }else{
               fullScreenApi.requestFullScreen(_containerDiv);
           }
       }else{
           if(VirtualViewer.BROWSER_TYPE == "4"){
               _width =  1024+"px";
               _height = 574 +"px";
               jQuery(_containerDiv).width(_width);
               jQuery(_containerDiv).height(_height);
           }else{
               _width = document.documentElement.clientWidth +"px";
               _height = document.documentElement.clientHeight +"px";
               jQuery(_containerDiv).width(_width);
               jQuery(_containerDiv).height(_height);
           }
           _viewer.loadImage(_folderPath,true,_containerDiv,_preloaderId);
       }

    }
    this.normalscreenView = function(_paramWidth, _paramHeight){
        _width =  _paramWidth+"px";
        _height = _paramHeight +"px";
        jQuery(_containerDiv).width(_width);
        jQuery(_containerDiv).height(_height);
        _viewer.loadImage(_folderPath,true,_containerDiv,_preloaderId);
    }
    function fullscreenHandler(){
        if(fullScreenApi.isFullScreen()){
            _width = screen.width + "px";
            _height = screen.height + "px";
        }else{
            _width = _widthNor;
            _height = _heightNor;
        }
        jQuery("#"+_holderId).width(_width);
        jQuery("#"+_holderId).height(_height);
        _viewer.loadImage(_folderPath,true,_containerDiv,_preloaderId)
    }
    that.resetViewer = function(){
        _viewer.resetView();
    }
    that.clearViewer = function(){
        _viewer.unload(_holderId)
    }

    function isFullscreen(){
        switch(VirtualViewer.BROWSER_TYPE){
            case "0":
            case "2":
                return (document.webkitIsFullScreen)? true : false;
                break;
            case "1":
                return (document.mozFullScreen)? true : false;
                break;
            case "3":
                return (document.fullscreen)? true : false;
                break;
        }
    }
    function browsertype(){
        var ua = navigator.userAgent.toLowerCase();
        if(jQuery("#normalView").find()){
            jQuery("#normalView").hide();
        }
        if(ua.indexOf('chrome') > -1){
            document.addEventListener("webkitfullscreenchange",fullscreenHandler);
            return ("0");
        }else if (ua.indexOf('firefox') > -1) {
            //document.addEventListener("mozfullscreenchange",function () {fullscreenHandler((document.mozFullScreen)? true : false);}, false);
            document.addEventListener("mozfullscreenchange",fullscreenHandler )
            return ("1");
        } else if (ua.indexOf('opera') > -1) {
            document.addEventListener("webkitfullscreenchange",fullscreenHandler );
            return ("2");
        } else{
            if ((ua.indexOf('safari') > -1) && (ua.indexOf('chrome') == -1) && (ua.indexOf('firefox') == -1)) {
                if ((ua.indexOf('ipad') != -1) || (ua.indexOf('iphone') != -1) || (ua.indexOf('mobile') != -1)) {
                    document.addEventListener("webkitfullscreenchange",fullscreenHandler);
                    return ("4");
                }
                else{
                    document.addEventListener("webkitfullscreenchange",fullscreenHandler);
                    return ("4");
                }
            } else if (ua.indexOf('msie') > -1) {
                document.attachEvent("fullscreenchange",fullscreenHandler);
                if(jQuery("#normalView").find()){
                    jQuery("#normalView").show();
                }
                return ("3");
            }
        }
    }
 }

var Utils = function(){

}
Utils.log = function (msg) {

    /*if (Constants.DEBUG_MODE) {
     console.log(msg);
     }*/
}

hideprogressbar = function(){
   jQuery("#"+preloaderID).hide();
}
imageloadingerror = function(){
    jQuery("#vrholder").trigger("ioError", errorMessage);
}



