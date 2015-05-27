jQuery(document).ready(function(){
  //Video Js fix for IE
  if(jQuery("video").length>0){
    var attr_id = jQuery("video").attr("id");
    var player_html = jQuery("#"+attr_id).clone().wrap('<div/>').parent().html();
    var pid = jQuery("#"+attr_id).parents('div').attr('id');
    var c=0;    
    videojs(attr_id).ready(function(){
      var myPlayer = this;
      var browser = new cBrowser();
      if( browser.isIE ){  
        myPlayer.on('ended', function(){
          jQuery("div.video-js div.vjs-control-bar div.vjs-play-control").trigger('click');
        });
      }
    });   
  }
});  