/**
 * Created with JetBrains WebStorm.
 * User: nsaini
 * Date: 1/20/14
 * Time: 4:01 PM
 * To change this template use File | Settings | File Templates.
 */
var Pano = function() {
    this.krPanoSWFObjectId = null;
    var that = this;
    var errorMessage = "Unexpected server error occured. Please try again later.";

    this.clearCanvas = function(){
        var krPano = document.getElementById(that.krPanoSWFObjectId);
        if (krPano && krPano.unload) {
            krPano.unload();
            removepano(that.krPanoSWFObjectId);
        }
    }
    this.loadPano = function(container, vrFolderPath, _preloaderId, _cameraAngleTitlY){
        that.krPanoSWFObjectId = "krpano" + container.id;

        jQuery("#"+_preloaderId).show();
        var width = (jQuery(container).width()) + "px";
        var height = (jQuery(container).height()) + "px";
        jQuery("#"+_preloaderId).width(width);
        //jQuery("#"+_preloaderId).height(height);
        //jQuery("#"+_preloaderId).find('.Loader_Container').css({"top":"0%"});

        var hlookat = null;
        var vlookat = _cameraAngleTitlY;
        //JVI 2013_07_04 in ie we where using the "_cameraAngleTitlY" make 25% less bz of the browser height is less due to the address bar,
        if(_cameraAngleTitlY >= 30){
            _cameraAngleTitlY = _cameraAngleTitlY-(_cameraAngleTitlY *.25)
        }

        var xmlPath = null;
        var imagePath = vrFolderPath + vrFolderPath.substr(vrFolderPath.lastIndexOf('/'), vrFolderPath.length);
        xmlPath = '<krpano version="1.16.9" onstart="startup();"><action name="startup">loadscene(get(scene[0].name), null, MERGE);</action>' +
            '<progress showload="none" showwait="none" />' +
            '<autorotate enabled="false" waittime="3" speed="2.0" horizon="0.0" accel="0.5"/>' +
            '<scene name="scene_vorraum" title="vorraum" onstart="" >' +
            '<view fovtype="MFOV"/>' +
            '<image>' +
            '<back url="'+ imagePath + '_Back.jpg"></back>' +
            '<down url="'+ imagePath + '_Down.jpg"></down>' +
            '<front url="'+ imagePath + '_Front.jpg"></front>' +
            '<left url="'+ imagePath + '_Left.jpg"></left>' +
            '<right url="'+ imagePath + '_Right.jpg"></right>' +
            '<up url="'+ imagePath + '_Up.jpg"></up>' +
            '<mobile>' +
            '<back url="'+ imagePath + '_Back.jpg"></back>' +
            '<down url="'+ imagePath + '_Down.jpg"></down>' +
            '<front url="'+ imagePath + '_Front.jpg"></front>' +
            '<left url="'+ imagePath + '_Left.jpg"></left>' +
            '<right url="'+ imagePath + '_Right.jpg"></right>' +
            '<up url="'+ imagePath + '_Up.jpg"></up>' +
            '</mobile>' +
            '<tablet>' +
            '<back url="'+ imagePath + '_Back.jpg"></back>' +
            '<down url="'+ imagePath + '_Down.jpg"></down>' +
            '<front url="'+ imagePath + '_Front.jpg"></front>' +
            '<left url="'+ imagePath + '_Left.jpg"></left>' +
            '<right url="'+ imagePath + '_Right.jpg"></right>' +
            '<up url="'+ imagePath + '_Up.jpg"></up>' +
            '</tablet>' +
            '</image>' +
            '</scene>' +
            '<contextmenu fullscreen="false">' +
            '</contextmenu>'+
            '<events onkeydown="action(keydown);" onkeyup="action(keydown);" onmousedown="set(syncother,true);" onloadcomplete="js(hideLoadingBar('+ _preloaderId +'));" onviewchanged="if(syncother, js( sync_1to2()));" />' +
            '<action name="keydown">set(control.usercontrol, mouse); </action> ' +
            '</krpano>';


        jQuery(container).empty();

       //if (0) {//jQuery.browser.msie && ((jQuery.browser.version == '9.0') || ((jQuery.browser.version == '8.0')) || (jQuery.browser.version == '7.0'))) {
       //
       //}
       //else {
       // if(! ajaxCall(imagePath+"_Right.jpg")){ return dispatcheventHandler(); }
       // if(! ajaxCall(imagePath+"_Left.jpg")){ return dispatcheventHandler(); }
       // if(! ajaxCall(imagePath+"_Up.jpg")){ return dispatcheventHandler(); }
       // if(! ajaxCall(imagePath+"_Down.jpg")){ return dispatcheventHandler(); }
       // if(! ajaxCall(imagePath+"_Front.jpg")){ return dispatcheventHandler(); }
       // if(! ajaxCall(imagePath+"_Back.jpg")){ return dispatcheventHandler(); }
       //}

        var viewer = null;
        viewer = createPanoViewer({ swf: "/sites/all/modules/custom/custom_seat_view/swfs/swfkrpano.swf", id: that.krPanoSWFObjectId, xml:"", target: jQuery(container).attr('id'), html5:"prefer+webgl", passQueryParameters:true });

        viewer.addVariable("simulatedevice", "useragent");
        viewer.addParam("wmode", "transparent");
        viewer.addParam("allowFullScreen", "true");
        viewer.addParam("width", '100%');
        viewer.addParam("height", '100%');
        viewer.addVariable("debugmode", "false");
        viewer.addVariable("showErrors", "false");
        viewer.addVariable("view.hlookat", 0);
        viewer.addVariable("view.vlookat", _cameraAngleTitlY);
        viewer.addVariable("view.fov", 90);
        viewer.addVariable("view.fovmin", 90);
        viewer.addVariable("view.fovmax", 90);
        viewer.embed();

        var panoload = setInterval(function(){
            try{
                document.getElementById(that.krPanoSWFObjectId).call("loadxml(" + (xmlPath) + ", null, MERGE)");
                window.clearInterval(panoload);
            }catch(e){}}, 500);


        //eventDispatcher.bind(Constants.EVENT_RESET_PANO, resetView);       
		jQuery('.vrview-reset-view').bind("resetView", resetView);
        function resetView(){
            var krpanoDivContainer = jQuery("#krpano"+container.id).get(0);
            if(krpanoDivContainer){
                krpanoDivContainer.set("view.hlookat", 0);
                krpanoDivContainer.set("view.vlookat", _cameraAngleTitlY);
            }
        }

        function ajaxCall(_paramPath){
            var returnval;			
				jQuery.ajax({
					async: false,
					//url: "http://vv-msg-knicks-2013.s3.amazonaws.com/media/vrview/images/Section_113/VR_Section_113_13_10/VR_Section_113_13_10_Front.jpg",
					url: _paramPath,
					success: function (a) {
						returnval =  true;
					},
					error:function(e){
						console.log(e.statusText);
						returnval= false;
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
            jQuery("#"+_preloaderId).hide();
            jQuery("#vrholder").trigger("ioError", errorMessage);
            return false;
        }
    }

}