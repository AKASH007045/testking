var _folderPath = '';
var _cameraTiltY = 0;
var seat_info_txt = '';
var virtualViewer = new Pano();
var resetFlag = true;

function clearErrorDivs(){
	jQuery('#vrholder').parent().remove('.error');
}

jQuery(document).ready(function(){
  jQuery('#vrholder').bind('ioError', errorHandler);
	_folderPath = jQuery('.seat-vv-info li:first a').attr('href');
	_cameraTiltY = (jQuery('.seat-vv-info li:first a').attr('data-tilt') == undefined)?0:jQuery('.seat-vv-info li:first a').attr('data-tilt');
  var cl = "";
	jQuery('.seat-vv-info a').click(function(e){
		errorHandler("", "");
		resetFlag = true;
		jQuery('.seat-view-error').remove();
		e.preventDefault();		
		cl = jQuery(this).attr('class');
		
		if(cl!=""){
                   jQuery('.vr-seat-view-wrapper div.view-seat-info .seat-info-row').hide();
		   jQuery('.vr-seat-view-wrapper div.view-seat-info div.' + cl).show();
		}
		
		_folderPath = jQuery(this).attr('href');
		console.log(_folderPath);
		_cameraTiltY = (jQuery(this).attr('data-tilt') == undefined)?0:jQuery(this).attr('data-tilt');
		jQuery('.seatview-dd').text("");
		jQuery('.seatview-dd').html(jQuery('span', this).html());
		clearErrorDivs();
		virtualViewer.clearCanvas();
		//virtualViewer.loadViewer("vrholder", _folderPath,"preloader");
		var panoObject = jQuery("#vrholder").get(0);
		virtualViewer.loadPano(panoObject, _folderPath, "preloader", _cameraTiltY);
	});
	
	//on page load
	jQuery('.seatview-dd').text("");
	
	cl = jQuery('.seat-vv-info li:first-child a').attr('class');
	
	if(cl!=""){
		jQuery('.vr-seat-view-wrapper div.view-seat-info .seat-info-row').hide();
		jQuery('.vr-seat-view-wrapper div.view-seat-info div.' + cl).show();
	}
	jQuery('.seatview-dd').html(jQuery('.seat-vv-info li:first a span').html()+ '<div class="mInfo mgTicket"><a href="javascript:void(0);" class="more-info">SEAT INFO</a></div>');
	clearErrorDivs();
	//virtualViewer.loadViewer("vrholder", _folderPath,"preloader");
	var panoObject = jQuery("#vrholder").get(0);
	virtualViewer.loadPano(panoObject, _folderPath, "preloader", _cameraTiltY);
	
	jQuery('.vrview-full-screen-view').click(function(e){
		e.preventDefault();		
		virtualViewer.fullscreenViewer();
	});
	
	jQuery('.vrview-reset-view').click(function(e){
		e.preventDefault();	
		if(resetFlag){
			//virtualViewer.resetView();
			jQuery('.vrview-reset-view').trigger('resetView');
		}
		
	});
	if(jQuery('.seat-view-error').length !=0) {jQuery(".vrview-reset-view").hide();jQuery(".vrview-full-screen-view").hide();}
	bindClick();
	jQuery('.seat-vv-info a').bind('click', function(e){
		e.preventDefault();
		var atagclass = '';
		viewhtml=jQuery(this).html() + '<div class="mInfo mgTicket"><a href="javascript:void(0);" class="more-info">SEAT INFO</a></div>';
		jQuery('.seatview-dd').html(viewhtml);
		bindClick();
		onloadRsvpInfo()
		jQuery('.seatview-dd li .fa').trigger('click');
	});
	
});
function bindClick(){
  jQuery('body').find('.seatview-dd li .fa').parent().bind('click', function(e){
    e.preventDefault();
    jQuery('.seatview-dd div.mInfo a.active').click();
    jQuery(this).parents('.seatview-dd').toggleClass('seatview-dd-active').next('.seat-vv-info').slideToggle();
  });
}
function errorHandler(_e, theData){
  if( jQuery('#vrholder').parent().find(".seat-view-error").length >= 1 ){
  }
  else {
    if (theData){
	resetFlag = false;
        jQuery('#vrholder').html("<div class='seat-view-error'></div>");
    }
  }	
}