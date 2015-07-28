jQuery(document).ready(function(){
     jQuery("#carousel-twitter").carousel({interval: 5000});
     jQuery("#carousel-facebook").carousel();
     jQuery("#carousel-instagram").carousel();
     jQuery("div.social-icons a").click(function() {
       var id = jQuery(this).attr('id');
       jQuery("div.social-icons a").removeClass('active');
       jQuery(this).addClass('active');
       jQuery(".block-custom-social-feed div.carousel").hide();
       jQuery(".block-custom-social-feed #carousel-"+id).show();
     });
});