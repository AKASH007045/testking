/**
 * @file
 * Attaches the behaviors for the Overlay child pages.
 * script created by debraj
 */

(function ($) {

  // Adjust the overlay dimensions.
  Drupal.behaviors.custom_modal = {  
    attach: function (context) {
      $('#overlay:not(.custom_modal-adjusted)', context).each(function() {		  
        
	//hide view/edit tabs from specific node types	
        if ($("body").hasClass('node-type-promo-tile') || $("body").hasClass('node-type-games')
	    || $("body").hasClass('node-type-other-event') || $("body").hasClass('node-type-team')){
	      $('#overlay-tabs', this).hide();  // hide "tabs"
	    }
	    
	if ($("body").hasClass('node-type-event')){	
		
		 // adjust the overlay
          $(this).css({
            /*'width'     : '100%',*/
            'min-width' : '450px',			
          });
          $('.add-or-remove-shortcuts', this).hide();  // hide "add short-cut" button
		  $('#overlay-tabs', this).hide();  // hide "tabs"
		  
          $('#branding', this).hide();  // hide branding container
		}		
      }).addClass('custom_modal-adjusted');
    }
  };
})(jQuery);