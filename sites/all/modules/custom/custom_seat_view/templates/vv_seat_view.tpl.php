<div class="vr-seat-view-wrapper">
 <div class="content">
<div class="seatview-dd"> </div>
<?php
  print $seat_holds;
?>
<?php if ($seat_count) {?>
<div class="seat-info-box more-info" id="more-info"><?php print views_embed_view('seat_info', 'block');?><a href="javascript:void(0);" class="upArrow"><div ></div></a></div>
  <div id="preloader"><img src="/sites/all/themes/sports/images/loader_big.gif"></div>
  <div id="vrholder" class="vrviwer-window" style="width:100%;height: 560px;">No Seat Available</div>
  <a href="javascript:void(0);" class="vrview-reset-view"><?php print t('<i class="fa fa-repeat"></i>') ;?></a>
  <div class="view-port-dimantion"><img src="/sites/all/themes/sports/images/view-dimantion.png"></div>
  <?php } else { ?>
  <img src="/sites/all/modules/custom/custom_seat_view/images/seat-error.png">
    <?php } ?>
  </div>
 </div>

<?php  $footer =  variable_get('seat_view_footer', array());?>
<div class="seat-view-important-information"><div class="field-items"><div class="field-item even">
<?php //print $footer['value'];?>
</div></div></div>

