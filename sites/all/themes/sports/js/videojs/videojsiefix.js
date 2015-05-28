jQuery(document).ready(function(){
	base_url = Drupal.settings.basePath;
    jQuery('#videojs-1631-field-video-video_html5_api').attr('poster',base_url+'sites/default/files/video/sacramento.jpg');
	jQuery('#videojs-1631-field-video-video_html5_api').get(0).play();
	/*jQuery("#videojs-1631-field-video-video_html5_api").ready(function(){
    var myPlayer = this;

  // EXAMPLE: Start playing the video.
    myPlayer.play();

});*/
	
  //Video Js fix for IE
  if(jQuery("video").length>0){
    var attr_id = jQuery("video").attr("id");
    var player_html = jQuery("#"+attr_id).clone().wrap('<div/>').parent().html();
    var pid = jQuery("#"+attr_id).parents('div').attr('id');
    var c=0;    
    videojs(attr_id).ready(function(){
      var myPlayer = this;
      myPlayer.poster('http://stpkings.io2/sites/default/files/video/sacramento.jpg');
      var browser = new cBrowser();
      if( browser.isIE ){  
        myPlayer.on('ended', function(){
          jQuery("div.video-js div.vjs-control-bar div.vjs-play-control").trigger('click');
        });
      }
    });   
  }
});  