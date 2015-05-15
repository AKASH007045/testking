<!-- Chang URLs to wherever Video.js files will be hosted -->
<link href="/sites/all/modules/custom/custom_renew/videojs/video-js.css" rel="stylesheet" type="text/css">
<!-- video.js must be in the <head> for older IEs to work. -->
<script src="/sites/all/modules/custom/custom_renew/videojs/video.js"></script>
<!-- Unless using the CDN hosted version, update the URL to the Flash SWF -->
<script>
  videojs.options.flash.swf = "/sites/all/modules/custom/custom_renew/videojs/video-js.swf";
  
  jQuery(document).ready(function() {
    videojs('ev1').ready(function() { 
      var myPlayer = this;
      myPlayer.play();
      myPlayer.on('ended', function(){
        myPlayer.posterImage.show(); 
        myPlayer.bigPlayButton.show();
      });
      myPlayer.on('play', function(){
        myPlayer.posterImage.hide(); 
        myPlayer.bigPlayButton.hide();
      });
      myPlayer.on('pause', function(){ 
        myPlayer.bigPlayButton.show();
      });
    });
  });
</script>
<div id="renew" class="renew-takeover">
  <div class="videotakeover">
    <video id="ev1" class="video-js vjs-default-skin" controls preload="none" width="438" height="246"
        poster="/sites/default/files/video/sacramento.jpg"
        data-setup="{}">        
      <source type="video/mp4" src="/sites/default/files/video/Fan_Proud_Work.mp4" />
    </video>
    <br />
    <br />
  </div>
  <p><a class="btn btn-light cnt-btm" href="invoice">Renew NOW</a> <a class="btn btn-light cnt-btm" href="/sites/default/files/<?php print $pdf_link;?>" target="_blank">Seating Map</a></p>
  <p>Renew by February 20, 2015.</p>
  <a href="/">Continue to Homepage</a>
</div>
