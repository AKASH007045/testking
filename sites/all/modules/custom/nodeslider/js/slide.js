/**
 * jquery added by debraj@osscube.com
 */
function call_flex(flex_id){
  jQuery('body').find('div#'+flex_id).flexslider({
    controlNav: true,
    directionNav:true,  
    animationLoop: false,
    smoothHeight: true,
    reverse: false,
    animation: "slide",//String: Select your animation type, "fade" or "slide"
    slideshow: false,   //Boolean: Animate slider automatically  
    slideshowSpeed: 7000,
    animationSpeed: 500,
    initDelay: 0
  });
}

jQuery(document).ready(function() {
  var href_value = "";
  var flex_id = "";
  
  //show/hide body and slider
	//first hide the body content	
	jQuery('.flexslider-off').hide();			
	jQuery('.region-content ul.tabs-primary-live li.edit-link').click(function() {
    var node_id_var_arr = jQuery(this).attr('data-id');
    //console.log(node_id_var_arr[1]);
    flex_id = jQuery('body').find('article.node-stp-page').find("div.flexslider").attr('id');
    if(flex_id == node_id_var_arr[1]){
      //console.log('im in the way...');
    }
    //var off_id = 'off-'.node_id_var_arr[1];
	  jQuery('.flexslider').hide(); //if any authorized person click on the edit link hide the slider
	  jQuery('.flexslider-off').show(); //if any authorized person click on the edit link after hide the slider show the body content
	});
  
	if(jQuery('body').find('#block-views-tab-pages-block').length>0){
    //Page load handling
    if(jQuery('body').find('.flexslider').length>0){
      href_value = jQuery('#block-views-tab-pages-block ul.nav-tabs li:first-child > a').attr('href');
      if(jQuery('body').find("div"+href_value).find('article.node-stp-page').length>0){
        flex_id = jQuery('body').find("div"+href_value).find('article.node-stp-page').find("div.flexslider").attr('id');
        call_flex(flex_id); 
      }
    }
  
    //tab switch handling
	  jQuery('body').find('#block-views-tab-pages-block ul.nav-tabs li > a').one('click', function(){
      jQuery('body').find('.flexslider').flexslider('destroy');
      href_value = jQuery(this).attr('href'); 
      if(jQuery('body').find("div"+href_value).find('article.node-stp-page').length>0){
        flex_id = jQuery('body').find("div"+href_value).find('article.node-stp-page').find("div.flexslider").attr('id');
        call_flex(flex_id); 
      }
	  }); 
	}
	else{
	  if(jQuery("article.node-stp-page div.flexslider").length>0){
      flex_id = jQuery("article.node-stp-page div.flexslider").attr('id'); 
      call_flex(flex_id);
	  }
  }
});