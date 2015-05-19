jQuery(document).ready(function(){
 // jQuery('#next-game-active').center();
     jQuery('html, body').animate({
        scrollTop: jQuery("#next-game-active").prev().prev().prev().offset().top
    }, 1000);
});