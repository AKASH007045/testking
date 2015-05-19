jQuery(document).ready(function(){
  jQuery('.views-bootstrap-tab-plugin-style li a').click(function(e){
       var tabid = jQuery(this).attr('href');
			 var iframe = jQuery(tabid).find('iframe');
			 if (iframe) {
				iframe.attr('src', iframe.attr('src'));
			 }  
    });
});