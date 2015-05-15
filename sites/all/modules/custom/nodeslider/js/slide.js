/**
 * jquery added by debraj@osscube.com
 */

jQuery(document).ready(function() {

    //show/hide body and slider
	//first hide the body content
	jQuery('.field-items').hide();
		
	jQuery('.region-content ul.tabs-primary-live li.edit-link').click(function() {		   
	  jQuery('.flexslider').hide(); //if any authorized person click on the edit link hide the slider
	  jQuery('.field-items').show(); //if any authorized person click on the edit link after hide the slider show the body content
	});	
	
/* Slider */
 jQuery('.flexslider').flexslider({
  controlNav: true,
  directionNav:true,  
  animationLoop: false,
  smoothHeight: true,
  reverse: false,
  animation: "slide",//String: Select your animation type, "fade" or "slide"
  slideshow: false,   //Boolean: Animate slider automatically  
  slideshowSpeed: 7000,
  animationSpeed: 500,
  initDelay: 0,  
  });
  
});
