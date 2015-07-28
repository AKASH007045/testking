var ttextCounter = false;
var ttextCounter1 = true;
function isiDevice(){
    var isiPad = navigator.userAgent.match(/iPad/i) != null;
    var isiPhone = navigator.platform.indexOf("iPhone") != -1;
    var isiPod = navigator.platform.indexOf("iPod") != -1;
    if(isiPad || isiPhone || isiPod){
      return true;
    }
    else{
      return false;
    }
  }

jQuery(document).ready(function(){
  //Hide/Show Source button
  /*************  For Ticket STPDEV-740 *****************************************/
  var interv="";
  function qtimer() {
    interv = setInterval(function(){
      var len = jQuery("body").find('a.cke_button__source').length;
      var temp = 'show';
      
      if(len>0){
        var iframe = jQuery('iframe.cke_wysiwyg_frame');
        jQuery('*', iframe.contents()).on('keydown', null, 'ctrl+up', function () {
          if (temp == 'show') {
            jQuery("body").find('a.cke_button__source').show();
            temp = 'hide';
          }
          else if (temp == 'hide') {
            jQuery("body").find('a.cke_button__source').hide();
            temp = 'show';
          }
        });
        jQuery('body').on('keydown', null, 'ctrl+up', function () {
          if (temp == 'show') {
            jQuery("body").find('a.cke_button__source').show();
            temp = 'hide';
          }
          else if (temp == 'hide') {
            jQuery("body").find('a.cke_button__source').hide();
            temp = 'show';
          }
        });
        jQuery("body").find('a.cke_button__source').hide(); 
        clearInterval(interv);
      }
    }, 2000);
  }
  qtimer();
 /************  End For the ticket STPDEV-740 ********/
 jQuery('.page-node-delete.node-type-promo-tile .form-actions a#edit-cancel').click(function() {
   jQuery('a#overlay-close').click();
 });
//AddClasses on buttons
jQuery('input.form-submit').each(function(){
	var val1 = jQuery(this).val();
	//alert(val);
	if(val1 == "Remove" || val1 == "Delete"){
		jQuery(this).addClass('rmve-btn');
	}
	if(val1 == "Save"){
		jQuery(this).addClass('sve-btn');
	}
	if(val1 == "Upload"){
		jQuery(this).addClass('upload-btn');
	}
});

  jQuery('.page-admin-manage-pages-add-pdf a#overlay-close').click(function() {
	alertifyreset () ;
        alertify.confirm("You have unsaved changes. Do you want discard changes?", function (e) {
            if (e) {
              var wraper = jQuery(this).parents('div.tile-image-form');
              wraper.css('visibility','hidden');
              ttextCounter1 = false;
              location.reload(true);
            } else {
                e.preventDefault();
            }
        });
    });
  
  jQuery('#views-form-take-over-pages-page .views-field-nid').hide();


});
jQuery(document).ajaxComplete(function(event, request, settings) {
//AddClasses on buttons
jQuery('input.form-submit').each(function(){
	var val1 = jQuery(this).val();
	//alert(val);
	if(val1 == "Remove" || val1 == "Delete"){
		jQuery(this).addClass('rmve-btn');
	}
	if(val1 == "Save"){
		jQuery(this).addClass('sve-btn');
	}
	if(val1 == "Upload"){
		jQuery(this).addClass('upload-btn');
	}
});
});