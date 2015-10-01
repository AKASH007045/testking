jQuery(document).ready(function(){
    if (jQuery('.node-type-tab-page .view-tab-pages ul.nav-tabs li').length <= 1) {
      jQuery('.node-type-tab-page .view-tab-pages ul.nav-tabs').css('display', 'none');
    }
    
    /* Event Gallery: Hide arrows for single slide*/
    jQuery('.carousel-inner').each(function() {
        if (jQuery(this).children('div').length === 1){
            jQuery(this).siblings('.carousel-control, .carousel-indicators').hide();
        }else{
            jQuery(this).siblings('.carousel-control, .carousel-indicators').show();
        }
    });
});