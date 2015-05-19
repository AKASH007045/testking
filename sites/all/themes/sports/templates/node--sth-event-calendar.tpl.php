<?php if (isset($event['show_calendar']) && $event['show_calendar']): ?>
<div class="open-game-popup <?php  print isset($event['class']) ? $event['class'] : ''; ?> sth-calendar-event" <?php print $attributes; ?>>
<a class="inline" data-target="#modal_bootstrap" data-toggle="modal" href="#cal-event-<?php print $node->nid; ?>" ga-title="<?php  print $node->title; ?>">
  <div class="team-info event">
    <div class="field field-name-field-image field-type-image field-label-hidden" data-id="cal-event-<?php print $node->nid; ?>"><div class="field-items"><div class="field-item even">
     <img src="/sites/all/themes/sports/images/bg-event-legend.png" />
    </div></div></div>
  </div>
  </a>
</div>

<div id="cal-event-<?php print $node->nid; ?>" class="modal fade game-popup sth-cal-poup event-gallary-big-box" role="dialog" aria-hidden="true">
    <div class="modal-dialog">
    <div class="modal-content">
      <div class="modal-header">
          <div class="poupatstud"><?php print $node->title;//print $title;?> - <span><?php print $event['date'];?> </span> - <span><?php print $event['time'];?></span> <span><?php print $event['timezone']; ?></span></div>
          <div class="pptic-evt">              
              <!--<div><a href="#" target="_blank">MORE INFO</a></div>-->
			  <div class="mInfo mgTicket"><a href="javascript:void(0);" class="more-info">MORE INFO</a></div>               		  
              <div class="closePopup btn btn-default" data-dismiss="modal"></div>
        </div>
      </div>
      <div class="modal-body">
          <div class="pouptopwrap">
		  <div class="sthevent-image">
            <?php print $gallery; ?>
			</div>
			<div class="sthevent-body more-info openpop" id="more-info"><?php print render($content); ?><a href="javascript:void(0);" class="upArrow"><div></div></a></div>
          </div>
      </div>      
    </div>
  </div>  
</div>
<?php endif; ?>