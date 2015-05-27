var _folderPath = '';
var seat_info_txt = '';
var virtualViewer = new VirtualViewer();

function clearErrorDivs(){
	jQuery('#vrholder').parent().remove('.error');
}

jQuery(document).ready(function(){	
  jQuery('#vrholder').bind('ioError', errorHandler);
	_folderPath = jQuery('.seat-vv-info li.first a').attr('href');
  var cl = "";
	jQuery('.seat-vv-info a').click(function(e){
		errorHandler("", "");
		e.preventDefault();		
		cl = jQuery(this).attr('class');
		
		if(cl!=""){
			jQuery('#block-custom-block-my-seat-info div.content > div').hide();
			jQuery('#block-custom-block-my-seat-info div.content > div.' + cl).show();
		}
		
		_folderPath = jQuery(this).attr('href');    
		jQuery('.seatview-dd').text("");
		jQuery('.seatview-dd').text(jQuery('span', this).text());
		clearErrorDivs();
		virtualViewer.loadViewer("vrholder", _folderPath,"preloader");
	});
	
	//on page load
	jQuery('.seatview-dd').text("");
	
	cl = jQuery('.seat-vv-info li.first a').attr('class');
	
	if(cl!=""){
		jQuery('#block-custom-block-my-seat-info div.content > div').hide();
		jQuery('#block-custom-block-my-seat-info div.content > div.'+cl).show();
	}
	jQuery('.seatview-dd').text(jQuery('.seat-vv-info li.first a span').text());
	clearErrorDivs();
	virtualViewer.loadViewer("vrholder", _folderPath,"preloader");
	
	jQuery('.vrview-full-screen-view').click(function(e){
		e.preventDefault();		
		virtualViewer.fullscreenViewer();
	});
	
	jQuery('.vrview-reset-view').click(function(e){
		e.preventDefault();		
		virtualViewer.resetViewer();
	});
});

function errorHandler(_e, theData){
	if( jQuery('#vrholder').parent().find(".seat-view-error").length >= 1 ){
	}
	else {
		if(theData){
			jQuery('#vrholder').parent().append("<div class='seat-view-error'>"+theData+"</div>");
		}
	}	
}