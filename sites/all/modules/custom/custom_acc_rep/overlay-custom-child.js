/**
 * @file
 * Attaches the behaviors for the Overlay child pages.
 * script created by debraj
 */

(function ($) {

  // Adjust the overlay dimensions.
  Drupal.behaviors.custom_acc_rep = {  
    attach: function (context) {
      $('#overlay:not(.custom_acc_rep-adjusted)', context).each(function() {	  
        var $test = $('body').find('.node-type-acct-rep');
		if ($("body").hasClass('node-type-acct-rep')){
		 // adjust the overlay
          $('.add-or-remove-shortcuts', this).hide();  // hide "add short-cut" button
		  $('#overlay-tabs', this).hide();  // hide "tabs"
		  
          $('#branding', this).hide();  // hide branding container
		}		
      }).addClass('custom_acc_rep-adjusted');
    }
  };
})(jQuery);