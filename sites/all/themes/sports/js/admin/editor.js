var ttextCounter = false;
var ttextCounter1 = true;
var selectopen_crop =0;
var shouldsubmit = false; // Represent the button should ask for confirmation or not.
window.onbeforeunload = confirmUnloading;

jQuery(document).ready(function(e) {
  /*set cookies for editor pannel*/
  var cookies;
  function setCookie(){
    jQuery.cookie('stp-editorp-toggle', '1');
  }
  function deleteCookie(){
    jQuery.cookie('stp-editorp-toggle', null);
  }
  function getCookie(){
    cookies = jQuery.cookie("stp-editorp-toggle");
  }
	getCookie();
	if (cookies == "1"){
		jQuery('.highlighted').addClass('slide');
		jQuery('.home-tgl').find('i').addClass('fa-angle-left').removeClass('fa-angle-right');
	}
	jQuery('.home-tgl').click(function(){   
		if(jQuery('.highlighted').hasClass('slide')){
			setCookie();
		}
		else{
			deleteCookie();   
		}
	});
	jQuery('.navbar-header button.navbar-toggle').click(function(){
		jQuery('body').toggleClass('slide-nav');
		jQuery('html').toggleClass('slidenav');
	});
  
  var url = document.location.toString();
  if (url.match('#')) {
    // Clear active item
    // Activate item number #hash
    var index = url.split('#')[1];
    if(index){
      if(jQuery('#node-'+index).length){
          jQuery('.carousel-inner div').removeClass('active');        
          jQuery('#node-'+index).parent().addClass('active');
      }
    }    
  }
  
  jQuery('#views-bootstrap-carousel-1 .carousel-control').bind( 'click', function(e) {
    // Update location based on slide (index is 0-based)
    setTimeout(function(){
      var a = jQuery('#views-bootstrap-carousel-1 .carousel-inner .active div').first();
      var id = a[0].id;
      var path = id.replace("node-", "");
      window.location.hash = "#"+path;
    }, 1000);
  })
  // add class on left menu for separtor
  jQuery(".left_menu .navbar-nav li a").addClass("pipe_mark");
  jQuery(".left_menu .navbar-nav li a").hover(function(){
		  jQuery(".left_menu .navbar-nav li a").removeClass("pipe_mark");
  });

  jQuery(".left_menu .navbar-nav li a").mouseout(function(){
		  jQuery(".left_menu .navbar-nav li a").addClass("pipe_mark");
  });
  
  jQuery('#block-custom-block-admin-home-link li.edit a').click(function(e) {
    e.preventDefault();
    if (ttextCounter == true) {
      alertifyreset ();
      alertify.confirm("You have unsaved changes. ", function (e) {
        if (e) {
          ttextCounter1 = false;
          location.reload(true);
        } 
        else { 
          ttextCounter1 = true; 
        }
      });
      return;
    }
    else {
      jQuery('div.tile-image-form').css('visibility','hidden');
    }
    if (jQuery('.view-home .contextual-links li.quick-quickedit a').length >0 ) {
      jQuery('#block-custom-block-admin-home-link li.view a').removeClass('active');
      jQuery('#block-custom-block-admin-home-link li.edit a').addClass('active');
      jQuery('.view-home .contextual-links li.quick-quickedit a').click();
    }
    return false;
  });   
  jQuery('.region-content ul.tabs-primary-live li.edit-link').click(function() {
    var curr_data_id = jQuery(this).attr('data-id');
    if (jQuery('#' + curr_data_id + ' .contextual-links li.quick-quickedit a').length >0 ) {
      jQuery('.main-wrap ul.tabs-primary-live li.node-view a').removeClass('active');
      jQuery('.main-wrap ul.tabs-primary-live li.edit-link a').addClass('active');
      jQuery('#' + curr_data_id + ' .contextual-links li.quick-quickedit a').click();
    }
    return false;
  });
  
  jQuery('.tile-image-form .popup-close').click(function() {
  var wraper = jQuery(this).parents('div.tile-image-form');
  if (ttextCounter == true) {
    alertifyreset () ;
    alertify.confirm("You have unsaved changes. ", function (e) {
      if (e) {
        wraper.css('visibility','hidden');
        wraper.css('display','none');
        console.log(wraper);
        ttextCounter1 = false;
        ttextCounter = false;
        //   location.reload(true);
      } 
      else {
        
      }
    });
    return;
  }
  else {
    wraper.css('visibility','hidden');
    wraper.find('.tile-image-form-wrapper').html('<div class="ajax-progress ajax-progress-throbber"><div class="throbber">&nbsp;</div></div>');
  }
  });
	
	jQuery('.main-wrap .highlighted .home-tgl').click(function(){
		right: "0"
  });

  jQuery("#block-custom-block-promo-tile div.home-page-tile").each(function() {
    var $id = '';
    $id = jQuery(this).attr('id');
    jQuery("#" + $id + " form.node-form .field-name-field-type select.form-select").each(function(){
      var $pid = jQuery(this).parents('div.home-page-tile').attr('id');
      hidepromoSelect(this, $pid);
      jQuery(this).on('change', function(){
        var $pid = jQuery(this).parents('div.home-page-tile').attr('id')
        hidepromoSelect(this, $id);
      });
    });
  });

  //Slide Toggle right
  jQuery('.home-tgl').click(function() {	
    jQuery(this).toggleClass('click').parents('.highlighted, .stp-editor-links').toggleClass('slide');
    jQuery(this).toggleClass('click').parents('.page-tabs').toggleClass('slide');
    jQuery(this).find('i').toggleClass('fa-angle-left fa-angle-right');
  });

  jQuery('.home-tgl').append('<span class="bg"/>');

  if(jQuery("body").hasClass('quickedit-init-processed')) {
    var interv = '';
    function qtimer() {
      interv = setInterval(function () {
        var len = jQuery("body").find('a.cke_button__sourcedialog').length;
        var temp = 'hide';
        if (len > 0) {
          jQuery("body").on('keydown', null, 'ctrl+up', function () {
            if (temp == 'show') {
              jQuery("body").find('a.cke_button__sourcedialog').show();
              temp = 'hide';
            }
            else if (temp == 'hide') {
              jQuery("body").find('a.cke_button__sourcedialog').hide();
              temp = 'show';
            }
          });
          clearInterval(interv);
        }
      }, 2000);
    }

    qtimer();
  }
});